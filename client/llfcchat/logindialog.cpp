#include "logindialog.h"
#include "ui_logindialog.h"
#include <QDebug>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    ui->forget_label->SetState("normal","hover","","selected","selected_hover","");
    ui->forget_label->setCursor(Qt::PointingHandCursor);
}

LoginDialog::~LoginDialog()
{
    qDebug()<<"destruct LoginDlg";
    delete ui;
}
