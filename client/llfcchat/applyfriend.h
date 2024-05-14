#ifndef APPLYFRIEND_H
#define APPLYFRIEND_H

#include <QDialog>

namespace Ui {
class ApplyFriend;
}

class ApplyFriend : public QDialog
{
    Q_OBJECT

public:
    explicit ApplyFriend(QWidget *parent = nullptr);
    ~ApplyFriend();
    void InitTestLbs();
    QPoint AddTestLbs(QString str, int x, int y);

private:
    Ui::ApplyFriend *ui;

public slots:
    void ShowMoreLabel();
};

#endif // APPLYFRIEND_H
