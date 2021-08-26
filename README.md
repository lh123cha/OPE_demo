# README

## 项目结构

```
├─Client 客户端
├─cmake-build-debug #编译文件不用管
│  ├─CMakeFiles
│  │  ├─3.17.5
│  │  │  ├─CompilerIdC
│  │  │  │  └─tmp
│  │  │  └─CompilerIdCXX
│  │  │      └─tmp
│  │  ├─BSTree_test.dir
│  │  │  ├─Client
│  │  │  ├─Server
│  │  │  └─Util
│  │  ├─CMakeTmp
│  │  ├─OPE_DEMO.dir
│  │  │  └─Server
│  │  └─ope_server.dir
│  │      ├─Server
│  │      └─Util
│  └─Testing
│      └─Temporary
├─Server#服务端
└─Util#加密工具
```

Server中BSTree为存储数据的基本结构，采用二叉树存储搜索树的数据。

ope_server.cpp为服务端处理客户端发来插入、搜索、范围搜索的程序。

Client中ope_client.cpp为客户端向服务器端发送请求的程序。



## 需要做的工作

修改客户端和服务器端的程序，完成多线模式，客户端线程发送请求到服务器线程，服务器线程处理请求。