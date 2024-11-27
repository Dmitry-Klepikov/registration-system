#include "headers/regEntryFunctons.h"

QString root;
QString path;
QString localPatch;

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
