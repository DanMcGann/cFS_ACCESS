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
** File Name:  sem_mission_cfg.h
**
** Title:  Mission Configuration Header File for SEM Application
**
** $Author:    Daniel McGann
** $Revision: 1.1 $
** $Date:      2018-04-24
**
** Purpose:  This header file contains declartions and definitions of all SEM's 
**           mission-specific configurations.
**
** Modification History:
**   Date | Author | Description
**   ---------------------------
**   2018-04-24 | Daniel McGann | Build #: Code Started
**
**=====================================================================================*/
    
#ifndef _SEM_MISSION_CFG_H_
#define _SEM_MISSION_CFG_H_

/*
** Include Files
*/
#include "cfe.h"



/*
** SEM Mission Configuration Parameter Definitions
*/

/* TODO SEM MISSION CFG - DOCS */

#define SEM_MAX_ACCEPT_INVALID_ACTIONS 10
#define SEM_MAX_ACCEPT_PREREQ_FAILURES 10
#define SEM_MAX_ACCEPT_INVALID_PLANS 3

#endif /* _SEM_MISSION_CFG_H_ */

/*=======================================================================================
** End of file sem_mission_cfg.h
**=====================================================================================*/
    
