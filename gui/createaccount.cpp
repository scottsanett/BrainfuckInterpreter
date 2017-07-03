#include "createaccount.hpp"
#include "ui_createaccount.h"

#include <iostream>

CreateAccount::CreateAccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateAccount)
{
    ui->setupUi(this);
}

CreateAccount::~CreateAccount()
{
    delete ui;
}

void CreateAccount::on_buttonBox_accepted()
{
    username.clear(); password.clear();
    while (username.size() == 0 && password.size() == 0) {
        username = ui->TextUsername->text();
        password = ui->TextPassword->text();
    }
    emit signal_create_account_accepted(username, password);
}

void CreateAccount::on_pushButton_clicked()
{
    accept();
    auto login = new LoginWindow(parentWidget());
    QObject::connect(login, SIGNAL(signal_login_accepted(QString, QString)), this, SLOT(slot_login(QString, QString)));
    QObject::connect(login, SIGNAL(rejected()), this, SLOT(on_actionLogin_triggered()));
    login->setModal(true);
    login->setWindowTitle("Login to your account");
    login->show();
    parentWidget()->show();
    this->close();
}
