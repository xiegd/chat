#ifndef FRIENDINFOPAGE_H
#define FRIENDINFOPAGE_H

#include <QWidget>
#include "userdata.h"

namespace Ui {
class FriendInfoPage;
}

class FriendInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit FriendInfoPage(QWidget *parent = nullptr);
    ~FriendInfoPage();
    void SetInfo(std::shared_ptr<UserInfo> ui);
private:
    Ui::FriendInfoPage *ui;
    std::shared_ptr<UserInfo> _user_info;
};

#endif // FRIENDINFOPAGE_H
