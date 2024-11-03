#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <askwindow.h>
#include <QDebug>
#include <user.h>

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
//    User *user;
    Askwindow * askwin;
//    QString login;
    void showAskWindow();
    void closewindow();


private slots:
    void setLogin();


};
#endif // MAINWINDOW_H
