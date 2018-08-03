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
** File Name:  conds_platform_cfg.h
**
** Title:  Platform Configuration Header File for CONDS Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-03-26
**
** Purpose:  This header file contains declartions and definitions of all CONDS's 
**           platform-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-03-26 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _CONDS_PLATFORM_CFG_H_
#define _CONDS_PLATFORM_CFG_H_

/*
** conds Platform Configuration Parameter Definitions
*/

/**
**  \condscfg Application Name 
**
**  \par Description:
**       This definition must match the name used at startup by the cFE
**       Executive Services when creating the CONDS application.
**
**  \par Limits:
**       CONDS places no formal requirements on the name of this application.
**       However, it is highly suggested to use the default name CONDS.
**
*/
#define CONDS_APP_NAME "CONDS"


/**
**  \condscfg Pipe Depths
**
**  \par Description:
**       Maximum number of messages that will be allowed in each of CONDS' 
**       pipes at one time. Used during the initilization in the 
**       call to #CFE_SB_CreatePipe
**
**  \par Limits:
**       This parameter cannot exceed the maximum value for an unsigned 16 bit
**       integer (65535)
**
*/
#define CONDS_SCH_PIPE_DEPTH  256
#define CONDS_CMD_PIPE_DEPTH  256
#define CONDS_TLM_PIPE_DEPTH  256


/**
**  \condscfg Constellation State Table -- Table Name 
**
**  \par Description:
**       This parameter defines the name of the CONDS Constellation state table.
**       This name is used when refrenced via cFE Table Services.
**
**  \par Limits:
**       The String Length (including string terminator) cannot 
**       exceed #CFE_TBL_MAX_NAME_LENGTH.
*/
#define CONDS_CONSTATE_TBL_NAME "STATE_TBL"


/**
**  \condscfg Constellation State Table -- default table filename 
**
**  \par Description:
**       This definition is the default file name and location for the CONDS
**       constellation state table.
**
**  \par Limits:
**       The string length including terminator shall not exceed #OS_MAX_PATH_LEN.
*/
#define CONDS_CONSTATE_FILENAME "/cf/conds_state.tbl"


/**
**  \condscfg Constellation State Table -- number data structure types
**
**  \par Description:
**       This parameter sets the size of the Constellation state table. This should
**       correspond to the number of data structure types are needed to store all
**       constellations state data. e.g. Satellite Neighbors, Satellite Plan, Activity
**       priorities.
**
**  \par Limits:
**       This number must be greater than 0, but no upper limit is enforced.
*/
#define CONDS_CONSTATE_TYPE_CNT 1 //Note Value of 2 Was used for testing.

/**
**  \condscfg Constellation State Table type - Max Number of Format Table Entries
**
**  \par Description:
**       This parameter sets the size size allotted to each of the format tables
**       that define constellation data types. therefore presents a limit on the
**       number of entries each format table can have.
**
**  \par Limits:
**       This number must be greater than 0, but no upper limit is enforced.
*/
#define CONDS_MAX_FRMT_SIZE 5

/**
**  \condscfg Constellation State Table -- make tables critical
**
**  \par Description:
**       This parameter sets whether or not the Constellation state Table,
**       and all of its included format tables will be defined critical
**       and therefore saved in the CFE_Critical Data Store
**
**  \par Limits:
**       This parameter is limited to a 0 (False) or 1 (TRUE)
*/
#define CONDS_MAKE_TABLES_CRITICAL 0

/*
**	\condscfg Memory Pool Buffer
**
**	\par Description:
**		This parameter sets the size of the memory pool for the conds application
**		this is the dedicated storage location of all dynamically allocated
**		constellation system data.
**
**	\par Limits:
**		This parameter should be large enought enought so that the buffer can hold
**		all required constellations tate information. As well as the necissary memory
**		pool descriptors.
*/
#define CONDS_MEM_POOL_BUFFER_SIZE 1024

#endif /* _CONDS_PLATFORM_CFG_H_ */

/*======================================================================================
** End of file conds_platform_cfg.h
**=====================================================================================*/
    
