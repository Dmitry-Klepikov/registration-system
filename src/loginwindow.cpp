#include "headers/loginwindow.h"
#include "ui_loginwindow.h"


LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    ui->passRadioButton->setChecked(false);
    ui->passLine->setEchoMode(QLineEdit::Password);
    this->setEnabled(false);
            licensewin = new Activationwindow;
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    settings->deleteLater();
//    settings->clear();
//    settings->sync();
    if(!settings->contains("license")){
        qDebug()<<settings->value("license").toString();

//        licensewin->deleteLater();
        licensewin->show();
        if(licensewin->exec()){
            this->setEnabled(true);
        }else{
//            this->deleteLater();
        }
    }else{
        if(checkLicense()){
            this->setEnabled(true);
        }else{

            licensewin->show();
            if(licensewin->exec()){
                this->setEnabled(true);
            }else{
//                this->deleteLater();
            }
        }
    }


//    соединяемся с бд

    connect(ui->applyButton, &QAbstractButton::clicked, this, &LoginWindow::applyButton_clicked);
    connect(ui->regButton, &QAbstractButton::clicked, this, &LoginWindow::regButton_clicked);
    connect(this, &QDialog::finished, this, &LoginWindow::closeWindow);
    connect(ui->passRadioButton, &QAbstractButton::clicked, this, &LoginWindow::isShowPass);
}

LoginWindow::~LoginWindow()
{
    if(db.isOpen())
        db.close();
    if(!isEntry)
        emit errCanseled();
    delete licensewin;
    delete ui;
    qDebug()<<"logwin was deleted";
}

bool LoginWindow::checkLicense(){
    db = QSqlDatabase::database();

    if(!db.open()){ //проверка подключения
        qDebug()<<db.lastError().text();
        qDebug()<<"error of database connection";
        return false;
    }

    QSqlQuery query(db);

    query.prepare("SELECT uu_id FROM license WHERE license_key = :license");
    query.bindValue(":license", settings->value("license"));

    if(!query.exec()){
        return false;
    }

    if(query.first()){
        if (query.value(0).toString() == getUUID()){
            return true;
        }else{
            return false;
        }
    }

    return false;
}

QString LoginWindow::getUUID(){

    QProcess proc;
      proc.start("C:/Windows/System32/cmd.exe", QStringList() << "/C" << "wmic baseboard get serialnumber");
      proc.waitForFinished();

      // Получаем стандартный вывод процесса
      QString output = proc.readAllStandardOutput();

      // Разбиваем вывод на строки
      QStringList lines = output.split("\n");

      // Ищем строку с SerialNumber
      QString serialNumber =lines.value(1).trimmed();

//      qDebug()<<serialNumber;

      // Возвращаем SerialNumber
      return serialNumber;

}

void LoginWindow::applyButton_clicked(){
    db = QSqlDatabase::database();

    if(!db.open()){ //проверка подключения
        qDebug()<<db.lastError().text();
        qDebug()<<"error of database connection";
        return ;
    }

    if(!entry(db)){ //проверка входа
        qDebug()<<" wrong login or password ";
        return ;
    }

    if(!setUser(db)){ //проверка получения пользовательских данных
        qDebug()<<"error of getting info";
        return ;
    }

    this->accept();
}

bool LoginWindow::entry(QSqlDatabase db){
    QSqlQuery query = QSqlQuery(db);

    query.prepare(" declare @Result bit, @id int "
                  "exec verification :Login, :Pass, @Result output, @id output "
                  "select @Result as res, @id as id");

    query.bindValue(":Login", ui->loginLine->text());
    query.bindValue(":Pass", ui->passLine->text());
    query.bindValue("@Result",QVariant(0));
    query.bindValue("@id",QVariant(1));

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            User::setId(query.value(1).toString());
            return true;
        }
    }

    return false;
}

bool LoginWindow::setUser(QSqlDatabase db){
    QSqlQuery query = QSqlQuery(db);

    query.prepare("select lastname from user_info where u_id = :id");

    query.bindValue(":id",User::getId());
    query.bindValue("lastname", QVariant(0));

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    if(query.next()){
        User::setUsername(query.value(0).toString());
        return true;
    }

    return false;
}
void LoginWindow::regButton_clicked(){
    regwin = new Regwindow;

    regwin->setModal(true);

    if (regwin->exec()){
//        correctEntry();
    }
    delete regwin;
}

void LoginWindow::closeWindow(){
    if(!isEntry)
        emit errCanseled();
}

void LoginWindow::isShowPass(){
    if(ui->passRadioButton->isChecked()){
        ui->passLine->setEchoMode(QLineEdit::Normal);
    }else{
        ui->passLine->setEchoMode(QLineEdit::Password);
    }
}
