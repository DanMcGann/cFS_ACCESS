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
*   File: frmt_tbls.h
*
*   Purpose: Specifications for access format tables
*
*   AUTHOR: Daniel McGann
*
*******************************************************************************/

#ifndef _frmt_tbl_h_
#define _frmt_tbl_h_

/*******************************************************************************
*   Includes
*******************************************************************************/
#include "cfe.h"
#include "string.h"

/*******************************************************************************
*  MACRO Definitions 
*******************************************************************************/
#define FRMT_TBL_NOFIELD_ERR (-1)
#define FRMT_TBL_POINTER_ERR (-2)
#define FRMT_TBL_VERIFY_ERR (-3)

#define FRMT_TBL_NOFLAG -999

/*******************************************************************************
*  Type Definitions 
*******************************************************************************/
/**
**  \brief Format Table Feild Structure
*/
typedef struct {
    uint32          FieldId; /**< \brief The identification number of the 
                        data field. Should be the same as its relative order
                        in the struct starting at 0. */

    uint32          Offset; /**< \brief The bit offset of this data field in 
                        bits from the beginning of the struct. */

    uint32          Size;   /**< \brief The size of this data field in bits. */

    int32           Flag;   /**< \brief Flag used by ACCESS applications to mark
                        data needed for the functioning of that application. */
} FRMT_TBL_Entry_t;

#define FRMT_TBL_ENTRY_SIZE sizeof ( FRMT_TBL_Entry_t )


typedef struct {
	uint32 NumEntries; /*<< \brief The number of entries in this format table */

	FRMT_TBL_Entry_t	Entries[]; /* << \brief The entries of this format table
	 	 	 	 	 	 can be of any length */
} FRMT_TBL_t;

/*******************************************************************************
*  Function Prototypes
*******************************************************************************/

/******************************************************************************/
/** \brief Gets the total size of the format table
**
**  \par Description 
**      This function return the total size of a given format table
**      as a number of bits.
**
**  \par Assumptions, External Events, and Notes:
**      None
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] NumEntries The Number of entries in the given format table
**
**  \returns 
**  \retstmt Returns uint32 number representing the number of bytes in the format
**      table.
**  \endreturns
**
**  \sa #FRMT_TBL_GetTotalSize
**
*******************************************************************************/
int32 FRMT_TBL_GetTotalSize(FRMT_TBL_t* TblPtr);

/******************************************************************************/
/** \brief Gets the Field Id with the specified flag
**
**  \par Description
**      This function traverses a format table and extracts the Field Id of the
**      first entry flagged with the given flag, if such an entry exists.
**
**  \par Assumptions, External Events, and Notes:
**      This function will only return the first feild with the flag.
**      Application Flags should only flag one feild per format table. 
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] NumEntries The number of entries in the given format table
**
**  \param [in] Flag    Specific Application Falg of desired data
**
**  \returns 
**  \retstmt Returns the Format Table Field Id of the entry with the given flag
**  \retstmt Returns -1 if no entry can be found witht the given flag
**  \endreturns
**
**  \sa #FRMT_TBL_GetFlaggedFieldId
**
*******************************************************************************/
int32 FRMT_TBL_GetFlaggedFieldId(FRMT_TBL_t* TblPtr, int32 Flag);

/******************************************************************************/
/** \brief Gets the offset to the given field
**
**  \par Description 
**      This function calculates the total byte offset to the given field 
**      format table field, in the struct that the format table describes.
**
**  \par Assumptions, External Events, and Notes:
**      None
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] NumEntries The number of entries in this format table
**
**  \param [in] Id      The field id of the desired data
**
**  \returns 
**  \retstmt Returns int32 number representing the number of offset of the
**      specified feidl in the struct that the format table describes.
**  \retstmt Returns FRMT_TBL_NOFIELD_ERR (-1) if no entry can be found witht he given Field Id
**  \endreturns
**
**  \sa #FRMT_TBL_GetOffsetToField
**
*******************************************************************************/
int32 FRMT_TBL_GetOffsetToField(FRMT_TBL_t* TblPtr, int32 Id);

