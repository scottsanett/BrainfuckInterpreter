#include "brainfuckide.hpp"
#include "ui_brainfuckide.h"

scott::Server BrainfuckIDE::server(3333);
scott::client::Intermediary BrainfuckIDE::intermediary;
scott::server::Delegate BrainfuckIDE::server_delegate;

BrainfuckIDE::BrainfuckIDE(QWidget *parent) :
    QMainWindow(parent),
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
    QObject::connect(&intermediary.delegate, SIGNAL(signal_load_history_code(QString)), this, SLOT(slot_load_history_code(QString)));
}

BrainfuckIDE::~BrainfuckIDE()
{
    intermediary.close();
    server.stop();
    delete ui;
}

void BrainfuckIDE::slot_login(QString usr, QString pwd) {
    intermediary.on_login_clicked(usr.toStdString(), pwd.toStdString());
    ui->menuVersion->clear();
    ui->menuVersion->addAction("Version control");
    ui->menuVersion->addSeparator();
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
    std::string key;
    std::istringstream iss(versions_str);
    while (iss >> key) {
        auto current_action = new MenuAction(key.c_str());
        ui->menuVersion->addAction(current_action);
        QObject::connect(current_action, SIGNAL(triggered(bool)), current_action, SLOT(slot_trigger(bool)));
        QObject::connect(current_action, SIGNAL(get_history_key(QString)), this, SLOT(slot_get_histoy_key(QString)));
    }
}

void BrainfuckIDE::slot_get_histoy_key(QString key) {
    intermediary.choose_version(key.toStdString());
}

void BrainfuckIDE::slot_load_history_code(QString content) {
    ui->CodeEditor->setPlainText(content);
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
    auto login = new LoginWindow(this);
    login->setModal(true);
    login->setWindowTitle("Login");
    login->show();
    QObject::connect(login, SIGNAL(signal_login_accepted(QString, QString)), this, SLOT(slot_login(QString, QString)));
    intermediary.get_version();
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
    ui->menuVersion->addAction("Version control");
    ui->menuVersion->addSeparator();
}

void BrainfuckIDE::on_actionSave_triggered()
{
    code = ui->CodeEditor->toPlainText();
    auto flag = is_saved.second;
    if (flag) {
        intermediary.save_file(code.toStdString());
    }
    else {
        filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   "./untitled.bf",
                                   tr("Source files (*.bf *.ook)"));
        intermediary.save_new_file(filename.toStdString(), code.toStdString());
        flag = true;
    }
    intermediary.get_version();
}

void BrainfuckIDE::on_actionSave_as_triggered()
{
    filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "./untitled.bf",
                               tr("Source files (*.bf *.ook)"));
    intermediary.save_new_file(filename.toStdString(), code.toStdString());
    intermediary.get_version();
}
