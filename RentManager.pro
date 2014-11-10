#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T17:03:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RentManager
TEMPLATE = app


SOURCES += main.cpp\
        rentmanagermainwindow.cpp \
    publics.cpp \
    companyinitializationdialog.cpp \
    companiesdialog.cpp \
    propertiesdialog.cpp

HEADERS  += rentmanagermainwindow.h \
    publics.h \
    companyinitializationdialog.h \
    companiesdialog.h \
    propertiesdialog.h

FORMS    += rentmanagermainwindow.ui \
    companyinitializationdialog.ui \
    companiesdialog.ui \
    propertiesdialog.ui
