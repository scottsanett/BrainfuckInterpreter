#include "loginwindow.hpp"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_buttonBox_accepted()
{
    username = ui->TextUsername->text();
    password = ui->TextPassword->text();
    emit signal_login_accepted(username, password);
}
