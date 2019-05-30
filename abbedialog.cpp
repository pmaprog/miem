#include "abbedialog.h"
#include "ui_abbedialog.h"

#include <QDebug>

AbbeDialog::AbbeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AbbeDialog)
{
    ui->setupUi(this);
}

AbbeDialog::~AbbeDialog()
{
    delete ui;
}
