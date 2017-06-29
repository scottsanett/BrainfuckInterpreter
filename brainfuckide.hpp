#ifndef BRAINFUCKIDE_H
#define BRAINFUCKIDE_H

#include <QMainWindow>
#include <QString>

#include "server.hpp"
#include "intermediary.hpp"

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
    void on_actionExit_triggered();

    void on_actionExecute_triggered();

    void on_actionLogin_triggered();

private:
    scott::Server server;
    scott::client::Intermediary intermediary;
    QString code;
    QString input;
    QString result;
    Ui::BrainfuckIDE *ui;
};

#endif // BRAINFUCKIDE_H
