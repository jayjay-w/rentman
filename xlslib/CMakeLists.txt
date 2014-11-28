cmake_minimum_required(VERSION 2.8.7)

PROJECT(xlslib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/xlslib
)

add_library(xlslib
    common/overnew.cpp
    oledoc/oleprop.cpp
    oledoc/olefs.cpp
    oledoc/oledoc.cpp
    oledoc/binfile.cpp
    xlslib/workbook.cpp
    xlslib/unit.cpp
    xlslib/summinfo.cpp
    xlslib/sheetrec.cpp
    xlslib/row.cpp
    xlslib/record.cpp
    xlslib/recdef.cpp
    xlslib/range.cpp
    xlslib/number.cpp
    xlslib/note.cpp
    xlslib/merged.cpp
    xlslib/label.cpp
    xlslib/index.cpp
    xlslib/HPSF.cpp
    xlslib/globalrec.cpp
    xlslib/formula_expr.cpp
    xlslib/formula_estimate.cpp
    xlslib/formula_cell.cpp
    xlslib/formula.cpp
    xlslib/format.cpp
    xlslib/font.cpp
    xlslib/extformat.cpp
    xlslib/err.cpp
    xlslib/docsumminfo.cpp
    xlslib/datast.cpp
    xlslib/continue.cpp
    xlslib/colors.cpp
    xlslib/colinfo.cpp
    xlslib/cell.cpp
    xlslib/cbridge.cpp
    xlslib/boolean.cpp
    xlslib/blank.cpp
    xlslib/assert_assist.cpp
)

target_link_libraries(xlslib ${libs})

