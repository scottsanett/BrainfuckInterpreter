#ifndef BRAINFUCKIDE_H
#define BRAINFUCKIDE_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <memory>
#include <algorithm>
#include <stack>

#include "client/client.hpp"
#include "client/intermediary.hpp"
#include "client/client_delegate.hpp"
#include "server/server.hpp"

#include "gui/loginwindow.hpp"
#include "gui/createaccount.hpp"
#include "gui/menuaction.hpp"

namespace Ui {
class BrainfuckIDE;
}

class BrainfuckIDE : public QMainWindow
{
    Q_OBJECT

    friend class scott::client::Intermediary;
    friend class scott::server::Delegate;
    friend class scott::server::Session;

public:
    explicit BrainfuckIDE(QWidget *parent = 0);
    ~BrainfuckIDE();

signals:
    void signal_get_history_key(QString);

private slots:
    void slot_login(QString, QString);

    void slot_create_account(QString, QString);

    void slot_load_result(QString);

    void slot_load_err_info(QString);

    void slot_load_file_path(QString);

    void slot_load_file(QString);

    void slot_load_history_version(QString);

    void slot_load_history_code(QString);

    void slot_get_histoy_key(QString);

    void slot_file_saved();

    void slot_create_account_failure(QString);

    void slot_authentication_failure(QString);

    void slot_file_access_failure(QString);

    void slot_save_file_failure(QString);

    void slot_create_account_success(QString);

    void slot_authentication_success(QString);

    void slot_file_access_success(QString) {}

    void on_actionExit_triggered();

    void on_actionExecute_triggered();

    void on_actionLogin_triggered();

    void on_actionCreate_new_account_triggered();

    void on_actionOpen_triggered();

    void on_actionLogout_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionNew_triggered();

private:
    static scott::Server server;
    static scott::Client client;
    static scott::client::Intermediary intermediary;
    static scott::server::Delegate server_delegate;
    static scott::client::Delegate client_delegate;
    static scott::Interpreter interpreter;

    QString input;
    QString result;
    QString filename = "untitled.bf";
    bool is_saved = false;
    bool is_logged_in = false;
    std::stack<QString> text_edit_pushed;
    std::stack<QString> text_edit_popped;

    /* UI elements */
    Ui::BrainfuckIDE *ui;

    std::string format_code();
    void clear_history_menu();
    void clear_gui();

public:

};

#endif // BRAINFUCKIDE_H
