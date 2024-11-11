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
 *   This file contains the source code for the Test Consumer App.
 */

/*
** Include Files:
*/
#include "tst_csmr_app_events.h"
#include "tst_csmr_app_version.h"
#include "tst_csmr_app.h"
#include "tst_csmr_app_table.h"
#include "tst_prod_app_msgids.h"


#include <string.h>


/*
** global data
*/
TST_CSMR_APP_Data_t TST_CSMR_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void TST_CSMR_APP_Main(void)
{
    int32            status;
    CFE_SB_Buffer_t *SBBufPtr;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(TST_CSMR_APP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = TST_CSMR_APP_Init();
    if (status != CFE_SUCCESS)
    {
        TST_CSMR_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** TST_CSMR_APP Runloop
    */
    while (CFE_ES_RunLoop(&TST_CSMR_APP_Data.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(TST_CSMR_APP_PERF_ID);

        status = CFE_SB_ReceiveBuffer(&SBBufPtr, TST_CSMR_APP_Data.WakeupPipe, CFE_SB_PEND_FOREVER);

        OS_printf("Received Wakeup...\n");
        while (CFE_SB_ReceiveBuffer(&SBBufPtr, TST_CSMR_APP_Data.CommandPipe, CFE_SB_POLL) != CFE_SB_NO_MESSAGE)
        {
            /*
            ** Performance Log Entry Stamp
            */
            CFE_ES_PerfLogEntry(TST_CSMR_APP_PERF_ID);

            if (status == CFE_SUCCESS)
            {
                TST_CSMR_APP_ProcessCommandPacket(SBBufPtr);
            }
            else
            {
                CFE_EVS_SendEvent(TST_CSMR_APP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                                "TST_CSMR_APP: SB Pipe Read Error, App Will Exit");

                TST_CSMR_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
            }
        }
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(TST_CSMR_APP_PERF_ID);

    CFE_ES_ExitApp(TST_CSMR_APP_Data.RunStatus);
}

void TST_CSMR_APP_SubscribeTstMids(void)
{
    int32 status;

    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_TRAFFIC_0001_MID), TST_CSMR_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Subscribing to TST_MID, RC = 0x%08lX\n", (unsigned long)status);

        return;
    }

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_CSMR_APP_Init(void)
{
    int32 status;

    TST_CSMR_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    TST_CSMR_APP_Data.CmdCounter = 0;
    TST_CSMR_APP_Data.ErrCounter = 0;

    /*
    ** Initialize app configuration data
    */
    TST_CSMR_APP_Data.PipeDepth = TST_CSMR_APP_PIPE_DEPTH;

    strncpy(TST_CSMR_APP_Data.PipeName, "TST_CSMR_APP_CMD_PIPE", sizeof(TST_CSMR_APP_Data.PipeName));
    TST_CSMR_APP_Data.PipeName[sizeof(TST_CSMR_APP_Data.PipeName) - 1] = 0;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_MSG_Init(CFE_MSG_PTR(TST_CSMR_APP_Data.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(TST_CSMR_APP_HK_TLM_MID),
                 sizeof(TST_CSMR_APP_Data.HkTlm));

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&TST_CSMR_APP_Data.CommandPipe, TST_CSMR_APP_Data.PipeDepth, TST_CSMR_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_CSMR_APP_SEND_HK_MID), TST_CSMR_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to ground command packets
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_CSMR_APP_CMD_MID), TST_CSMR_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);

        return status;
    }

    /*
    ** Create Software Bus message pipe for wakeup message (TST_CSMR_WAKEUP_CMD_MID).
    */
    /*
    ** Initialize app configuration data
    */
    strncpy(TST_CSMR_APP_Data.PipeName, "TST_CSMR_APP_WAKEUP_PIPE", sizeof(TST_CSMR_APP_Data.PipeName));
    TST_CSMR_APP_Data.PipeName[sizeof(TST_CSMR_APP_Data.PipeName) - 1] = 0;
    status = CFE_SB_CreatePipe(&TST_CSMR_APP_Data.WakeupPipe, TST_CSMR_APP_Data.PipeDepth, TST_CSMR_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to wakeup mid request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_CSMR_WAKEUP_CMD_MID), TST_CSMR_APP_Data.WakeupPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }
    else
    {
        OS_printf("SUBSCRIBED TO WAKEUP MID (0x%07X)\n\n\n\n\n", TST_CSMR_WAKEUP_CMD_MID);
    }

    (void)TST_CSMR_APP_SubscribeTstMids();

    /*
    ** Register Table(s)
    */
    status = CFE_TBL_Register(&TST_CSMR_APP_Data.TblHandles[0], "TstCsmrAppTable", sizeof(TST_CSMR_APP_Table_t),
                              CFE_TBL_OPT_DEFAULT, TST_CSMR_APP_TblValidationFunc);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Registering Table, RC = 0x%08lX\n", (unsigned long)status);

        return status;
    }
    else
    {
        status = CFE_TBL_Load(TST_CSMR_APP_Data.TblHandles[0], CFE_TBL_SRC_FILE, TST_CSMR_APP_TABLE_FILE);
    }

    CFE_EVS_SendEvent(TST_CSMR_APP_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_CSMR_APP Initialized.%s",
                      TST_CSMR_APP_VERSION_STRING);

    return CFE_SUCCESS;
}

