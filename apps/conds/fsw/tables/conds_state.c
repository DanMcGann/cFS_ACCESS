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
/******************************************************************************
*	File: conds_constate_tbl.c
*
*	Purpose: Defines default constellation state table
*
*	Notes: Mission Software developer should modify this table for mission
*	specific constellation state information.
*
*	Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "conds_platform_cfg.h"
#include "conds_app.h"
#include "conds_msg.h"


/*
 * Default Constellation State Table
 */
CONDS_ConStateEntry_t CONDS_StateTable[CONDS_CONSTATE_TYPE_CNT] =
{
		/*
		 * Data Type Entry 0
		 */
		{
				.FrmtName = "TYPE0",				/* Data Type Name */
				.FrmtFilename =  "/cf/conds_type0.tbl",		/* Format Table Filename */
				.FrmtPtr = NULL, 				/* Format Table Pointer */
				.MsgId =  0x000,				/* Incoming Data Message ID */
				.DataPtr =  NULL
		}
};

/*
 * Table Header
 */
CFE_TBL_FILEDEF(CONDS_StateTable, // @suppress("Unused variable declaration in file scope")
		CONDS_APP.STATE_TBL,
		CONDS Con State Data Def Table,
		conds_state.tbl)

/**********************
* End of File Comment *
**********************/
