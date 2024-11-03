#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDebug>
#include <user.h>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QAbstractButton::clicked, this, &LoginWindow::applyButton_clicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
    qDebug()<<"logwin was deleted";
}

void LoginWindow::applyButton_clicked()
{
    //проверка логина и пароля

    if(true){
        User::setUsername("new username");
        this->accept(); //reject
    }else{
        this->reject();
    }
}

