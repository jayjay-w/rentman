project (rentmanager)
 
cmake_minimum_required(VERSION 2.8.7)

set(QT5 OFF)
set(MAKE_TESTS OFF)

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
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5PrintSupport REQUIRED)
set(CMAKE_AUTOMOC ON)
else()
message(STATUS "Using Qt4")
find_package(Qt4 REQUIRED)
add_definitions(${QT_DEFINITIONS})
SET(QT_USE_QTXML TRUE)
SET(QT_USE_QTNETWORK TRUE)
SET(QT_USE_QTSQL TRUE)
include(${QT_USE_FILE})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)


if(QT5)
set(libs Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)
else()
set(libs ${QT_LIBRARIES})
endif()

if(MAKE_TESTS)
add_subdirectory(testapp)
endif(MAKE_TESTS)
add_subdirectory(openrpt)
add_subdirectory(rentmanager)

ADD_CUSTOM_TARGET(win-nsi
                        COMMAND make
                        COMMAND makensis rentmanager.nsi
                )
