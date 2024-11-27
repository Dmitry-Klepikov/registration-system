#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include "headers/regEntryFunctons.h"
#include <QtSql>
#include <headers/user.h>

namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QWidget *parent = nullptr);
    ~NewFileDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewFileDialog *ui;
    QSqlDatabase db;
    bool checkName();
};

#endif // NEWFILEDIALOG_H
