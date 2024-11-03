#ifndef USER_H
#define USER_H

#include <QString>



class User
{
    static QString username;

public:
    User();
    ~User();
    static void setUsername(QString u);
    static QString getUsername();
};


#endif // USER_H
