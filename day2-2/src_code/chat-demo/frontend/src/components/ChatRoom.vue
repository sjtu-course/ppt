<template>
  <div class="tpl">
    <div class="wt-warp" v-if="loginUser">
      <div class="wt-menu-box">
        <div class="wt-login-avatar">
          <img :src="loginUser.avatarUrl" alt="" />
        </div>
        <div class="wt-menu-list">
          <div class="wt-menu-item" :class="{active:currentMenu==='session'}" @click="currentMenu='session'">
            <span class="wt-menu-icon iconfont icon-comments"></span>
          </div>
        </div>
      </div>
      <div class="wt-container">
        <div class="wt-session-warp" style="display: none;" v-show="currentMenu==='session'">
          <div class="wt-session-list">
            <div class="wt-session-search">
              <i class="iconfont icon-search"></i>
              <input type="text" class="wt-search-input" placeholder="搜索" v-model="keyword" />
            </div>
            <div class="wt-session-users">
              <div class="wt-session-listBox wt-scroll">
                <div class="wt-users-item" v-for="item in searchUser(keyword)" :key="item.id" :class="{active:item.id===sessionId}"
                     @click="changeSession(item)">
                  <div class="wt-user-avatar">
                    <img :src="item.avatarUrl" alt="" />
                  </div>
                  <div class="wt-item-info">
                    <div class="wt-item-name">
                      {{item.name}}
                      <span class="wt-message-time" v-if="getLastMessage(item.id).time">{{getLastMessage(item.id).time |
                          friendlyTime}}</span>
                    </div>
                    <div class="wt-item-message">
                      {{getLastMessage(item.id).content}}
                      <span class="wt-message-num"
                            v-if="getUnReaderNum(item.id)!==0">{{getUnReaderNum(item.id)}}</span>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
          <div class="wt-session-container">
            <div class="wt-session-window" v-if="session">
              <div class="wt-window-header">
                <div class="wt-session-info">
                  <img :src="session.avatarUrl" alt="" />
                  <span>{{session.name}}</span>
                  <span class="wt-icon iconfont icon-phone wt-icon-phone" v-if="session.deviceType==='touch'"></span>
                  <span class="wt-icon iconfont icon-pc wt-icon-pc" v-if="session.deviceType==='pc'"></span>
                  <span class="wt-session-address">{{session.address}}</span>
                  <template v-if="session.type==='group'">
                    <span class="wt-session-address">({{sessionList.length}})</span>
                  </template>
                </div>
              </div>
              <div class="wt-window-container">
                <div class="wt-window-messageList wt-scroll" ref="messageList">
                  <template v-for="item in messageList">
                    <div class="wt-message-item" :key="item.id"
                         :class="item.from.id===loginUser.id?'wt-message-send':'wt-message-receive'">
                      <div class="wt-avatarBox">
                        <img :src="item.from.avatarUrl" alt="" />
                      </div>
                      <div class="wt-message-body">
                        <template v-if="setting.isName">
                          <div class="wt-from-info" v-if="item.from.id!==loginUser.id">
                            <span>{{item.from.name}}</span>
                            <span class="wt-time-span" v-if="setting.isTime">{{item.time | friendlyTime}}</span>
                          </div>
                          <div class="wt-from-info" v-else>
                            <span class="wt-time-span" v-if="setting.isTime">{{item.time | friendlyTime}}</span>
                            <span>{{item.from.name}}</span>
                          </div>
                        </template>
                        <div class="wt-message-text" v-html="item.content"></div>
                      </div>
                    </div>
                  </template>
                </div>
              </div>
              <div class="wt-window-footer">
                <div class="wt-toolBar-box">
                </div>
                <div class="wt-message-box">
                    <textarea class="wt-message-input wt-scroll" v-model="text" placeholder="请输入信息"
                              @keyup.enter="sendMessage(text)"></textarea>
                </div>
                <div class="wt-sendBtn-box">
                  <span class="wt-sendBtn" @click="sendMessage(text)">发送</span>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
    <transition name="slider">
      <div class="wt-alert-message" v-show="isAlert">{{alertMessage}}</div>
    </transition>
    <audio src="../assets/bell.mp3" ref="audio"></audio>
  </div>
