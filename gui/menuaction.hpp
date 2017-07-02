#ifndef MENUACTION_H
#define MENUACTION_H

#include <QAction>

class MenuAction: public QAction
{
    Q_OBJECT

public:
    MenuAction(QString);

signals:
    void get_history_key(QString);

private slots:
    void slot_trigger(bool);

};

#endif // MENUACTION_H
