#ifndef ABBEDIALOG_H
#define ABBEDIALOG_H

#include <QDialog>

namespace Ui {
class AbbeDialog;
}

class AbbeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AbbeDialog(QWidget *parent = nullptr);
    ~AbbeDialog();

private:
    Ui::AbbeDialog *ui;
};

#endif // ABBEDIALOG_H
