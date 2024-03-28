## 注册功能
实现注册功能，先实现客户端发送post请求, 将注册ui中确定按钮改为sure_btn，并为其添加click槽函数
``` cpp
//day11 添加确认槽函数
void RegisterDialog::on_sure_btn_clicked()
{
    if(ui->user_edit->text() == ""){
        showTip(tr("用户名不能为空"), false);
        return;
    }

    if(ui->email_edit->text() == ""){
        showTip(tr("邮箱不能为空"), false);
        return;
    }

    if(ui->pass_edit->text() == ""){
        showTip(tr("密码不能为空"), false);
        return;
    }

    if(ui->confirm_edit->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }

    if(ui->confirm_edit->text() != ui->pass_edit->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }

    if(ui->varify_edit->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }

    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->pass_edit->text();
    json_obj["confirm"] = ui->confirm_edit->text();
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                 json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}
```
再添加http请求回复后收到处理流程
``` cpp
void RegisterDialog::initHttpHandlers()
{
    //...省略
    //注册注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug()<< "email is " << email ;
    });
}
```
## Server端接受注册请求
Server注册user_register逻辑
``` cpp
RegPost("/user_register", [](std::shared_ptr<HttpConnection> connection) {
    auto body_str = boost::beast::buffers_to_string(connection->_request.body().data());
    std::cout << "receive body is " << body_str << std::endl;
    connection->_response.set(http::field::content_type, "text/json");
    Json::Value root;
    Json::Reader reader;
    Json::Value src_root;
    bool parse_success = reader.parse(body_str, src_root);
    if (!parse_success) {
        std::cout << "Failed to parse JSON data!" << std::endl;
        root["error"] = ErrorCodes::Error_Json;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }
    //先查找redis中email对应的验证码是否合理
    std::string  varify_code;
    bool b_get_varify = RedisMgr::GetInstance()->Get(src_root["email"].asString(), varify_code);
    if (!b_get_varify) {
        std::cout << " get varify code expired" << std::endl;
        root["error"] = ErrorCodes::VarifyExpired;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    if (varify_code != src_root["varifycode"].asString()) {
        std::cout << " varify code error" << std::endl;
        root["error"] = ErrorCodes::VarifyCodeErr;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    //访问redis查找
    bool b_usr_exist = RedisMgr::GetInstance()->ExistsKey(src_root["user"].asString());
    if (b_usr_exist) {
        std::cout << " user exist" << std::endl;
        root["error"] = ErrorCodes::UserExist;
        std::string jsonstr = root.toStyledString();
        beast::ostream(connection->_response.body()) << jsonstr;
        return true;
    }

    //查找数据库判断用户是否存在

    root["error"] = 0;
    root["email"] = src_root["email"];
    root ["user"]= src_root["user"].asString();
    root["passwd"] = src_root["passwd"].asString();
    root["confirm"] = src_root["confirm"].asString();
    root["varifycode"] = src_root["varifycode"].asString();
    std::string jsonstr = root.toStyledString();
    beast::ostream(connection->_response.body()) << jsonstr;
    return true;
    });
```
## 安装Mysql
先介绍Windows环境下安装mysql

