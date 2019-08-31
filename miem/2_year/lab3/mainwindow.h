#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fstream>

#include <QMainWindow>
#include <QDebug>
#include <QLabel>
#include <QTableWidget>
#include <QFileDialog>
#include <QMessageBox>

#include "dict.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QLabel *lblLoadFactor;

private slots:
    void onLeWordReturnPressed() const;
    void onLeWordTextEdited(const QString&) const;
    void onBtnInsertClicked();
    void onTabsCurrentChanged(int index);
    void onBtnSyncClicked();
    void onMQuitTriggered();
    void onMOpenTriggered();
    void onMSaveTriggered() const;
    void onMClearTriggered();
    void onBtnSetClicked();
    void onBtnDelClicked();
    void onTableCurrentItemChanged(QTableWidgetItem*, QTableWidgetItem*) const;

private:
    Ui::MainWindow *ui;

    void updateTable(QTableWidget*);
    void updateStatusBar();

    Dict<string> _dictPC, _dictPhone, *_curDict;
    QTableWidget *_curTable;
};

#endif // MAINWINDOW_H