/******************************************************************************/
/** \brief Gets the size of the specified field
**
**  \par Description 
**      This function calculates the size of the specified field from the given 
**      format table.
**
**  \par Assumptions, External Events, and Notes:
**      None
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] Id      The field id of the desired data
**
**  \returns 
**  \retstmt Returns int32 number representing the size of the flagged data 
**      as described in the fiven format table.
**  \retstmt Returns FRMT_TBL_NOFIELD_ERR (-1) if no entry can be found witht the given Field Id
**  \endreturn
**
**  \sa #FRMT_TBL_GetSizeOfField
**
*******************************************************************************/
int32 FRMT_TBL_GetSizeOfField(FRMT_TBL_t* TblPtr, int32 Id);

/******************************************************************************/
/** \brief Gets the data from specified Field in the struct
**
**  \par Description 
**      This function is responsible for pulling out the data of a specific 
**      Field Id from a structure described in the given format table
**
**  \par Assumptions, External Events, and Notes:
**      The StructPtr, points to a valid data region of the strcut type
**      that was used to generate the format table.
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] NumEntries The Number of entries in the format table
**  
**  \param [in] StructPtr Pointer to a data struct/region with the desired data
**
**  \param [in] Id      Field Id of the desired data
**
**  \returns 
**  \retstmt Returns pointer flagged data if the data was found
**  \retstmt Returns FRMT_TBL_NOFIELD_ERR (-1) if field does not exist
**  \retstmt Returns FRMT_TBL_POINTER_ERR (-2) if there was an error when 
**      extracting or copying data
**  \endreturns
**
**  \sa #FRMT_TBL_GetFieldData
**
*******************************************************************************/
int32 FRMT_TBL_GetFieldData(FRMT_TBL_t* TblPtr, char* StructPtr, int32 Id, void* RetPtr);

/******************************************************************************/
/** \brief Adds data to unknown struct
**
**  \par Description 
**      This function takes a pointer to a piece of data, and inserts it as
**      a field in the given struct
**
**  \par Assumptions, External Events, and Notes:
**      The StructPtr points to a struct of the type defined int the format 
**      table.
**      The DataPtr points to a piece of data defined by the Given FieldID
**      of the format table.
**      The Size of the data at location DataPtr Must be equal to the size of
**      the field specified by field id.
**
**  \param [in] TblPtr  Pointer to a format table
**
**  \param [in] NumEntries The number of entries in the format table
**
**  \param [in] Id      The FieldId of the struct field to be populated
**
**  \param [in] DataPtr The data to be added to the struct
**
**  \param [in] StructPtr The Struct to which the data should be added
**
**  \returns 
**  \retstmt Returns CFE_SUCCESS if operation is successful
**  \retstmt Returns FRMT_TBL_NOFEILD (-1) if feild id does not exist 
**  \retstmt Returns FRMT_TBL_POINTER_ERR (-2) if there was an error when 
**      extracting or copying data
**  \endreturns
**
**  \sa #FRMT_TBL_InsertData
**
*******************************************************************************/
int32 FRMT_TBL_InsertData(FRMT_TBL_t* TblPtr, int32 Id, char* DataPtr, char* StructPtr);

/******************************************************************************/
/** \brief Validates the Format Table
**
**  \par Description 
**      This function Preforms Necissary table validation for any given 
**      format table
**
**  \par Assumptions, External Events, and Notes:
**          This function CANNOT BE USED AS a  validatation callback during 
**          table registration
**          Because format tables have little to no self knowlege this function
**          must be called from within a application specific format table 
**          verification function. Otherwise the format table would have no 
**          information on the number of field entries.  
**
**  \param [in] TblPtr - A pointer to the table data.
**
**  \param [in] NumEntries - The number of entries in this format table
**
**  \returns 
**  \retstmt Returns CFE_SUCCESS if table is valid
**  \retstmt Returns FRMT_TBL_VERIFY_ERR (-3) if a an entry in the table 
                not valid. 
**  \endreturns
*******************************************************************************/
int32 FRMT_TBL_Verify(FRMT_TBL_t* TblPtr);

#endif /* _frmt_tbls_h_ */
