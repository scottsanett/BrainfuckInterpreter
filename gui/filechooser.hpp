#ifndef FILECHOOSER_H
#define FILECHOOSER_H

#include <QDialog>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>

namespace Ui {
    class FileChooser;
}

class FileChooser : public QDialog
{
    Q_OBJECT

public:
    explicit FileChooser(QWidget *parent = 0);
    FileChooser(QVector<QString> const &, bool read_only, QWidget* parent = 0);
    ~FileChooser();

signals:
    void signal_on_file_chosen(QString, bool);

private slots:
    void slot_on_file_clicked(QModelIndex);
    void slot_on_filename_edited();
    void slot_on_accepted();

private:
    Ui::FileChooser *ui;
    bool read_only;
    QString filename;
    QStandardItemModel * item_model;
};

#endif // FILECHOOSER_H
