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
*	File: sem_pft
*
*	Purpose: Defines default plan format table
*
*	Notes: Mission Software developer should modify this table for mission
*	specific plan data structure ouput by the systems autonomy layer
*
*	Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "access_lib.h"
#include "sem_tbldefs.h"


struct Tbl {
	FRMT_TBL_t Tbl;
	FRMT_TBL_Entry_t Entries[2];
} Tbl = { {2},
		  { {0, 0, 4, SEM_PFT_ACTION_ID_FLAG},
		    {1, 4, 8, SEM_PFT_START_TIME_FLAG}}
		};

/*
 * Table Header
 */
CFE_TBL_FILEDEF(Tbl, // @suppress("Unused variable declaration in file scope")
		SEM_APP.PFT,
		SEM Plan Frmt Tbl,
		sem_pft.tbl)

/**********************
* End of File Comment *
**********************/
