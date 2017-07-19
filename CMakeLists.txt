project (rentmanager)
 
cmake_minimum_required(VERSION 2.8.7)

IF(COMMAND cmake_policy)
 CMAKE_POLICY(SET CMP0003 NEW)
CMAKE_POLICY(SET CMP0020 NEW)
 CMAKE_POLICY(SET CMP0005 OLD)   # remove this and the quoted arguments in
                                 # ADD_DEFINITONS() once CMake 2.6 is used
ENDIF(COMMAND cmake_policy)

set(QT5 ON)
set(MAKE_TESTS ON)

if(MINGW)
set(QT5 ON)
endif()

if(APPLE)
set(QT5 ON)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "1")
SET(VERSION_SUFFIX "alpha")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
IF(VERSION_SUFFIX)
	SET (VERSION "${VERSION}-${VERSION_SUFFIX}")
ENDIF(VERSION_SUFFIX)

if(QT5)
message(STATUS "Using Qt5")
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
set(CMAKE_AUTOMOC ON)
else()
message(STATUS "Using Qt4")
find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
SET(QT_USE_QTSQL TRUE)
SET(QT_USE_QTWEBKIT TRUE)
include(${QT_USE_FILE})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


if(QT5)
set(libs Qt5::Widgets Qt5::Sql Qt5::PrintSupport Qt5::WebKitWidgets)
else()
set(libs ${QT_LIBRARIES})
endif()

if(MAKE_TESTS)
add_subdirectory(tests/qgraphics)
endif(MAKE_TESTS)
add_subdirectory(rentmanager)
add_subdirectory(xlslib)

ADD_CUSTOM_TARGET(win-nsi-qt4
                        COMMAND make
                        COMMAND makensis rentmanager-qt4.nsi
                )