</template>

<script>
import io from 'socket.io-client'
import bus from "@/utils/bus"

export default {
  name: "ChatRoom",
  data(){
    return {
      loginUser:null ,
      setting:{
        isVoice:true,
        isTime:true,
        isName:true
      },
      isShow:false,
      isAlert:false,
      currentMenu:"session",
      sessionList:[],
      sessionId:"",
      messages:{},
      text:"",
      keyword:"",
      alertMessage:""
    }
  },
  created(){
    this.initSocket();
    bus.$on('login', (user)=>(
        this.userLogin(user)
    ))
  },
  computed:{
    session(){
      let session=null,
          sessionId=this.sessionId;

      this.sessionList.forEach((item)=>{
        if(item.id===sessionId){
          session=item;
        }
      })
      return session;
    },

    messageList(){
      if(this.sessionId===''){
        return []
      }
      if(this.messages[this.sessionId]){
        return this.messages[this.sessionId]
      }else {
        return []
      }
    },
  },
  methods:{
    initSocket(){
      let _this=this;

      console.log("backend address: ", process.env.VUE_APP_BACKEND_URL)
      _this.socket=io(process.env.VUE_APP_BACKEND_URL, {transports: ['websocket']});

      _this.socket.on("success",([user,users])=>{
        _this.loginUser=user;
        _this.sessionList=users;

        bus.$emit("hide")
      })

      _this.socket.on("fail",(message)=>{
        _this.showAlertMessage(message)
        console.log(message)
      })

      _this.socket.on("message",([from,to,message,type])=>{
        _this.receiveMessage(from,to,message,type)
      })

      _this.socket.on("join", (user)=>{
        if(user.id === _this.loginUser.id) {
          return
        }

        _this.addUser(user);
      })

      _this.socket.on("leave", (user)=>{
        if(user.id === _this.loginUser.id) {
          return
        }

        _this.removeUser(user);
      })

      _this.socket.on("error",()=>{
        console.log("出错了！！")
      })

      _this.socket.on("connect",(data)=>{
        _this.isOnline = true;
        console.log("连接成功！",data)
      })

      _this.socket.on("disconnect",(data)=>{
        _this.socket.emit("disconnect", data)
        _this.isOnline=false;
        console.log(JSON.stringify(data)+ ' - disconnect');
      })

      _this.socket.on("connect_error",(data)=>{
        _this.isOnline=false;
        console.log(JSON.stringify(data)+ ' - connect_error')
      })

      _this.socket.on("connect_timeout",(data)=>{
        _this.isOnline=false;
        console.log(JSON.stringify(data)+ ' - connect_timeout')
      })

      _this.socket.on("reconnect",(data)=>{
        console.log(JSON.stringify(data)+ ' - reconnect')
      })

      _this.socket.on("reconnect_attempt",()=>{
        if(_this.loginUser){
          _this.socket.io.opts.query={
            User:_this.loginUser.id ? JSON.stringify(_this.loginUser) : ''
          }
        }
      })
    },

    changeSession(user){
      let sessionId=user.id;
      let messageList=this.messages[sessionId];
      if(messageList){
        messageList.forEach((item)=>{
          item.isRead=true;
        })
      }
      this.sessionId=sessionId;
      this.scrollFooter();
    },

    sendMessage(msg){
      if(msg===""){
        return;
      }

      let message={
        channelId:this.sessionId,
        from:this.loginUser,
        to:this.session,
        content:msg,
        time:new Date().getTime(),
        isRead:true
      }

      if(!this.messages[this.sessionId]){
        this.$set(this.messages,this.sessionId,[])
      }

      this.messages[this.sessionId].push(message);
      this.text="";
      this.scrollFooter();

      if (this.socket) {
        let args = {
          from: message.from,
          to: message.to,
          msg: message.content,
        }
        this.socket.emit("message", args);
      }
    },

    receiveMessage(from,to,msg,type){
      if(msg==="") {
        return;
      }

      if (to.type ==='group' && from.id === this.loginUser.id) {
        return;
      }

      let channelId = (to.type==='group'?to.id:from.id);
      let isRead = (channelId===this.sessionId);
      let message={
        channelId:channelId,
        from:from,
        to:to,
        content:msg,
        time:new Date().getTime(),
        type:type,
        isRead: isRead
      }

      if(!this.messages[channelId]){
        this.$set(this.messages,channelId,[])
      }

      this.messages[channelId].push(message);
      this.text="";

      if(isRead){
        this.scrollFooter();
        return
      }

      if(this.setting.isVoice && this.$refs.audio && to.type!=='group'){
          this.$refs.audio.play();
      }
    },

    getLastMessage(sessionId){
      let messageList=this.messages[sessionId];

      if (messageList && messageList.length > 0){
        return {
          time: messageList[messageList.length-1].time,
          content: messageList[messageList.length-1].content
        }
      }

      return {
        time:null,
        content:""
      }
    },

    getUnReaderNum(sessionId){
      let messageList=this.messages[sessionId];

      if(messageList){
        let len=messageList.length;

        if (len <= 0) {
          return 0
        }

        let count=0;
        messageList.forEach((item)=>{
          if(!item.isRead){
            count++;
          }
        })

        if(count>99){
          return "99+"
        }

        return count
      }

      return 0
    },

    searchUser(keyword){
      let arr=[];

      this.sessionList.forEach( (item )=>{
        if((item.name.indexOf(keyword) !== -1)||(item.id.indexOf(keyword) !== -1)){
          arr.push(item)
        }
      })

      return arr;
    },

    userLogin(user){
      if(this.socket){
        this.socket.emit("login",user);
        return
      }

      user.id="user_"+new Date().getTime();
      user.deviceType="pc";
      this.loginUser=user;
    },

    addUser(user){
      let _this=this;
      let index=-1;

      for (let i = 0; i < _this.sessionList.length; i++) {
        let item = _this.sessionList[i];
        if(user.id===item.id){
          index=i;
          _this.sessionList[i]=user;
        }
      }

      if(index===-1){
        _this.sessionList.push(user);
      }
    },

    removeUser(user){
      let _this=this;

      for (let i = 0; i < _this.sessionList.length; i++) {
        let item = _this.sessionList[i];
        if(user.id===item.id){
          _this.sessionList.splice(i,1);
          break;
        }
      }
    },

    showAlertMessage(message){
      let _this=this;
      this.alertMessage=message;
      _this.isAlert=true;

      let inter=setTimeout(()=>{
        clearTimeout(inter);
        _this.isAlert=false;
      },3000)
    },

    scrollFooter(){
      let $list=this.$refs['messageList'];

      if($list){
        this.$nextTick(()=>{
          $list.scrollTop = $list.scrollHeight
        })
      }
    }
  },
  filters:{
    friendlyTime(value){
      let time=new Date().getTime();
      time=parseInt((time-value)/1000);
      //存储转换值
      let s;
      if(time<60*3){//三分钟内
        return '刚刚';
      }else if((time<60*60)&&(time>=60*3)){
        //超过十分钟少于1小时
        s = Math.floor(time/60);
        return  s+"分钟前";
      }else if((time<60*60*24)&&(time>=60*60)){
        //超过1小时少于24小时
        s = Math.floor(time/60/60);
        return  s+"小时前";
      }else if((time<60*60*24*3)&&(time>=60*60*24)){
        //超过1天少于3天内
        s = Math.floor(time/60/60/24);
        return s+"天前";
      }else{
        //超过3天
        let date= new Date(value);
        return date.getFullYear()+"."+(date.getMonth()+1)+"."+date.getDate();
      }
    },

    formatTime(value){
      let date=new Date(value);
      let year=date.getFullYear();
      let month=date.getMonth()+1;
      let day=date.getDate();
      let hour=date.getHours()>9?date.getHours():("0"+date.getHours());
      let minutes=date.getMinutes()>9?date.getMinutes():("0"+date.getMinutes());
      let seconds=date.getSeconds()>9?date.getSeconds():("0"+date.getSeconds());
      return year+"."+month+"."+day+" "+hour+":"+minutes+":"+seconds;
    }
  }
}
</script>

