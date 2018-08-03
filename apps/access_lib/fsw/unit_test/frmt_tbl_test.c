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
*   File: frmt_tbl_test.c
*
*   Purpose: This file provides unit tests for all format table functions
*
*   AUTHOR: Daniel McGann 3/22/2018
*
*******************************************************************************/

/*
 * Includes
 */
#include "access_lib.h"
#include "frmt_tbl_test.h"
#include "access_lib_test_utils.h"
#include "ut_osapi_stubs.h"
#include "ut_cfe_sb_stubs.h"
#include "ut_cfe_es_stubs.h"
#include "ut_cfe_es_hooks.h"
#include "ut_cfe_evs_stubs.h"
#include "ut_cfe_evs_hooks.h"
#include "ut_cfe_time_stubs.h"
#include "ut_cfe_psp_memutils_stubs.h"
#include "ut_cfe_tbl_stubs.h"
#include "ut_cfe_fs_stubs.h"
#include "ut_cfe_time_stubs.h"
#include <sys/fcntl.h>
#include <unistd.h>

/*******************************************************************************
*   Test Functions
*******************************************************************************/

/*******************************************************************************
*   Get Size Tests
*******************************************************************************/
void ACCESS_LIB_TEST_frmtGetSizeZero(void) {
    FRMT_TBL_t* frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 0));
    memset(frmt_tbl, 0, sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 0));

    /* Run test code */
    int32 size = FRMT_TBL_GetTotalSize(frmt_tbl);

    UtAssert_True(size == 0, "size == 0");
} /* End frmtGetSizeZero */

void ACCESS_LIB_TEST_frmtGetSizeAligned(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 4, FRMT_TBL_NOFLAG} //int32
                                    };

    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetTotalSize(frmt_tbl);
    char actexp1[100];
	sprintf(actexp1, "EXP: size == 16 | ACT: size = %ld", size);
    UtAssert_True(size == 16, actexp1);

    free(frmt_tbl);
} /* End frmtGetSizeAligned */

void ACCESS_LIB_TEST_frmtGetSizeMisAligned(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };

    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetTotalSize(frmt_tbl);


    UtAssert_True(size == 16, "size == 16");

    free(frmt_tbl);
} /* End frmtGetSizeMisAligned */

void ACCESS_LIB_TEST_frmtGetSizeMisAligned2(void) {
    FRMT_TBL_Entry_t frmt_es[3] = {
                                         {0, 0, 8, FRMT_TBL_NOFLAG}, //int32
                                         {1, 8, 4, FRMT_TBL_NOFLAG}, //char
                                         {2, 12, 1, FRMT_TBL_NOFLAG}, //float
                                    };

    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 3));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 3));
    frmt_tbl->NumEntries = 3;

    int32 size = FRMT_TBL_GetTotalSize(frmt_tbl);


    UtAssert_True(size == 16, "size == 16");

    free(frmt_tbl);
} /* End frmtGetSizeMisAligned */


void ACCESS_LIB_TEST_frmtGetSizeMisOrdered(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {3, 12, 1, FRMT_TBL_NOFLAG}, //char
                                         {1, 4, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 8, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetTotalSize(frmt_tbl);

    UtAssert_True(size == 16, "size == 16");

    free(frmt_tbl);
} /* End frmtGetSizeMisOrdered */

/*******************************************************************************
*   Get Field From Flag Tests
*******************************************************************************/

void ACCESS_LIB_TEST_frmtGetFlaggedFieldNominal(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, 1}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 id = FRMT_TBL_GetFlaggedFieldId(frmt_tbl, 1);

    UtAssert_True(id == 1, "id == 1");

    free(frmt_tbl);
} /* End frmtGetFlaggedFieldNominal */

void ACCESS_LIB_TEST_frmtGetFlaggedFieldMultiFlag(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, 1}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, 2} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 id = FRMT_TBL_GetFlaggedFieldId(frmt_tbl, 2);

    UtAssert_True(id == 3, "id == 3");

    free(frmt_tbl);
} /* End frmtGetFlaggedFieldMultiFlags */

