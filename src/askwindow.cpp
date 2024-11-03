#include "askwindow.h"
#include "ui_askwindow.h"

Askwindow::Askwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Askwindow)
{
    ui->setupUi(this);
    mySignals();
    this->setModal(true);
}

Askwindow::~Askwindow()
{
    delete ui;
    qDebug()<<"delete askwin";
}

void Askwindow::mySignals(){
     // открываем диалоговое окно авторизации
    connect(ui->loginButton, &QAbstractButton::clicked, this, &Askwindow::loginButton_clicked);
    connect(ui->regButton, &QAbstractButton::clicked, this, &Askwindow::regButton_clicked);
}

void Askwindow::loginButton_clicked(){
    logwin = new LoginWindow;

    logwin->setModal(true);

    if (logwin->exec()){
        emit correctLogin();
        this->accept();
        qDebug()<<"was apply";
    }else{
        emit canseled();
        this->reject();
        qDebug()<<"was not apply";
    }
    delete logwin;
}

void Askwindow::regButton_clicked(){
    regwin = new Regwindow;
    regwin->setModal(true);
    if (regwin->exec()){
        emit correctLogin();
        this->accept();
        qDebug()<<"was apply";
    }else{
        emit canseled();
        this->reject();
        qDebug()<<"was not apply";
    }
    delete regwin;

}
