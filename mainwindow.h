#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <fstream>
#include <sstream>

#include "sample.h"
#include "abbedialog.h"

#include <QDebug>

#include <QMainWindow>
#include <QString>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_select_file_triggered();

    void on_show_abbe_triggered();

    void on_btn_calc_clicked();

    void on_rb_001_clicked();

    void on_rb_01_clicked();

    void on_rb_05_clicked();

private:
    Ui::MainWindow *ui;
    AbbeDialog *dlg_abbe;

    Sample _sample;
    double q;
    std::vector<double> vq[3];

    void update_tbl_data();
};

#endif // MAINWINDOW_H
