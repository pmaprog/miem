#include "abbedialog.h"
#include "ui_abbedialog.h"
#include <fstream>

AbbeDialog::AbbeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AbbeDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setColumnWidth(0,120);

    int n, i = 0;
    double q1, q2, q3;
    std::ifstream fin("/home/kuder/abbData.txt");

    while (fin) {
        fin >> n >> q1 >> q2 >> q3;

        int lastRow = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(lastRow);

        QTableWidgetItem *item = new QTableWidgetItem();
        QString str = QString::number(n);
        item->setText(QString("%1").arg(str));
        ui->tableWidget->setItem(i, 0, item);

        QTableWidgetItem *item1 = new QTableWidgetItem();
        str = QString::number(q1);
        item1->setText(QString("%1").arg(str));
        ui->tableWidget->setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem();
        str = QString::number(q2);
        item2->setText(QString("%1").arg(str));
        ui->tableWidget->setItem(i, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem();
        str = QString::number(q3);
        item3->setText(QString("%1").arg(str));
        ui->tableWidget->setItem(i, 3, item3);

        ++i;
    }
    int lastRow = ui->tableWidget->rowCount();
    ui->tableWidget->removeRow(lastRow-1);
}

AbbeDialog::~AbbeDialog()
{
    delete ui;
}
