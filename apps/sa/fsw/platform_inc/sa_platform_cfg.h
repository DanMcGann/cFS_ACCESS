/*********************************************************************************
*DISTRIBUTION STATEMENT A. Approved for public release. Distribution is unlimited.
*
*This material is based upon work supported under Air Force Contract No. FA8721-05-C-0002
*and/or FA8702-15-D-0001. Any opinions, findings, conclusions or recommendations
*expressed in this material are those of the author(s) and do not necessarily reflect the
*views of the U.S. Air Force.
*
*© 2018 Massachusetts Institute of Technology.
*
*The software/firmware is provided to you on an As-Is basis
*
*Delivered to the U.S. Government with Unlimited Rights, as defined in DFARS Part
*252.227-7013 or 7014 (Feb 2014). Notwithstanding any copyright notice, U.S. Government
*rights in this work are defined by DFARS 252.227-7013 or DFARS 252.227-7014 as detailed
*above. Use of this work other than as specifically authorized by the U.S. Government may
*violate any copyrights that exist in this work.
*********************************************************************************/ 
/*=======================================================================================
** File Name:  sa_platform_cfg.h
**
** Title:  Platform Configuration Header File for SA Application
**
** $Author:   Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-18
**
** Purpose:  This header file contains declartions and definitions of all SA's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-18 | unk | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SA_PLATFORM_CFG_H_
#define _SA_PLATFORM_CFG_H_

/*
** SA Platform Configuration Parameter Definitions
*/
#define SA_SCH_PIPE_DEPTH  2
#define SA_CMD_PIPE_DEPTH  10
#define SA_STATE_PIPE_DEPTH  10


/* TODO:  Add more platform configuration parameter definitions here, if necessary. */

/* sa Table Configuration */
#define SA_SATFRMT_TBLNAME "SSF"
#define SA_SATFRMT_FILENAME "/cf/sa_ssf.tbl"

#define SA_SATFRMT_MAX_LEN 1

#define SA_MEM_POOL_BUFFER_SIZE 1000

#endif /* _SA_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file sa_platform_cfg.h
**=====================================================================================*/
    
