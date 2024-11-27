#ifndef REGENTRYFUNCTONS_H
#define REGENTRYFUNCTONS_H
#include <QCryptographicHash>
#include <QString>
#include <QMessageBox>

extern QString root;
extern QString path;
extern QString localPatch;

QString hashString(const QString &input);
void callMessageBox(const QString msg);

#endif // REGENTRYFUNCTONS_H

