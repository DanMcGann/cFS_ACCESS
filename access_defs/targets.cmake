################################################################################
#   File: targets.cmake
#
#   Describes the target architecture and apps to use on each CPU
#
#   NOTE: This file should be customized for each mission which the ACCESS
#   framework is beings used. Items that should be configured for mission 
#   specific parameters are marked with a ##TODO## flag. This flag is followed 
#   by a short description of what the Mission Flight Software Team should 
#   input as a parameter.
#
#   AUTHOR: Daniel McGann   
################################################################################

# MISSION TARGET INFORMATION 
################################################################################

#   MISSION_NAME
#
#   The name of the mission for which this flight software is used
#
SET(MISSION_NAME "ACCESS_MISSION_1") ##TODO## Set mission specific name

#   SPACECRAFT_ID
#
#   The unique spacecraft identification number for the spacecraft that will 
#   run off of this flight software.
#
SET(SPACECRAFT_ID 1) ##TODO## Set mission specific spacecraft id


# ACCESS TARGET INFORMATION 
################################################################################

#   ACCESS_REQURED_APPLIST
#
#   List of applications that are required for the ACCESS Framework to function. 
#
SET(ACCESS_REQUIRED_APPLIST cfs_lib satcheck_lib sch ds sc hs lc access_lib conds gsds sa sem comm)

#   ACCESS_CONFIG_APPLIST
#
#   List of applications whos functionality is required for ACCESS to work, but
#   can be configured for mission specific requirements. 
#
#   ex. cf (file transfer application) can be substituted with another file 
#   transfer application that may use different propocalls. 
#   SEE access_mission_cfg.h for more information 
#
SET(ACCESS_CONFIG_APPLIST cf) ##TODO## Replace any needed configable access apps

#   ACCESS_APPLIST
#
#   List of all applications used by the ACCESS Framework.
#
SET(ACCESS_APPLIST ${ACCESS_REQUIRED_APPLIST} ${ACCESS_CONFIG_APPLIST})

# TARGET INFORMATION 
################################################################################

#   TGT1_NAME 
#
#   The name of the processor target on which the specified Flight
#   software will run. 
#
#   By default this specifies the cross compiler
#   toolchain that will be used to beuild the system. The build 
#   script will look for a toolchain file titled 
#   toolchain-${TGT1_NAME}.cmake
#   
#   NOTE: This variable can also be set using the TGT1 Platform
#       variable which will override this default behavior. This    
#       allows the name and he toolchain to be specified independently
#
SET(TGT1_NAME cpu1) ##TODO## set the CPU name that this FSW will run on

#   TGT1_MISSION_APPLIST
#
#   The names of the mission specific cFS applications to be build for 
#   this target
#
SET(TGT1_MISSION_APPLIST ) ##TODO## List all mission specific applications

#   TGT1_APPLIST
#
#   The names of all applications that will be build to this target
#   By default all ACCESS Applications are build to this target
#
SET(TGT1_APPLIST ${ACCESS_APPLIST} ${TGT1_MISSION_APPLIST})

# TGT1_FILELIST
#
#   A space seperates list of files to copy to the target during the 
#   make install process.
#
SET(TGT1_FILELIST cfe_es_startup.scr) ##TODO## Add mission specific files
