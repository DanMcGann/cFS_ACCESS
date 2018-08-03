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
** File Name:  comm_platform_cfg.h
**
** Title:  Platform Configuration Header File for COMM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  This header file contains declartions and definitions of all COMM's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _COMM_PLATFORM_CFG_H_
#define _COMM_PLATFORM_CFG_H_


/* TODO Platform config docs */

/*
** comm Platform Configuration Parameter Definitions
*/
#define COMM_SCH_PIPE_DEPTH  2
#define COMM_CMD_PIPE_DEPTH  10
#define COMM_TLM_PIPE_DEPTH  10


/* Communication capabilities information */
#define COMM_DLINK_DATA_RATE 10 /* Bytes per second */
#define COMM_XLINK_DATA_RATE 5 /* is Bytes per second */


/* Table information */
#define COMM_MAX_DLINK_FILES 100
#define COMM_MAX_XLINK_FILES 10

#include "comm_ft.h"

#define COMM_SEND_FILE_FUNC(File) COMM_CF_SendFile(File)
#define COMM_STOP_LINK_FUNC(void) COMM_CF_StopTrans()

#define COMM_DLINK_TBL_NAME			"DLINK"
#define COMM_DLINK_FILENAME			"/cf/comm_dlink.tbl"

#define COMM_XLINK_TBL_NAME			"XLINK"
#define COMM_XLINK_FILENAME			"/cf/comm_xlink.tbl"




#endif /* _COMM_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file comm_platform_cfg.h
**=====================================================================================*/
    
