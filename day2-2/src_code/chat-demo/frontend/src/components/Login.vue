<template>
  <div class="wt-loginBox">
    <div class="wt-loginBanner">
      <div class="wt-error-message" v-if="errorMsg">{{errorMsg}}</div>
    </div>
    <div class="wt-loginForm-box">
      <form class="wt-login-form">
        <div class="wt-user-avatarBox">
          <img :src="user.avatarUrl" alt="" @click.stop="isShow=!isShow" />
          <transition name="slider">
            <div class="wt-select-imgBox" v-show="isShow">
              <ul class="wt-avatar-list">
                <li :class="{active:user.avatarUrl==item}" v-for="item in avatars" :key="item.id">
                  <img :src="item" alt="" @click="user.avatarUrl=item" />
                </li>
              </ul>
            </div>
          </transition>
        </div>
        <div class="wt-form-inputBox">
          <input type="text" class="wt-user-name" placeholder="昵称" v-model="user.name" />
          <input type="text" class="wt-user-room" placeholder="房间号" v-model="user.room_id" />
          <button type="button" class="wt-formBtn" @click="login(user)">
            进入聊天室
          </button>
        </div>
      </form>
    </div>
  </div>
</template>

<script>
import bus from "@/utils/bus"

export default {
  name: "Login",
  data(){
    return {
      user:{
        name:"",
        room_id:"",
        avatarUrl:"",
        type:"user",
      },
      avatars:[
        'http://q.qlogo.cn/headimg_dl?dst_uin=956411242&spec=100',
        'http://q.qlogo.cn/headimg_dl?dst_uin=956411091&spec=100',
        'http://q.qlogo.cn/headimg_dl?dst_uin=1361514346&spec=100',
        'http://q.qlogo.cn/headimg_dl?dst_uin=624748513&spec=100',
        'http://q.qlogo.cn/headimg_dl?dst_uin=157509895&spec=100',
      ],
      isShow:false,
      errorMsg:"",
    }
  },
  created(){
    let _this=this;
    document.addEventListener('click',()=>{
      _this.isShow=false;
    })
    _this.addAvatar();
  },
  methods:{
    login(user){
      if(user.name===""){
        this.showErrorMsg('请输入用户昵称！')
        return
      }

      if(user.room_id===""){
        this.showErrorMsg('请输入房间号！')
        return
      }

      bus.$emit('login',user)
    },
    showErrorMsg(message){
      let _this=this;
      this.errorMsg=message;
      let inter=setTimeout(()=>{
        _this.errorMsg='';
        clearTimeout(inter);
      },3000)
    },

    randomNumber(){
      let num=parseInt(Math.random()*3+6);
      let firstNum=parseInt(Math.random()*9+1);
      let res=""+firstNum;

      for (let i = 0; i < num; i++) {
        res += parseInt(Math.random()*9);
      }

      return res;
    },
    addAvatar(){
      let _this=this;
      let qq=_this.randomNumber();
      let url="http://q.qlogo.cn/headimg_dl?dst_uin=" + qq + "&spec=100";
      if(this.avatars.indexOf(url)===-1){
        this.avatars.push(url);
      }
      _this.user.avatarUrl=url;
    }
  }
}
</script>

<style scoped>
  .wt-loginBox{
    position: fixed;
    width: 380px;
    top: 50%;
    left: 50%;
    margin-left: -190px;
    margin-top: -190px;
    z-index: 999;
  }

  .wt-loginBanner{
    position: relative;
    height: 160px;
    background: url("../assets/banner.jpg") no-repeat center;
    background-size: cover;
    border-radius: 4px 4px 0 0;
  }

  .wt-login-form{
    display: inline-block;
  }

  .wt-login-form:after{
    display: block;
    content: "";
    clear: both;
  }

  .wt-loginForm-box{
    text-align: center;
    padding: 20px 0;
    background: #FFFFFF;
    border-radius: 0 0 4px 4px;
  }

  .wt-user-avatarBox{
    width: 70px;
    height: 70px;
    position: relative;
    float: left;
  }

  .wt-user-avatarBox img{
    display: block;
    width: 100%;
    height: 100%;
    border-radius: 4px;
  }

  .wt-form-inputBox {
    margin-left: 80px;
  }

  .wt-form-inputBox input{
    font-size: 14px;
    display: block;
    line-height: 35px;
    height: 35px;
    padding: 0 10px;
    border: 1px solid #C0C4CC;
    box-sizing: border-box;
    margin: -1px;
    width: 220px;
    outline: none;
    position: relative;
  }

  .wt-formBtn{
    margin-top: 15px;
    display: block;
    width: 100%;
    line-height: 30px;
    font-size: 12px;
    background: #4db3ff;
    color: #ffffff;
    border: none;
    outline: none;
    border-radius: 4px;
    cursor: pointer;
  }

  .wt-user-name{
    border-radius: 4px 4px 0 0;
  }

  .wt-user-name:focus,.wt-user-room:focus{
    border-color: #4db3ff;
    z-index: 9;
  }

  .wt-user-room{
    border-radius:0 0 4px 4px;
  }

  .wt-select-imgBox{
    position: absolute;
    width: 360px;
    padding: 10px;
    background: #ffffff;
    border-radius: 4px;
    top: 80px;
    left: -15px;
    border: 1px solid #d1d1d1;
  }

  .wt-select-imgBox:before{
    display: block;
    position: absolute;
    content: "";
    width: 0;
    height: 0;
    bottom: 100%;
    border-left: 10px solid transparent;
    border-right: 10px solid transparent;
    border-bottom: 10px solid #d1d1d1;
    left: 40px;
    margin-bottom: 0px;
  }

  .wt-select-imgBox:after{
    display: block;
    position: absolute;
    content: "";
    width: 0;
    height: 0;
    bottom: 100%;
    border-left: 10px solid transparent;
    border-right: 10px solid transparent;
    border-bottom: 10px solid #ffffff;
    left: 40px;
    margin-bottom: -1px;
  }

  .wt-avatar-list{
    list-style: none;
    padding: 0;
    margin: 0;
  }

  .wt-avatar-list:after{
    display: block;
    content: "";
    clear: both;
  }

  .wt-avatar-list li{
    float: left;
    margin: 5px;
    width: 50px;
    height: 50px;
    position: relative;
    border: 2px solid transparent;
    box-sizing: border-box;
    border-radius: 4px;
    cursor: pointer;
    overflow: hidden;
  }
  .wt-avatar-list li img{
    display: block;
    width: 100%;
    height: 100%;
  }

  .wt-avatar-list li.active{
    border: 2px solid #4db3ff;
  }

  .wt-avatar-list li.active:after{
    display: block;
    position: absolute;
    width: 15px;
    height: 15px;
    background-color: #4db3ff;
    content:'✔';
    color: #fff;
    bottom: 0;
    right: 0;
    text-align: right;
    line-height: 18px;
    font-size: 12px;
    border-radius: 10px 0 0 0;
  }

  .wt-error-message{
    position: absolute;
    bottom: 0;
    left: 0;
    width: 100%;
    font-size: 12px;
    line-height: 30px;
    background-color: rgba(242, 242, 242, 0.5);
    color: #eb5951;
    padding: 0 15px;
    box-sizing: border-box;
  }

</style>