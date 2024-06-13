#include "authenfriend.h"
#include "ui_authenfriend.h"

AuthenFriend::AuthenFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthenFriend)
{
    ui->setupUi(this);
}

AuthenFriend::~AuthenFriend()
{
    delete ui;
}
