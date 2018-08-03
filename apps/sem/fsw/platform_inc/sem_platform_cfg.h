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
** File Name:  sem_platform_cfg.h
**
** Title:  Platform Configuration Header File for SEM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  This header file contains declartions and definitions of all SEM's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SEM_PLATFORM_CFG_H_
#define _SEM_PLATFORM_CFG_H_

/*
** sem Platform Configuration Parameter Definitions
*/
#define SEM_SCH_PIPE_DEPTH  2
#define SEM_CMD_PIPE_DEPTH  10
#define SEM_TLM_PIPE_DEPTH  10


/* TODO:  Add more platform configuration parameter definitions here, if necessary. */

#define SEM_MAX_ACTIONS 20
#define SEM_MAX_ACTION_CMD_LEN 50
#define SEM_MAX_SCHEDULE_LEN 100

#define SEM_MEM_POOL_BUFFER_SIZE 1000

#define SEM_MAKE_TABLES_CRITICAL TRUE

#define SEM_ACTIONTBL_NAME "ACT_TBL"
#define SEM_ACTIONTBL_FILENAME "/cf/sem_act.tbl"

#define SEM_SCHEDULETBL_NAME "SCH_TBL"
#define SEM_SCHEDULETBL_FILENAME "/cf/sem_sch.tbl"
#define SEM_SCHEDULETBL_INITDEFAULT TRUE

#define SEM_PLANFRMTTBL_NAME "PFT"
#define SEM_PLANFRMTTBL_FILENAME "/cf/sem_pft.tbl"
#define SEM_PLANFRMTTBL_ENTRIES 2


/* Planning Layer Configurations (APL*/ //TODO - APL CFG Make these top level configurations
#define APL_PLAN_REQ_MSGID 0x0000
#define APL_PLAN_REQ_CC	2


#endif /* _SEM_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file sem_platform_cfg.h
**=====================================================================================*/
    
