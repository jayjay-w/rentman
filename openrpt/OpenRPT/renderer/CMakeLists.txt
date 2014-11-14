cmake_minimum_required(VERSION 2.8.7)

PROJECT(renderer)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ../common/ ../../common/ ${CMAKE_CURRENT_BINARY_DIR})



add_library(renderer openreports.cpp 
          3of9.cpp 
          ext3of9.cpp 
          i2of5.cpp 
          code128.cpp 
          codeean.cpp 
          crosstab.cpp 
          graph.cpp 
          orutils.cpp 
          orprerender.cpp 
          orprintrender.cpp 
          renderobjects.cpp 
          previewdialog.cpp 
          labelpaintengine.cpp 
          labelprintengine.cpp 
          satopaintengine.cpp 
          satoprintengine.cpp 
          zebrapaintengine.cpp 
          zebraprintengine.cpp 
          reportprinter.cpp 
          textelementsplitter.cpp 
          ../../MetaSQL/metasql.cpp 
          ../../MetaSQL/metasqlqueryparser.cpp 
          ../../MetaSQL/regex/regex.c 
          ../common/builtinformatfunctions.cpp 
          ../common/builtinSqlFunctions.cpp 		# MANU
          ../common/labelsizeinfo.cpp 
          ../common/pagesizeinfo.cpp 
          datamatrix.cpp )

target_link_libraries(renderer Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)
