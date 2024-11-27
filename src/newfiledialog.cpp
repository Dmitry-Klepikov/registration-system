#include "headers/newfiledialog.h"
#include "ui_newfiledialog.h"

NewFileDialog::NewFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    switch (User::getAccess().toInt()) {
    case 3:
        ui->accessComboBox->addItem("3");
    case 2:
        ui->accessComboBox->addItem("2");
    case 1:
        ui->accessComboBox->addItem("1");
        break;
    }
    ui->accessComboBox->setCurrentIndex(2);
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}

void NewFileDialog::on_buttonBox_accepted()
{
    if(!checkName()){
        return ;
    }
    QFile file(root+"//"+ui->nameLineEdit->text()+".txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)){
        file.close();
    }else{
        return ;
    }

    QSqlQuery query(db);
    query.prepare(  "INSERT INTO [dbo].[files_access] "
                    "(file_name   "
                    ",file_access)"
                    " VALUES    "
                    "( :file_name,  "
                    "  :access)");
    query.bindValue(":file_name", ui->nameLineEdit->text()+".txt");
    query.bindValue(":access", ui->accessComboBox->currentText().toInt());
    if(!query.exec()){
        qDebug()<<"err updete";
    }

}

bool NewFileDialog::checkName(){
    if(ui->nameLineEdit->text().contains('.')){
        callMessageBox("В имени создоваемого файла не должен присутствовать символ '.'");
        return false;
    }
    db = QSqlDatabase::database(localPatch);
    QSqlQuery query(db);
    query.prepare("select file_name from files_access where file_name = :file_name");
    query.bindValue(":file_name", ui->nameLineEdit->text()+".txt");
    if(!query.exec()){
        qDebug()<<"err updete";
        return false;
    }
    if(query.first()){
        callMessageBox("файл с таким именем уже существует");
        return false;
    }
    return true;
}

