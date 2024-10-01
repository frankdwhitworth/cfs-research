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
 *   Specification for the BLINKER_APP command and telemetry
 *   message data types.
 *
 * @note
 *   Constants and enumerated types related to these message structures
 *   are defined in blinker_app_msgdefs.h.
 */
#ifndef BLINKER_APP_MSGSTRUCT_H
#define BLINKER_APP_MSGSTRUCT_H

/************************************************************************
 * Includes
 ************************************************************************/

#include "blinker_app_mission_cfg.h"
#include "blinker_app_msgdefs.h"
#include "cfe_msg_hdr.h"

/************************************************************************
 * Commands
 ************************************************************************/

/**
 * No Argument Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BLINKER_APP_NoArgs_t;

/**
 * Display Parameters Structures
 */
typedef struct
{
    CFE_MSG_CommandHeader_t           CommandHeader; /**< \brief Command header */
    BLINKER_APP_DisplayParam_Payload_t Payload;
} BLINKER_APP_DisplayParamCmd_t;

/** 
 * Specific Blinker Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
    uint16 BlinkerNumber; /**<  16 bit unsigned integer for Blinker number */
    uint16 Padding; /**< 16 bit unsigned integer for padding */
} BLINKER_APP_SpecBlinker_t;

/** 
 * Swap two Blinker Command Structure
 */
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader;
    uint16 BlinkerNumber1; /**<  16 bit unsigned integer for Blinker number 1 */
    uint16 BlinkerNumber2; /**<  16 bit unsigned integer for Blinker number 2 */
} BLINKER_APP_SwapBlinkers_t;


/************************************************************************
 * Telemetry
 ************************************************************************/

/*
** Type definition (Blinker App housekeeping)
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CommandHeader; /**< \brief Command header */
} BLINKER_APP_SendHkCmd_t;

typedef struct BLINKER_APP_HkTlm_Payload
{
    uint16 CommandErrorCounter;
    uint16 CommandCounter;
    uint8  BlinkerStatus[BLINKER_APP_TOTAL_BLINKERS];
    uint32 BlinkerCount[BLINKER_APP_TOTAL_BLINKERS];
} BLINKER_APP_HkTlm_Payload_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader; /**< \brief Telemetry header */
    BLINKER_APP_HkTlm_Payload_t Payload;         /**< \brief Telemetry payload */
} BLINKER_APP_HkTlm_t;

#endif /* BLINKER_APP_MSGSTRUCT_H */
