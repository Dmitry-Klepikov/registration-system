#ifndef ACTIVATIONWINDOW_H
#define ACTIVATIONWINDOW_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QProcess>
#include <QSettings>

namespace Ui {
class Activationwindow;
}

class Activationwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Activationwindow(QWidget *parent = nullptr);
    ~Activationwindow();

private:
    Ui::Activationwindow *ui;
    QSqlDatabase db;
    QSettings *settings;

    QString getUUID();
    bool checkKey();
    bool checkLicense();
    void addDevice();

private slots:
    void activationButton_clicked();
};

#endif // ACTIVATIONWINDOW_H
