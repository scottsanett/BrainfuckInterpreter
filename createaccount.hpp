#ifndef CREATEACCOUNT_H
#define CREATEACCOUNT_H

#include <QDialog>

namespace Ui {
    class CreateAccount;
}

class CreateAccount : public QDialog
{
    Q_OBJECT

public:
    explicit CreateAccount(QWidget *parent = 0);
    ~CreateAccount();

signals:
    void signal_create_account_accepted(QString, QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateAccount *ui;
    QString username;
    QString password;
};

#endif // CREATEACCOUNT_H
