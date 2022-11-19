QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chatserverform.cpp \
    chattingform.cpp \
    chattingthread.cpp \
    clientmanagerform.cpp \
    delegate.cpp \
    logthread.cpp \
    main.cpp \
    mainwindow.cpp \
    orderdelegate.cpp \
    ordermanagerform.cpp \
    productdelegate.cpp \
    productmanagerform.cpp

HEADERS += \
    chatserverform.h \
    chattingform.h \
    chattingthread.h \
    clientmanagerform.h \
    delegate.h \
    logthread.h \
    mainwindow.h \
    orderdelegate.h \
    ordermanagerform.h \
    productdelegate.h \
    productmanagerform.h

FORMS += \
    chatserverform.ui \
    chattingform.ui \
    clientmanagerform.ui \
    mainwindow.ui \
    ordermanagerform.ui \
    productmanagerform.ui

TRANSLATIONS += \
    Osstem_Project_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=
