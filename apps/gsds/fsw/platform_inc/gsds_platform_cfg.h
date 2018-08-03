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
** File Name:  gsds_platform_cfg.h
**
** Title:  Platform Configuration Header File for GSDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-04
**
** Purpose:  This header file contains declartions and definitions of all GSDS's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-04 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _GSDS_PLATFORM_CFG_H_
#define _GSDS_PLATFORM_CFG_H_

//TODO ADD DOCS
/*
** gsds Platform Configuration Parameter Definitions
*/
#define GSDS_SCH_PIPE_DEPTH  		2
#define GSDS_CMD_PIPE_DEPTH  		10
#define GSDS_GSSTATE_PIPE_DEPTH  	256

#define GSDS_MAKE_TBLS_CRITICAL		0

#define GSDS_MAX_GS					50
#define GSDS_MAX_DATA_LEN		50

#define GSDS_STATE_TBL_NAME			"STATE"
#define GSDS_STATE_FILENAME			"/cf/gsstate.tbl"

#define GSDS_GSFRMT_TBL_NAME		"GSFRMT"
#define GSDS_GSFRMT_FILENAME		"/cf/gsfrmt.tbl"

#define GSDS_MAKE_TABLES_CRITICAL 	0

#endif /* _GSDS_PLATFORM_CFG_H_ */

/*=======================================================================================
** End of file gsds_platform_cfg.h
**=====================================================================================*/
    
