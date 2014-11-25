cmake_minimum_required(VERSION 2.8.7)
project(testapp)


QT4_WRAP_UI(uis
    mainwindow.ui
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

set(src ${uis}
    main.cpp
    mainwindow.cpp
)

set(hdrs mainwindow.h)

set(ExecutableName testapp)

qt4_wrap_cpp(hdrs_moc ${hdrs})
add_executable(${ExecutableName} WIN32 MACOSX_BUNDLE ${src} ${hdrs_moc})


target_link_libraries(${ExecutableName} ${QT_LIBRARIES})


