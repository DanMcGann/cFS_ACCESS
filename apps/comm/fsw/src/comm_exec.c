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
** File Name:  comnm_exec.c
**
** Title:  Source File for COMM Application executive functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To define functional implementation for the COMM application executibe
** 			functions.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "comm_tables.h"
#include "comm_app.h"

/****************************************************************************************
 * COMM_InitLink
****************************************************************************************/
int32 COMM_InitLink(COMM_LinkType_t Type, CFE_TIME_SysTime_t LinkDur) {
	int32 Status = CFE_SUCCESS;
	int32 files_out = 0;
	int32 files_bad = 0;

	/* Calculate the max amount of data that can be transfered for this link type */
	int32 rate;
	if (Type == DLINK) {
		rate = COMM_DLINK_DATA_RATE;
	}
	else if (Type == XLINK) {
		rate = COMM_XLINK_DATA_RATE;
	}

	/* Perform pre-link housekeeping */
	if (COMM_PreLinkUpdate(Type) == CFE_SUCCESS) {
		/* The link table shoul dnow be in prioirty order */

		int32 bytes = LinkDur.Seconds * rate;

		char file[OS_MAX_PATH_LEN];
		int index = 0;
		os_fstat_t stats;

		while (bytes > 0) {
			/* The entry is a valid one*/
			if (COMM_GetFileName(Type, index, file) == CFE_SUCCESS) {

				/* Get statistics on this file */
				OS_stat(file, &stats);


				/* This file can be sent*/
				if ((bytes - stats.st_size) > 0) {


					/* update the remaining bandwidth for the link */
					bytes = bytes - stats.st_size;

					/* send the file using configurable send function */
					Status = COMM_SEND_FILE_FUNC(file);

					if (Status != CFE_SUCCESS) {
						CFE_EVS_SendEvent(COMM_INITLINK_ERR_EID, CFE_EVS_ERROR,
							"COMM: Send File operations failed | Error: 0x%08X",
							Status);
						files_bad++;
					}
					else {
						files_out++;
					}

				}
				/* this file is too large to be sent at this time */
				else {
					/* Do nothing */
				}
			}
			/* There are no more files listed in the table */
			else {
				/* set flag to exit while loop */
				bytes = 0;
			}

			/* Increment the index of the file to check */
			index++;
		}
	}
	else {
		CFE_EVS_SendEvent(COMM_INITLINK_ERR_EID, CFE_EVS_ERROR,
				"COMM: Pre-link operations failed for link of type %d aborting link",
				Type);
		return COMM_INITLINK_ERR_EID;
	}



	/* report results */
	if (files_out > 0) {
		/* IF the function outputs any files it is considered a success */
		CFE_EVS_SendEvent(COMM_INITLINK_INF_EID, CFE_EVS_INFORMATION,
				"COMM: Link of type %d initiated: %d files sent - %d files failed to send",
				Type, files_out, files_bad);
		Status = CFE_SUCCESS;
	}
	else {
		CFE_EVS_SendEvent(COMM_INITLINK_ERR_EID, CFE_EVS_ERROR,
				"COMM: Link of type %d FAILED to output: %d files sent - %d files failed to send",
				Type, files_out, files_bad);
		Status = COMM_INITLINK_ERR_EID;
	}

	return Status;
}
