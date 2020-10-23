<template>
  <div id="app">
    <Login v-if="!isLogin"></Login>
    <ChatRoom/>
  </div>
</template>

<script>
import Login from "@/components/Login";
import ChatRoom from "@/components/ChatRoom";
import bus from "@/utils/bus";

export default {
  name: 'App',
  components: {
    Login,
    ChatRoom,
  },
  data(){
    return{
      isLogin:false,
    }
  },
  created(){
    bus.$on('hide', ()=>(
        this.isLogin = true
    ))
  },
}
</script>

<style>
  html{
    height: 100%;
  }
  body{
    height: 100%;
    background: url("./assets/background.jpg") no-repeat center;
    background-size: cover;
    margin: 0;
  }
</style>
