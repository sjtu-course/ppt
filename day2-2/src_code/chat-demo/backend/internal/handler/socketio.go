package handler

import (
	user "chat-demo-backend/internal/model"
	"reflect"
	"sync"
	"time"

	gosocketio "github.com/graarh/golang-socketio"
	"github.com/graarh/golang-socketio/transport"
	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
)

const (
	OnMessage = "message"
	OnLogin   = "login"
	OnJoin    = "join"
	OnLeave   = "leave"
	OnSuccess = "success"
	OnFail    = "fail"

	GroupNamePrefix = "【群聊】"
	GroupAddress    = ""

	GroupType = "group"
	UserType  = "user"
)

func InitSocketIOServer() *gosocketio.Server {
	server := gosocketio.NewServer(transport.GetDefaultWebsocketTransport())
	RegisterSocketIOServer(server)

	return server
}

func RegisterSocketIOServer(server *gosocketio.Server) {
	var (
		roomToUser sync.Map
		idToRoom   sync.Map
	)

	server.On(gosocketio.OnConnection, func(c *gosocketio.Channel) {
		log.WithField("ID", c.Id()).Info("Connected")
	})

	server.On(gosocketio.OnDisconnection, func(c *gosocketio.Channel) {
		log.WithField("ID", c.Id()).Info("Disconnected")

		v, loaded := idToRoom.LoadAndDelete(c.Id())
		if !loaded {
			log.WithField("ID", c.Id()).Error("user is not in any existed room")
			return
		}
		roomID, ok := v.(string)
		if !ok {
			log.WithField("Type", reflect.TypeOf(roomID)).Error("invalid roomID type")
			return
		}

		v, loaded = roomToUser.Load(roomID)
		if !loaded {
			log.WithField("RoomID", roomID).Error("room is not existed yet")
			return
		}
		users, ok := v.(user.Users)
		if !ok {
			log.WithField("Type", reflect.TypeOf(users)).Error("invalid users type")
			return
		}

		// 删除用户信息
		usr := users.DeleteUser(c.Id())
		roomToUser.Store(roomID, users)

		// 广播用户离开房间
		server.BroadcastTo(roomID, OnLeave, usr)
	})

	server.On(OnLogin, func(c *gosocketio.Channel, usr user.User) {
		log.WithFields(log.Fields{
			"ID":     c.Id(),
			"RoomID": usr.RoomID,
		}).Info("Try to login")

		var users user.Users
		// 如果聊天群不存在则创建新聊天群，否则获取已经在群内的用户
		if v, loaded := roomToUser.Load(usr.RoomID); !loaded {
			avatarUrl := viper.GetString("frontend.group_avatar_url")
			groupUser := user.User{
				ID:        usr.RoomID,
				RoomID:    usr.RoomID,
				Name:      GroupNamePrefix + usr.RoomID,
				AvatarURL: avatarUrl,
				Type:      GroupType,
				Address:   GroupAddress,
				LoginTime: time.Now(),
			}

			users = append(users, groupUser)
		} else {
			vv, ok := v.(user.Users)
			if !ok {
				log.WithField("Type", reflect.TypeOf(users)).Error("invalid users type")
				return
			}
			users = vv
		}

		// 检查是否存在同名用户
		if users.IsUserRegistered(usr) {
			log.WithField("Name", usr.Name).Info("User is already existed")
			if err := c.Emit(OnFail, []interface{}{"登录失败，昵称已经存在！"}); err != nil {
				log.WithFields(log.Fields{
					"Method": OnFail,
					"Error":  err,
				}).Error("fail to emit")
			}
			return
		}

		idToRoom.Store(c.Id(), usr.RoomID)
		usr.ID = c.Id()
		usr.Address = c.Ip()
		usr.LoginTime = time.Now()
		usr.Type = UserType
		if err := c.Join(usr.RoomID); err != nil { // 加入该群虚拟房间
			log.WithFields(log.Fields{
				"RoomID": usr.RoomID,
				"Error":  err,
			})
		}
		if err := c.Join(usr.ID); err != nil { // 为该用户创建虚拟房间，用于单聊
			log.WithFields(log.Fields{
				"RoomID": usr.ID,
				"Error":  err,
			})
		}

		// 同意登录，并发送已经在聊天群内的其余用户信息
		if err := c.Emit(OnSuccess, []interface{}{usr, users}); err != nil {
			log.WithFields(log.Fields{
				"Method": OnSuccess,
				"Error":  err,
			}).Error("fail to emit")
		}

		users = append(users, usr)
		roomToUser.Store(usr.RoomID, users)

		// 广播新用户进入群聊
		server.BroadcastTo(usr.RoomID, OnJoin, usr)
	})

	type ArgType struct {
		From user.User
		To   user.User
		Msg  string
	}
	server.On(OnMessage, func(c *gosocketio.Channel, arg ArgType) {
		// 单聊
		if arg.To.Type == UserType {
			server.BroadcastTo(arg.To.ID, OnMessage, []interface{}{arg.From, arg.To, arg.Msg})
			return
		}

		// 群聊
		server.BroadcastTo(arg.To.RoomID, OnMessage, []interface{}{arg.From, arg.To, arg.Msg})
	})
}
