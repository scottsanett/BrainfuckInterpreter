#include "gui/brainfuckide.hpp"
#include "ui_brainfuckide.h"

scott::server::server BrainfuckIDE::server(3333);
scott::client::client BrainfuckIDE::client;
scott::intermediary::intermediary BrainfuckIDE::intermediary;

BrainfuckIDE::BrainfuckIDE(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BrainfuckIDE)
{
    server.start();
    ui->setupUi(this);
    // setting up all the connections between the client.delegate and the ui class
    connect(&client.delegate, SIGNAL(signal_load_result(QString)), this, SLOT(slot_load_result(QString)));
    connect(&client.delegate, SIGNAL(signal_load_err_info(QString)), this, SLOT(slot_load_err_info(QString)));
    connect(&client.delegate, SIGNAL(signal_load_file(QString)), this, SLOT(slot_load_file(QString)));
    connect(&client.delegate, SIGNAL(signal_load_history_version(QString)), this, SLOT(slot_load_history_version(QString)));
    connect(&client.delegate, SIGNAL(signal_load_history_code(QString)), this, SLOT(slot_load_history_code(QString)));
    connect(&client.delegate, SIGNAL(signal_file_saved()), this, SLOT(slot_file_saved()));
    connect(&client.delegate, SIGNAL(signal_create_account_success(QString)), this, SLOT(slot_create_account_success(QString)));
    connect(&client.delegate, SIGNAL(signal_create_account_error(QString)), this, SLOT(slot_create_account_failure(QString)));
    connect(&client.delegate, SIGNAL(signal_authentication_success(QString)), this, SLOT(slot_authentication_success(QString)));
    connect(&client.delegate, SIGNAL(signal_authentication_error(QString)), this, SLOT(slot_authentication_failure(QString)));
    connect(&client.delegate, SIGNAL(signal_save_file_failure(QString)), this, SLOT(slot_save_file_failure(QString)));
    connect(&client.delegate, SIGNAL(signal_file_access_failure(QString)), this, SLOT(slot_file_access_failure(QString)));
    connect(&client.delegate, SIGNAL(signal_open_file_request(QString)), this, SLOT(slot_open_file_request(QString)));
    connect(&client.delegate, SIGNAL(signal_save_file_request(QString)), this, SLOT(slot_save_file_request(QString)));
    on_actionLogin_triggered();
}

BrainfuckIDE::~BrainfuckIDE()
{
    intermediary.close();
    server.stop();
    delete ui;
}

void BrainfuckIDE::slot_login(QString usr, QString pwd) {
    intermediary.on_login_clicked(usr.toStdString(), pwd.toStdString());
    clear_history_menu();
}

void BrainfuckIDE::slot_create_account(QString usr, QString pwd) {
    intermediary.on_create_account_clicked(usr.toStdString(), pwd.toStdString());
}

void BrainfuckIDE::slot_load_result(QString result) {
    ui->Output->setPlainText(result);
}

void BrainfuckIDE::slot_load_err_info(QString error) {
    ui->statusBar->showMessage(error, 3000);
}

void BrainfuckIDE::slot_open_file_request(QString files) {
    std::istringstream iss(files.toStdString());
    std::string file;
    QVector<QString> file_vec;
    while (iss >> file) {
        file_vec.push_back(file.c_str());
    }
    if (file_vec.empty()) {
        ui->statusBar->showMessage("The current user hasn't saved any file.", 3000);
    }
    else {
        auto file_chooser = new FileChooser(file_vec, true); //read only
        connect(file_chooser, SIGNAL(signal_on_file_chosen(QString, bool)), this, SLOT(slot_on_file_chosen(QString, bool)));
        file_chooser->show();
    }
}

void BrainfuckIDE::slot_save_file_request(QString files) {
    std::istringstream iss(files.toStdString());
    std::string file;
    QVector<QString> file_vec;
    while (iss >> file) {
        file_vec.push_back(file.c_str());
    }
    auto file_chooser = new FileChooser(file_vec, false); //not read only
    connect(file_chooser, SIGNAL(signal_on_file_chosen(QString, bool)), this, SLOT(slot_on_file_chosen(QString, bool)));
    file_chooser->show();
}

void BrainfuckIDE::slot_on_file_chosen(QString url, bool read_only) {
    if (read_only) {
        intermediary.choose_file(url.toStdString());
    }
    else {
        auto code = format_code();
        intermediary.save_new_file(url.toStdString(), code);
    }
}

void BrainfuckIDE::slot_load_file(QString content) {
    clear_gui();
    ui->CodeEditor->setPlainText(content);
    intermediary.get_version();
    is_saved = true;
}

