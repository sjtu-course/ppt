# Chat Demo

一个简单的在线聊天室 demo，支持群聊、私聊、搜索、消息提示

前端使用**Vue**框架，后端使用**Golang**，通过**socket.io**通信

支持`shell`脚本直接部署和通过`docker-compose`容器化部署

# 部署

**首先需要修改`frontend/.env.prod`中的后端 IP，改为部署机器的 IP，否则只能本机访问**

## 脚本部署

**golang 的版本需要大于等于`1.15`,否则编译失败**

```shell
# 开发环境启动
./run.sh dev
# 生产环境启动
./run.sh prod

# 停止
./stop.sh
```

## 容器部署

**需要预先安装 docker 和 docker-compose**

```shell
# 如果代码修改过，需要通过--build参数重新生成镜像
docker-compose up -d [--build]

# 停止
docker-compose stop
```

# 目录结构

```shell
.
├── /backend
│   ├── Dockerfile # 后端镜像构建文件
│   ├── Makefile # 编译文件
│   ├── /cmd # 程序入口
│   ├── codecov.sh # 测试覆盖率检测脚本
│   ├── /conf # 配置文件
│   ├── go.mod # 包管理
│   │   ├── go.sum
│   └── /internal
│       ├── /conf #读取配置
|       ├── /handler
│       │   ├── middleware.go # 中间件
│       │   ├── router.go # 路由
│       │   └── socketio.go # 聊天室主逻辑
│       └── /model
│           └── user.go # 用户
├── /frontend
│   ├── Dockerfile # 前端镜像构建文件
│   ├── babel.config.js # babel配置
│   ├── package-lock.json # 包版本锁定
│   ├── package.json # 配置、启动
│   ├── /public # html
│   └── /src
│       ├── /assets # 图片、音乐等
│       ├── /components # 登录、聊天室模块
│       ├── App.vue # Vue框架入口
│       ├── main.js # 程序入口
├── docker-compose.yaml # 容器启动脚本
├── run.sh # shell启动脚本
├── stop.sh # 停止脚本
└── README.md
```

# 效果

## 登录

![image-20200823150923529](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0rhbfguwj30p80letni.jpg)

## 单聊

![image-20200823151006658](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0ri2alu2j310m0u0q7h.jpg)

## 群聊

![image-20200823151107140](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0rj3z66yj310h0u079i.jpg)

# 整体架构

![image-20200823150003464](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0r7pit01j310c0u0dnu.jpg)

# 流程图

## 用户登录

![image-20200823150112320](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0r8um8orj30yo0u0gv2.jpg)

## 用户登出

![image-20200823150118502](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0r8wnxd9j31bv0u0jw8.jpg)

## 单聊

![image-20200823150129814](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0r93klajj31dm0u0djc.jpg)

## 群聊

![image-20200823150138520](https://tva1.sinaimg.cn/large/007S8ZIlgy1gi0r98qbx7j31cr0u042m.jpg)

# 后端接口说明

## 请求登录

|           | Val   | Type                  |
| --------- | ----- | --------------------- |
| **cmd**   | login | string                |
| **data1** | user  | type User struct{...} |

```json

42[

   "login",

   {

​        "name": "user1",

​        "roomid": "room1",

​        "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​        "type": "user"

   }

]

```

## 登录成功

|           | Val   | Type                  |
| --------- | ----- | --------------------- |
| **cmd**   | login | string                |
| **data1** | user  | type User struct{...} |
| **data2** | Users | []User                |

```json

42[

   "success",

   [

​        {

​            "name": "user1",

​            "roomid": "room1",

​            "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​            "type": "user"

​        },

​        [

​            {

​                "name": "user1",

​                "roomid": "room1",

​                "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​                "type": "user"

​            },

​            {

​                "name": "user2",

​                "roomid": "room1",

​                "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​                "type": "user"

​            }

​        ]

   ]

]

```

## 登录失败

|           | Val  | Type   |
| --------- | ---- | ------ |
| **cmd**   | fail | string |
| **data1** | msg  | string |

```json

42[

​    "fail",

​    ["登录失败，昵称已经存在"]

]

```

## 进房间

|           | Val  | Type                  |
| --------- | ---- | --------------------- |
| **cmd**   | join | string                |
| **data1** | user | type User struct{...} |

```json

42[

   "join",

   {

​        "name": "user1",

​        "roomid": "room1",

​        "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​        "type": "user",

​        "address":"localhost:4396"

​        "id":"_6P40FV2ZxHhM_697jWV",

​        "login_time":"2020-08-21T10:43:06.962059+08:00"

   }

]

```

## 离开房间

|           | Val   | Type                  |
| --------- | ----- | --------------------- |
| **cmd**   | leave | string                |
| **data1** | user  | type User struct{...} |

```json

42[

   "leave",

   {

​        "name": "user1",

​        "roomid": "room1",

​        "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​        "type": "user",

​        "address":"localhost:4396"

​        "id":"_6P40FV2ZxHhM_697jWV",

​        "login_time":"2020-08-21T10:43:06.962059+08:00"

   }

]

```

## 发消息

|           | Val   | Type                  |
| --------- | ----- | --------------------- |
| **cmd**   | login | string                |
| **data1** | from  | type User struct{...} |
| **data2** | to    | type User struct{...} |
| **data3** | msg   | string                |

```json

42[

   "message",

   {

​        "name": "user1",

​        "roomid": "room1",

​        "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​        "type": "user",

​        "address":"localhost:4396"

​        "id":"_6P40FV2ZxHhM_697jWV",

​        "login_time":"2020-08-21T10:43:06.962059+08:00"

   },

   {

​        "name": "user1",

​        "roomid": "room1",

​        "avatarUrl":"http://q.qlogo.cn/headimg_dl?dst_uin=5684277&spec=100",

​        "type": "user",

​        "address":"localhost:4396"

​        "id":"_6P40FV2ZxHhM_697jWV",

​        "login_time":"2020-08-21T10:43:06.962059+08:00"

   },

   "hello world!"

]

```

## 备注

-   `42`中的`4`是`engine.io`的`message`类型标识，`2`是`socket.io`的`EVENT`类型标识
      - 参考资料：[socket.io 原理分析](https://www.jianshu.com/p/a3e06ec1a3a0#3)

-   详细的通信过程请通过 Chrome 的检查功能（`F12`）查阅

![image-20200918172031176](https://tva1.sinaimg.cn/large/007S8ZIlgy1giuxdsnlmrj30y50u0x2z.jpg)
