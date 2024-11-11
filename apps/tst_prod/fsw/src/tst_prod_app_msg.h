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
 * Define TST_PROD_APP Messages and info
 */

#ifndef TST_PROD_APP_MSG_H
#define TST_PROD_APP_MSG_H

/*
** TST_PROD_APP command codes
*/
#define TST_PROD_APP_NOOP_CC            0
#define TST_PROD_APP_RESET_COUNTERS_CC  1
#define TST_PROD_APP_PROCESS_CC         2
#define TST_PROD_APP_START_TEST_CC      3
#define TST_PROD_APP_COMPLETE_TEST_CC   4
#define TST_PROD_APP_TST_MSG_CC         5
#define TST_PROD_APP_CHANGE_NUM_MSGS_CC 6

/*************************************************************************/

/*
** Type definition (generic "no arguments" command)
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
} TST_PROD_APP_NoArgsCmd_t;

/*
** Type definition (uint32 as payload)
*/
typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader; /**< \brief Command header */
    uint32 NumMessages;
} TST_PROD_APP_NumMessagesCmd_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader;
    uint8 Payload;
} TST_PROD_APP_TestMsg_t;

typedef struct
{
    CFE_MSG_CommandHeader_t CmdHeader;
} TST_PROD_APP_WakeupMsg_t;

/*
** The following commands all share the "NoArgs" format
**
** They are each given their own type name matching the command name, which
** allows them to change independently in the future without changing the prototype
** of the handler function
*/
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_NoopCmd_t;
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_ResetCountersCmd_t;
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_ProcessCmd_t;
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_StartCmd_t;
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_CompleteCmd_t;
typedef TST_PROD_APP_NoArgsCmd_t TST_PROD_APP_TestCmd_t;

/*************************************************************************/
/*
** Type definition (Test Producer App housekeeping)
*/

typedef struct
{
    uint8 CommandErrorCounter;
    uint8 CommandCounter;
    uint8 spare[2];
} TST_PROD_APP_HkTlm_Payload_t;

typedef struct
{
    CFE_MSG_TelemetryHeader_t  TelemetryHeader; /**< \brief Telemetry header */
    TST_PROD_APP_HkTlm_Payload_t Payload;         /**< \brief Telemetry payload */
} TST_PROD_APP_HkTlm_t;


#endif /* TST_PROD_APP_MSG_H */
