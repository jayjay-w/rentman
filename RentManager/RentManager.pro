#-------------------------------------------------
#
# Project created by QtCreator 2014-11-09T17:03:03
#
#-------------------------------------------------

QT += xml sql network widgets printsupport

TARGET = RentManager
TEMPLATE = app

INCLUDEPATH += ../openrpt/common ../openrpt/OpenRPT/renderer ../openrpt/OpenRPT/common ../openrpt/OpenRPT/images
#LIBS += -L../openrpt/lib  -lwrtembed -lcommon -lrenderer -lDmtx_Library
LIBS += -L../openrpt/lib -lrenderer -lcommon -lDmtx_Library

DESTDIR = ../build


win32-msvc* {
  PRE_TARGETDEPS += ../openrpt/lib/wrtembed.lib \
                    ../openrpt/lib/common.lib   \
                    ../openrpt/lib/renderer.lib
} else {
  PRE_TARGETDEPS += ../openrpt/lib/libwrtembed.a \
                    ../openrpt/lib/libcommon.a   \
                    ../openrpt/lib/librenderer.a
}

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
    paymentsdialog.cpp \
    myprinter.cpp

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
    paymentsdialog.h \
    myprinter.h

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

macx:ICON = Cottage.icns

OTHER_FILES += \
    Windows.rc
