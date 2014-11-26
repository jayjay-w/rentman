cmake_minimum_required(VERSION 2.8.7)

PROJECT(common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(ui_files parameterproperties.ui 
           paramlistedit.ui 
           login.ui 
           loginOptions.ui 
           parameteredit.ui)

set(qrc_files ../OpenRPT/images/OpenRPTCommon.qrc)

set(h_files data.h 
          parameter.h 
          dbtools.h 
          xsqlquery.h 
          quuencode.h 
          loginOptions.h 
          xvariant.h 
          languageoptions.h 
          parameterproperties.h 
          paramlistedit.h 
          parameteredit.h 
          parsexmlutils.h 
          querysource.h 
          reportpageoptions.h 
          memdbloader.h)

set(cpp_files data.cpp 
          parameter.cpp 
          dbtools.cpp 
          xsqlquery.cpp 
          quuencode.cpp 
          login.cpp 
          loginOptions.cpp 
          xvariant.cpp 
          languageoptions.cpp 
          parameterproperties.cpp 
          paramlistedit.cpp 
          parameteredit.cpp 
          parsexmlutils.cpp 
          querysource.cpp 
          reportpageoptions.cpp 
          memdbloader.cpp)

if(QT5)
QT5_WRAP_UI(uis ${ui_files})
QT5_ADD_RESOURCES (qrc ${qrc_files})
add_library(common ${uis} ${qrc} ${cpp_files})
target_link_libraries(common ${libs})
else()
qt4_wrap_cpp(hdrs_moc ${h_files})
qt4_add_resources (qrc ${qrc_files})
qt4_wrap_ui(uis ${ui_files})
qt4_wrap_cpp(moc ${h_files})
add_library(common ${uis} ${qrc} ${cpp_files} ${moc})
target_link_libraries(common ${QT_LIBRARIES})
endif()
          

