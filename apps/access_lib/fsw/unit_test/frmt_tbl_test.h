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
*   File: frmt_tbl_test.h
*
*   Purpose: This file provides the specifications for format table unit tests
*
*   AUTHOR: Daniel McGann 3/22/2018
*
*******************************************************************************/
#ifndef _FRMT_TBL_TEST_H_
#define _FRMT_TBL_TEST_H_

/*
 * Includes
 */
#include "utassert.h"
#include "uttest.h"


/*******************************************************************************
*   Test Function Prototypes
*******************************************************************************/

void ACCESS_LIB_TEST_frmtGetSizeZero(void);
void ACCESS_LIB_TEST_frmtGetSizeAligned(void);
void ACCESS_LIB_TEST_frmtGetSizeMisAligned(void);
void ACCESS_LIB_TEST_frmtGetFlaggedFieldNominal(void);
void ACCESS_LIB_TEST_frmtGetFlaggedFieldMultiFlag(void);
void ACCESS_LIB_TEST_frmtGetFlaggedFieldDupeFlag(void);
void ACCESS_LIB_TEST_frmtGetFlaggedFieldNoFlag(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldNominal(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldMisOrdered(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldNoField(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundExist(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundNoExist(void);
void ACCESS_LIB_TEST_frmtGetSizeOfFieldNegField(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldNominal(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldMisOrdered(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldNoField(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundExist(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundNoExist(void);
void ACCESS_LIB_TEST_frmtGetOffsetToFieldNegField(void);
void ACCESS_LIB_TEST_frmtGetFieldDataNominalInt(void);
void ACCESS_LIB_TEST_frmtGetFieldDataNominalFloat(void);
void ACCESS_LIB_TEST_frmtGetFieldDataNominalChar(void);
void ACCESS_LIB_TEST_frmtGetFieldDataNominalString(void);
void ACCESS_LIB_TEST_frmtGetFieldDataNoField(void);
void ACCESS_LIB_TEST_frmtInsertDataNominalInt(void);
void ACCESS_LIB_TEST_frmtInsertDataNominalFloat(void);
void ACCESS_LIB_TEST_frmtInsertDataNominalChar(void);
void ACCESS_LIB_TEST_frmtInsertDataNominalString(void);
void ACCESS_LIB_TEST_frmtInsertDataNoField(void);


void ACCESS_LIB_Frmt_Tbl_AddTestCases(void);


#endif /* _FRMT_TBL_TEST_H_ */
