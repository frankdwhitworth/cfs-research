/************************************************************************
 * NASA Docket No. GSC-18,719-1, and identified as “core Flight System: Bootes”
 *
 * Copyright (c) 2020 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License. You may obtain
 * a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ************************************************************************/

/**
 * @file
 *
 * Main header file for the Blinker application
 */

#ifndef BLINKER_APP_H
#define BLINKER_APP_H

/*
** Required header files.
*/
#include "cfe.h"
#include "cfe_config.h"

#include "blinker_app_mission_cfg.h"
#include "blinker_app_platform_cfg.h"

#include "blinker_app_perfids.h"
#include "blinker_app_msgids.h"
#include "blinker_app_msg.h"

/************************************************************************
** Type Definitions
*************************************************************************/

/*
** Global Data
*/
typedef struct
{
    /*
    ** Command interface counters...
    */
    uint8 CmdCounter;
    uint8 ErrCounter;

    /*
    ** Housekeeping telemetry packet...
    */
    BLINKER_APP_HkTlm_t HkTlm;

    /*
    ** Run Status variable used in the main processing loop
    */
    uint32 RunStatus;

    /*
    ** Operational data (not reported in housekeeping)...
    */
    CFE_SB_PipeId_t CommandPipe;

    /* Blinker increment data pipe */
    CFE_SB_PipeId_t BlinkerPipe;

    /*
    ** Initialization data (not reported in housekeeping)...
    */
    char   PipeName[CFE_MISSION_MAX_API_LEN];
    uint16 PipeDepth;

    CFE_TBL_Handle_t TblHandles[BLINKER_APP_NUMBER_OF_TABLES];

    uint16 BlinkerCounts[BLINKER_APP_TOTAL_BLINKERS];
    uint8 BlinkerStatuses[BLINKER_APP_TOTAL_BLINKERS];
} BLINKER_APP_Data_t;

/*
** Global data structure
*/
extern BLINKER_APP_Data_t BLINKER_APP_Data;

/****************************************************************************/
/*
** Local function prototypes.
**
** Note: Except for the entry point (BLINKER_APP_Main), these
**       functions are not called from any other source module.
*/
void         BLINKER_APP_Main(void);
CFE_Status_t BLINKER_APP_Init(void);

#endif /* BLINKER_APP_H */
