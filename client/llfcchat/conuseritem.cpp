#include "conuseritem.h"
#include "ui_conuseritem.h"

ConUserItem::ConUserItem(QWidget *parent) :
    ListItemBase(parent),
    ui(new Ui::ConUserItem)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CONTACT_USER_ITEM);
}

ConUserItem::~ConUserItem()
{
    delete ui;
}

QSize ConUserItem::sizeHint() const
{
    return QSize(250, 70); // 返回自定义的尺寸
}

void ConUserItem::SetInfo(QString name, QString head)
{
    _head = head;
    _name = name;

    // 加载图片
    ui->icon_lb->SetPixmap(head);
    ui->user_name_lb->setText(_name);

}
