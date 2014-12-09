cmake_minimum_required(VERSION 2.8.7)
project(qgraphics)

set(ui_files mainw.ui )

set(h_files
    mainw.h )

set(cpp_files main.cpp mainw.cpp)


if(QT5)
qt5_wrap_ui(uis ${ui_files})
qt5_add_resources(qrcs ${qrc_files})
set(moc )
else()
qt4_wrap_ui(uis ${ui_files})
qt4_add_resources(qrcs ${qrc_files})
qt4_wrap_cpp(moc ${h_files})
endif()

set(src ${uis} ${cpp_files} ${moc})

set(ExecutableName qgraphics)
add_executable(${ExecutableName} WIN32 MACOSX_BUNDLE ${src})
target_link_libraries(${ExecutableName} xlslib ${libs})
