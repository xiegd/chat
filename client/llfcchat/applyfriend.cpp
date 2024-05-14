#include "applyfriend.h"
#include "ui_applyfriend.h"
#include "clickedlabel.h"

ApplyFriend::ApplyFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplyFriend)
{
    ui->setupUi(this);
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->name_ed->setPlaceholderText(tr("恋恋风辰"));
    connect(ui->more_lb, &ClickedOnceLabel::clicked, this, &ApplyFriend::ShowMoreLabel);
    InitTestLbs();
}

ApplyFriend::~ApplyFriend()
{
    delete ui;
}

void ApplyFriend::InitTestLbs()
{
    QList<QString> lables = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                             "父与子学Python","nodejs开发指南","go 语言开发指南",
                                "游戏伙伴","金融投资","微信读书","拼多多拼友"};

    int max_width = 267;
    int max_height = 67;
    int pos_x = 5;
    int pos_y = 5;
    int next_pos_y;
    for(int i = 0; i < lables.size(); i++){
        if(pos_x > max_width){
            pos_x =5;
            pos_y = next_pos_y;
        }
        auto point = AddTestLbs(lables[i],pos_x,pos_y);
        pos_x = point.x();
        next_pos_y = point.y();
    }

}

QPoint ApplyFriend::AddTestLbs(QString str, int x, int y)
{
    auto *lb = new ClickedLabel(ui->lb_list);
    lb->SetState("normal","hover","pressed","selected_normal",
                 "selected_hover","selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(str);
    lb->move(x,y);

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度

    qDebug()<< "width is " << textWidth ;
    QPoint point(lb->pos().x() + textWidth + 15,lb->pos().y()+ textHeight+15);
   return point;
}

void ApplyFriend::ShowMoreLabel()
{

}
