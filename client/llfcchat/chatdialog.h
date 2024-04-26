#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>

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
    void addChatUserList();
    Ui::ChatDialog *ui;
    bool _b_loading;

public slots:
    void slot_loading_chat_user();

};

#endif // CHATDIALOG_H
