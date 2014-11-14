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

add_executable(rentmanager ${uis} ${qrcs}
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

target_link_libraries(rentmanager renderer common Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)