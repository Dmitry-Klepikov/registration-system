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
}

MainWindow::~MainWindow()
{
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
    closeFile();
    if(index.isValid()){
        filePath = model->rootPath()+"//"+index.data().toString();
        if(!checkFileAccess(index.data().toString())){
            return;
        }
        QFile file(filePath);
        if(file.open(QIODevice::ReadOnly)){
            QString str = file.readAll();
            ui->filesTextEdit->setPlainText(str);
            file.close();
        }
    }
}

void MainWindow::closeFile(){
    QFile file(filePath);
    if(file.isOpen()){
        file.close();
    }
    filePath = "";
    ui->filesTextEdit->clear();
}


void MainWindow::on_saveFile_triggered()
{
    if(User::getAccess().toInt()!=3){
        callMessageBox("Недостаточный уровень доступа");
    }else{
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(ui->filesTextEdit->toPlainText().toUtf8());
            file.waitForBytesWritten(1000);
            callMessageBox("Данные сохранены");
            file.close();
        }
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
    query.prepare("select file_access from files_access where file_name = :file_name");
    query.bindValue(":file_name", fileName);
    if(!query.exec()){
        return false;
    }
    if(query.first()){
        if(query.value(0).toInt()<=User::getAccess()){
            return true;
        }
    }
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

