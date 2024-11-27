#include "headers/user.h"


QString User::username = "N/A";
QString User::u_id = "0";
QString User::u_access;

void User::setUsername(QString u){
    username = u;
}

QString User::getUsername(){
    return username;
}

void User::setId(QString u){
    u_id = u;
}

QString User::getId(){
    return u_id;
}

void User::setAccess(QString u){
    u_access = u;
}
QString User::getAccess(){
    return u_access;
}

