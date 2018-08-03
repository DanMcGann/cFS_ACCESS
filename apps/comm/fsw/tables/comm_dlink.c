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
*	File: comm_dlink.c
*
*	Purpose: Defines default satellite downlink file table
*
*	Notes: Mission Software developer should modify this table for mission
*	specific files to downlinkSS on satellite's initial downlink
*
*	Author: Daniel McGann
******************************************************************************/

/*
 * Includes
 */
#include "cfe.h"
#include "cfe_tbl_filedef.h"
#include "comm_tables.h"
#include "comm_platform_cfg.h"

COMM_LinkFile_t COMM_DLinkTbl[COMM_MAX_DLINK_FILES] = {

		/* Entry 0 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 1 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 2 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 3 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 4 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 5 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 6 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 7 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 8 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 9 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 10 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 11 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 12 ***********************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 13 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 14 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 15 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 16************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 17************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 18************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 19************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 20************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 21************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 22************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 23************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 24************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 0 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 1 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 2 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 3 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 4 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 5 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 6 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 7 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 8 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 9 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 10 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 11 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 12 ***********************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 13 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 14 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 15 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 16************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 17************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 18************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 19************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 20************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 21************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 22************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 23************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 24************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 0 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 1 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 2 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 3 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 4 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 5 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 6 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 7 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 8 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 9 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 10 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 11 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 12 ***********************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 13 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 14 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 15 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 16************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 17************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 18************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 19************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 20************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 21************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 22************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 23************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 24************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 25 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 26 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 27 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 28 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 29************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 5 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 6 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 7 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 8 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 9 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 10 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 11 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 12 ***********************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 13 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 14 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 15 ************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 16************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 17************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 18************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 19************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 20************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 21************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 22************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 23************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		},
		/* Entry 24************************************************************************************/
		{
				.Used = FALSE,			/* Is this entry in use*/
				.RetainFlag = FALSE,	/* Should this file be down linked on each link */
				.DeleteOnLink = TRUE,	/* Should this file be removed from the file system after link*/
				.LinkAttempts = 0,		/* The number of links this files has failed to be linked */
				.MaxLinkAttempts = 0,	/* The Max number of links that should be attempted before
										   this file is considered stale and is thrown out*/
				.Priority = 255,		/* The priority of linking this file  1 [HIGH] -> 256 [LOW] */
				.File = ""				/* The path and name of the file to be linked */
		}
};




/*
 * Table Header
 */
CFE_TBL_FILEDEF(COMM_DLinkTbl, // @suppress("Unused variable declaration in file scope")
		COMM_APP.DLINK,
		COMM Downlink file Table,
		comm_dlink.tbl)

/**********************
* End of File Comment *
**********************/
