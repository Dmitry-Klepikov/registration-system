#include "headers/user.h"


QString User::username = "N/A";
QString User::u_id = "0";
bool User::isActivated = false;

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
