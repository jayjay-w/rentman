#-------------------------------------------------
#
# Project created by QtCreator 2013-04-27T09:47:20
#
#-------------------------------------------------

QT       -= core gui
CONFIG  += qt warn_on staticlib
TARGET = xlslib
TEMPLATE = lib
DESTDIR += ../../build
DEFINES += XLSLIB_LIBRARY
DEFINES += 'PACKAGE_NAME=\'\"xlslib"\''
INCLUDEPATH += .
SOURCES += \
    common/overnew.cpp \
    oledoc/oleprop.cpp \
    oledoc/olefs.cpp \
    oledoc/oledoc.cpp \
    oledoc/binfile.cpp \
    xlslib/workbook.cpp \
    xlslib/unit.cpp \
    xlslib/summinfo.cpp \
    xlslib/sheetrec.cpp \
    xlslib/row.cpp \
    xlslib/record.cpp \
    xlslib/recdef.cpp \
    xlslib/range.cpp \
    xlslib/number.cpp \
    xlslib/note.cpp \
    xlslib/merged.cpp \
    xlslib/label.cpp \
    xlslib/index.cpp \
    xlslib/HPSF.cpp \
    xlslib/globalrec.cpp \
    xlslib/formula_expr.cpp \
    xlslib/formula_estimate.cpp \
    xlslib/formula_cell.cpp \
    xlslib/formula.cpp \
    xlslib/format.cpp \
    xlslib/font.cpp \
    xlslib/extformat.cpp \
    xlslib/err.cpp \
    xlslib/docsumminfo.cpp \
    xlslib/datast.cpp \
    xlslib/continue.cpp \
    xlslib/colors.cpp \
    xlslib/colinfo.cpp \
    xlslib/cell.cpp \
    xlslib/cbridge.cpp \
    xlslib/boolean.cpp \
    xlslib/blank.cpp \
    xlslib/assert_assist.cpp

HEADERS +=\
        xlslib_global.h \
    xlslib.h \
    common/xlsys.h \
    common/xlstypes.h \
    common/xls_pshpack4.h \
    common/xls_pshpack2.h \
    common/xls_pshpack1.h \
    common/xls_poppack.h \
    common/timespan.h \
    common/systype.h \
    common/stringtok.h \
    common/overnew.h \
    oledoc/oleprop.h \
    oledoc/olefs.h \
    oledoc/oledoc.h \
    oledoc/binfile.h \
    xlslib/workbook.h \
    xlslib/unit.h \
    xlslib/tostr.h \
    xlslib/summinfo.h \
    xlslib/sheetrec.h \
    xlslib/row.h \
    xlslib/rectypes.h \
    xlslib/record.h \
    xlslib/recdef.h \
    xlslib/range.h \
    xlslib/number.h \
    xlslib/note.h \
    xlslib/merged.h \
    xlslib/label.h \
    xlslib/index.h \
    xlslib/HPSF.h \
    xlslib/globalrec.h \
    xlslib/formula_expr.h \
    xlslib/formula_estimate.h \
    xlslib/formula_const.h \
    xlslib/formula_cell.h \
    xlslib/formula.h \
    xlslib/formtags.h \
    xlslib/format.h \
    xlslib/font.h \
    xlslib/extformat.h \
    xlslib/err.h \
    xlslib/docsumminfo.h \
    xlslib/datast.h \
    xlslib/continue.h \
    xlslib/common.h \
    xlslib/colors.h \
    xlslib/colinfo.h \
    xlslib/cell.h \
    xlslib/cbridge.h \
    xlslib/boolean.h \
    xlslib/blank.h \
    xlslib/biffsection.h \
    common/xlconfig.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE28ED3E4
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = xlslib.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

OTHER_FILES += \
    common/xlconfig.h.in \
    xlslib/formula.txt
