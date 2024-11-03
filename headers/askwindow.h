#ifndef ASKWINDOW_H
#define ASKWINDOW_H

#include <QDialog>
#include <loginwindow.h>
#include <qdebug.h>
#include <regwindow.h>

namespace Ui {
class Askwindow;
}

class Askwindow : public QDialog
{
    Q_OBJECT

public:
    explicit Askwindow(QWidget *parent = nullptr);
    ~Askwindow();

signals:
    void canseled();
    void correctLogin();


private:
    Ui::Askwindow *ui;
    LoginWindow *logwin;
    Regwindow *regwin;
    void mySignals();

private slots:
        void loginButton_clicked();
        void regButton_clicked();
};

#endif // ASKWINDOW_H
