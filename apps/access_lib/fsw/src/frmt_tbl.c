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
*   File: frmt_tbl.c
*
*   Purpose: Provides implementation of format table functions
*
*   Author: Daniel McGann
*
*******************************************************************************/

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "frmt_tbl.h"


/*******************************************************************************
*   Function Definitions
*******************************************************************************/

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Gets total size of given format table                                     */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_GetTotalSize(FRMT_TBL_t* TblPtr) {
    int32 result = 0;
    int32 last_offset = 0;
    int32 last_size = 0;

    //iterate through table adding size of each field
    for (int32 i = 0; i < TblPtr->NumEntries; i++) {
        if (TblPtr->Entries[i].Offset >= last_offset) {
            last_offset = TblPtr->Entries[i].Offset;
            last_size = TblPtr->Entries[i].Size;
        }   
    }

    //get total size and ensure it is 32bit boundary aligned
    result = last_offset + last_size;
    if ( (result % 4) != 0) {
        result = result + (4 - (result % 4));
    }

    return result;
} /* End GetTotalSize */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Gets the Field id of the flagged entry                                    */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_GetFlaggedFieldId(FRMT_TBL_t* TblPtr, int32 Flag) {
    for(int32 i = 0; i < TblPtr->NumEntries; i++) {
        // If we have found the correct Id set result to its offset
        if (TblPtr->Entries[i].Flag == Flag) {
            return TblPtr->Entries[i].FieldId;
        }
    }
    //The flag was not found in the format table
    return FRMT_TBL_NOFIELD_ERR;
} /* End GetFlaggedFieldId */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Gets offset to specified field id                                         */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_GetOffsetToField(FRMT_TBL_t* TblPtr, int32 Id) {
    
    //If field Id's are correctly configured Field if should = index of entry
    if ((Id < TblPtr->NumEntries && Id >= 0) && TblPtr->Entries[Id].FieldId == Id) {
        return TblPtr->Entries[Id].Offset;
    }
    else {
        //If the id at TblPtr[Id] does not match check the rest of the table
        for(int32 i = 0; i < TblPtr->NumEntries; i++) {
            // If we have found the correct Id set result to its offset
            if (TblPtr->Entries[i].FieldId == Id) {
                return TblPtr->Entries[i].Offset;
            }
        }
    }

    return FRMT_TBL_NOFIELD_ERR;
} /* GetOffsetToField */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Gets size of data of the specified field id                               */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_GetSizeOfField(FRMT_TBL_t* TblPtr, int32 Id) {

    //If field Id's are correctly configured Field if should = index of entry
    if ((Id < TblPtr->NumEntries && Id >= 0) && TblPtr->Entries[Id].FieldId == Id) {
        return TblPtr->Entries[Id].Size;
    }
    else {
        //If the id at TblPtr[Id] does not match check the rest of the table
        for(int32 i = 0; i < TblPtr->NumEntries; i++) {
            // If we have found the correct Id set result to its offset
            if (TblPtr->Entries[i].FieldId == Id) {
                return  TblPtr->Entries[i].Size;
            }
        }
    }

    return FRMT_TBL_NOFIELD_ERR;
} /* End GetSizeOfField */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Gets a pointer to the data of the desired field                           */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32  FRMT_TBL_GetFieldData(FRMT_TBL_t* TblPtr, char* StructPtr, int32 Id, void* RetPtr) {
    int32 field_size = FRMT_TBL_GetSizeOfField(TblPtr, Id);
    int32 field_offset = FRMT_TBL_GetOffsetToField(TblPtr, Id);

    // Was that field Id Found in the struct
    if (field_size < 0 || field_offset < 0) {
        return FRMT_TBL_NOFIELD_ERR;
    }
    
    //A pointer to the beginning of the field data
    char* field_ptr;
    field_ptr = StructPtr + field_offset;

    //copy field data into return pointer location
    memcpy(RetPtr, field_ptr, field_size);
    return CFE_SUCCESS;
} /* End GetFieldData() */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Inserts data into format table defined struct                             */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_InsertData(FRMT_TBL_t* TblPtr, int32 Id, char* DataPtr, char* StructPtr) {
    int32 field_offset = FRMT_TBL_GetOffsetToField(TblPtr, Id);
    int32 field_size = FRMT_TBL_GetSizeOfField(TblPtr, Id);

    //Does this field exist
    if (field_size < 0 || field_offset < 0) {
        return FRMT_TBL_NOFIELD_ERR;
    }
    
    //Pointer to field destination in struct
    char* field_ptr;
    field_ptr = StructPtr + field_offset;

    //copy given data into desired location
    memcpy(field_ptr, DataPtr, field_size);

    return CFE_SUCCESS;
} /* End InsertData() */


/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**                                                                           */
/** Verify's Tables Contents                                                  */
/**                                                                           */
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 FRMT_TBL_Verify(FRMT_TBL_t* TblPtr) {
    int32 Status = CFE_SUCCESS;
    return Status;
} /* End Verify */
