package user

import "time"

type User struct {
	ID        string    `json:"id"`
	RoomID    string    `json:"room_id"`
	Name      string    `json:"name"`
	AvatarURL string    `json:"avatarUrl"`
	Type      string    `json:"type"`
	Address   string    `json:"address"`
	LoginTime time.Time `json:"login_time"`
}

type Users []User

func (u *Users) IsUserRegistered(user User) bool {
	for _, usr := range *u {
		if usr.Name == user.Name && usr.Type == user.Type {
			return true
		}
	}

	return false
}

func (u *Users) GetUser(id string) (User, bool) {
	for _, usr := range *u {
		if usr.ID == id {
			return usr, true
		}
	}

	return User{}, false
}

func (u *Users) DeleteUser(id string) User {
	for idx, usr := range *u {
		if usr.ID == id {
			*u = append((*u)[:idx], (*u)[idx+1:]...)
			return usr
		}
	}

	return User{}
}

func (u *Users) AddUser(user User) {
	for _, usr := range *u {
		if usr.ID == user.ID {
			return
		}
	}

	*u = append(*u, user)
}
