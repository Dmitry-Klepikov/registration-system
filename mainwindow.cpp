#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "user.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    askwin = new Askwindow;
    askwin->show();
    connect(askwin, &Askwindow::canseled, this, &MainWindow::closewindow);
    connect(askwin, &Askwindow::correctLogin, this, &MainWindow::setLogin);
}

MainWindow::~MainWindow()
{
    delete  askwin;
    delete ui;
    qDebug()<<"dfd";
}

void MainWindow::closewindow(){
    this->close();
}

void MainWindow::setLogin(){
    ui->label->setText(User::getUsername());
}

void MainWindow::showAskWindow()
{






////    askwin = new Askwindow;
////    askwin->setModal(true);

//    if(askwin->exec()){
//        ui->label->setText(User::getUsername());
//    }else{
//        emit canseled();
//        qDebug()<<"here";
//        this->close();
//    }
//    delete askwin;

}
