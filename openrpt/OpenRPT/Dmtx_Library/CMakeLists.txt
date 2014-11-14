cmake_minimum_required(VERSION 2.8.7)

PROJECT(Dmtx_Library)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})



add_library(Dmtx_Library 
    simple_test.c 
    dmtxvector2.c 
    dmtxtime.c 
    dmtxsymbol.c 
    dmtxscangrid.c 
    dmtxregion.c 
    dmtxreedsol.c 
    dmtxplacemod.c 
    dmtxmessage.c 
    dmtxmatrix3.c 
    dmtximage.c 
    dmtxencodestream.c 
    dmtxencodescheme.c 
    dmtxencodeoptimize.c 
    dmtxencodeedifact.c 
    dmtxencodec40textx12.c 
    dmtxencodebase256.c 
    dmtxencodeascii.c 
    dmtxencode.c 
    dmtxdecodescheme.c 
    dmtxdecode.c 
    dmtxbytelist.c 
    dmtx.c )




target_link_libraries(Dmtx_Library Qt5::Widgets Qt5::Xml Qt5::Network Qt5::Sql Qt5::PrintSupport)
