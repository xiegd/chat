#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

private slots:
    void on_return_btn_clicked();

    void on_varify_btn_clicked();

private:
    bool checkUserValid();
    bool checkPassValid();
    void showTip(QString str,bool b_ok);
    bool checkEmailValid();
    bool checkVarifyValid();
    void AddTipErr(TipErr te,QString tips);
    void DelTipErr(TipErr te);
    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;
signals:
    void switchLogin();
};

#endif // RESETDIALOG_H
