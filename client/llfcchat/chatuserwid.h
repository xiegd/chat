#ifndef CHATUSERWID_H
#define CHATUSERWID_H

#include <QWidget>
#include "listitembase.h"
namespace Ui {
class ChatUserWid;
}

class ChatUserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit ChatUserWid(QWidget *parent = nullptr);
    ~ChatUserWid();
    QSize sizeHint() const override;
    void SetInfo(int uid, QString name, QString head, QString msg);
    void ShowRedPoint(bool bshow);
private:
    Ui::ChatUserWid *ui;
    QString _name;
    QString _head;
    QString _msg;
    QString _uid;
};

#endif // CHATUSERWID_H