点击mysql安装包下载链接：[https://dev.mysql.com/downloads/mysql](https://dev.mysql.com/downloads/mysql/)

选择window版本，点击下载按钮，如下所示

![https://cdn.llfc.club/4aa44fdafe578d8f2626d3e280d608f.png](https://cdn.llfc.club/4aa44fdafe578d8f2626d3e280d608f.png)

不用登录直接下载

![https://cdn.llfc.club/1711349001944.jpg](https://cdn.llfc.club/1711349001944.jpg)

下载好mysql安装包后，将其解压到指定目录，并记下解压的目录，后续用于环境变量配置

![https://cdn.llfc.club/1711349518362.jpg](https://cdn.llfc.club/1711349518362.jpg)

在bin目录同级下创建一个文件，命名为my.ini
编辑my.ini文件
``` ini
[mysqld]
# 设置3308端口
port=3308
# 设置mysql的安装目录 ---这里输入你安装的文件路径----
basedir=D:\cppsoft\mysql
# 设置mysql数据库的数据的存放目录
datadir=D:\mysql\data
# 允许最大连接数
max_connections=200
# 允许连接失败的次数。
max_connect_errors=10
# 服务端使用的字符集默认为utf8
character-set-server=utf8
# 创建新表时将使用的默认存储引擎
default-storage-engine=INNODB
# 默认使用“mysql_native_password”插件认证
#mysql_native_password
default_authentication_plugin=mysql_native_password
[mysql]
# 设置mysql客户端默认字符集
default-character-set=utf8
[client]
# 设置mysql客户端连接服务端时默认使用的端口
port=3308
default-character-set=utf8
```
有两点需要注意修改的：

A、basedir这里输入的是mysql解压存放的文件路径

B、datadir这里设置mysql数据库的数据存放目录

打开cmd进入mysql的bin文件下

![https://cdn.llfc.club/1711349826275.jpg](https://cdn.llfc.club/1711349826275.jpg)

依次执行命令

第一个命令为：

``` cpp
//安装mysql  安装完成后Mysql会有一个随机密码
.\mysqld.exe --initialize --console
```
如下图，随机密码要记住，以后我们改密码会用到

![https://cdn.llfc.club/83635680847f591980ade3501655f8d.png](https://cdn.llfc.club/83635680847f591980ade3501655f8d.png)

接下来在cmd执行第二条命令

``` cpp
//安装mysql服务并启动   
.\mysqld.exe --install mysql
```

如果出现以下情况，说明cmd不是以管理员形式执行的，改用为管理员权限执行即可。

![https://cdn.llfc.club/2872369cb66fa7803e19575be3cd63b.png](https://cdn.llfc.club/2872369cb66fa7803e19575be3cd63b.png)

成功如下

![https://cdn.llfc.club/87a224f42f4dccb254481470d2f1b8e.png](https://cdn.llfc.club/87a224f42f4dccb254481470d2f1b8e.png)

目前为止安装完毕，大家如果mysql官网下载缓慢，可以去我的网盘下载

[https://pan.baidu.com/s/1BTMZB31FWFUq4mZZdzcA9g?pwd=6xlz](https://pan.baidu.com/s/1BTMZB31FWFUq4mZZdzcA9g?pwd=6xlz)

提取码：6xlz 

## 修改mysql密码

1  在本机启动mysql服务：

点击桌面我的电脑，右键选择管理进去：

![https://cdn.llfc.club/1711350803255.jpg](https://cdn.llfc.club/1711350803255.jpg)

点击后选择服务

![https://cdn.llfc.club/1711350871137.jpg](https://cdn.llfc.club/1711350871137.jpg)

点击服务后可查看当前计算机启动的所有服务，找到mysql，然后右键点击设为启动，同时也可设置其为自动启动和手动启动

![https://cdn.llfc.club/1711350989964.jpg](https://cdn.llfc.club/1711350989964.jpg)

继续在cmd上执行以下命令

``` bash
mysql -uroot -p
```

回车后输入上面安装时保存的初始密码，进入mysql里面：

![https://cdn.llfc.club/b33134d93210412a6d301c9eedfa8a5.png](https://cdn.llfc.club/b33134d93210412a6d301c9eedfa8a5.png)

在mysql里面继续执行以下命令：

``` cpp
//修改密码为123mysql
ALTER USER 'root'@'localhost' IDENTIFIED BY '123456';
```

回车按照指引执行完后，代表密码修改成功，再输入exit;退出即可

## 配置环境变量

为了方便使用mysql命令，可以将mysql目录配置在环境变量里

新建系统变量：

变量名：MYSQL_HOME

变量值：msql目录

![https://cdn.llfc.club/1711352568377.jpg](https://cdn.llfc.club/1711352568377.jpg)

修改系统的path变量

编辑path，进去后添加 %MYSQL_HOME%\bin

![https://cdn.llfc.club/1711352718673.jpg](https://cdn.llfc.club/1711352718673.jpg)

## 测试连接

为了方便测试，大家可以使用navicat等桌面工具测试连接。以后增删改查也方便。

可以去官网下载

[https://www.navicat.com.cn/](https://www.navicat.com.cn/)

或者我得网盘下载

[https://pan.baidu.com/s/10jApYUrwaI19j345dpPGNA?pwd=77m2](https://pan.baidu.com/s/10jApYUrwaI19j345dpPGNA?pwd=77m2)

验证码： 77m2

效果如下：

![https://cdn.llfc.club/1711531330919.jpg](https://cdn.llfc.club/1711531330919.jpg)

## Docker环境配置mysql

拉取mysql镜像

``` bash
docker pull mysql:8.0
```
先启动一个测试版本，然后把他的配置文件拷贝出来
``` bash
docker run --name mysqltest \
-p 3307:3306 -e MYSQL_ROOT_PASSWORD=root \
-d mysql
```
创建三个目录，我得目录是
``` bash
mkdir -p /home/zack/llfc/mysql/config
mkdir -p /home/zack/llfc/mysql/data
mkdir -p /home/zack/llfc/mysql/logs
```

进入docker中
``` bash
docker exec -it mysqltest bash
```
之后可以通过搜寻找到配置在`/etc/mysql/my.cnf`

所以接下来退出容器，执行拷贝命令
``` bash
docker cp mysqltest:/etc/mysql/my.cnf  /home/zack/llfc/mysql/config
```
然后删除测试用的mysql docker
``` bash
docker rm -f mysqltest 
```
然后启动我们的容器
``` bash
docker run --restart=on-failure:3 -d   \
-v /home/zack/llfc/mysql/config/my.cnf:/etc/mysql/my.cnf \
-v /home/zack/llfc/mysql/data/:/var/lib/mysql \
-v /home/zack/llfc/mysql/logs:/logs -p 3308:3306  \
--name llfcmysql -e MYSQL_ROOT_PASSWORD=123456 mysql:8.0
```
## 设置远程访问

进入docker 
``` bash
docker exec -it llfcmysql bash
```
登录mysql
``` bash
mysql -u root -p
```
设置允许远程访问，我不设置也能访问的，这里介绍一下。
``` bash
use mysql
ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
flush privileges;
```

再次用navicat连接，是可以连接上了。

## 完善GateServer配置
添加Redis和Mysql配置
``` ini
[Mysql]
Host = 81.68.86.146
Port = 3308
Passwd = 123456
[Redis]
Host = 81.68.86.146
Port = 6380
Passwd = 123456
```
## 封装mysql接口





