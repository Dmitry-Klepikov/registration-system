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
#include <QLockFile>


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
    void isUsed(QString);
    void isUnUsed(QString);

private:
    Ui::MainWindow *ui;
    LoginWindow *logwin;
    QSqlDatabase db;
    QFileSystemModel *model;

    QString filePath;
    QString fileHash;

    QString pass;

    QString currentFileName;
    QString lastFileName;


    QAction *doubleClick;

    void restrictionByAccess();
    bool checkFileAccess(QString);
    void closeFile();
    void updateHash();
    void sign();
    QString getZipPass(QString file);


private slots:
    void setLogin();
    void closewindow();

    void sendUnUsing(QString);
    void sendUsing(QString);

    void on_filesListView_doubleClicked(const QModelIndex &index);
    void on_saveFile_triggered();
    void on_createNewFile_triggered();
    void on_closeFile_triggered();
};


#endif // MAINWINDOW_H