void ACCESS_LIB_TEST_frmtGetFlaggedFieldDupeFlag(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, 2}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, 2} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 id = FRMT_TBL_GetFlaggedFieldId(frmt_tbl, 2);

    UtAssert_True(id == 1, "id == 1");

    free(frmt_tbl);
} /* End frmtGetFlaggedFieldDupeFlags */

void ACCESS_LIB_TEST_frmtGetFlaggedFieldNoFlag(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 id = FRMT_TBL_GetFlaggedFieldId(frmt_tbl, 1);

    UtAssert_True(id == FRMT_TBL_NOFIELD_ERR, "id == FRMT_TBL_NOFIELD_ERR");

    free(frmt_tbl);
} /* End frmtGetFlaggedFieldNoFlag */

/*******************************************************************************
*   GetSizeOfField Tests
*******************************************************************************/
void ACCESS_LIB_TEST_frmtGetSizeOfFieldNominal(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, 1);

    UtAssert_True(size == 1, "size == 1");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildNominal */

void ACCESS_LIB_TEST_frmtGetSizeOfFieldMisOrdered(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {3, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {0, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {1, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, 3);

    UtAssert_True(size == 4, "size == 4");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildMisOrdered */

void ACCESS_LIB_TEST_frmtGetSizeOfFieldNoField(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {3, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {4, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, 2);

    UtAssert_True(size == FRMT_TBL_NOFIELD_ERR, "size == FRMT_TBL_NOFIELD_ERR");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildNoField */

void ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundExist(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {4, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, 4);

    UtAssert_True(size == 1, "size == 1");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildOutBoundExist */

void ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundNoExist(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, 4);

    UtAssert_True(size == FRMT_TBL_NOFIELD_ERR, "size == FRMT_TBL_NOFIELD_ERR");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildOutBoundNoExist */

void ACCESS_LIB_TEST_frmtGetSizeOfFieldNegField(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {-22, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 size = FRMT_TBL_GetSizeOfField(frmt_tbl, -22);

    UtAssert_True(size == 1, "size == 1");

    free(frmt_tbl);
} /* End frmtGetSizeOfFeildNegField */

/*******************************************************************************
*   GetOffsetTofsetTo Tests
*******************************************************************************/
void ACCESS_LIB_TEST_frmtGetOffsetToFieldNominal(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, 1);

    UtAssert_True(offset == 4, "offset == 4");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildNominal */

void ACCESS_LIB_TEST_frmtGetOffsetToFieldMisOrdered(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {3, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {0, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {1, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, 3);

    UtAssert_True(offset == 0, "offset == 0");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildMisOrdered */

void ACCESS_LIB_TEST_frmtGetOffsetToFieldNoField(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {3, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {4, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, 2);

    UtAssert_True(offset == FRMT_TBL_NOFIELD_ERR, "offset == FRMT_TBL_NOFIELD_ERR");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildNoField */

void ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundExist(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {4, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, 4);

    UtAssert_True(offset == 12, "offset == 12");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildOutBoundExist */

void ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundNoExist(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {1, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, 4);

    UtAssert_True(offset == FRMT_TBL_NOFIELD_ERR, "offset == FRMT_TBL_NOFIELD_ERR");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildOutBoundNoExist */

void ACCESS_LIB_TEST_frmtGetOffsetToFieldNegField(void) {
    FRMT_TBL_Entry_t frmt_es[4] = {
                                         {0, 0, 4, FRMT_TBL_NOFLAG}, //int32
                                         {-22, 4, 1, FRMT_TBL_NOFLAG}, //char
                                         {2, 8, 4, FRMT_TBL_NOFLAG}, //float
                                         {3, 12, 1, FRMT_TBL_NOFLAG} //char
                                    };
    FRMT_TBL_t*  frmt_tbl = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    /* Copy Entries into table */
    memcpy(frmt_tbl->Entries, frmt_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl->NumEntries = 4;

    int32 offset = FRMT_TBL_GetOffsetToField(frmt_tbl, -22);

    UtAssert_True(offset == 4, "offset == 4");

    free(frmt_tbl);
} /* End frmtGetOffsetToFeildNegField */

/*******************************************************************************
*   GetFieldData
*******************************************************************************/
typedef struct {
    int32 int_data;
    float float_data;
    char  char_data;
    char string_data[6];
} Frmt_Tbl1_t;

FRMT_TBL_Entry_t frmt_tbl1_es[4] = {
                                    {0, 0, 4, FRMT_TBL_NOFLAG},
                                    {1, 4, 4, FRMT_TBL_NOFLAG},
                                    {2, 8, 1, FRMT_TBL_NOFLAG},
                                    {3, 9, 6, FRMT_TBL_NOFLAG}
                             };
FRMT_TBL_t* frmt_tbl1;


void ACCESS_LIB_TEST_frmtGetFieldDataNominalInt(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    strcpy(data.string_data, "hello");
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;

    int32 ret;
    int32 status;
    
    status = FRMT_TBL_GetFieldData(frmt_tbl1, (char*)(&data), 0, (void*)&ret);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
    
    char actexp2[100];
    sprintf(actexp2, "EXP: ret == 33 | ACT: ret = %ld", ret);
    UtAssert_True(ret == 33, actexp2);
    free(frmt_tbl1);
} /* End GetFieldDataNominalInt */

void ACCESS_LIB_TEST_frmtGetFieldDataNominalFloat(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;

    float ret;
    int32 status;
    
    status = FRMT_TBL_GetFieldData(frmt_tbl1, (char*)(&data), 1, (void*)&ret);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
    
    char actexp2[100];
    sprintf(actexp2, "EXP: ret == 3.14 | ACT: ret = %f", ret);
    UtAssert_True(ret > (3.1399) && ret < (3.14001), actexp2);
    free(frmt_tbl1);
} /* End GetFieldDataNominalFloat */

void ACCESS_LIB_TEST_frmtGetFieldDataNominalChar(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    strcpy(data.string_data, "hello");
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;

    char ret;
    int32 status;
    
    status = FRMT_TBL_GetFieldData(frmt_tbl1, (char*)(&data), 2, (void*)&ret);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
    
    char actexp2[100];
    sprintf(actexp2, "EXP: ret == a | ACT: ret = %c", ret);
    UtAssert_True(ret == 'a', actexp2);
    free(frmt_tbl1);
} /* End GetFieldDataNominalChar */

void ACCESS_LIB_TEST_frmtGetFieldDataNominalString(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    strcpy(data.string_data, "hello");
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;

    char ret[6];
    int32 status;
    
    status = FRMT_TBL_GetFieldData(frmt_tbl1, (char*)(&data), 3, (void*)&ret);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
    
    char actexp2[100];
    sprintf(actexp2, "EXP: ret == hello | ACT: ret = %s", ret);
    UtAssert_True(strcmp(ret, "hello") == 0, actexp2);
    free(frmt_tbl1);
} /* End GetFieldDataNominalString */

void ACCESS_LIB_TEST_frmtGetFieldDataNoField(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;

    float ret;
    int32 status;
    
    status = FRMT_TBL_GetFieldData(frmt_tbl1, (char*)(&data), 12, (void*)&ret);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == FRMT_TBL_NOFIELD_ERR | ACT: status = %ld", status);
    UtAssert_True(status == FRMT_TBL_NOFIELD_ERR, actexp1);
    free(frmt_tbl1);
} /* End GetFieldDataNoField */

/*******************************************************************************
*   InsertData Tests
*******************************************************************************/
void ACCESS_LIB_TEST_frmtInsertDataNominalInt(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;
    
    int32 in = 55;
    int32 status;
    status = FRMT_TBL_InsertData(frmt_tbl1, 0, (char*)&in, (char*)&data);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
   
    //Int data is being changed 
    char actexp2[100];
    sprintf(actexp2, "EXP: int_data == 55 | ACT: int_data = %ld", data.int_data);
    UtAssert_True(data.int_data == 55, actexp2);

    char actexp3[100];
    sprintf(actexp3, "EXP: float_data == 3.14 | ACT: float_data = %f", data.float_data);
    UtAssert_True(data.float_data < 3.141 && data.float_data > 3.139, actexp3);

    char actexp4[100];
    sprintf(actexp4, "EXP: char_data == 'a' | ACT: char_data = %c", data.char_data);
    UtAssert_True(data.char_data == 'a', actexp4);
    
    char actexp5[100];
    sprintf(actexp5, "EXP: string_data == hello | ACT: string_data = %s", data.string_data);
    UtAssert_True(strcmp(data.string_data, "hello") == 0, actexp5);
    free(frmt_tbl1);
} /* End InsertDataNominalInt */

void ACCESS_LIB_TEST_frmtInsertDataNominalFloat(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;
    
    float in = 1.12345;
    int32 status;
    status = FRMT_TBL_InsertData(frmt_tbl1, 1, (char*)&in, (char*)&data);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
   
    char actexp2[100];
    sprintf(actexp2, "EXP: int_data == 33 | ACT: int_data = %ld", data.int_data);
    UtAssert_True(data.int_data == 33, actexp2);

    // Float data is being changed
    char actexp3[100];
    sprintf(actexp3, "EXP: float_data == 1.12345 | ACT: float_data = %f", data.float_data);
    UtAssert_True(data.float_data < 1.12351 && data.float_data > 1.123449, actexp3);

    char actexp4[100];
    sprintf(actexp4, "EXP: char_data == 'a' | ACT: char_data = %c", data.char_data);
    UtAssert_True(data.char_data == 'a', actexp4);
    
    char actexp5[100];
    sprintf(actexp5, "EXP: string_data == hello | ACT: string_data = %s", data.string_data);
    UtAssert_True(strcmp(data.string_data, "hello") == 0, actexp5);
    free(frmt_tbl1);
} /* End InsertDataNominalFloat */

void ACCESS_LIB_TEST_frmtInsertDataNominalChar(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;
    
    char in = 'z';
    int32 status;
    status = FRMT_TBL_InsertData(frmt_tbl1, 2, (char*)&in, (char*)&data);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
   
    char actexp2[100];
    sprintf(actexp2, "EXP: int_data == 33 | ACT: int_data = %ld", data.int_data);
    UtAssert_True(data.int_data == 33, actexp2);

    char actexp3[100];
    sprintf(actexp3, "EXP: float_data == 3.14 | ACT: float_data = %f", data.float_data);
    UtAssert_True(data.float_data < 3.141 && data.float_data > 3.139, actexp3);

    //Char data is being changed
    char actexp4[100];
    sprintf(actexp4, "EXP: char_data == 'z' | ACT: char_data = %c", data.char_data);
    UtAssert_True(data.char_data == 'z', actexp4);
    
    char actexp5[100];
    sprintf(actexp5, "EXP: string_data == hello | ACT: string_data = %s", data.string_data);
    UtAssert_True(strcmp(data.string_data, "hello") == 0, actexp5);
    free(frmt_tbl1);
} /* End InsertDataNominalChar */

void ACCESS_LIB_TEST_frmtInsertDataNominalString(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;
    
    char in[6] = {'p','a','s','t','a',0};
    int32 status;
    status = FRMT_TBL_InsertData(frmt_tbl1, 3, (char*)&in, (char*)&data);

    char actexp1[100];
    sprintf(actexp1, "EXP: status == CFE_SUCCESS | ACT: status = %ld", status);
    UtAssert_True(status == CFE_SUCCESS, actexp1);
   
    char actexp2[100];
    sprintf(actexp2, "EXP: int_data == 33 | ACT: int_data = %ld", data.int_data);
    UtAssert_True(data.int_data == 33, actexp2);

    char actexp3[100];
    sprintf(actexp3, "EXP: float_data == 3.14 | ACT: float_data = %f", data.float_data);
    UtAssert_True(data.float_data < 3.141 && data.float_data > 3.139, actexp3);

    char actexp4[100];
    sprintf(actexp4, "EXP: char_data == 'a' | ACT: char_data = %c", data.char_data);
    UtAssert_True(data.char_data == 'a', actexp4);
   
    //String data is being changed 
    char actexp5[100];
    sprintf(actexp5, "EXP: string_data == pasta | ACT: string_data = %s", data.string_data);
    UtAssert_True(strcmp(data.string_data, "pasta") == 0, actexp5);
    free(frmt_tbl1);
} /* End InsertDataNominalString */

void ACCESS_LIB_TEST_frmtInsertDataNoField(void) {
    Frmt_Tbl1_t data;
    data.int_data = 33;
    data.float_data = 3.14;
    data.char_data = 'a';
    char str[6] = {'h','e','l','l','o',0};
    strcpy(data.string_data, str);
    frmt_tbl1 = malloc(sizeof(FRMT_TBL_t) + (FRMT_TBL_ENTRY_SIZE * 4));
    memcpy(frmt_tbl1->Entries, frmt_tbl1_es, (FRMT_TBL_ENTRY_SIZE * 4));
    frmt_tbl1->NumEntries = 4;
    
    char in[6] = {'p','a','s','t','a',0};
    int32 status;
    status = FRMT_TBL_InsertData(frmt_tbl1, 12, (char*)&in, (char*)&data);

    // No feilds should have changed
    char actexp1[100];
    sprintf(actexp1, "EXP: status == FRMT_TBL_NOFIELD_ERR | ACT: status = %ld", status);
    UtAssert_True(status == FRMT_TBL_NOFIELD_ERR , actexp1);
   
    char actexp2[100];
    sprintf(actexp2, "EXP: int_data == 33 | ACT: int_data = %ld", data.int_data);
    UtAssert_True(data.int_data == 33, actexp2);

    char actexp3[100];
    sprintf(actexp3, "EXP: float_data == 3.14 | ACT: float_data = %f", data.float_data);
    UtAssert_True(data.float_data < 3.141 && data.float_data > 3.139, actexp3);

    char actexp4[100];
    sprintf(actexp4, "EXP: char_data == 'a' | ACT: char_data = %c", data.char_data);
    UtAssert_True(data.char_data == 'a', actexp4);
   
    char actexp5[100];
    sprintf(actexp5, "EXP: string_data == hello | ACT: string_data = %s", data.string_data);
    UtAssert_True(strcmp(data.string_data, "hello") == 0, actexp5);

    free(frmt_tbl1);
} /* End InsertDataNoField */



/*******************************************************************************
*   AddTestCases
*******************************************************************************/
void ACCESS_LIB_Frmt_Tbl_AddTestCases(void) {
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeZero, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetTotalSizeZero");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeAligned, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetTotalSizeAligned");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeMisAligned, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetTotalSizeMisAligned");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeMisOrdered, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetTotalSizeMisOrdered");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFlaggedFieldNominal, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFlaggedFieldNominal");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFlaggedFieldMultiFlag, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFlaggedFieldMultiFlag");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFlaggedFieldDupeFlag, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFlaggedFieldDupeFlag");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFlaggedFieldNoFlag, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFlaggedFieldNoFlag");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldNominal, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldNominal");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldMisOrdered, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldMisOrdered");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldNoField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldNoField");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundExist, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldOutBoundExist");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldOutBoundNoExist, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldOutBoundNoExist");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeOfFieldNegField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetSizeOfFieldNegField");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldNominal, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldNominal");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldMisOrdered, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldMisOrdered");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldNoField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldNoField");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundExist, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldOutBoundExist");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldOutBoundNoExist, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldOutBoundNoExist");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetOffsetToFieldNegField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetOffsetToFieldNegField");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFieldDataNominalInt, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFieldDataNominalInt");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFieldDataNominalFloat, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFieldDataNominalFloat");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFieldDataNominalChar, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFieldDataNominalChar");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFieldDataNominalString, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFieldDataNominalString");
    UtTest_Add(ACCESS_LIB_TEST_frmtGetFieldDataNoField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtGetFieldDataNoField");
    UtTest_Add(ACCESS_LIB_TEST_frmtInsertDataNominalInt, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtInsertDataNominalInt");
    UtTest_Add(ACCESS_LIB_TEST_frmtInsertDataNominalFloat, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtInsertDataNominalFloat");
    UtTest_Add(ACCESS_LIB_TEST_frmtInsertDataNominalChar, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtInsertDataNominalChar");
    UtTest_Add(ACCESS_LIB_TEST_frmtInsertDataNominalString, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtInsertDataNominalString");
    UtTest_Add(ACCESS_LIB_TEST_frmtInsertDataNoField, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "frmtInsertDataNoField");

    UtTest_Add(ACCESS_LIB_TEST_frmtGetSizeMisAligned2, ACCESS_LIB_Test_Setup, ACCESS_LIB_Test_TearDown, "ACCESS_LIB_TEST_frmtGetSizeMisAligned2");


} /* End AddTestCases */
