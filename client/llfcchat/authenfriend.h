#ifndef AUTHENFRIEND_H
#define AUTHENFRIEND_H

#include <QDialog>

namespace Ui {
class AuthenFriend;
}

class AuthenFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AuthenFriend(QWidget *parent = nullptr);
    ~AuthenFriend();

private:
    Ui::AuthenFriend *ui;
};

#endif // AUTHENFRIEND_H
