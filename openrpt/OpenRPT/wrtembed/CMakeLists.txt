cmake_minimum_required(VERSION 2.8.7)

PROJECT(wrtembed)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ../common/ ../../common/ ../images/ ../renderer/ ${CMAKE_CURRENT_BINARY_DIR})


QT5_WRAP_UI(uis labeleditor.ui 
           labeldefinitions.ui 
           labeldefinitioneditor.ui 
           queryeditor.ui 
           querylist.ui 
           pagesetup.ui 
           sectioneditor.ui 
           detailsectiondialog.ui 
           fieldeditor.ui 
           texteditor.ui 
           barcodeeditor.ui 
           reportproperties.ui 
           reportparameter.ui 
           reportparameterlist.ui 
           reportparameterlistitem.ui 
           imageeditor.ui 
           coloreditor.ui 
           colorlist.ui 
           grapheditor.ui 
           detailgroupsectiondialog.ui 
           editpreferences.ui 
           dbfiledialog.ui 
           crosstabeditor.ui 
           dmatrixsquareconfig.ui 
           dmatrixrectconfig.ui 
           dbarcodeconfig.ui 
           dmatrixpreview.ui 
           patheditor.ui )

QT5_ADD_RESOURCES(qrc ../images/OpenRPTWrtembed.qrc 
             ../images/OpenRPTCommon.qrc)

add_library(wrtembed ${uis} ${qrc} reportgridoptions.cpp
           reporthandler.cpp 
           documentwindow.cpp 
           documentview.cpp 
           documentscene.cpp 
           graphicsitems.cpp 
           graphicssection.cpp 
           labeldefinitions.cpp 
           labeldefinitioneditor.cpp 
           labeleditor.cpp 
           queryeditor.cpp 
           querylist.cpp 
           pagesetup.cpp 
           sectioneditor.cpp 
           detailsectiondialog.cpp 
           fieldeditor.cpp 
           texteditor.cpp 
           barcodeeditor.cpp 
           reportproperties.cpp 
           reportparameter.cpp 
           reportparameterlist.cpp 
           reportparameterlistitem.cpp 
           imageeditor.cpp 
           coloreditor.cpp 
           colorlist.cpp 
           grapheditor.cpp 
           detailgroupsectiondialog.cpp 
           editpreferences.cpp 
           dbfiledialog.cpp 
           crosstabeditor.cpp 
           querycombobox.cpp 
           fontutils.cpp 
           ../common/labelsizeinfo.cpp 
           ../common/pagesizeinfo.cpp 
           ../common/builtinSqlFunctions.cpp 
           ../common/builtinformatfunctions.cpp 
           dmatrixsquareconfig.cpp 
           dmatrixrectconfig.cpp 
           dbarcodeconfig.cpp 
           dmatrixpreview.cpp 
           patheditor.cpp )




target_link_libraries(wrtembed Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)

