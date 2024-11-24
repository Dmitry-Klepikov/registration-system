#include "headers/regEntryFunctons.h"

QString hashString(const QString &input) {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(input.toUtf8());
    return hash.result().toHex();
}

void callMessageBox(QString msg){
    QMessageBox msgbox;
    msgbox.setText(msg);
    msgbox.exec();
}
