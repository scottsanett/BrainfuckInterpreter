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
