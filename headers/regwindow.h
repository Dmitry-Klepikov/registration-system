#ifndef REGWINDOW_H
#define REGWINDOW_H

#include <QDialog>

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <cctype>

#include <headers/user.h>



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
    QSqlDatabase db;

    bool checkPass();
    bool isCorrectPass();
    bool checkLogin();
    bool checkEmail();
    bool checkTelephone();
    bool registration();

    bool setLogPass();
    int getUserId();
    bool pushUserInfo(int);
    bool entry();
    void setUser();
    void isShowPass();


private slots:
    void applyButton_clicked();
    void cancellButton_clicked();
};

#endif // REGWINDOW_H
