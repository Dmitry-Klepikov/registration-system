QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    askwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    regwindow.cpp \
    src/askwindow.cpp \
    src/loginwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/regwindow.cpp \
    src/user.cpp \
    user.cpp

HEADERS += \
    askwindow.h \
    headers/askwindow.h \
    headers/loginwindow.h \
    headers/mainwindow.h \
    headers/regwindow.h \
    headers/user.h \
    loginwindow.h \
    mainwindow.h \
    regwindow.h \
    user.h

FORMS += \
    askwindow.ui \
    forms/askwindow.ui \
    forms/loginwindow.ui \
    forms/mainwindow.ui \
    forms/regwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    regwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
