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
** File Name:  comm_ft.h
**
** Title:  Type Header File for COMM Application file Transfer Functions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  This header file contains declarations for a set of functions that allow
** COMM to transfer files through commonly used file transfer applications. These
** functions can be used for the COMM Configuration SEND_FILE_FUNC parameter.
**
** NOTES:
** If the mission is using a file transport application not supported "out of the box"
** mission specific functions can be declared in this file, and written in the
** the corresponding source file. OR the source code for the files can be included in
** comm/fsw/platform_cfg/comm_platform_cfg.h file.
**
**
** Send file - Transmits a file to the linked target
**
** All SEND_FILE_FUNC Functions should have the signature
**
** int32 <func_name>(char File[OS_MAX_PATH_LEN]
**
** and should return  CFE_SUCCESS [0] on a successful operation and anything else to
** represent an error.
**
**
** ** All SEND_FILE_FUNC Functions should have the signature
**
** int32 <func_name>(char File[OS_MAX_PATH_LEN]
**
** and should return  CFE_SUCCESS [0] on a successful operation and anything else to
** represent an error.
**
**
**
** StopLink - Stops the transmission of any files currently being processed
** 	by the configurable file transfer application.
**
** ALL STOP_LINK_FUNC Functions should have the signature
**
** int32 <func_name>(void)
**
** and should return CFE_SUCCESS if the operation is successful and anything esle to
** represent an error
**
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
#include "cfe.h"

#ifndef _COMM_FT_H_
#define _COMM_FT_H_

/****************************************************************************************
* \brief Commands CF Application to send the given file
*
* \par Description: This functions interfaces with the CFS CF Application, commanding
* it to send out the given file under default settings, with acknowledgment.
*
* \returns
* 	\retstmt CFE_SUCCESS - If all operations function nominally
* \endreturns
****************************************************************************************/
int32 COMM_CF_SendFile(char File[OS_MAX_PATH_LEN]);

/****************************************************************************************
* \brief Commands CF Application to stop all file transfers being processed
*
* \par Description: This functions interfaces with the CFS CF Application, commanding
* it to stop all file transactions currently taking place. and clears the CF queues to
* ensure that they are clear for the next link opportunity, which may not be with the same
* target and therefore cannot be retained.
*
* \returns
* 	\retstmt CFE_SUCCESS - If all operations function nominally
* \endreturns
****************************************************************************************/
int32 COMM_CF_StopTrans(void);


#endif /* _COMM_FT_H_ */
