cmake_minimum_required(VERSION 2.8.7)

PROJECT(common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})


QT5_WRAP_UI(uis parameterproperties.ui 
           paramlistedit.ui 
           login.ui 
           loginOptions.ui 
           parameteredit.ui)

QT5_ADD_RESOURCES (qrc ../OpenRPT/images/OpenRPTCommon.qrc)

add_library(common ${uis} ${qrc} data.cpp 
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

          
target_link_libraries(common Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)

