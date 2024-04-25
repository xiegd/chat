#include "chatuserlist.h"
#include<QScrollBar>

ChatUserList::ChatUserList(QWidget *parent)
{
     this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
     this->viewport()->installEventFilter(this);

    QScrollBar *verticalScrollBar =  this->verticalScrollBar();
    verticalScrollBar->setSingleStep(5); // 设置垂直滚动幅度为20
}
