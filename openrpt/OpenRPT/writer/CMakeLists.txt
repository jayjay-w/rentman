cmake_minimum_required(VERSION 2.8.7)

PROJECT(writer)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
  ../common/ ../../common/ ../renderer/ ../wrtembed/ ../images/
  ${CMAKE_CURRENT_BINARY_DIR})


QT5_ADD_RESOURCES (qrcs writer.qrc)

add_executable(writer rptwrt.cpp 
           ../common/builtinSqlFunctions.cpp  
           reportwriterwindow.cpp)

target_link_libraries(writer common renderer wrtembed Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)


