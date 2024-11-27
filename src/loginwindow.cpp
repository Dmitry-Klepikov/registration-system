#include "headers/loginwindow.h"
#include "ui_loginwindow.h"
#include <QTimer>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    db = QSqlDatabase::addDatabase("QODBC","publisherName");
    db.setDatabaseName("srv");
    path = db.connectionName();
    qDebug()<<path;
    dbLocal = QSqlDatabase::addDatabase("QODBC","localName");

    timer = new QTimer;
    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->passRadioButton->setChecked(false);
    ui->passLine->setEchoMode(QLineEdit::Password);

    this->setEnabled(false);

    licensewin = new Activationwindow;

    settings = new QSettings("settings.ini", QSettings::IniFormat);

    settings->deleteLater();


    if(!settings->contains("license")){ //если отсуттвует ключ license в файле конфигурации или самого файла конфигурации
        licensewin->show();

        if(licensewin->exec()){
            this->setEnabled(true);
        }else{
            QMetaObject::invokeMethod(this,"closeWindow", Qt::QueuedConnection);
        }
    }else{
        if(checkLicense()){//проверяем ключ лицензии из файла конфигурации
            this->setEnabled(true);
        }else{
            licensewin->show();

            if(licensewin->exec()){
                this->setEnabled(true);
            }else{
                QMetaObject::invokeMethod(this,"closeWindow", Qt::QueuedConnection);
            }
        }
    }

    connect(ui->applyButton, &QAbstractButton::clicked, this, &LoginWindow::applyButton_clicked);
    connect(ui->regButton, &QAbstractButton::clicked, this, &LoginWindow::regButton_clicked);
    connect(this, &QDialog::finished, this, &LoginWindow::closeWindow);
    connect(ui->passRadioButton, &QAbstractButton::clicked, this, &LoginWindow::isShowPass);
    connect(timer, &QTimer::timeout, this, &LoginWindow::stan);
    connect(this, &LoginWindow::lockLogin, this, &LoginWindow::lockCurrentLogin);
}

LoginWindow::~LoginWindow()
{
    if(db.isOpen()){
        db.close();
    }

    delete licensewin;
    delete timer;
    delete ui;
}

bool LoginWindow::checkLicense(){
//    db = QSqlDatabase::database();

    if(!db.open()){
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


    QString serialNumber = lines.value(1).trimmed();

    return serialNumber;

}

void LoginWindow::applyButton_clicked(){
    dbLocal.setDatabaseName(ui->DSNLine->text());
    localPatch = dbLocal.connectionName();

    if(!db.open()){
        callMessageBox(db.lastError().text());
        qDebug()<<"error of database connection";
        return ;
    }

    if(!dbLocal.open()){
        callMessageBox(dbLocal.lastError().text());
        qDebug()<<"error of database connection";
        return ;
    }

    if(!entry(db)){ //проверка входа
        callMessageBox("Неправильный логин или пароль");
        counter += 1;
        checkLocking();
        if(counter == 3){
            counter = 0;
            stan();
        }
        return ;
    }

    if(!dbLocal.open()){
        callMessageBox(dbLocal.lastError().text());
        qDebug()<<"error of databaseLocal connection";
        return ;
    }

    if(!checkAccess()){
        callMessageBox("ошибка доступа");
        return ;
    }

    if(!setUser(db)){ //проверка получения пользовательских данных
        callMessageBox("Ошибка чтения данных");
        return ;
    }

    isEntry=true;
    this->accept();

}

bool LoginWindow::entry(QSqlDatabase db){
    QSqlQuery query = QSqlQuery(db);

    query.prepare(" declare @Result bit, @id int "
                  "exec verification :Login, :Pass, @Result output, @id output "
                  "select @Result as res, @id as id");

    query.bindValue(":Login", ui->loginLine->text());
    query.bindValue(":Pass", hashString(ui->passLine->text())); // отправляем хеш в процедуру верификации
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

    query.prepare("select login from user_logins where id = :id");

    query.bindValue(":id",User::getId());
    query.bindValue("login", QVariant(0));

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

    regwin->show();
    regwin->exec();

    delete regwin;
}

void LoginWindow::closeWindow(){
    if(!isEntry){
        emit errCanseled();
    }else{
        emit correctLogin();
    }

}

void LoginWindow::isShowPass(){
    if(ui->passRadioButton->isChecked()){
        ui->passLine->setEchoMode(QLineEdit::Normal);
    }else{
        ui->passLine->setEchoMode(QLineEdit::Password);
    }
}

void LoginWindow::stan(){
    if(ui->applyButton->isEnabled()){
        ui->applyButton->setEnabled(false);
        callMessageBox("Из-за частых ошибок доступ ограничен на 5 секунд. Пожалуйста, проверьте логин и пароль");
        timer->start(5000);
    }else{
        ui->applyButton->setEnabled(true);
    }
}

bool LoginWindow::checkAccess(){
    QSqlQuery query = QSqlQuery(dbLocal);
    query.prepare("select access from user_access where login = :login");
    query.bindValue(":login", ui->loginLine->text());
    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    if(query.next()){
        User::setAccess(query.value(0).toString());
        return true;
    }

    return false;

}

void LoginWindow::checkLocking(){
    errLoginCount++;
    qDebug()<<errLoginCount;
    if(errLoginCount == 3){
        emit lockLogin();
    }
}

void LoginWindow::lockCurrentLogin(){
    qDebug()<<"here";
    QSqlQuery query = QSqlQuery(dbLocal);
    query.prepare("UPDATE user_access "
                  "SET locking = :locking "
                  "WHERE login = :login");
    query.bindValue(":locking", 1);
    query.bindValue(":login",currentLogin);
    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return ;
    }
    callMessageBox("Пользователь заблокирован");
}

void LoginWindow::on_loginLine_textChanged(const QString &arg1)
{
    errLoginCount = 0;
    currentLogin = arg1;
}

