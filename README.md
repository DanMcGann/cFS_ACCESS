# ACCESS
## Cubesat Autonomy Framework

This repository contains the applications that make up the ACCESS Cubesat autonomy Framework. These applications are developed to be used with NASA's Core Flgiht System (cFS) an opensource satellite software system, to enable to utilization of constellation autonomy algorithms on satellites with a variety of hardware configurations. 

For a detailed description of the ACCESS Framework, its theory, its implementation, and how to use it see the ACCESS Documentation in the docs folder.

For a guide to integrate these applications with cFS See Below.



### Adding ACCESS to cFS

  ACCESS is a set of applications designed to run as components in cFS. Therefore to use ACCESS, cFS must be aquired and then all ACCESS applications must be integrated to the cFS build system and run time operations. This can be acomplished using the following steps.
  
  1. Download and Install the cFS open source release. This can be acomplished by following the instructions provided on the cFS github page located [here](https://github.com/nasa/cFE).
  
  2. Download and Install the contents of this repository. This can be done using either git clone, or download via zip.
  
  3. Arrange ACCESS Files
      1. Copy the contents of this repositories app directrory, into the apps directory in your cFS directory
      2. Copy the entire access_defs folder into the top level of the cFS directory
      3. Rename this cFS directory to represent your current project name, all instructions will refer to this directory as your project directory. 
      4. Rename the access_defs folder to represent your project making the new name ```<project name>_defs'''
    
  4. Setup ACCESS Applications for build
    ACCESS makes use of cFS's CMake style build system. Information on this system can be located in the cFS opensource release under ``` <cFS Directory>/cfe/cmake/ ``` The following is a simplified instructional list for addition explinations refer to the cFS CMake Documentation.
      1. Copy Makefile.sample from ```<cFS Directory>/cfe/cmake/``` into the top level of your project directory, and rename the file ```Makefile```
      2. Copy the following files from ```<cFS Directory>/cfe/cmake/sample_defs``` into your ```<project name>_defs directory.
          ```
          - cpu1_cfe_es_startup.scr
          - cpu1_msgids.h
          - cpu1_platform_cfg.h
          - default_osconfig.h
          - sample_mission_cfg.h
          - toolchain-cpu1.cmake
          ```
      3. Rename the copied file ```sample_mission_cfg.h``` to match your project name making the file ```<project name>_mission_cfg.h```
      4. Modify cFS configurations for mission and platform specific information. For each file that was just copied as well as targets.cmake review the configuration parameters makeing changes where necissary. Refer to inline documentation for descriptions of each configuration parameter. Note: other than mission name no changes will likely need to be make for a default build on a Linux computer. 
      5. cFS and all ACCESS Applications should now build using the following build steps
      ```
      make prep 
      make 
      make install
      '''
  5. Configure ACCESS to run
      1. Add all ACCESS Applications applications to ```<project name>_defs/cpu1_cfe_es_startup.scr``` following the format outlined in that document.
      2. Add all ACCESS Applictions to the cFS Scheduler application (either sch or sch_lab). Refer to thoes applicaions documentation for details. 
      NOTE: all access Applications should have a wake up message at a minimum rate of 1 Hz. This is vital for proper application interactions and operations.
      
 ACCESS Applications and the required cFS Applications should now built and run on startup without error. For information on how to use thses applications and incooporate mission specific applications refer to the ACCESS Documentation.
