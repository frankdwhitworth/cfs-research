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
 * \file
 *   This file contains the source code for the Blinker App Ground Command-handling functions
 */

/*
** Include Files:
*/
#include "blinker_app.h"
#include "blinker_app_cmds.h"
#include "blinker_app_msgids.h"
#include "blinker_app_eventids.h"
#include "blinker_app_version.h"
#include "blinker_app_tbl.h"
#include "blinker_app_utils.h"
#include "blinker_app_msg.h"

/* The blinker_lib module provides the BLINKER_Function() prototype */
// #include "blinker_lib.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t BLINKER_APP_SendHkCmd(const BLINKER_APP_SendHkCmd_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    BLINKER_APP_Data.HkTlm.Payload.CommandErrorCounter = BLINKER_APP_Data.ErrCounter;
    BLINKER_APP_Data.HkTlm.Payload.CommandCounter      = BLINKER_APP_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(BLINKER_APP_Data.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(BLINKER_APP_Data.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < BLINKER_APP_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(BLINKER_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* BLINKER NOOP commands                                                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t BLINKER_APP_NoopCmd(const BLINKER_APP_NoopCmd_t *Msg)
{
    BLINKER_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(BLINKER_APP_NOOP_INF_EID, CFE_EVS_EventType_INFORMATION, "BLINKER: NOOP command %s",
                      BLINKER_APP_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t BLINKER_APP_ResetCountersCmd(const BLINKER_APP_ResetCountersCmd_t *Msg)
{
    BLINKER_APP_Data.CmdCounter = 0;
    BLINKER_APP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(BLINKER_APP_RESET_INF_EID, CFE_EVS_EventType_INFORMATION, "BLINKER: RESET command");

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
CFE_Status_t BLINKER_APP_ProcessCmd(const BLINKER_APP_ProcessCmd_t *Msg)
{
    CFE_Status_t               status;
    void *                     TblAddr;
    BLINKER_APP_ExampleTable_t *TblPtr;
    const char *               TableName = "BLINKER_APP.ExampleTable";

    /* Blinker Use of Example Table */

    status = CFE_TBL_GetAddress(&TblAddr, BLINKER_APP_Data.TblHandles[0]);

    if (status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Blinker App: Fail to get table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    TblPtr = TblAddr;
    CFE_ES_WriteToSysLog("Blinker App: Example Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

    BLINKER_APP_GetCrc(TableName);

    status = CFE_TBL_ReleaseAddress(BLINKER_APP_Data.TblHandles[0]);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Blinker App: Fail to release table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    /* Invoke a function provided by BLINKER_APP_LIB */
    // BLINKER_LIB_Function();

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* A simple example command that displays a passed-in value                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
CFE_Status_t BLINKER_APP_DisplayParamCmd(const BLINKER_APP_DisplayParamCmd_t *Msg)
{
    CFE_EVS_SendEvent(BLINKER_APP_VALUE_INF_EID, CFE_EVS_EventType_INFORMATION,
                      "BLINKER_APP: ValU32=%lu, ValI16=%d, ValStr=%s", (unsigned long)Msg->Payload.ValU32,
                      (int)Msg->Payload.ValI16, Msg->Payload.ValStr);

    return CFE_SUCCESS;
}
