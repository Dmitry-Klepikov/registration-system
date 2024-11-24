#ifndef ACTIVATIONWINDOW_H
#define ACTIVATIONWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QProcess>
#include <QSettings>

#include "headers/regEntryFunctons.h"

namespace Ui {
class Activationwindow;
}

class Activationwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Activationwindow(QWidget *parent = nullptr);
    ~Activationwindow();

signals:
    void errCanceled();
    void correctLicense();

private:
    Ui::Activationwindow *ui;
    QSqlDatabase db;
    QSettings *settings;
    bool isEntry = false;

    QString getUUID();
    bool checkKey();
    bool checkLicense();
    void addDevice();

private slots:
    void activationButton_clicked();
    void sendSignal();
};

#endif // ACTIVATIONWINDOW_H
