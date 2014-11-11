#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T17:03:03
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RentManager
TEMPLATE = app

DESTDIR = ../build

SOURCES += main.cpp\
        rentmanagermainwindow.cpp \
    publics.cpp \
    companyinitializationdialog.cpp \
    companiesdialog.cpp \
    propertiesdialog.cpp \
    unitsdialog.cpp \
    tenantsdialog.cpp \
    assignunittotenantdialog.cpp \
    terminateoccupationdialog.cpp \
    receivepaymentdialog.cpp \
    paymentsdialog.cpp

HEADERS  += rentmanagermainwindow.h \
    publics.h \
    companyinitializationdialog.h \
    companiesdialog.h \
    propertiesdialog.h \
    unitsdialog.h \
    tenantsdialog.h \
    assignunittotenantdialog.h \
    terminateoccupationdialog.h \
    receivepaymentdialog.h \
    paymentsdialog.h

FORMS    += rentmanagermainwindow.ui \
    companyinitializationdialog.ui \
    companiesdialog.ui \
    propertiesdialog.ui \
    unitsdialog.ui \
    tenantsdialog.ui \
    assignunittotenantdialog.ui \
    terminateoccupationdialog.ui \
    receivepaymentdialog.ui \
    paymentsdialog.ui

RESOURCES += \
    res.qrc

RC_FILE = Windows.rc

ICON = Cottage.icns

OTHER_FILES += \
    Windows.rc
