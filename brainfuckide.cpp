#include "brainfuckide.hpp"
#include "ui_brainfuckide.h"

BrainfuckIDE::BrainfuckIDE(QWidget *parent) :
    QMainWindow(parent),
    server(3333),
    save_file_dialog(new SaveFileDialog),
    ui(new Ui::BrainfuckIDE)
{
    server.start();
    ui->setupUi(this);

    /* initializing members*/
    is_saved = std::make_pair(filename, false);

    // setting up all the connections between the client_delegate and the ui class
    QObject::connect(&intermediary.delegate, SIGNAL(signal_load_result(QString)), this, SLOT(slot_load_result(QString)));
    QObject::connect(&intermediary.delegate, SIGNAL(signal_load_err_info(QString)), this, SLOT(slot_load_err_info(QString)));
    QObject::connect(&intermediary.delegate, SIGNAL(signal_open_file_path(QString)), this, SLOT(slot_load_file_path(QString)));
    QObject::connect(&intermediary.delegate, SIGNAL(signal_load_history_version(QString)), this, SLOT(slot_load_history_version(QString)));
}

BrainfuckIDE::~BrainfuckIDE()
{
    intermediary.close();
    server.stop();
    delete ui;
}

void BrainfuckIDE::slot_login(QString usr, QString pwd) {
    intermediary.on_login_clicked(usr.toStdString(), pwd.toStdString());
    intermediary.get_version();
}

void BrainfuckIDE::slot_create_account(QString usr, QString pwd) {
    intermediary.on_create_account_clicked(usr.toStdString(), pwd.toStdString());
}

void BrainfuckIDE::slot_load_result(QString result) {
    ui->Output->setPlainText(result);
}

void BrainfuckIDE::slot_load_err_info(QString error) {
    ui->statusBar->showMessage(error, 5000);
}

void BrainfuckIDE::slot_load_file_path(QString filepath) {
    QString url = QFileDialog::getExistingDirectory(this, tr("Open File"), filepath);
}

void BrainfuckIDE::slot_load_history_version(QString versions) {
    auto versions_str = versions.toStdString();
    std::string temp;
    std::istringstream iss(versions_str);
    while (iss >> temp) {
        ui->menuVersion->addAction(temp.c_str());
    }
}

void BrainfuckIDE::on_actionExit_triggered()
{
    this->close();
}

void BrainfuckIDE::on_actionExecute_triggered()
{
    auto code = ui->CodeEditor->toPlainText().toStdString();
    auto input = ui->InputEditor->toPlainText().toStdString();
    intermediary.on_run_clicked(code, input);
}

void BrainfuckIDE::on_actionLogin_triggered()
{
    QString username, password;
    auto login = new LoginWindow(this);
    login->setModal(true);
    login->setWindowTitle("Login");
    login->show();
    QObject::connect(login, SIGNAL(signal_login_accepted(QString, QString)), this, SLOT(slot_login(QString, QString)));
}

void BrainfuckIDE::on_actionCreate_new_account_triggered()
{
    QString username, password;
    auto create = new CreateAccount(this);
    create->setModal(true);
    create->setWindowTitle("Create new account");
    create->show();
    QObject::connect(create, SIGNAL(signal_create_account_accepted(QString, QString)), this, SLOT(slot_create_account(QString, QString)));
}

void BrainfuckIDE::on_actionOpen_triggered()
{
    intermediary.open_file();
}

void BrainfuckIDE::on_actionLogout_triggered()
{
    intermediary.on_logout_clicked();
    ui->menuVersion->clear();
}

void BrainfuckIDE::on_actionSave_triggered()
{
    code = ui->CodeEditor->toPlainText();
    auto flag = is_saved.second;
    if (flag) {
        intermediary.save_file(code.toStdString());
    }
    else {
        filename = save_file_dialog->getSaveFileName(this, tr("Save File"),
                                   "./untitled.bf",
                                   tr("Source files (*.bf *.ook)"));

    }
}

void BrainfuckIDE::on_actionSave_as_triggered()
{
    filename = save_file_dialog->getSaveFileName(this, tr("Save File"),
                               "./untitled.bf",
                               tr("Source files (*.bf *.ook)"));
}
