cmake_minimum_required(VERSION 2.8.7)

PROJECT(writer)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ../common/ ../../common/ ../renderer/ ../wrtembed/ ../images/
  ${CMAKE_CURRENT_BINARY_DIR})


QT5_ADD_RESOURCES (qrcs writer.qrc)

if(MINGW)
    #MinGW resource compilation
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/writer.o
    COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/writer.rc
            -o ${CMAKE_CURRENT_BINARY_DIR}/writer.o )
    set(rc ${CMAKE_CURRENT_BINARY_DIR}/writer.o )
else(MINGW)
    #set(rc windows.rc)
endif(MINGW)

set(writerSources rptwrt.cpp
           ../common/builtinSqlFunctions.cpp
           reportwriterwindow.cpp )

if( APPLE )
    add_executable(writer MACOSX_BUNDLE ${writerSources} )
else( APPLE )
add_executable(writer WIN32 ${rc} ${writerSources})
endif( APPLE )

target_link_libraries(writer renderer common  wrtembed Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)


