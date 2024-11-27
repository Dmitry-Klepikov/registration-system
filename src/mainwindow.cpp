#include "headers/mainwindow.h"
#include "ui_mainwindow.h"


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
    delete model;
    delete ui;
}

void MainWindow::closewindow(){
    delete logwin;
    this->close();
}

QString changeExtension(const QString& path, const QString& oldExt, const QString& newExt)
{
    QFile file(path);
    QString newPath = path;
    newPath.replace(oldExt, newExt);
    file.rename(newPath);
    return newPath;
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

        if(!checkFileAccess(index.data().toString())){
            return;
        }


            emit isUnUsed(currentFileName);
            qDebug()<<"free: "<<currentFileName;
            currentFileName = index.data().toString();


        filePath = model->rootPath()+"//"+index.data().toString();



//        QString fileName =index.data().toString();
//        // Разделить имя файла и расширение
//        QStringList parts = fileName.split(".");

//        // Изменить расширение
//        parts[1] = "txt";

//        // Установить новое имя файла
//        fileName = parts.join(".");

        filePath = changeExtension(filePath,"secretextension","txt");
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly)){
            emit isUsed(currentFileName);
//            QLockFile lockfile(filePath);
//            lockfile.lock();

            QString str = file.readAll();
            ui->filesTextEdit->setPlainText(str);
            file.close();

//            lockfile.unlock();

        }
       filePath = changeExtension(filePath,"txt","secretextension");
    }
}

void MainWindow::closeFile(){
    QFile file(filePath);
    if(file.isOpen()){
        file.close();
    }
    emit isUnUsed(currentFileName);
    filePath = "";
    currentFileName="";
    ui->filesTextEdit->clear();
}


void MainWindow::on_saveFile_triggered()
{
    if(User::getAccess().toInt()!=3){
        callMessageBox("Недостаточный уровень доступа");
    }else{

        filePath = changeExtension(filePath,"secretextension","txt");
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {

//            QLockFile lockfile(filePath);
//            lockfile.lock();

            file.write(ui->filesTextEdit->toPlainText().toUtf8());
            file.close();
            sign();
            filePath = changeExtension(filePath,"txt","secretextension");
            callMessageBox("Данные сохранены");

//            lockfile.unlock();

        }else{
            filePath = changeExtension(filePath,"txt","secretextension");
        }

    }
}

void MainWindow::sign(){
    db = QSqlDatabase::database(localPatch);
    if(!db.open()){

    }
    qDebug()<<ui->filesListView->currentIndex().data().toString();
    qDebug()<<User::getUsername();
    QSqlQuery query(db);
    query.prepare("UPDATE files_access "
                  "SET modified_by = :modified_by "
                "WHERE file_name = :file_name");
    query.bindValue(":modified_by", User::getUsername());
    query.bindValue(":file_name", ui->filesListView->currentIndex().data().toString());
    if(!query.exec()){
        qDebug()<<"err updete";
    }
}

void MainWindow::restrictionByAccess(){
    switch (User::getAccess().toInt()) {
    case 1:
        ui->filesTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        break;
    case 2:
        ui->filesTextEdit->setReadOnly(true);
        break;
    }
}

bool MainWindow::checkFileAccess(QString fileName){
    db = QSqlDatabase::database(localPatch);
    QSqlQuery query(db);
    query.prepare("select file_access, isUsed from files_access where file_name = :file_name");
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
            qDebug()<<"stat"<<0;

            return true;
        }
        if(query.value(1).toInt() == 1){
            qDebug()<<"stat: "<<1;
            return false;
        }
    }
    qDebug()<<"in check"<<fileName;
    return true;
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
    qDebug()<<"clean";
//    qDebug()<<ui->filesListView->currentIndex().data().toString();
//    qDebug()<<User::getUsername();
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
