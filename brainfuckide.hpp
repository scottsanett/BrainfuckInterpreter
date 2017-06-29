#ifndef BRAINFUCKIDE_H
#define BRAINFUCKIDE_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

#include "server.hpp"
#include "intermediary.hpp"
#include "client_delegate.hpp"

#include "loginwindow.hpp"
#include "createaccount.hpp"
#include "savefiledialog.hpp"

namespace Ui {
class BrainfuckIDE;
}

class BrainfuckIDE : public QMainWindow
{
    Q_OBJECT

public:
    explicit BrainfuckIDE(QWidget *parent = 0);
    ~BrainfuckIDE();

private slots:
    void slot_login(QString, QString);

    void slot_create_account(QString, QString);

    void slot_load_result(QString);

    void slot_load_err_info(QString);

    void slot_load_file_path(QString);

    void slot_load_history_version(QString);

    void on_actionExit_triggered();

    void on_actionExecute_triggered();

    void on_actionLogin_triggered();

    void on_actionCreate_new_account_triggered();

    void on_actionOpen_triggered();

    void on_actionLogout_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

private:
    scott::Server server;
    scott::client::Intermediary intermediary;

    QString code;
    QString input;
    QString result;
    QString filename = "untitled.bf";
    std::pair<QString, bool> is_saved;

    /* UI elements */
    SaveFileDialog *save_file_dialog;
    Ui::BrainfuckIDE *ui;

};

#endif // BRAINFUCKIDE_H
