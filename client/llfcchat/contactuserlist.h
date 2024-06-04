#ifndef CONTACTUSERLIST_H
#define CONTACTUSERLIST_H
#include <QListWidget>
#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDebug>

class ContactUserList : public QListWidget
{
    Q_OBJECT
public:
    ContactUserList(QWidget* parent = nullptr);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override ;

public slots:
     void slot_item_clicked(QListWidgetItem *item);
signals:
    void sig_loading_contact_user();
};

#endif // CONTACTUSERLIST_H
