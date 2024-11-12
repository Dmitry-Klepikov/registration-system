#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSettings>


#include <headers/user.h>
#include <headers/regwindow.h>
#include <headers/activationwindow.h>


namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void errCanseled();
    void correctLogin();

private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;
    Regwindow *regwin;
    Activationwindow *licensewin;
    QSettings *settings;
    bool isEntry = false;

    void dbConnect(int);
    bool entry(QSqlDatabase);
    bool setUser(QSqlDatabase);
    bool checkLicense();
    QString getUUID();
    void isShowPass();

private slots:
    void applyButton_clicked();
    void regButton_clicked();
    void closeWindow();

};

#endif // LOGINWINDOW_H
