#include "brainfuckide.hpp"
#include "ui_brainfuckide.h"

BrainfuckIDE::BrainfuckIDE(QWidget *parent) :
    QMainWindow(parent),
    server(3333),
    ui(new Ui::BrainfuckIDE)
{
    server.start();
    ui->setupUi(this);
}

BrainfuckIDE::~BrainfuckIDE()
{
    intermediary.close();
    server.stop();
    delete ui;
}

void BrainfuckIDE::on_actionExit_triggered()
{

}

void BrainfuckIDE::on_actionExecute_triggered()
{
    auto code = ui->CodeEditor->toPlainText().toStdString();
    auto input = ui->InputEditor->toPlainText().toStdString();
    intermediary.on_run_clicked(code, input);
}

void BrainfuckIDE::on_actionLogin_triggered()
{


}
