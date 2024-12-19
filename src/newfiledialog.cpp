#include "headers/newfiledialog.h"
#include "ui_newfiledialog.h"
#include <QLockFile>

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
    QString filePath = root+"//"+ui->nameLineEdit->text()+".txt";
    QFile file(filePath);
    QString hashFile;
    if(file.open(QIODevice::WriteOnly)){
        hashFile = hashString(" ");
        file.write(" ");
        file.close();
        qDebug()<<hashFile<<"new";
    }else{
        return ;
    }

     filePath = changeExtension(filePath,"txt","secretextension");
//     if(pass == ""){
         QString pass = User::getUsername();
//     }
     qDebug()<<"pass to close: "+pass;
     if(makeArhive(root, filePath, pass)){
         qDebug()<<"makeArh"<<filePath+".7z";

     }


    QSqlQuery query(db);
    query.prepare(  "INSERT INTO [dbo].[files_access] "
                    "(file_name   "
                    ",file_access,"
                    "modified_by, "
                    "hash ) "
                    " VALUES    "
                    "( :file_name,  "
                    "  :access, "
                    " :modified_by, "
                    "   :hash)");
    query.bindValue(":file_name", ui->nameLineEdit->text()+".secretextension");
    query.bindValue(":access", ui->accessComboBox->currentText().toInt());
    query.bindValue(":modified_by",  User::getUsername());

    query.bindValue(":hash", hashFile);
    if(!query.exec()){
        qDebug()<<"err update";
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
    query.bindValue(":file_name", ui->nameLineEdit->text()+".secretextension");
    if(!query.exec()){
        qDebug()<<"err update";
        return false;
    }
    if(query.first()){
        callMessageBox("файл с таким именем уже существует");
        return false;
    }
    return true;
}

