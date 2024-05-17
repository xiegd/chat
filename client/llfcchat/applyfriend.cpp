#include "applyfriend.h"
#include "ui_applyfriend.h"
#include "clickedlabel.h"
#include "friendlabel.h"

ApplyFriend::ApplyFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplyFriend),_label_point(2,6)
{
    ui->setupUi(this);
    // 隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("ApplyFriend");
    this->setModal(true);
    ui->name_ed->setPlaceholderText(tr("恋恋风辰"));
    ui->lb_ed->setPlaceholderText("搜索、添加标签");
    ui->back_ed->setPlaceholderText("燃烧的胸毛");
    connect(ui->more_lb, &ClickedOnceLabel::clicked, this, &ApplyFriend::ShowMoreLabel);
    InitTestLbs();
    //链接输入标签回车事件
    connect(ui->lb_ed, &CustomizeEdit::returnPressed, this, &ApplyFriend::SlotLabelEnter);

    ui->lb_ed->move(2,2);
    ui->lb_ed->setFixedHeight(20);
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

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.width(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度

    if(cur_point.x() +textWidth > ui->lb_list->width()){
        cur_point.setX(5);
        cur_point.setY(cur_point.y()+textHeight+15);
    }

    lb->move(cur_point);
    lb->show();
    _add_labels.insert(str, lb);
    _add_label_keys.push_back(str);

    if(cur_point.x() +textWidth > ui->lb_list->width()){
        next_point.setY(lb->pos().y()+ textHeight+15);
        next_point.setX(5);
        return true;
    }

    next_point.setX(lb->pos().x() + textWidth + 15);
    next_point.setY(lb->pos().y());

   return false;
}



void ApplyFriend::ShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->more_lb_wid->hide();

    QList<QString> lables = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                             "父与子学Python","nodejs开发指南","go 语言开发指南",
                                "游戏伙伴","金融投资","微信读书","拼多多拼友"};

    ui->lb_list->setFixedWidth(325);
    QPoint cur_point(5,5);
    auto next_point = cur_point;
    int textWidth;
    int textHeight;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.width(added_lb->text()); // 获取文本的宽度
        textHeight = fontMetrics.height(); // 获取文本的高度

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
        next_point.setY(cur_point.y());
        cur_point = next_point;

    }

    //添加未添加的
    for(int i = 0; i < lables.size(); i++){
        auto iter = _add_labels.find(lables[i]);
        if(iter != _add_labels.end()){
            continue;
        }

        bool line_break = AddTestLbs(lables[i],cur_point,next_point);
        cur_point = next_point;
    }

   ui->lb_list->setFixedSize(QSize(325, next_point.y()+textHeight+5));

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();

}

void ApplyFriend::SlotLabelEnter()
{
    if(ui->lb_ed->text().isEmpty()){
        return;
    }

    auto tmplabel = new FriendLabel(ui->gridWidget);
    tmplabel->SetText(ui->lb_ed->text());
    tmplabel->setObjectName("FriendLabel");

    auto max_width = ui->gridWidget->width();
    //todo... 添加宽度统计
    if( _label_point.x() + tmplabel->width() > max_width) {
        _label_point.setY(_label_point.y()+tmplabel->height()+6);
        _label_point.setX(2);
    }else{

    }


    tmplabel->move(_label_point);
    tmplabel->show();
    _friend_labels[tmplabel->Text()] = tmplabel;
    _friend_label_keys.push_back(tmplabel->Text());

    connect(tmplabel, &FriendLabel::sig_close, this, &ApplyFriend::SlotRemoveFriendLabel);

     _label_point.setX(_label_point.x() + tmplabel->width()+2);

    if(_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->gridWidget->width()){
        ui->lb_ed->move(2,_label_point.y()+tmplabel->height()+2);
    }else{
        ui->lb_ed->move(_label_point);
    }

    ui->lb_ed->clear();

    if(ui->gridWidget->height() < _label_point.y()+tmplabel->height()+2){
         ui->gridWidget->setFixedHeight(_label_point.y()+tmplabel->height()*2+2);
    }

}

void ApplyFriend::SlotRemoveFriendLabel(QString name)
{
    qDebug() << "receive close signal";
   auto iter =  _friend_labels.find(name);
   if(iter == _friend_labels.end()){
        return;
   }

   auto old_rect = iter.value()->geometry();

   if(_friend_label_keys.size() == 1){
       delete  iter.value();
       _friend_labels.erase(iter);
       _friend_label_keys.clear();
       ui->lb_ed->move(old_rect.x(), old_rect.y());
       return;
   }

   auto find_index = -1;
   for(int i = 0; i < _friend_label_keys.size(); i++){
       if(_friend_label_keys[i] == name){
            find_index = i;
       }
   }

   if(find_index == -1){
       return;
   }

  auto last =  _friend_label_keys.back();
  auto last_iter = _friend_labels.find(last);
  if(last_iter == _friend_labels.end()){
    return;
  }

  auto last_rect = last_iter.value()->geometry();

  last_iter.value()->move(old_rect.x(), old_rect.y());

  _friend_label_keys[find_index] = last;

  _friend_label_keys.pop_back();

   delete  iter.value();

  _friend_labels.erase(iter);

  ui->lb_ed->move(last_rect.x(), last_rect.y());

}
