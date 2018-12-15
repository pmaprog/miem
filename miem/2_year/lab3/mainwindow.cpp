#include "mainwindow.h"
#include "ui_mainwindow.h"

// todo
// скрывать неиспользуемуе кнопки
// использовать везде camelCase, сделать const методы

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->leWord, &QLineEdit::returnPressed,
            this, &MainWindow::onLeWordReturnPressed);
    connect(ui->leWord, &QLineEdit::textEdited,
            this, &MainWindow::onLeWordTextEdited);

    connect(ui->tablePC, &QTableWidget::currentItemChanged,
            this, &MainWindow::onTableCurrentItemChanged);
    connect(ui->tablePhone, &QTableWidget::currentItemChanged,
            this, &MainWindow::onTableCurrentItemChanged);

    connect(ui->btnInsert, &QPushButton::clicked,
            this, &MainWindow::onBtnInsertClicked);
    connect(ui->btnDel, &QPushButton::clicked,
            this, &MainWindow::onBtnDelClicked);
    connect(ui->btnSet, &QPushButton::clicked,
            this, &MainWindow::onBtnSetClicked);
    connect(ui->btnSync, &QPushButton::clicked,
            this, &MainWindow::onBtnSyncClicked);

    connect(ui->mOpen, &QAction::triggered,
            this, &MainWindow::onMOpenTriggered);
    connect(ui->mSave, &QAction::triggered,
            this, &MainWindow::onMSaveTriggered);
    connect(ui->mQuit, &QAction::triggered,
            this, &MainWindow::onMQuitTriggered);
    connect(ui->mClear, &QAction::triggered,
            this, &MainWindow::onMClearTriggered);

    connect(ui->tabs, &QTabWidget::currentChanged,
            this, &MainWindow::onTabsCurrentChanged);

    lblLoadFactor = new QLabel(statusBar());

    _curDict = &_dictPC;
    _curTable = ui->tablePC;

    updateStatusBar();
    ui->gbSelectedRow->setVisible(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onLeWordReturnPressed() const {
    emit ui->btnInsert->clicked();
}

void MainWindow::onLeWordTextEdited(const QString &text) const {
    ui->btnInsert->setEnabled(text != "");
}

void MainWindow::onBtnInsertClicked() {
    QString word = ui->leWord->text();
    Uint count = ui->sbCount->text().toUInt();

    _curDict->insert(word.toStdString(), count);

    updateTable(_curTable);
}

void MainWindow::onTabsCurrentChanged(int index) {
    _curDict = index == 0 ? &_dictPC : &_dictPhone;
    _curTable = index == 0 ? ui->tablePC : ui->tablePhone;

    emit _curTable->currentItemChanged(_curTable->currentItem(), nullptr);

    updateStatusBar();
}

void MainWindow::updateTable(QTableWidget *table) {
    table->setRowCount(0);

    Dict<string> *dict = (table == ui->tablePC) ? &_dictPC : &_dictPhone;

    for (auto i = dict->begin(); i != dict->end(); ++i) {
        table->insertRow(table->rowCount());
        int row = table->rowCount() - 1;
        table->setItem(row, 0, new QTableWidgetItem(
                           QString::fromStdString(i.word())));
        table->setItem(row, 1, new QTableWidgetItem(
                           QString::number(i.count())));
    }

    ui->btnSync->setEnabled(ui->tablePhone->rowCount() != 0);

    updateStatusBar();
}

void MainWindow::updateStatusBar() {
    lblLoadFactor->setText("LoadFactor: " +
                           QString::number(_curDict->getLoadFactor()));
    lblLoadFactor->adjustSize();
}

void MainWindow::onBtnSyncClicked() {
    _dictPC = _dictPC || _dictPhone;
    _dictPhone.clear();
    updateTable(ui->tablePC);
    updateTable(ui->tablePhone);
}

void MainWindow::onMQuitTriggered() {
    QApplication::quit();
}

void MainWindow::onMOpenTriggered() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Открыть файл с данными",
                                                    "",
                                                    "Все файлы (*)");
    if (fileName != "") {
        ifstream fin(fileName.toStdString());
        fin >> *_curDict;
        updateTable(_curTable);
    }
}

void MainWindow::onMSaveTriggered() const {
    if (_curDict->size() != 0) {
        QString fileName = QFileDialog::getSaveFileName(nullptr,
                                                        "Сохранить данные",
                                                        "",
                                                        "Все файлы (*)");
        if (fileName != "") {
            ofstream fout(fileName.toStdString());
            fout << *_curDict;
        }
    } else {
        QMessageBox::critical(nullptr, "Ошибка!", "Контейнер пуст!");
    }
}

void MainWindow::onMClearTriggered() {
    _curDict->clear();
    updateTable(_curTable);
}

void MainWindow::onBtnSetClicked() {
    _curDict->set(_curTable->item(_curTable->currentRow(), 0)->text().toStdString(),
                  ui->sbCount2->text().toUInt());
    updateTable(_curTable);
}

void MainWindow::onBtnDelClicked() {
    _curDict->remove(_curTable->item(_curTable->currentRow(), 0)->text().toStdString());
    updateTable(_curTable);
}

void MainWindow::onTableCurrentItemChanged(QTableWidgetItem* cur,
                                           QTableWidgetItem* prev) const {
    ui->gbSelectedRow->setVisible(cur != nullptr);
}
