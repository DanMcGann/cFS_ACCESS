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
** File Name:  comm_linkdefs.h
**
** Title:  Header File for COMM Application Link Data definitions
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-05-14
**
** Purpose:  To Define data definitions for common communication link information.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-05-14 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/

#ifndef _COMM_LINKDEFS_H_
#define _COMM_LINKDEFS_H_


/* \brief Types of satellite communication links */
typedef enum {
	DLINK,
	XLINK

} COMM_LinkType_t;


/* \brief Data Prefixes - values are in number of bytes in each magnitude*/
typedef enum {
	BYTE = 1,
	KILO = 1000,
	MEGA = 1000000,
} COMM_RateUnit_t;



#endif /* _COMM_LINKDEFS_H_ */
