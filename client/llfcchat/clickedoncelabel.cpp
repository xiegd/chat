#include "clickedoncelabel.h"

ClickedOnceLabel::ClickedOnceLabel(QWidget *parent):QLabel(parent)
{
    setCursor(Qt::PointingHandCursor);
}


void ClickedOnceLabel::mouseReleaseEvent(QMouseEvent *event)
{
      QLabel::leaveEvent(event);
      // 调用基类的mousePressEvent以保证正常的事件处理
      QLabel::mousePressEvent(event);
}

