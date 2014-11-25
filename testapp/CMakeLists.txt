 
cmake_minimum_required(VERSION 2.8.7)
project(testapp)

#IF(COMMAND cmake_policy)
# CMAKE_POLICY(SET CMP0003 NEW)
# CMAKE_POLICY(SET CMP0005 OLD)   # remove this and the quoted arguments in
#                                 # ADD_DEFINITONS() once CMake 2.6 is used
#ENDIF(COMMAND cmake_policy)

QT5_WRAP_UI(uis
    mainwindow.ui
)

#QT5_ADD_RESOURCES(qrcs res.qrc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

#if(MINGW)
#    #MinGW resource compilation
#    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/windows.o
#    COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/windows.rc
#            -o ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
#    set(rc ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
#else(MINGW)
#    #set(rc windows.rc)
#endif(MINGW)

set(src ${uis}
    main.cpp
    mainwindow.cpp
)



#if (APPLE)
#      set (ExecutableName RentManager )
#      set (MACOSX_BUNDLE_ICON_FILE      cottage.icns)
#      set (MACOSX_BUNDLE_GUI_IDENTIFIER org.smartsoftware.rentman)
#      set (MACOSX_BUNDLE_LONG_VERSION_STRING ${VERSION})
#      set (MACOSX_BUNDLE_BUNDLE_NAME RentManager)
#      set (MACOSX_BUNDLE_SHORT_VERSION_STRING ${VERSION})
#      set (MACOSX_BUNDLE_BUNDLE_VERSION ${VERSION})
#      set (MACOSX_BUNDLE_COPYRIGHT smartsoftware.co.ke)
#else (APPLE)
#      set (ExecutableName rentmanager)
#endif (APPLE)

#if(MINGW)
    set(ExecutableName testapp)
#endif(MINGW)

add_executable(${ExecutableName} WIN32 MACOSX_BUNDLE ${src})

#IF( APPLE )
#    SET( RentManagerResourceDIR ${CMAKE_BINARY_DIR}/bin/${ExecutableName}.app/Contents/Resources )
#    message(${RentManagerResourceDIR})
#    ADD_CUSTOM_COMMAND( TARGET ${ExecutableName} POST_BUILD
#        COMMAND mkdir ARGS -p ${RentManagerResourceDIR}
#        COMMAND cp ARGS -f ${CMAKE_CURRENT_SOURCE_DIR}/cottage.icns ${RentManagerResourceDIR} )
#ENDIF( APPLE )


target_link_libraries(${ExecutableName}
    Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)

#if(APPLE)
#    set(plistPath ${CMAKE_CURRENT_SOURCE_DIR}/rentman.plist.in)
#      set_target_properties (${ExecutableName}
#          PROPERTIES
#          MACOSX_BUNDLE_INFO_PLIST ${plistPath})
#endif(APPLE)
