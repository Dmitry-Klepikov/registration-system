#include "headers/regwindow.h"
#include "ui_regwindow.h"

Regwindow::Regwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regwindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->passRadioButton->setChecked(false);
    ui->reppassLine->setEchoMode(QLineEdit::Password);

    db = QSqlDatabase::database();

    connect(ui->applyButtton, &QAbstractButton::clicked, this, &Regwindow::applyButton_clicked);
    connect(ui->cancellButton, &QAbstractButton::clicked, this, &Regwindow::cancellButton_clicked);
    connect(ui->passRadioButton, &QAbstractButton::clicked, this, &Regwindow::isShowPass);
}

Regwindow::~Regwindow()
{
    if(db.isOpen()){
        db.close();
    }

    delete ui;
}

void Regwindow::applyButton_clicked(){

    db.open();

    if (!db.isOpen()) {
           qDebug() << db.lastError().text();
           return;
       }

    if (!checkPass() || !checkEmail() || !checkTelephone() || !checkLogin()){
           return;
    }

    if (registration()){
           this->accept();
    }
}

bool Regwindow::checkPass(){
    if(ui->passLine->text() != ui->reppassLine->text()){
        qDebug()<<"пароли разные";
        return false;
    }

    if(ui->passLine->text().length() < 6){
        qDebug()<<"длина пароля должна составлять не менее 6 символов";
        return false;
    }

    if(!isCorrectPass()){
        qDebug()<<"пароль должен состоять как из символов так и из цифр";
        return false;
    }

    return true;
}

bool Regwindow::isCorrectPass(){
    bool hasDigit = false;
    bool hasLetter = false;

    for(char c : static_cast<std::string>(ui->passLine->text().toLatin1())){

        if(isdigit(c)){
            hasDigit = true;
        }

        if(isalpha(c)){
            hasLetter = true;
        }
    }

    return hasDigit&hasLetter;
}

bool Regwindow::checkLogin(){
    QSqlQuery query(db);

    query.prepare("select count(login)  from user_logins where login = :userName");

    query.bindValue(":userName", ui->loginLine->text());

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            qDebug()<<"такой логин уже зарегистрирован";
        }else{
            return true;
        }
    }

    qDebug()<<"ошибка обработки запроса";
    return false;
}

bool Regwindow::checkEmail(){
    QSqlQuery query(db);

    query.prepare("select count(email)  from user_info where email = :userMail");

    query.bindValue(":userMail", ui->mailLine->text());

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            qDebug()<<"такая почта уже зарегистрирована";
        }else{
            return true;
        }
    }

    qDebug()<<"ошибка обработки запроса";
    return false;
}

bool Regwindow::checkTelephone(){
    QSqlQuery query(db);

    query.prepare("select count(telephone)  from user_info where telephone = :userTelephone");

    query.bindValue(":userTelephone", ui->telephoneLine->text());

    if(!query.exec()){
      qDebug()<<query.lastError().text();
      return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            qDebug()<<"такой телефон уже зарегистрирован";
        }else{
            return true;
        }
    }

    qDebug()<<"ошибка обработки запроса";
    return false;
}

bool Regwindow::registration(){

    if(!setLogPass()){
        return false;
    }

    int id = getUserId();
    if (id == 0){
        return false;
    }

    if (!pushUserInfo(id)){
        return false;
    }

    return true;
}

bool Regwindow::setLogPass(){
    QSqlQuery query(db);

    query.prepare("INSERT INTO user_logins(login, password) "
                  "VALUES (:login, :password)");

    query.bindValue(":login", ui->loginLine->text());
    query.bindValue(":password", ui->passLine->text());

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }
    return true;
}

int Regwindow::getUserId(){
    QSqlQuery query(db);

    query.prepare(" declare @Result bit, @id int "
                  "exec verification :Login, :Pass, @Result output, @id output "
                  "select @Result as res, @id as id");

    query.bindValue(":Login", ui->loginLine->text());
    query.bindValue(":Pass", ui->passLine->text());
    query.bindValue("@Result",QVariant(0));
    query.bindValue("@id",QVariant(1));

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return 0;
    }

    if(!query.first()){
        qDebug()<<query.lastError().text();
        return 0;
    }

    if(query.value(0).toBool()){
        return query.value(1).toInt();
    }

    return 0;
}

bool Regwindow::pushUserInfo(int id){
    QSqlQuery query(db);

    query.prepare("INSERT INTO user_info(u_id, lastname, telephone, email) "
                  "VALUES(:id, :lastname, :telephone, :mail)");

    query.bindValue(":id", id);
    query.bindValue(":lastname", ui->lastnameLine->text());
    query.bindValue(":telephone", ui->telephoneLine->text());
    query.bindValue((":mail"), ui->mailLine->text());

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return false;
    }

    return true;
}

bool Regwindow::entry(){
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

    if(!query.first()){
            qDebug()<<query.lastError().text();
            return false;
        }
    if(query.value(0).toBool()){
        User::setId(query.value(1).toString());
        setUser();
        return true;
    }

    qDebug()<<"wrong login or password";
    return false;
}

void Regwindow::setUser(){
    QSqlQuery query = QSqlQuery(db);

    query.prepare("select lastname from user_info where u_id = :id");

    query.bindValue(":id",User::getId());
    query.bindValue("lastname", QVariant(0));

    if(!query.exec()){
        qDebug()<<query.lastError().text();
        return ;
    }

    if(query.next()){
        User::setUsername(query.value(0).toString());
        return ;
    }
    return ;
}

void Regwindow::cancellButton_clicked(){
    this->close();
}


void Regwindow::isShowPass(){
    if(ui->passRadioButton->isChecked()){
        ui->reppassLine->setEchoMode(QLineEdit::Normal);
    }else{
        ui->reppassLine->setEchoMode(QLineEdit::Password);
    }
}
