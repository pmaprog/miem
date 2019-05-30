#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    dlg_abbe = new AbbeDialog(this);
    dlg_abbe->setWindowFlag(Qt::WindowContextHelpButtonHint, false);

    q = 0.001;
}

MainWindow::~MainWindow() {
    delete ui;
    delete dlg_abbe;
}

void MainWindow::update_tbl_data() {
    ui->tbl_data->setRowCount(0);

    for (auto i : _sample) {
        ui->tbl_data->insertRow(ui->tbl_data->rowCount());
        int row = ui->tbl_data->rowCount() - 1;
        ui->tbl_data->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
    }

    ui->le_data_size->setText(QString::number(_sample.size()));
}

void MainWindow::on_select_file_triggered() {
//    QString filename = QFileDialog::getOpenFileName(this,
//                                                    "Открыть файл выборки",
//                                                    "",
//                                                    "Все файлы (*)");

    QString filename = "C:\\Users\\yngmrk\\Desktop\\Ab_v25_a.txt";

    if (filename != "") {
        _sample.load_from_file(filename.toStdString());
        update_tbl_data();
    }

    ui->btn_calc->setEnabled(true);
}

void MainWindow::on_show_abbe_triggered() {
    dlg_abbe->show();
}

void MainWindow::on_btn_calc_clicked() {
    ui->le_mean->setText(QString::number(_sample.mean()));
    ui->le_std->setText(QString::number(_sample.std()));
    ui->le_var->setText(QString::number(_sample.variance()));
    ui->le_var_seq->setText(QString::number(_sample.variance_seq()));
    ui->le_v->setText(QString::number(_sample.v()));

    ui->btn_report->setEnabled(true);
    ui->gb_q->setEnabled(true);
}

void MainWindow::on_rb_001_clicked() {
    q = 0.001;
}

void MainWindow::on_rb_01_clicked() {
    q = 0.01;
}

void MainWindow::on_rb_05_clicked() {
    q = 0.05;
}
