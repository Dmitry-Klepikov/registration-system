#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>
#include <QFileSystemModel>
#include <QListView>
#include <QAction>

#include <headers/loginwindow.h>
#include <headers/user.h>
#include <headers/regEntryFunctons.h>
#include <headers/user.h>
#include "newfiledialog.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void canseled();

private:
    Ui::MainWindow *ui;
    LoginWindow *logwin;
    QSqlDatabase db;
    QFileSystemModel *model;

    QString filePath;

    QAction *doubleClick;

    void restrictionByAccess();
    bool checkFileAccess(QString);
    void closeFile();


private slots:
    void setLogin();
    void closewindow();

    void on_filesListView_doubleClicked(const QModelIndex &index);
    void on_saveFile_triggered();
    void on_createNewFile_triggered();
    void on_closeFile_triggered();
};


#endif // MAINWINDOW_H
