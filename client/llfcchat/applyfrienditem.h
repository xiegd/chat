#ifndef APPLYFRIENDITEM_H
#define APPLYFRIENDITEM_H

#include <QWidget>
#include <listitembase.h>

namespace Ui {
class ApplyFriendItem;
}

class ApplyFriendItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit ApplyFriendItem(QWidget *parent = nullptr);
    ~ApplyFriendItem();
    void SetInfo(int uid, QString name, QString head, QString msg);
    void ShowAddBtn(bool bshow);
private:
    Ui::ApplyFriendItem *ui;
    QString _name;
    QString _head;
    QString _msg;
    int _uid;
signals:
    void sig_auth_friend(int uid);
};

#endif // APPLYFRIENDITEM_H
