#include "headers/activationwindow.h"
#include "ui_activationwindow.h"
#include <QMessageBox>

Activationwindow::Activationwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Activationwindow)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    settings = new QSettings("settings.ini", QSettings::IniFormat);

    connect(ui->ActivationButton, &QAbstractButton::clicked, this, &Activationwindow::activationButton_clicked);
    connect(this, &Activationwindow::finished, this, &Activationwindow::sendSignal);
}

Activationwindow::~Activationwindow()
{
    delete settings;
    delete ui;
}

void Activationwindow::callMessageBox(QString msg){
    QMessageBox msgbox;
    msgbox.setText(msg);
    msgbox.exec();
}

void Activationwindow::activationButton_clicked(){
    db = QSqlDatabase::database();

    if(!db.open()){
        qDebug()<<db.lastError().text();
        return ;
    }

    if(checkKey()){ //проверка введенного ключа лицензии
        callMessageBox("Ключ успешно принят!");
        addDevice();
        settings->setValue("license", ui->licenseLine->text());
        settings->sync();
        isEntry = true;
        this->accept();
    }

    if(checkLicense()){ //проверка сохраненного uu_id материнской платы из базы данных с пользовательским uu_id
        callMessageBox("Ключ успешно принят!");
        settings->setValue("license", ui->licenseLine->text());
        settings->sync();
        isEntry = true;
        this->accept();
    }
}

bool Activationwindow::checkKey(){
    QSqlQuery query(db);

    query.prepare("SELECT license_key, uu_id FROM license WHERE license_key = :key");
    query.bindValue(":key", ui->licenseLine->text());

    if(!query.exec()){
        return false;
    }

    if(!query.first()){
        callMessageBox("Неправильный ключ");
        return false;
    }else{
        if(query.value(1).toString().isEmpty()){
            return true;
        }else{
            return false;
        }
    }
}

bool Activationwindow::checkLicense(){
    db = QSqlDatabase::database();

    if(!db.open()){
        qDebug()<<"error of database connection";
        return false;
    }

    QSqlQuery query(db);

    query.prepare("SELECT uu_id FROM license WHERE license_key = :license");
    query.bindValue(":license", ui->licenseLine->text());

    if(!query.exec()){
        return false;
    }

    if(query.first()){
        if (query.value(0).toString() == getUUID()){
            return true;
        }else{
            callMessageBox("Ключ зарегистрирован на другого пользователя!");
            return false;
        }
    }

    return false;
}

void Activationwindow::addDevice(){
    QSqlQuery query(db);
    query.prepare("UPDATE license "
                  "SET uu_id = :uu_id "
                  "WHERE license_key = :key");
    query.bindValue(":key", ui->licenseLine->text());
    query.bindValue(":uu_id", getUUID());
    if(!query.exec()){
        return ;
    }
}

QString Activationwindow::getUUID(){
    QProcess proc;

    proc.start("C:/Windows/System32/cmd.exe", QStringList() << "/C" << "wmic baseboard get serialnumber");
    proc.waitForFinished();

    QString output = proc.readAllStandardOutput();

    QStringList lines = output.split("\n");

    QString serialNumber = lines.value(1).trimmed();

    return serialNumber;

}

void Activationwindow::sendSignal(){
    if(!isEntry)
        emit errCanceled();
    emit correctLicense();
}
