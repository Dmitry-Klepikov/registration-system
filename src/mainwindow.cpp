#include "headers/mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("srv");

    logwin = new LoginWindow;
    logwin->show();

    connect(logwin, &LoginWindow::errCanseled, this, &MainWindow::closewindow);
    connect(logwin, &LoginWindow::correctLogin, this, &MainWindow::setLogin);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closewindow(){
    delete logwin;
    this->close();
}

void MainWindow::setLogin(){
    delete logwin;
    ui->label->setText(User::getUsername());
}
