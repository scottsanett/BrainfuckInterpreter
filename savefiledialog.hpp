#ifndef SAVEFILEDIALOG_H
#define SAVEFILEDIALOG_H

#include <QWidget>
#include <QFileDialog>

class SaveFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit SaveFileDialog(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // SAVEFILEDIALOG_H
