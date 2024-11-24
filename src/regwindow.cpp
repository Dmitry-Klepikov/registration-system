#include "headers/regwindow.h"
#include "ui_regwindow.h"

Regwindow::Regwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regwindow)
{
    ui->setupUi(this);
    setWindowModality(Qt::ApplicationModal);
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

    if (!checkLogin()){
        return;
    }

    if(!checkPass()){
        return ;
    }

    if(!checkLastname()){
        return;
    }

    if(!checkTelephone()){
        return;
    }

    if(!checkEmail()){
        return;
    }

    if (registration()){
        callMessageBox("Регистрация выполнена успешно!");
           this->close();
    }
}



bool Regwindow::checkPass(){
    if(ui->passLine->text().isEmpty()){
        callMessageBox("Введите пароль");
        return false;
    }

    if(ui->passLine->text().length() < 6){
        callMessageBox("длина пароля должна составлять не менее 6 символов");
        return false;
    }

    if(!isCorrectPass()){
        callMessageBox("пароль должен состоять как из символов так и из цифр");
        return false;
    }

    if(ui->reppassLine->text().isEmpty()){
        callMessageBox("Введите проверку пароля");
        return false;
    }

    if(ui->passLine->text() != ui->reppassLine->text()){
        callMessageBox("Пароли разные");
        return false;
    }

    return true;
}


bool Regwindow::checkLastname(){
    if(ui->lastnameLine->text().isEmpty()){
        callMessageBox("Введите ФИО");
        return false;
    }

    QSqlQuery query(db);

    query.prepare("select count(lastname)  from user_info where lastname = :userName");

    query.bindValue(":userName", ui->lastnameLine->text());

    if(!query.exec()){
        callMessageBox(query.lastError().text());
        return false;
    }

    if(query.first()){
        return true;
    }

    return false;
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

bool Regwindow::isCorrectLogin(){

    bool hasLetter = false;

    for(char c : static_cast<std::string>(ui->loginLine->text().toLatin1())){

        if(isalpha(c)){
            hasLetter = true;
        }
    }

    return !hasLetter;
}

bool Regwindow::checkLogin(){
    if(ui->loginLine->text().isEmpty()){
        callMessageBox("Введите логин");
        return false;
    }

    if(ui->loginLine->text().length()!=4){
        callMessageBox("Логин должен состоять не более чем из 4 знаков");
        return false;
    }

    if(!isCorrectLogin()){
        callMessageBox("Логин должен состоять из цифр");
        return false;
    }

    QSqlQuery query(db);

    query.prepare("select count(login)  from user_logins where login = :userName");

    query.bindValue(":userName", ui->loginLine->text());

    if(!query.exec()){
        callMessageBox(query.lastError().text());
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            callMessageBox("такой логин уже зарегистрирован");
        }else{
            return true;
        }
    }
    return false;
}

bool Regwindow::checkEmail(){
    if(ui->mailLine->text().isEmpty()){
        callMessageBox("Введите почту");
        return false;
    }

    QSqlQuery query(db);

    query.prepare("select count(email)  from user_info where email = :userMail");

    query.bindValue(":userMail", ui->mailLine->text());

    if(!query.exec()){
        callMessageBox(query.lastError().text());
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            callMessageBox("такая почта уже зарегистрирована");
        }else{
            return true;
        }
    }
    callMessageBox("ошибка обработки запроса");
    return false;
}

bool Regwindow::checkTelephone(){
    if(ui->telephoneLine->text().isEmpty()){
        callMessageBox("Введите номер телефона");
        return false;
    }

    if(!isCorrectPhone()){
        callMessageBox("Номер телефона содержит символы, отличные от цифр");
        return false;
    }

    QSqlQuery query(db);

    query.prepare("select count(telephone)  from user_info where telephone = :userTelephone");

    query.bindValue(":userTelephone", ui->telephoneLine->text());

    if(!query.exec()){
        callMessageBox(query.lastError().text());
        return false;
    }

    if(query.first()){
        if(query.value(0).toBool()){
            callMessageBox("такой телефон уже зарегистрирован");
        }else{
            return true;
        }
    }
    callMessageBox("ошибка обработки запроса");
    return false;
}

bool Regwindow::isCorrectPhone(){
    bool hasLetter = false;

    for(char c : static_cast<std::string>(ui->telephoneLine->text().toLatin1())){

        if(isalpha(c)){
            hasLetter = true;
        }
    }

    return !hasLetter;
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
    query.bindValue(":password", hashString(ui->passLine->text())); // записываем хеш пароля в бд

    if(!query.exec()){
        callMessageBox(query.lastError().text());
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
    query.bindValue(":Pass", hashString(ui->passLine->text()));
    query.bindValue("@Result",QVariant(0));
    query.bindValue("@id",QVariant(1));

    if(!query.exec()){
        callMessageBox(query.lastError().text());
        return 0;
    }

    if(!query.first()){
        callMessageBox(query.lastError().text());
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
        callMessageBox(query.lastError().text());
        return false;
    }

    return true;
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
