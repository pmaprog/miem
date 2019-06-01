#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    ui->tbl_data->setFont(font());

    dlg_abbe = new AbbeDialog(this);

    std::ifstream fin("abbe.txt");
    size_t n;
    double q1, q2, q3;
    while (fin) {
        fin >> n >> q1 >> q2 >> q3;
        vq[0].push_back(q1);
        vq[1].push_back(q2);
        vq[2].push_back(q3);
    }

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
        auto *item = new QTableWidgetItem(QString::number(i));
        ui->tbl_data->setItem(row, 0, item);
    }

    ui->le_data_size->setText(QString::number(_sample.size()));
}

void MainWindow::clear_results() {
    for (auto i : ui->groupBox_3->children()) {
        QLineEdit *le = dynamic_cast<QLineEdit*>(i);
        if (le)
            le->clear();
    }
    ui->lbl_result->clear();
    ui->btn_report->setEnabled(false);
}

void MainWindow::on_select_file_triggered() {
    std::string filename = QFileDialog::getOpenFileName(this,
                               "Открыть файл выборки",
                               "",
                               "Все файлы (*)").toLocal8Bit().toStdString();

    if (filename == "")
        return;

    _sample.load_from_file(filename);
    update_tbl_data();
    clear_results();
    ui->btn_calc->setEnabled(true);
}

void MainWindow::on_show_abbe_triggered() {
    dlg_abbe->show();
}

void MainWindow::on_btn_calc_clicked() {
    if (_sample.size() < 4 || _sample.size() > 60) {
        QMessageBox::critical(this, "Ошибка!", "Расчет возможен только при размере выборки от 4 до 60 значений.");
        return;
    }

    double v = _sample.v();

    ui->le_mean->setText(QString::number(_sample.mean()));
    ui->le_std->setText(QString::number(_sample.std()));
    ui->le_var->setText(QString::number(_sample.variance()));
    ui->le_var_seq->setText(QString::number(_sample.variance_seq()));
    ui->le_v->setText(QString::number(v));

    int n = 0;
    if (ui->rb_01->isChecked()) n = 1;
    if (ui->rb_05->isChecked()) n = 2;
    ui->le_abbe->setText(QString::number(vq[n][_sample.size()-4]));

    ui->btn_report->setEnabled(true);

    if (v > vq[n][_sample.size()-4]) {
        ui->lbl_result->setText("v > vq. Переменная систематическая погрешность результатов измерений отсутствует.");
    } else {
        ui->lbl_result->setText("v < vq. Обнаружена переменная систематическая погрешность результатов измерений.");
    }
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

void MainWindow::on_btn_report_clicked() const {
    std::string filename = QFileDialog::getSaveFileName(nullptr,
                                            "Сохранить результат работы программы",
                                            "",
                                            "Текст (*.txt)").toLocal8Bit().toStdString();

    if (filename == "")
        return;

    std::ofstream fout(filename);

    fout << "Критерий Аббе, Vq: " << ui->le_abbe->text().toStdString() << "\n";
    fout << "Среднее арифметическое, Xavg: " << ui->le_mean->text().toStdString() << "\n";
    fout << "СКО, S: " << ui->le_std->text().toStdString() << "\n";
    fout << "Дисперсия (расчет обычным способом), S^2: " << ui->le_var->text().toStdString() << "\n";
    fout << "Дисперсия (последовательный расчет), Q^2: " << ui->le_var_seq->text().toStdString() << "\n";
    fout << "Критерий обнаружения систематических погрешностей, v: " << ui->le_v->text().toStdString() << "\n";
    fout << "Результат: " << ui->lbl_result->text().toStdString() << "\n";
}
