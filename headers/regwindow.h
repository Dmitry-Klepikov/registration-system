#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>

namespace Ui {
class Regwindow;
}

class Regwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Regwindow(QWidget *parent = nullptr);
    ~Regwindow();

private:
    Ui::Regwindow *ui;

private slots:
    void applyButton_clicked();
};

#endif // REGWINDOW_H
