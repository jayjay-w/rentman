cmake_minimum_required(VERSION 2.8.7)

PROJECT(rentmanager)

QT5_WRAP_UI(uis 
    rentmanagermainwindow.ui  
    companyinitializationdialog.ui  
    companiesdialog.ui  
    propertiesdialog.ui  
    unitsdialog.ui  
    tenantsdialog.ui  
    assignunittotenantdialog.ui  
    terminateoccupationdialog.ui  
    receivepaymentdialog.ui  
    paymentsdialog.ui  
    createinvoicedialog.ui  
    multiinvoicingdialog.ui
)

QT5_ADD_RESOURCES(qrcs res.qrc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
  ../openrpt/common/ ../openrpt/OpenRPT/common/ ../openrpt/OpenRPT/renderer/
  ${CMAKE_CURRENT_BINARY_DIR})

if(MINGW)
    #MinGW resource compilation
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/windows.o
    COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -i${CMAKE_CURRENT_SOURCE_DIR}/windows.rc
            -o ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
    set(rc ${CMAKE_CURRENT_BINARY_DIR}/windows.o )
else(MINGW)
    #set(rc windows.rc)
endif(MINGW)

add_executable(rentmanager WIN32 ${rc} ${uis} ${qrcs}
    main.cpp 
    rentmanagermainwindow.cpp  
    publics.cpp  
    companyinitializationdialog.cpp  
    companiesdialog.cpp  
    propertiesdialog.cpp  
    unitsdialog.cpp  
    tenantsdialog.cpp  
    assignunittotenantdialog.cpp  
    terminateoccupationdialog.cpp  
    receivepaymentdialog.cpp  
    paymentsdialog.cpp  
    myprinter.cpp  
    createinvoicedialog.cpp  
    multiinvoicingdialog.cpp
)


install(TARGETS rentmanager RUNTIME DESTINATION bin)

target_link_libraries(rentmanager renderer common Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)
