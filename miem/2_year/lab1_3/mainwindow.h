#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "abstractvalue.h"
#include "purchase.h"
#include "donation.h"
#include "container.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void tableAction();

private slots:
    void on_btnPush_clicked();

    void on_btnPop_clicked();

    void on_menuOpen_triggered();

    void on_menuSave_triggered();

    void on_menuQuit_triggered();

    void onTableActions();
    void checkLineEdits();

    void on_rbPurchase_toggled(bool checked);

    void on_menuDeleteAll_triggered();

    void on_btnCalc_clicked();

private:
    Ui::MainWindow *ui;
    Container c;

    bool isPurchaseToggled;

    void updateTable();
};

#endif // MAINWINDOW_H
