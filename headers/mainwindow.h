#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSqlDatabase>

#include <headers/loginwindow.h>
#include <headers/user.h>

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

private slots:
    void setLogin();
    void closewindow();
};
#endif // MAINWINDOW_H
