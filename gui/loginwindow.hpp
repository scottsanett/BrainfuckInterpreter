#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "createaccount.hpp"

namespace Ui {
    class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

//    QString get_username() { return username; }
//    QString get_password() { return password; }

signals:
    void signal_login_accepted(QString, QString);


private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private:
    Ui::LoginWindow *ui;
    QString username;
    QString password;
};

#endif // LOGINWINDOW_H
