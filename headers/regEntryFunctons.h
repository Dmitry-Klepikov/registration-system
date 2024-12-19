#ifndef REGENTRYFUNCTONS_H
#define REGENTRYFUNCTONS_H
#include <QCryptographicHash>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QProcess>

extern QString root;
extern QString path;
extern QString localPatch;

QString changeExtension(const QString& path, const QString& oldExt, const QString& newExt);
QString hashString(const QString &input);
void callMessageBox(const QString msg);
bool makeArhive(QString s1, QString s2, QString pass);
bool deleteArhive(QString path, QString fileName, QString pass);

#endif // REGENTRYFUNCTONS_H

