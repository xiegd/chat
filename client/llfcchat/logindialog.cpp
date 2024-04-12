#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
#include "httpmgr.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    ui->forget_label->setCursor(Qt::PointingHandCursor);
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_pwd);
}

LoginDialog::~LoginDialog()
{
    qDebug()<<"destruct LoginDlg";
    delete ui;
}

void LoginDialog::slot_forget_pwd()
{
    qDebug()<<"slot forget pwd";
    emit switchReset();
}

bool LoginDialog::checkUserValid(){

    auto user = ui->user_edit->text();
    if(user.isEmpty()){
        qDebug() << "User empty " ;
        return false;
    }

    return true;
}

bool LoginDialog::checkPwdValid(){
    auto pwd = ui->pass_edit->text();
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "Pass length invalid";
        return false;
    }

    return true;
}

void LoginDialog::on_login_btn_clicked()
{
    qDebug()<<"login btn clicked";
    if(checkUserValid() == false){
        return;
    }

    if(checkPwdValid() == false){
        return ;
    }

    auto user = ui->user_edit->text();
    auto pwd = ui->pass_edit->text();
    //发送http请求获取验证码
    QJsonObject json_obj;
    json_obj["user"] = user;
    json_obj["pwd"] = pwd;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                 json_obj, ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}
