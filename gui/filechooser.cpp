#include "filechooser.hpp"
#include "ui_filechooser.h"
#include <iostream>

FileChooser::FileChooser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileChooser)
{
    ui->setupUi(this);
}

FileChooser::FileChooser(QVector<QString> const & files, bool _read_only, QWidget *parent) :
    QDialog(parent),
    read_only(_read_only),
    ui(new Ui::FileChooser)
{
    ui->setupUi(this);
    if (read_only) { ui->FileChosen->setReadOnly(true); }
    else { ui->FileChosen->setReadOnly(false); }
    item_model = new QStandardItemModel;
    for (auto && i : files) {
        auto item = new QStandardItem(i);
        item_model->appendRow(item);
    }
    ui->FileList->setModel(item_model);
    connect(ui->FileList, SIGNAL(clicked(QModelIndex)), this, SLOT(slot_on_file_clicked(QModelIndex)));
    connect(ui->FileChosen, SIGNAL(editingFinished()), this, SLOT(slot_on_filename_edited()));
    connect(this, SIGNAL(accepted()), this, SLOT(slot_on_accepted()));
}

FileChooser::~FileChooser()
{
    delete ui;
}

void FileChooser::slot_on_file_clicked(QModelIndex index) {
    filename = index.data().toString();
    ui->FileChosen->setPlaceholderText(filename);
}

void FileChooser::slot_on_filename_edited() {
    filename = ui->FileChosen->text();
}

void FileChooser::slot_on_accepted() {
    emit signal_on_file_chosen(filename, read_only);
}

