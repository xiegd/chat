## 邮箱验证服务联调

我们启动GateServer和VarifyServer

我们启动客户端，点击注册按钮进入注册界面，输入邮箱并且点击获取验证码

![https://cdn.llfc.club/1710646053282.jpg](https://cdn.llfc.club/1710646053282.jpg)

GateServer收到Client发送的请求后，会调用grpc 服务 访问VarifyServer，VarifyServer会随机生成验证码，并且调用邮箱模块发送邮件给指定邮箱。而且把发送的结果给GateServer，GateServer再将消息回传给客户端。

## 设置验证码过期

我们的验证码是要设置过期的，可以用redis管理过期的验证码自动删除，key为邮箱，value为验证码，过期时间为3min。

## windows 安装redis服务

windows 版本下载地址:

[https://github.com/tporadowski/redis/releases](https://github.com/tporadowski/redis/releases)

下载速度慢可以去我的网盘

链接: https://pan.baidu.com/s/1v_foHZLvBeJQMePSGnp4Ow?pwd=yid3 提取码: yid3 

下载完成后解压

![https://cdn.llfc.club/1710649614458.jpg](https://cdn.llfc.club/1710649614458.jpg)

修改redis.windows.conf, 并且修改端口
``` cpp
port 6380
```
找到requirepass foobared，下面添加requirepass
``` cpp
# requirepass foobared
requirepass 123456
```
启动redis 服务器` .\redis-server.exe .\redis.windows.conf`

![https://cdn.llfc.club/1710649945760.jpg](https://cdn.llfc.club/1710649945760.jpg)

启动客户端 `.\redis-cli.exe -p 6380`, 输入密码登录成功

![https://cdn.llfc.club/1710650063208.jpg](https://cdn.llfc.club/1710650063208.jpg)

## Linux 安装redis服务

Linux安装容器后，直接用容器启动redis
``` bash
docker run -d --name llfc-redis -p 6380:6379 redis  --requirepass "123456"
```
为了方便测试能否链接以及以后查看数据,大家可以下载redis desktop manager

官网链接
[redisdesktop.com/](redisdesktop.com/)

下载速度慢可以去我的网盘

链接: https://pan.baidu.com/s/1v_foHZLvBeJQMePSGnp4Ow?pwd=yid3 提取码: yid3 

下载后安装

设置好ip和密码，点击测试连接连通就成功了

![https://cdn.llfc.club/1710657223612.jpg](https://cdn.llfc.club/1710657223612.jpg)

## widows编译和配置redis

Linux的redis库直接编译安装即可，windows反而麻烦一些，我们先阐述windows环境如何配置redis库， C++ 的redis库有很多种，最常用的有hredis和redis-plus-plus. 我们用redis-plus-plus. 这里介绍一种简单的安装方式---vcpkg

先安装vcpkg, 源码地址

[https://github.com/microsoft/vcpkg/releases](https://github.com/microsoft/vcpkg/releases)

下载源码后

windows版本redis下载地址

[https://github.com/microsoftarchive/redis](https://github.com/microsoftarchive/redis)

因为是源码，所以进入msvc目录

![https://cdn.llfc.club/1710725726234.jpg](https://cdn.llfc.club/1710725726234.jpg)

用visual studio打开sln文件，弹出升级窗口, 我的是vs2019所以升级到142

![https://cdn.llfc.club/1710725937787.jpg](https://cdn.llfc.club/1710725937787.jpg)

只需要生成hiredis工程和Win32_Interop工程即可，分别点击生成,生成hiredis.lib和Win32_Interop.lib即可

右键两个工程的属性，代码生成里选择运行时库加载模式为MDD(Debug模式动态运行加载)，为了兼容我们其他的库，其他的库也是MDD模式

![https://cdn.llfc.club/1710726777016.jpg](https://cdn.llfc.club/1710726777016.jpg)

编译Win32_Interop.lib时报错， system_error不是std成员，

![https://cdn.llfc.club/1710727129177.jpg](https://cdn.llfc.club/1710727129177.jpg)

解决办法为在Win32_variadicFunctor.cpp和Win32_FDAPI.cpp添加
`#include <system_error>`,再右键生成成功

![https://cdn.llfc.club/1710729372811.jpg](https://cdn.llfc.club/1710729372811.jpg)

将hiredis.lib和Win32_Interop.lib拷贝到`D:\cppsoft\reids\lib`

将`redis-3.0\deps\hiredis`文件夹拷贝到`D:\cppsoft\reids\include`

然后我们在visual studio中配置