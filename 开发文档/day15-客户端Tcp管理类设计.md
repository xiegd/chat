## 客户端TCP管理者
因为聊天服务要维持一个长链接，方便服务器和客户端双向通信，那么就需要一个TCPMgr来管理TCP连接。

而实际开发中网络模块一般以单例模式使用，那我们就基于单例基类和可被分享类创建一个自定义的TcpMgr类，在QT工程中新建TcpMgr类，会生成头文件和源文件，头文件修改如下

``` cpp
#ifndef TCPMGR_H
#define TCPMGR_H
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
class TcpMgr:public QObject, public Singleton<TcpMgr>,
        public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
public:
    TcpMgr();
private:
    QTcpSocket _socket;
    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
public slots:
    void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QString data);
signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QString data);
};

#endif // TCPMGR_H
```


