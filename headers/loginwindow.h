#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSettings>


#include "headers/user.h"
#include "headers/regwindow.h"
#include "headers/activationwindow.h"
#include "headers/regEntryFunctons.h"


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
    void lockLogin();

private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;
    QSqlDatabase dbLocal;

    Regwindow *regwin;
    Activationwindow *licensewin;
    QSettings *settings;
    QTimer *timer;

    int counter = 0;

    bool isEntry = false;

    int errLoginCount = 0;
    QString currentLogin;
    void dbConnect(int);
    bool entry(QSqlDatabase);
    bool setUser(QSqlDatabase);
    bool checkLicense();
    QString getUUID();
    void isShowPass();
    void stan();
    void checkLocking();
    bool checkAccess();
    void lockCurrentLogin();

private slots:
    void applyButton_clicked();
    void regButton_clicked();
    void closeWindow();
    void on_loginLine_textChanged(const QString &arg1);
};

#endif // LOGINWINDOW_H
