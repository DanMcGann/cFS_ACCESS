cmake_minimum_required(VERSION 2.6.4)
project(ACCESS_LIB C)

include_directories(fsw/public_inc)

include_directories(${CFECORE_SOURCE_DIR}/src/inc)
include_directories(${CFEOSO_SOURCE_DIR}/fsw/inc)

aux_source_directory(fsw/src LIB_SRC_FILES)

#create the app module
add_cfe_app(access_lib ${LIB_SRC_FILES})
