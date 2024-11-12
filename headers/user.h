#ifndef USER_H
#define USER_H

#include <QString>



class User
{
    static QString username;
    static QString u_id;
public:
    static bool isActivated;
public:
    User();
    ~User();

    static void setUsername(QString u);
    static void setId(QString u);

    static QString getUsername();
    static QString getId();
};


#endif // USER_H