<style scoped>
  .wt-warp{
    width: 740px;
    height: 600px;
    position: fixed;
    top: 0;
    left: 0;
    bottom: 0;
    right: 0;
    margin: auto;
  }

  .wt-menu-box{
    height: 100%;
    width: 60px;
    float: left;
    background-color: #26292e;
    position: relative;
  }

  .wt-container{
    height: 100%;
    margin-left: 60px;
    background-color: #eeeeee;
  }

  .wt-session-warp{
    display: block;
    width: 100%;
    height: 100%;
  }

  .wt-session-list{
    width: 220px;
    height: 100%;
    float: left;
    border-right: 1px solid #d1d1d1;
    background-color: #ffffff;
  }

  .wt-session-container{
    margin-left: 221px;
    height: 100%;
  }

  .wt-session-window{
    width: 100%;
    height: 100%;
    position: relative;
    background-color: #eeeeee;
  }

  .wt-window-header{
    height: 50px;
    border-bottom: 1px solid #d1d1d1;
    margin: 0 10px;
  }

  .wt-window-footer{

  }

  .wt-window-container{
    height: calc(100% - 190px);
    position: relative;
  }

  .wt-window-messageList{
    position: absolute;
    width: 100%;
    height: 100%;
    padding: 10px;
    box-sizing: border-box;
    overflow-y: auto;
  }

  .wt-toolBar-box{
    height: 30px;
    border-top: 1px solid #d1d1d1;
    margin: 0 10px;
    position: relative;
  }

  .wt-session-info{
    line-height: 30px;
    padding: 10px 0;
  }

  .wt-session-info img{
    width: 30px;
    height: 30px;
    border-radius: 4px;
    vertical-align: middle;
  }

  .wt-session-info span{
    display: inline-block;
    line-height: 30px;
    vertical-align: middle;
    margin-left: 5px;
    font-size: 15px;
    color: #606266;
  }

  .wt-session-info .wt-session-address{
    margin-left: 0;
    font-size: 12px;
    color: #6a737d;
  }

  .wt-toolBtn i{
    font-size: 24px;
    color: #606266;
  }

  .wt-session-search{
    padding: 10px;
    position: relative;
  }

  .wt-session-search i{
    position: absolute;
    font-size: 20px;
    left: 13px;
    top: 14px;
    color: #6a737d;
  }

  .wt-search-input{
    border: none;
    width: 100%;
    height: 30px;
    line-height: 30px;
    font-size: 12px;
    background: #f2f2f2;
    border-radius: 4px;
    box-sizing: border-box;
    padding-left: 26px;
    padding-right: 10px;
    color: #606266;
    outline: none;
  }

  .wt-session-users{
    height: calc(100% - 60px);
    position: relative;
    width: 100%;
  }

  .wt-session-listBox{
    position: absolute;
    width: 100%;
    height: 100%;
    box-sizing: border-box;
    overflow-y: auto;
  }

  .wt-sendBtn-box{
    text-align: right;
    padding: 5px 10px 0 0;
  }

  .wt-sendBtn{
    display: inline-block;
    border: 1px solid #c1c1c1;
    border-radius: 4px;
    -moz-border-radius: 4px;
    -webkit-border-radius: 4px;
    padding: 3px 30px;
    font-size: 14px;
    text-decoration: none;
    background-color: #fff;
    color: #222;
  }

  .wt-login-avatar{
    width: 40px;
    height: 40px;
    padding: 10px;
  }

  .wt-login-avatar img{
    display: block;
    border: none;
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .wt-menu-item{
    padding: 10px;
    cursor: pointer;
  }

  .wt-menu-item .wt-menu-icon{
    display: block;
    width: 20px;
    height: 20px;
    margin: 0 auto;
  }

  .wt-menu-item .wt-menu-icon{
    font-size: 20px;
  }

  .wt-menu-item .wt-menu-icon{
    color: #f2f2f2;
  }

  .wt-menu-item.active .wt-menu-icon,

  .wt-menu-item:hover .wt-menu-icon{
    color: #b2e281;
  }

  .wt-users-item:after{
    display: block;
    content: "";
    position: absolute;
    right: 0;
    top: 2px;
    bottom: 2px;
    width: 2px;
  }

  .wt-users-item{
    padding: 10px;
    position: relative;
    cursor: pointer;
  }

  .wt-users-item.active:after,

  .wt-users-item:hover:after{
    background: #b2e281;
  }

  .wt-users-item.active{
    background-color: #f2f2f2;
  }

  .wt-user-avatar {
    display: block;
    width: 40px;
    height: 40px;
    float: left;
  }

  .wt-user-avatar img{
    display: block;
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .wt-item-info{
    margin-left: 45px;
  }

  .wt-item-name{
    line-height: 20px;
    height: 20px;
    vertical-align: middle;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    position: relative;
    font-size: 12px;
    color: #333333;
    padding-right: 60px;
  }

  .wt-item-name .wt-message-time{
    display: block;
    position: absolute;
    right: 0;
    top: 0;
    height: 20px;
    line-height: 20px;
    font-size: 12px;
    color: #6a737d;
  }

  .wt-item-message{
    line-height: 20px;
    height: 20px;
    color: #6a737d;
    font-size: 12px;
    vertical-align: middle;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
    position: relative;
    padding-right: 26px;
  }

  .wt-item-message .wt-message-num{
    display: block;
    position: absolute;
    right: 0;
    top: 2px;
    height: 12px;
    min-width: 12px;
    line-height: 12px;
    font-size: 12px;
    color: #ffffff;
    text-align: center;
    padding: 2px;
    border-radius: 10px;
    background-color: #f56c6c;
    vertical-align: middle;
  }

  .wt-no-session svg{
    width: 100%;
    height: 100%;
  }

  .wt-no-session svg path{
    fill: #d9dce0;
  }

  .wt-setting-warp{
    display: block;
    width: 100%;
    height: 100%;
  }

  .wt-setting-container{
    padding: 0 10px;
    max-width: 460px;
  }

  .wt-loginInfo-warp{
    display: block;
    height: 40px;
    padding:10px;
    margin-top: 15px;
  }

  .wt-loginInfo-avatar{
    display: block;
    width: 40px;
    height: 40px;
    float: left;
  }

  .wt-loginInfo-avatar img{
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .wt-container-title{
    margin: 0;
    line-height: 30px;
    font-size: 14px;
    color: #606266;
  }

  .wt-loginInfo-container{
    margin-left: 45px;
  }

  .wt-user-subInfo{
    font-size: 12px;
    color: #606266;
    line-height: 20px;
  }

  .wt-user-name{
    font-size: 14px;
    color: #606266;
    line-height: 20px;
  }

  .wt-setting-item input[type='checkbox']{
    width: 20px;
    height: 20px;
    margin: 0;
    padding: 0;
    vertical-align: middle;
  }

  .wt-setting-item{
    font-size: 14px;
    line-height: 40px;
    color: #6a737d;
    border-top: 1px dotted #d1d1d1;
    padding-left: 10px;
  }

  .wt-setting-item span{
    margin-left: 10px;
  }

  .wt-icon{
    display: inline-block;
    vertical-align: middle;
    line-height: 20px;
  }

  .wt-icon.wt-icon-phone{
    color: #f96907;
  }

  .wt-icon.wt-icon-pc{
    color: #0b90f9;
  }

  .wt-scroll::-webkit-scrollbar{
    width: 5px;
    height: 5px;
    background-color: rgba(255, 255, 255, 0);
  }

  .wt-scroll::-webkit-scrollbar-track{
    -webkit-box-shadow: inset 0 0 6px rgba(240, 240, 240, 0);
    border-radius: 10px;
    background-color: rgba(0, 89, 255, 0);
  }

  .wt-scroll:hover::-webkit-scrollbar-thumb{
    width: 10px;
    height: 20px;
    border-radius: 10px;
    -webkit-box-shadow: inset 0 0 6px rgba(236, 236, 236, 0.3);
    background-color: rgba(203, 203, 203, 0.54);
    transition: all 0.5s;
  }

  .wt-message-item:after{
    display: block;
    content: "";
    clear: both;
  }

  .wt-message-item{
    margin-top: 10px;
  }

  .wt-message-item:first-child{
    margin-top: 0;
  }

  .wt-avatarBox{
    display: block;
    width: 40px;
    height: 40px;
    border-radius: 4px;
  }

  .wt-avatarBox img{
    display: block;
    border: none;
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .wt-from-info{
    line-height: 20px;
    font-size: 12px;
    color: #6d6d6d;
  }

  .wt-message-body{
    margin: 0 55px;
  }

  .wt-message-text{
    display: inline-block;
    line-height: 20px;
    min-height: 20px;
    padding: 10px;
    position: relative;
    border-radius: 4px;
    background: #FFFFFF;
    font-size: 12px;
    color: #6d6d6d;
    word-wrap: break-word;
    text-align: left;
    max-width: 100%;
    box-sizing: border-box;
  }

  .wt-message-text img {
    margin: 2px;
    border: 0;
    vertical-align: bottom;
  }

  .wt-time-span{
    margin: 0 5px;
  }

  .wt-message-send .wt-avatarBox{
    float: right;
  }

  .wt-message-send .wt-from-info{
    text-align: right;
  }

  .wt-message-send .wt-message-text{
    background: #b2e281;
  }

  .wt-message-send .wt-message-text:after{
    display: block;
    width: 0;
    height: 0;
    content: "";
    position: absolute;
    left: 100%;
    border-top: 5px solid transparent;
    border-left: 5px solid #b2e281;
    border-bottom: 5px solid transparent;
    top:15px;
  }

  .wt-message-send .wt-message-body{
    text-align: right;
  }

  .wt-message-receive .wt-avatarBox{
    float: left;
  }

  .wt-message-receive .wt-from-info{
    text-align: left;
  }

  .wt-message-receive .wt-message-text:after{
    display: block;
    width: 0;
    height: 0;
    content: "";
    position: absolute;
    right: 100%;
    border-top: 5px solid transparent;
    border-right: 5px solid #FFFFFF;
    border-bottom: 5px solid transparent;
    top:15px;
  }

  .wt-message-box{
    height: 60px;
    margin: 0 10px;
  }

  .wt-message-input{
    display: block;
    resize: none;
    height: 100%;
    width: 100%;
    padding: 10px;
    box-sizing: border-box;
    outline: none;
    margin: 0;
    background-color: transparent;
    border: none;
  }

  .wt-alert-message{
    position: absolute;
    top: 60px;
    line-height: 20px;
    font-size: 12px;
    padding: 5px 10px;
    color: #ff959f;
    border-radius: 4px;
    background-color: rgba(51, 51, 51, 0.8);
    left: 50%;
    transform: translateX(-50%);
  }

</style>