#include "menuaction.hpp"
#include "iostream"

MenuAction::MenuAction(QString str): QAction(str) {}

void MenuAction::slot_trigger(bool) {
    emit get_history_key(text());
}
