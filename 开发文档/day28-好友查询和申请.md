## 简介

本文介绍如何实现用户查找和好友申请功能。查找和申请好友会涉及前后端通信和rpc服务间调用。所以目前先从客户端入手，搜索用户后发送查找好友申请请求给服务器，服务器收到后判断是否存在，如果不存在则显示未找到，如果存在则显示查找到的结果

## 点击查询
客户端点击搜索列表的添加好友item后，先弹出一个模态对话框，上面有loading动作表示加载，直到服务器返回结果
``` cpp
void SearchList::slot_item_clicked(QListWidgetItem *item)
{
    QWidget *widget = this->itemWidget(item); //获取自定义widget对象
    if(!widget){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    // 对自定义widget进行操作， 将item 转化为基类ListItemBase
    ListItemBase *customItem = qobject_cast<ListItemBase*>(widget);
    if(!customItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    auto itemType = customItem->GetItemType();
    if(itemType == ListItemType::INVALID_ITEM){
        qDebug()<< "slot invalid item clicked ";
        return;
    }

    if(itemType == ListItemType::ADD_USER_TIP_ITEM){
        if(_send_pending){
            return;
        }

        if (!_search_edit) {
            return;
        }

        waitPending(true);
        auto search_edit = dynamic_cast<CustomizeEdit*>(_search_edit);
        auto uid_str = search_edit->text();
        QJsonObject jsonObj;
        jsonObj["uid"] = uid_str;

        QJsonDocument doc(jsonObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ,
                                                  jsonData);
        return;
    }

    //清楚弹出框
    CloseFindDlg();

}
```
_send_pending为新增的成员变量，如果为true则表示发送阻塞.构造函数中将其设置为false。

waitPending函数为根据pending状态展示加载框
``` cpp
void SearchList::waitPending(bool pending)
{
    if(pending){
        _loadingDialog = new LoadingDlg(this);
        _loadingDialog->setModal(true);
        _loadingDialog->show();
        _send_pending = pending;
    }else{
        _loadingDialog->hide();
        _loadingDialog->deleteLater();
        _send_pending = pending;
    }
}
```
当我们发送数据后服务器会处理，返回ID_SEARCH_USER_RSP包，所以客户端要实现对ID_SEARCH_USER_RSP包的处理
``` cpp
    _handlers.insert(ID_SEARCH_USER_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        qDebug()<< "handle id is "<< id << " data is " << data;
        // 将QByteArray转换为QJsonDocument
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        // 检查转换是否成功
        if(jsonDoc.isNull()){
           qDebug() << "Failed to create QJsonDocument.";
           return;
        }

        QJsonObject jsonObj = jsonDoc.object();

        if(!jsonObj.contains("error")){
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Login Failed, err is Json Parse Err" << err ;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if(err != ErrorCodes::SUCCESS){
            qDebug() << "Login Failed, err is " << err ;
            emit sig_login_failed(err);
            return;
        }

      auto search_info = std::make_shared<SearchInfo>(jsonObj["uid"].toInt(),
              jsonObj["name"].toString(), jsonObj["nick"].toString(),
              jsonObj["desc"].toString(), jsonObj["sex"].toInt(), jsonObj["icon"].toString());

       emit sig_user_search(search_info);
    });

```
将搜索到的结果封装为search_info发送给SearchList类做展示, search_list中连接信号和槽
``` cpp
//连接搜索条目
connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_user_search, this, &SearchList::slot_user_search);
```
slot_user_search槽函数弹出搜索结果
``` cpp
void SearchList::slot_user_search(std::shared_ptr<SearchInfo> si)
{
    waitPending(false);
    if(si == nullptr){
        _find_dlg = std::make_shared<FindFailDlg>(this);
    }else{
        //此处分两种情况，一种是搜多到已经是自己的朋友了，一种是未添加好友
        //查找是否已经是好友 todo...
        _find_dlg = std::make_shared<FindSuccessDlg>(this);
        std::dynamic_pointer_cast<FindSuccessDlg>(_find_dlg)->SetSearchInfo(si);
    }

    _find_dlg->show();
}
```
FindSuccessDlg是找到的结果展示，FindFailDlg是未找到结果展示。以下为FindSuccessDlg的ui布局

![https://cdn.llfc.club/1722655438089.jpg](https://cdn.llfc.club/1722655438089.jpg)

具体声明如下
``` cpp
class FindSuccessDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDlg(QWidget *parent = nullptr);
    ~FindSuccessDlg();
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    Ui::FindSuccessDlg *ui;
    std::shared_ptr<SearchInfo> _si;
    QWidget * _parent;

private slots:
    void on_add_friend_btn_clicked();
};

```
具体实现如下
``` cpp
FindSuccessDlg::FindSuccessDlg(QWidget *parent) :
    QDialog(parent), _parent(parent),
    ui(new Ui::FindSuccessDlg)
{
    ui->setupUi(this);
    // 设置对话框标题
    setWindowTitle("添加");
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path +
                             QDir::separator() + "static"+QDir::separator()+"head_1.jpg");
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_lb->size(),
            Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->head_lb->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal","hover","press");
    this->setModal(true);
}

FindSuccessDlg::~FindSuccessDlg()
{
    qDebug()<<"FindSuccessDlg destruct";
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lb->setText(si->_name);
    _si = si;
}

void FindSuccessDlg::on_add_friend_btn_clicked()
{
    //todo... 添加好友界面弹出
    this->hide();
    //弹出加好友界面
    auto applyFriend = new ApplyFriend(_parent);
    applyFriend->SetSearchInfo(_si);
    applyFriend->setModal(true);
    applyFriend->show();
}
```
类似的FindFailDlg也是这种思路，大家自己实现即可。

## 服务器查询逻辑
chatserver服务器要根据客户端发送过来的用户id进行查找,chatserver服务器需先注册ID_SEARCH_USER_REQ和回调函数
``` cpp
void LogicSystem::RegisterCallBacks() {
	_fun_callbacks[MSG_CHAT_LOGIN] = std::bind(&LogicSystem::LoginHandler, this,
		placeholders::_1, placeholders::_2, placeholders::_3);

	_fun_callbacks[ID_SEARCH_USER_REQ] = std::bind(&LogicSystem::SearchInfo, this,
		placeholders::_1, placeholders::_2, placeholders::_3);
}
```
SearchInfo根据用户uid查询具体信息
``` cpp
void LogicSystem::SearchInfo(std::shared_ptr<CSession> session, const short& msg_id, const string& msg_data) {
	Json::Reader reader;
	Json::Value root;
	reader.parse(msg_data, root);
	auto uid_str = root["uid"].asString();
	std::cout << "user SearchInfo uid is  " << uid_str << endl;

	Json::Value rtvalue;

	Defer deder([this, &rtvalue, session]() {
		std::string return_str = rtvalue.toStyledString();
		session->Send(return_str, ID_SEARCH_USER_RSP);
		});

	bool b_digit = isPureDigit(uid_str);
	if (b_digit) {
		GetUserByUid(uid_str, rtvalue);
	}
	else {
		GetUserByName(uid_str, rtvalue);
	}
}
```
到此客户端和服务器搜索查询的联调功能已经解决了。