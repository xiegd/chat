#include "applyfrienditem.h"
#include "ui_applyfrienditem.h"

ApplyFriendItem::ApplyFriendItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ApplyFriendItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::APPLY_FRIEND_ITEM);
    ui->addBtn->SetState("normal","hover", "press");
    ui->addBtn->hide();
    connect(ui->addBtn, &ClickedBtn::clicked,  [this](){
        emit this->sig_auth_friend(_uid);
    });
}

ApplyFriendItem::~ApplyFriendItem()
{
    delete ui;
}

void ApplyFriendItem::SetInfo(int uid , QString name, QString head, QString msg)
{
    _name = name;
    _head = head;
    _msg = msg;
    _uid = uid;
    // 加载图片
    QPixmap pixmap(_head);

    // 设置图片自动缩放
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->user_name_lb->setText(_name);
    ui->user_chat_lb->setText(_msg);
}

void ApplyFriendItem::ShowAddBtn(bool bshow)
{
    if (bshow) {
		ui->addBtn->show();
		ui->already_add_lb->hide();
    }
    else {
		ui->addBtn->hide();
		ui->already_add_lb->show();
    }
}



