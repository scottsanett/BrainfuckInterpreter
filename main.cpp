#include "gui/brainfuckide.hpp"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BrainfuckIDE w;
    w.show();
    return a.exec();
}