void TST_CSMR_APP_SendTestCmd(void)
{
    TST_CSMR_APP_Data.totMsgCount++;
    OS_printf("[CSMR] Received (%d)\n", TST_CSMR_APP_Data.totMsgCount);
    CFE_MSG_Init(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), CFE_SB_ValueToMsgId(TST_TRAFFIC_0002_MID),
                    sizeof(TST_CSMR_APP_Data.TestMsg));
    // CFE_MSG_SetFcnCode(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), 5);
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), true);
    OS_printf("[CSMR] Sent (%d)\n", TST_CSMR_APP_Data.totMsgCount);
    // CFE_EVS_SendEvent(TST_CSMR_APP_SENT_COMPLETE_INF_EID, CFE_EVS_EventType_INFORMATION, "Sending test complete");
}

void TST_CSMR_APP_SendCompleteCmd(void)
{
    TST_CSMR_APP_Data.totMsgCount++;
    OS_printf("[CSMR] Completing test (%d)\n", TST_CSMR_APP_Data.totMsgCount);
    CFE_MSG_Init(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), CFE_SB_ValueToMsgId(TST_TRAFFIC_0003_MID),
                    sizeof(TST_CSMR_APP_Data.TestMsg));
    // CFE_MSG_SetFcnCode(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), 4);
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.TestMsg.CmdHeader), true);
    CFE_EVS_SendEvent(TST_CSMR_APP_SENT_COMPLETE_INF_EID, CFE_EVS_EventType_INFORMATION, "Sending test complete");
}

