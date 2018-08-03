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
/*******************************************************************************
*   File: access_lib.h
*   
*   Purpose: Specifications for the access library.
*
*   AUTHOR: Daniel McGann
*  
*******************************************************************************/
#ifndef _SATCEHCK_LIB_H_
#define _SATCHECK_LIB_H_

// TODO - SATCHECK Docs
/*******************************************************************************
*   Includes
*******************************************************************************/
#include "cfe.h"

/*******************************************************************************
*   Macro Definitions
*******************************************************************************/
#define SATCHECK_MAX_PARAMS 		5
#define SATCHECK_MAX_ACTION_CHECKS 	10

/*******************************************************************************
*   Data Definitions
*******************************************************************************/

/* Checker Function pointer definition */
typedef boolean (* SATCHECK_CheckFun_t) (void*);

typedef struct {
	SATCHECK_CheckFun_t FunPtr;
	int32 InputFields[SATCHECK_MAX_PARAMS];
	int32 NumFields;
} SATCHECK_Checker_t;


/*******************************************************************************
*   Function Prototypes
*******************************************************************************/
int32 SATCHECK_LibInit(void);

#endif /* _ACCESS_LIB_H_ */
