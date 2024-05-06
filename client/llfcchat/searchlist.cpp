#include "searchlist.h"
#include<QScrollBar>

SearchList::SearchList(QWidget *parent):QListWidget(parent)
{
    Q_UNUSED(parent);
     this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
     this->viewport()->installEventFilter(this);
}