void BrainfuckIDE::slot_load_history_version(QString versions) {
    clear_history_menu();
    auto versions_str = versions.toStdString();
    std::string key;
    std::istringstream iss(versions_str);
    while (iss >> key) {
        auto current_action = new MenuAction(key.c_str());
        ui->menuVersion->addAction(current_action);
        connect(current_action, SIGNAL(triggered(bool)), current_action, SLOT(slot_trigger(bool)));
        connect(current_action, SIGNAL(get_history_key(QString)), this, SLOT(slot_get_histoy_key(QString)));
    }
}

void BrainfuckIDE::slot_get_histoy_key(QString key) {
    intermediary.choose_version(key.toStdString());
}

void BrainfuckIDE::slot_load_history_code(QString content) {
    clear_gui();
    intermediary.get_version();
    ui->CodeEditor->setPlainText(content);
}

void BrainfuckIDE::slot_file_saved() {
    is_saved = true;
    intermediary.get_version();
    ui->statusBar->showMessage("The file has been saved.", 3000);
}

void BrainfuckIDE::slot_authentication_failure(QString msg) {
    auto login = new LoginWindow(this);
    connect(login, SIGNAL(signal_login_accepted(QString, QString)), this, SLOT(slot_login(QString, QString)));
    connect(login, SIGNAL(rejected()), this, SLOT(on_actionLogin_triggered()));
    login->setModal(true);
    login->setWindowTitle("Login to your account");
    login->show();
    auto message_box = new QMessageBox(login);
    message_box->setText(msg);
    message_box->setModal(true);
    message_box->show();
}

void BrainfuckIDE::slot_authentication_success(QString msg) {
    ui->statusBar->showMessage(msg, 3000);
}

void BrainfuckIDE::slot_create_account_failure(QString msg) {
    auto create = new CreateAccount(this);
    connect(create, SIGNAL(signal_create_account_accepted(QString, QString)), this, SLOT(slot_create_account(QString, QString)));
    create->setModal(true);
    create->setWindowTitle("Create a new account");
    create->show();
    auto message_box = new QMessageBox();
    message_box->setText(msg);
    message_box->setModal(true);
    message_box->show();
}

void BrainfuckIDE::slot_create_account_success(QString msg) {
    ui->statusBar->showMessage(msg, 3000);
    on_actionLogin_triggered();
}

void BrainfuckIDE::slot_save_file_failure(QString msg) {
    is_saved = false;
    auto message_box = new QMessageBox();
    message_box->setText(msg);
    message_box->setModal(true);
    message_box->show();
}

void BrainfuckIDE::slot_file_access_failure(QString msg) {
    auto message_box = new QMessageBox();
    message_box->setText(msg);
    message_box->setModal(true);
    message_box->show();
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
    QDesktopWidget desktop;
    login->move(desktop.screen()->rect().center() - login->rect().center());
    connect(login, SIGNAL(signal_login_accepted(QString, QString)), this, SLOT(slot_login(QString, QString)));
    connect(login, SIGNAL(rejected()), this, SLOT(on_actionLogin_triggered()));
    login->setModal(true);
    login->setWindowTitle("Login");
    login->show();
}

void BrainfuckIDE::on_actionCreate_new_account_triggered()
{
    auto create = new CreateAccount(this);
    connect(create, SIGNAL(signal_create_account_accepted(QString, QString)), this, SLOT(slot_create_account(QString, QString)));
    create->setModal(true);
    create->setWindowTitle("Create new account");
    create->show();
}

void BrainfuckIDE::on_actionOpen_triggered()
{
    intermediary.open_file_request();
}

void BrainfuckIDE::on_actionLogout_triggered()
{
    intermediary.on_logout_clicked();
    clear_gui();
    on_actionLogin_triggered();
}

void BrainfuckIDE::on_actionSave_triggered()
{
    auto code = format_code();
    if (is_saved) {
        intermediary.save_file(code);
    }
    else {
        intermediary.save_file_request();
    }
}

void BrainfuckIDE::on_actionSave_as_triggered()
{
    intermediary.save_file_request();
}

void BrainfuckIDE::on_actionNew_triggered()
{
    filename = "untitled.bf";
    is_saved = false;
    ui->CodeEditor->clear();
    ui->InputEditor->clear();
    ui->Output->clear();
    clear_history_menu();
}

void BrainfuckIDE::clear_history_menu() {
    ui->menuVersion->clear();
    ui->menuVersion->addAction("Version control");
    ui->menuVersion->addSeparator();
}

void BrainfuckIDE::clear_gui() {
    clear_history_menu();
    ui->CodeEditor->clear();
    ui->InputEditor->clear();
    ui->Output->clear();
}

std::string BrainfuckIDE::format_code() {
    auto code = ui->CodeEditor->toPlainText().toStdString();
    code.erase(std::remove_if(code.begin(), code.end(), [](char ch){ return !(ch == '+' || ch == '-' || ch == '>' || ch == '<' || ch == '.' || ch == ',' || ch == '[' || ch == ']'); }), code.end());
    return code;
}
