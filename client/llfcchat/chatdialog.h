#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QList>
#include "statelabel.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();
private:
    void AddLBGroup(StateLabel* lb);
    void addChatUserList();
    void ClearLabelState(StateLabel* lb);
    Ui::ChatDialog *ui;
    bool _b_loading;
    QList<StateLabel*> _lb_list;

public slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
private slots:
    void on_send_btn_clicked();
};



#endif // CHATDIALOG_H
