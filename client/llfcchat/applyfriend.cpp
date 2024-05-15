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

    int max_width = ui->lb_list->width();
    QPoint cur_point(5,5);
    auto next_point = cur_point;
    int lines = 1;
    for(int i = 0; i < lables.size(); i++){

       bool line_break = AddTestLbs(lables[i],cur_point,next_point);
       if(line_break){
            lines++;
       }

       if(lines >= 3){
            break;
       }

       cur_point = next_point;
    }

}

bool ApplyFriend::AddTestLbs(QString str, QPoint cur_point, QPoint &next_point)
{
    auto *lb = new ClickedLabel(ui->lb_list);
    lb->SetState("normal","hover","pressed","selected_normal",
                 "selected_hover","selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(str);
    lb->move(cur_point);
    _add_labels.insert(str, lb);
    _add_label_keys.push_back(str);

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度

    if(cur_point.x() +textWidth > ui->lb_list->width()){
        next_point.setY(lb->pos().y()+ textHeight+15);
        next_point.setX(5);
        return true;
    }

    next_point.setX(lb->pos().x() + textWidth + 15);

   return false;
}



void ApplyFriend::ShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->more_lb_wid->hide();

    QList<QString> lables = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                             "父与子学Python","nodejs开发指南","go 语言开发指南",
                                "游戏伙伴","金融投资","微信读书","拼多多拼友"};

    int max_width = ui->lb_list->width();
    QPoint cur_point(5,5);
    auto next_point = cur_point;
    int textWidth;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.width(added_lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if(cur_point.x() +textWidth > ui->lb_list->width()){
            cur_point.setX(5);
            cur_point.setY(cur_point.y()+textHeight+15);
        }
        added_lb->move(cur_point);


        if(cur_point.x() +textWidth > ui->lb_list->width()){
            next_point.setY(added_lb->pos().y()+ textHeight+15);
            next_point.setX(5);
            cur_point = next_point;
            continue;
        }

        next_point.setX(added_lb->pos().x() + textWidth + 15);
        cur_point = next_point;

    }

//    auto temp = QPoint(100,105);
//    auto *lb = new ClickedLabel(ui->lb_list);
//    lb->SetState("normal","hover","pressed","selected_normal",
//                 "selected_hover","selected_pressed");
//    lb->setObjectName("tipslb");
//    lb->setText("str111111111111111111111");
//    lb->move(QPoint(5,5));
    //添加未添加的
    for(int i = 0; i < lables.size(); i++){
        auto iter = _add_labels.find(lables[i]);
        if(iter != _add_labels.end()){
            continue;
        }

        bool line_break = AddTestLbs(lables[i],cur_point,next_point);
        cur_point = next_point;
    }

   ui->lb_list->setFixedSize(QSize(ui->lb_list->width(), next_point.y()+textWidth+5));

}
