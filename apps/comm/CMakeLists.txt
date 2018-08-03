cmake_minimum_required(VERSION 2.6.4) 
project(ACCESS C)

include_directories(fsw/src)
include_directories(fsw/mission_inc)
include_directories(fsw/platform_inc)
include_directories(${cfs_lib_MISSION_DIR}/fsw/public_inc)
include_directories(${access_lib_MISSION_DIR}/fsw/public_inc)

## File Transfer Incldues ##
include_directories(${cf_MISSION_DIR}/fsw/src)
include_directories(${cf_MISSION_DIR}/fsw/platform_inc)

aux_source_directory(fsw/src APP_SRC_FILES)
aux_source_directory(fsw/tables APP_TBL_FILES)

#create the app module
add_cfe_app(comm ${APP_SRC_FILES})

#create the app tables 
add_cfe_tables(comm ${APP_TBL_FILES})
