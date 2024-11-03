#include "regwindow.h"
#include "ui_regwindow.h"
#include <user.h>

Regwindow::Regwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regwindow)
{
    ui->setupUi(this);
    connect(ui->applyButton, &QAbstractButton::clicked, this, &Regwindow::applyButton_clicked);}

Regwindow::~Regwindow()
{
    delete ui;
}

void Regwindow::applyButton_clicked(){
    if(true){
        User::setUsername("new username");
        this->accept(); //reject
    }else{
        this->reject();
    }
}
