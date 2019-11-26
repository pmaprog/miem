#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    isPurchaseToggled = true;

    ui->leAmount->setValidator(new QDoubleValidator(0, 1e9, 2));

    connect(this, &MainWindow::tableAction, this, &MainWindow::onTableActions);

    connect(ui->leAmount, &QLineEdit::textChanged, this, &MainWindow::checkLineEdits);
    connect(ui->leFundName, &QLineEdit::textChanged, this, &MainWindow::checkLineEdits);

    QDateTime cur = QDateTime::currentDateTime();
    ui->dteTime->setDateTime(cur);
    ui->time1->setDateTime(cur);
    ui->time2->setDateTime(cur);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnPush_clicked() {
    ui->table->insertRow(ui->table->rowCount());

    int row = ui->table->rowCount() - 1;
    ui->table->setItem(row, 0, new QTableWidgetItem(isPurchaseToggled ? "Purchase" : "Donation"));
    ui->table->setItem(row, 1, new QTableWidgetItem(ui->dteTime->dateTime().toString()));
    ui->table->setItem(row, 2, new QTableWidgetItem(ui->leAmount->text()));
    ui->table->setItem(row, 3, new QTableWidgetItem(isPurchaseToggled ? "-" : ui->leFundName->text()));

    time_t time = ui->dteTime->dateTime().toTime_t();
    double amount = ui->leAmount->text().replace(',', '.').toDouble();
    std::string name = ui->leFundName->text().replace(' ', '_').toStdString();
    if (isPurchaseToggled) {
        Purchase p(time, amount);
        c.push(&p);
        // ?
        // c.push(new Purchase(time, amount));
    } else {
        Donation d(time, amount, name);
        c.push(&d);
    }

    emit tableAction();
}

void MainWindow::on_btnPop_clicked() {
    c.pop();
    ui->table->removeRow(0);
    emit tableAction();
}

void MainWindow::onTableActions() {
    ui->btnPop->setEnabled(c.size() != 0);
}

void MainWindow::checkLineEdits() {
    bool isValid = false;

    isValid = (ui->leAmount->text() != "");

    if (!isPurchaseToggled)
        isValid = isValid && (ui->leFundName->text() != "");

    ui->btnPush->setEnabled(isValid);
}

void MainWindow::updateTable() {
    ui->table->setRowCount(0);

    for (Container::Iterator i = c.begin(); i != c.end(); ++i) {
        ui->table->insertRow(ui->table->rowCount());

        QTableWidgetItem *type = new QTableWidgetItem(QString::fromStdString((*i)->getType())),
                         *time = new QTableWidgetItem(QDateTime::fromTime_t((*i)->getTime()).toString()),
                         *amount = new QTableWidgetItem(QString::number((*i)->getAmount()).replace('.', ',')),
                         *name = new QTableWidgetItem((*i)->getType() == "Purchase" ? "-" : QString::fromStdString(dynamic_cast<Donation*>(*i)->getName()).replace('_', ' '));

        int row = ui->table->rowCount() - 1;

        ui->table->setItem(row, 0, type);
        ui->table->setItem(row, 1, time);
        ui->table->setItem(row, 2, amount);
        ui->table->setItem(row, 3, name);
    }

    emit tableAction();
}

void MainWindow::on_rbPurchase_toggled(bool checked) {
    ui->leFundName->setEnabled(!checked);
    isPurchaseToggled = checked;
    checkLineEdits();
}

void MainWindow::on_menuDeleteAll_triggered() {
    c.clear();
    updateTable();
}

void MainWindow::on_btnCalc_clicked() {
    time_t t1 = ui->time1->dateTime().toTime_t(),
           t2 = ui->time2->dateTime().toTime_t();
    QString result = QString::number(c.calculateRevenue(t1, t2));
    QMessageBox::information(this, "Выручка", "За заданный период выручка равна: " + result);
}

void MainWindow::on_menuOpen_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Открыть файл с данными",
                                                    "",
                                                    "Все файлы (*)");
    if (fileName != "") {
        c.read(fileName.toStdString());
        updateTable();
    }
}

void MainWindow::on_menuSave_triggered() {
    if (c.size() != 0) {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "Сохранить данные",
                                                        "",
                                                        "Все файлы (*)");
        c.write(fileName.toStdString());
    } else {
        QMessageBox::critical(this, "Ошибка!", "Контейнер пуст!");
    }
}

void MainWindow::on_menuQuit_triggered() {
    QCoreApplication::quit();
}
