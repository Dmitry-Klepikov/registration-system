#include "headers/regEntryFunctons.h"
#include <QDebug>

QString root;
QString path;
QString localPatch;

QString changeExtension(const QString& path, const QString& oldExt, const QString& newExt)
{
    QFile file(path);
    QString newPath = path;
    newPath.replace(oldExt, newExt);
    file.rename(newPath);
    return newPath;
}

QString hashString(const QString &input) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(input.toUtf8());
    return hash.result().toHex();
}

void callMessageBox(const QString msg){
    QMessageBox msgbox;
    msgbox.setText(msg);
    msgbox.exec();
}


bool makeArhive(QString path, QString fileName, QString pass){
    // Создать процесс для запуска 7z
    QProcess process;
    process.setProgram("C:/Program Files/7-Zip/7z.exe");

    // Добавить аргументы для создания архива
    QStringList arguments;
    arguments << "a"<< "-p"+pass << fileName + ".zip"  << fileName;
    process.setArguments(arguments);

    // Запустить процесс
    process.start();

    // Дождаться завершения процесса
    process.waitForFinished();

    QFile zip(fileName);
    zip.remove();

    // Проверить результат
    if (process.exitCode() == 0) {
        return true;
    } else {
        return false;
    }
}

bool deleteArhive(QString path, QString fileName, QString pass){
    // Создать процесс для запуска 7z
        QProcess process;
        process.setProgram("C:/Program Files/7-Zip/7z.exe");

        // Добавить аргументы для извлечения архива
        QStringList arguments;
        arguments << "e" <<path+"/"+fileName<<"-p"+pass<<"-o"+path;
        qDebug()<<path+"/"+fileName<<pass<<path;
        process.setArguments(arguments);

        // Запустить процесс
        process.start();

        // Дождаться завершения процесса
        process.waitForFinished();
        QFile zip(path+"/"+fileName);
        zip.remove();
        // Проверить результат
        if (process.exitCode() == 0) {
            return true;
        } else {
            return false;
        }
}