void TST_CSMR_APP_ProcessTestMID(TST_PROD_APP_TestMsg_t *Msg)
{
    // CFE_EVS_SendEvent(TST_CSMR_APP_RCVD_TST_MID_INF_EID, CFE_EVS_EventType_INFORMATION, "Received test MID (%d)", Msg->Payload);

    switch(Msg->Payload)
    {
        case TST_START_MSG:
            TST_CSMR_APP_Data.TestInProgress = 0;
            TST_CSMR_APP_Data.totMsgCount = 0;
            OS_printf("[CSMR] Starting test (%d)\n", TST_CSMR_APP_Data.totMsgCount);
            // break;
        case TST_MIDDLE_MSG:
            TST_CSMR_APP_SendTestCmd();
            break;
        case TST_END_MSG:
            TST_CSMR_APP_SendCompleteCmd();
            TST_CSMR_APP_Data.TestInProgress = 0;
            break;
        default:
            CFE_EVS_SendEvent(TST_CSMR_APP_BAD_TEST_MSG_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TST_CSMR_APP: invalid test message with bad payload rcvd");
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the TST_CSMR_APP */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void TST_CSMR_APP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case TST_CSMR_APP_CMD_MID:
            TST_CSMR_APP_ProcessGroundCommand(SBBufPtr);
            break;

        case TST_CSMR_APP_SEND_HK_MID:
            TST_CSMR_APP_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        case TST_TRAFFIC_0001_MID:
            TST_CSMR_APP_ProcessTestMID((TST_PROD_APP_TestMsg_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(TST_CSMR_APP_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TST_CSMR_APP: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_CSMR_APP ground commands                                                 */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void TST_CSMR_APP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" TST_CSMR_APP ground commands
    */
    switch (CommandCode)
    {
        case TST_CSMR_APP_NOOP_CC:
            if (TST_CSMR_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_CSMR_APP_NoopCmd_t)))
            {
                TST_CSMR_APP_Noop((TST_CSMR_APP_NoopCmd_t *)SBBufPtr);
            }

            break;

        case TST_CSMR_APP_RESET_COUNTERS_CC:
            if (TST_CSMR_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_CSMR_APP_ResetCountersCmd_t)))
            {
                TST_CSMR_APP_ResetCounters((TST_CSMR_APP_ResetCountersCmd_t *)SBBufPtr);
            }

            break;

        case TST_CSMR_APP_PROCESS_CC:
            if (TST_CSMR_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_CSMR_APP_ProcessCmd_t)))
            {
                TST_CSMR_APP_Process((TST_CSMR_APP_ProcessCmd_t *)SBBufPtr);
            }

            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(TST_CSMR_APP_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
                              "Invalid ground command code: CC = %d", CommandCode);
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function is triggered in response to a task telemetry request */
/*         from the housekeeping task. This function will gather the Apps     */
/*         telemetry, packetize it and send it to the housekeeping task via   */
/*         the software bus                                                   */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 TST_CSMR_APP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    TST_CSMR_APP_Data.HkTlm.Payload.CommandErrorCounter = TST_CSMR_APP_Data.ErrCounter;
    TST_CSMR_APP_Data.HkTlm.Payload.CommandCounter      = TST_CSMR_APP_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_CSMR_APP_Data.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < TST_CSMR_APP_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(TST_CSMR_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_CSMR_APP NOOP commands                                                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_CSMR_APP_Noop(const TST_CSMR_APP_NoopCmd_t *Msg)
{
    TST_CSMR_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(TST_CSMR_APP_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_CSMR_APP: NOOP command %s",
                      TST_CSMR_APP_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 TST_CSMR_APP_ResetCounters(const TST_CSMR_APP_ResetCountersCmd_t *Msg)
{
    TST_CSMR_APP_Data.CmdCounter = 0;
    TST_CSMR_APP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(TST_CSMR_APP_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_CSMR_APP: RESET command");

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 TST_CSMR_APP_Process(const TST_CSMR_APP_ProcessCmd_t *Msg)
{
    int32               status;
    TST_CSMR_APP_Table_t *TblPtr;
    const char *        TableName = "TST_CSMR_APP.TstCsmrAppTable";

    /* Test Consumer Use of Table */

    status = CFE_TBL_GetAddress((void *)&TblPtr, TST_CSMR_APP_Data.TblHandles[0]);

    if (status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Fail to get table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    CFE_ES_WriteToSysLog("Test Consumer App: Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

    TST_CSMR_APP_GetCrc(TableName);

    status = CFE_TBL_ReleaseAddress(TST_CSMR_APP_Data.TblHandles[0]);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Fail to release table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Verify command packet length                                               */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool TST_CSMR_APP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
{
    bool              result       = true;
    size_t            ActualLength = 0;
    CFE_SB_MsgId_t    MsgId        = CFE_SB_INVALID_MSG_ID;
    CFE_MSG_FcnCode_t FcnCode      = 0;

    CFE_MSG_GetSize(MsgPtr, &ActualLength);

    /*
    ** Verify the command packet length.
    */
    if (ExpectedLength != ActualLength)
    {
        CFE_MSG_GetMsgId(MsgPtr, &MsgId);
        CFE_MSG_GetFcnCode(MsgPtr, &FcnCode);

        CFE_EVS_SendEvent(TST_CSMR_APP_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        TST_CSMR_APP_Data.ErrCounter++;
    }

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify contents of First Table buffer contents                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TST_CSMR_APP_TblValidationFunc(void *TblData)
{
    int32               ReturnCode = CFE_SUCCESS;
    TST_CSMR_APP_Table_t *TblDataPtr = (TST_CSMR_APP_Table_t *)TblData;

    /*
    ** Test Consumer Table Validation
    */
    if (TblDataPtr->Int1 > TST_CSMR_APP_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range, return an appropriate error code */
        ReturnCode = TST_CSMR_APP_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Output CRC                                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TST_CSMR_APP_GetCrc(const char *TableName)
{
    int32          status;
    uint32         Crc;
    CFE_TBL_Info_t TblInfoPtr;

    status = CFE_TBL_GetInfo(&TblInfoPtr, TableName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Consumer App: Error Getting Table Info");
    }
    else
    {
        Crc = TblInfoPtr.Crc;
        CFE_ES_WriteToSysLog("Test Consumer App: CRC: 0x%08lX\n\n", (unsigned long)Crc);
    }
}
