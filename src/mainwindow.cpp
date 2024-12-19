#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include <zlib.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    logwin = new LoginWindow;
    logwin->show();

    model = new QFileSystemModel;

    model->setRootPath("C:\\dev\\files");

    connect(logwin, &LoginWindow::errCanseled, this, &MainWindow::closewindow);
    connect(logwin, &LoginWindow::correctLogin, this, &MainWindow::setLogin);
    connect(this, &MainWindow::isUsed, this, &MainWindow::sendUsing);
    connect(this, &MainWindow::isUnUsed, this, &MainWindow::sendUnUsing);
}

MainWindow::~MainWindow()
{
    emit isUnUsed(currentFileName);
    closeFile();
    delete model;
    delete ui;
}

void MainWindow::closewindow(){
    delete logwin;
    this->close();
}



void MainWindow::setLogin(){
    delete logwin;
    restrictionByAccess();
    ui->label->setText(User::getUsername());
    ui->filesListView->setModel(model);
    ui->filesListView->setRootIndex(model->index(model->rootPath()));
    root = model->rootPath();
}

void MainWindow::on_filesListView_doubleClicked(const QModelIndex &index)
{
    if(index.isValid()){
        QString str = index.data().toString();
        qDebug()<<str;
        if (str.contains(".zip")) {

            str.remove(".zip"); // Отбросить подстроку после второй точки

            if(currentFileName != str){
                closeFile();
            }
            qDebug()<<str<<"nozip";
        }

        if (index.data().toString().contains(".zip")){
            if(!checkFileAccess(str)){
                    return;
            }

            deleteArhive(root, index.data().toString(), getZipPass(str));

        }else{
        if(!checkFileAccess(index.data().toString())){
            closeFile();
            return;
        }
            closeFile();
            currentFileName = index.data().toString();

        filePath = model->rootPath()+"/"+index.data().toString();
        qDebug()<<"try open file:";

        filePath = changeExtension(filePath,"secretextension","txt");
        QFile file(filePath);
        qDebug()<<filePath;
        if(file.open(QIODevice::ReadOnly)){
            qDebug()<<"file was open";
            emit isUsed(currentFileName);
            QString strnj = file.readAll();
//            qDebug()<<fileHash;
            qDebug()<<strnj+"   in file";
                if(fileHash == hashString(strnj)){
                    qDebug()<<fileHash+"   from bd";
                    qDebug()<<hashString(strnj)+ "     from file";
                    ui->filesTextEdit->setPlainText(strnj);
                    file.close();
//                    filePath = changeExtension(filePath,"txt","secretextension");
                    file.remove();
                }else{
                    qDebug()<<fileHash;
                    qDebug()<<hashString(strnj);
                    qDebug()<<strnj;
                    file.close();
                    filePath = changeExtension(filePath,"txt","secretextension");

                    callMessageBox("Несоответсвие хеша. Файл будет закрыт");
                    closeFile();
                }
        }
    }
    }
}

QString MainWindow::getZipPass(QString file){
    db = QSqlDatabase::database(localPatch);
        QSqlQuery query(db);
        query.prepare("select modified_by from files_access where file_name = :file_name");
        query.bindValue(":file_name", file);
        if(!query.exec()){
            return "";
        }
        if(query.first()){
//            pass = query.value(0).toString();
            qDebug()<<"pass: "+pass;
            return query.value(0).toString();
        }

}

void MainWindow::closeFile(){
    QFile file(filePath);
    if(file.open(QIODevice::ReadOnly)){
        file.close();
    }
    emit isUnUsed(currentFileName);
//    if(pass == ""){
//        pass = User::getUsername();
//    }
    qDebug()<<"pass to close: "+pass;
    if(makeArhive(model->rootPath(), filePath, pass)){
        qDebug()<<"makeArh"<<filePath+".7z";

    }

    filePath = "";
    currentFileName="";
    ui->filesTextEdit->clear();
}


void MainWindow::on_saveFile_triggered()
{

        filePath = changeExtension(filePath,"secretextension","txt");
        qDebug()<<filePath;
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
//            fileHash = hashString(ui->filesTextEdit->toPlainText().toUtf8());
            file.write(ui->filesTextEdit->toPlainText().toUtf8());
            file.close();
            sign();
            pass = User::getUsername();
            filePath = changeExtension(filePath,"txt","secretextension");
            callMessageBox("Данные сохранены");

        }else{
            filePath = changeExtension(filePath,"txt","secretextension");
        }

}

void MainWindow::sign(){
    db = QSqlDatabase::database(localPatch);
    if(!db.open()){

    }
    qDebug()<<User::getUsername();
    QSqlQuery query(db);
    query.prepare("UPDATE files_access "
                  "SET modified_by = :modified_by, hash = :hash "
                "WHERE file_name = :file_name");
    query.bindValue(":modified_by", User::getUsername());
    query.bindValue(":file_name", currentFileName);
    query.bindValue(":hash", hashString(ui->filesTextEdit->toPlainText()));
    if(!query.exec()){
        qDebug()<<"err updete";
    }
}

void MainWindow::restrictionByAccess(){
    switch (User::getAccess().toInt()) {
    case 1:
//        ui->filesTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        break;
    case 2:
//        ui->filesTextEdit->setReadOnly(true);
        break;
    }
}

bool MainWindow::checkFileAccess(QString fileName){
    db = QSqlDatabase::database(localPatch);
    QSqlQuery query(db);
    query.prepare("select file_access, isUsed, hash, modified_by from files_access where file_name = :file_name");
    query.bindValue(":file_name", fileName);
    if(!query.exec()){
        return false;
    }
    if(query.first()){
        qDebug()<<query.value(0);
        qDebug()<<query.value(1);
        if(query.value(0).toInt()>User::getAccess().toInt()){
            qDebug()<<query.value(0).toInt();
            callMessageBox("недостаточный уровень доступа");
            return false;
        }
        if(query.value(1).toInt() == 0){
            if(query.value(0)<User::getAccess().toInt()){
                ui->filesTextEdit->setReadOnly(true);
            }else{
                ui->filesTextEdit->setReadOnly(false);
            }
            fileHash = query.value(2).toString();
            pass = query.value(3).toString();
            return true;
        }
    }
    callMessageBox("Файл уже используется.");
    return false;
}

void MainWindow::on_createNewFile_triggered()
{
    NewFileDialog *dialog = new NewFileDialog;
    dialog->setModal(true);
    dialog->exec();
    delete dialog;
}


void MainWindow::on_closeFile_triggered()
{
    closeFile();
}

void MainWindow::sendUsing(QString str){
    db = QSqlDatabase::database(localPatch);
    if(!db.open()){

    }
//    qDebug()<<ui->filesListView->currentIndex().data().toString();
//    qDebug()<<User::getUsername();
    QSqlQuery query(db);
    query.prepare("UPDATE files_access "
                  "SET isUsed = :isUsed "
                "WHERE file_name = :file_name");
    query.bindValue(":isUsed", 1);
    query.bindValue(":file_name", str);
    if(!query.exec()){
        qDebug()<<"err updete";
    }
}

void MainWindow::sendUnUsing(QString str){
    db = QSqlDatabase::database(localPatch);
    if(!db.open()){

    }
    QSqlQuery query(db);
    query.prepare("UPDATE files_access "
                  "SET isUsed = :isUsed "
                "WHERE file_name = :file_name");
    query.bindValue(":isUsed", 0);
    qDebug()<<ui->filesListView->currentIndex().data().toString();

    query.bindValue(":file_name", str);
    if(!query.exec()){
        qDebug()<<"err updete";
    }
}
