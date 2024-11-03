#include "user.h"


QString User::username = "N/A";
void User::setUsername(QString u){
    username = u;
}

QString User::getUsername(){
    return username;
}
