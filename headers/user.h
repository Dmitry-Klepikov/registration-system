#ifndef USER_H
#define USER_H

#include <QString>



class User
{
    static QString username;
    static QString u_id;
    static QString u_access;

public:
    User();
    ~User();

    static void setUsername(QString u);
    static void setId(QString u);
    static void setAccess(QString u);

    static QString getUsername();
    static QString getId();
    static QString getAccess();


};


#endif // USER_H
