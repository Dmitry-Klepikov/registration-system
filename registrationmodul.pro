QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/newfiledialog.cpp \
    src/activationwindow.cpp \
    src/loginwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/regEntryFunctions.cpp \
    src/regwindow.cpp \
    src/user.cpp \

HEADERS += \
    headers/activationwindow.h \
    headers/loginwindow.h \
    headers/mainwindow.h \
    headers/regEntryFunctons.h \
    headers/regwindow.h \
    headers/user.h \
    headers/newfiledialog.h

FORMS += \
    forms/activationwindow.ui \
    forms/loginwindow.ui \
    forms/mainwindow.ui \
    forms/regwindow.ui \
    forms/newfiledialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
