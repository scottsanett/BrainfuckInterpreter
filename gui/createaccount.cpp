#include "createaccount.hpp"
#include "ui_createaccount.h"

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
    username = ui->TextUsername->text();
    password = ui->TextPassword->text();
    emit signal_create_account_accepted(username, password);
}

void CreateAccount::on_pushButton_clicked()
{
    accept();
    auto login = new LoginWindow(parentWidget());
    login->setModal(true);
    login->setWindowTitle("Login to your account");
    login->show();
    parentWidget()->show();
    this->close();
}
