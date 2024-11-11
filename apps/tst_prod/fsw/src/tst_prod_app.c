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
 *   This file contains the source code for the Test Producer App.
 */

/*
** Include Files:
*/
#include "tst_prod_app_events.h"
#include "tst_prod_app_version.h"
#include "tst_prod_app.h"
#include "tst_prod_app_table.h"


#include <string.h>


/*
** global data
*/
TST_PROD_APP_Data_t TST_PROD_APP_Data;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
/*                                                                            */
/* Application entry point and main process loop                              */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * *  * * * * **/
void TST_PROD_APP_Main(void)
{
    int32            status;
    CFE_SB_Buffer_t *SBBufPtr;
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    /*
    ** Create the first Performance Log entry
    */
    CFE_ES_PerfLogEntry(TST_PROD_APP_PERF_ID);

    /*
    ** Perform application specific initialization
    ** If the Initialization fails, set the RunStatus to
    ** CFE_ES_RunStatus_APP_ERROR and the App will not enter the RunLoop
    */
    status = TST_PROD_APP_Init();
    if (status != CFE_SUCCESS)
    {
        TST_PROD_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
    }

    /*
    ** TST_PROD_APP Runloop
    */
    while (CFE_ES_RunLoop(&TST_PROD_APP_Data.RunStatus) == true)
    {
        /*
        ** Performance Log Exit Stamp
        */
        CFE_ES_PerfLogExit(TST_PROD_APP_PERF_ID);

        if (TST_PROD_APP_Data.TestInProgress == 1)
        {
            status = CFE_SB_ReceiveBuffer(&SBBufPtr, TST_PROD_APP_Data.WakeupPipe, CFE_SB_PEND_FOREVER);

            while (CFE_SB_ReceiveBuffer(&SBBufPtr, TST_PROD_APP_Data.TestPipe, CFE_SB_POLL) != CFE_SB_NO_MESSAGE)
            {
                /*
                ** Performance Log Entry Stamp
                */
                CFE_ES_PerfLogEntry(TST_PROD_APP_PERF_ID);


                CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

                switch (CFE_SB_MsgIdToValue(MsgId))
                {
                    case TST_TRAFFIC_0002_MID:
                        TST_PROD_APP_TestMsg((TST_PROD_APP_TestCmd_t *)SBBufPtr);
                        break;

                    case TST_TRAFFIC_0003_MID:
                        TST_PROD_APP_Complete((TST_PROD_APP_TestCmd_t *)SBBufPtr);
                        break;

                    default:
                        CFE_EVS_SendEvent(TST_PROD_APP_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                                        "TST_PROD_APP: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
                        break;
                }
            }
        }

        /* Pend on receipt of command packet */
        status = CFE_SB_ReceiveBuffer(&SBBufPtr, TST_PROD_APP_Data.CommandPipe, CFE_SB_PEND_FOREVER);

        /*
        ** Performance Log Entry Stamp
        */
        CFE_ES_PerfLogEntry(TST_PROD_APP_PERF_ID);

        if (status == CFE_SUCCESS)
        {
            TST_PROD_APP_ProcessCommandPacket(SBBufPtr);
        }
        else
        {
            CFE_EVS_SendEvent(TST_PROD_APP_PIPE_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TST_PROD_APP: SB Pipe Read Error, App Will Exit");

            TST_PROD_APP_Data.RunStatus = CFE_ES_RunStatus_APP_ERROR;
        }
    }

    /*
    ** Performance Log Exit Stamp
    */
    CFE_ES_PerfLogExit(TST_PROD_APP_PERF_ID);

    CFE_ES_ExitApp(TST_PROD_APP_Data.RunStatus);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  */
/*                                                                            */
/* Initialization                                                             */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_PROD_APP_Init(void)
{
    int32 status;

    TST_PROD_APP_Data.RunStatus = CFE_ES_RunStatus_APP_RUN;

    /*
    ** Initialize app command execution counters
    */
    TST_PROD_APP_Data.CmdCounter = 0;
    TST_PROD_APP_Data.ErrCounter = 0;

    TST_PROD_APP_Data.NumMsgsPerTest = TST_PROD_APP_DEFAULT_NUM_MSG_PER_TEST;

    /*
    ** Initialize app configuration data
    */
    TST_PROD_APP_Data.PipeDepth = TST_PROD_APP_PIPE_DEPTH;

    strncpy(TST_PROD_APP_Data.PipeName, "TST_PROD_APP_CMD_PIPE", sizeof(TST_PROD_APP_Data.PipeName));
    TST_PROD_APP_Data.PipeName[sizeof(TST_PROD_APP_Data.PipeName) - 1] = 0;

    /*
    ** Register the events
    */
    status = CFE_EVS_Register(NULL, 0, CFE_EVS_EventFilter_BINARY);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Registering Events, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Initialize housekeeping packet (clear user data area).
    */
    CFE_MSG_Init(CFE_MSG_PTR(TST_PROD_APP_Data.HkTlm.TelemetryHeader), CFE_SB_ValueToMsgId(TST_PROD_APP_HK_TLM_MID),
                 sizeof(TST_PROD_APP_Data.HkTlm));


    CFE_MSG_Init(CFE_MSG_PTR(TST_PROD_APP_Data.TestMsg.CmdHeader), CFE_SB_ValueToMsgId(TST_TRAFFIC_0001_MID), 
                 sizeof(TST_PROD_APP_Data.TestMsg));

    CFE_MSG_Init(CFE_MSG_PTR(TST_PROD_APP_Data.WakeupMsg.CmdHeader), CFE_SB_ValueToMsgId(TST_CSMR_WAKEUP_CMD_MID), 
                 sizeof(TST_PROD_APP_Data.WakeupMsg));

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&TST_PROD_APP_Data.CommandPipe, TST_PROD_APP_Data.PipeDepth, TST_PROD_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_PROD_APP_SEND_HK_MID), TST_PROD_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to ground command packets
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_PROD_APP_CMD_MID), TST_PROD_APP_Data.CommandPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Subscribing to Command, RC = 0x%08lX\n", (unsigned long)status);

        return status;
    }


    strncpy(TST_PROD_APP_Data.PipeName, "TST_PROD_APP_WAKEUP_PIPE", sizeof(TST_PROD_APP_Data.PipeName));
    TST_PROD_APP_Data.PipeName[sizeof(TST_PROD_APP_Data.PipeName) - 1] = 0;

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&TST_PROD_APP_Data.WakeupPipe, TST_PROD_APP_Data.PipeDepth, TST_PROD_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_PROD_WAKEUP_CMD_MID), TST_PROD_APP_Data.WakeupPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }


    strncpy(TST_PROD_APP_Data.PipeName, "TST_PROD_APP_TEST_PIPE", sizeof(TST_PROD_APP_Data.PipeName));
    TST_PROD_APP_Data.PipeName[sizeof(TST_PROD_APP_Data.PipeName) - 1] = 0;

    /*
    ** Create Software Bus message pipe.
    */
    status = CFE_SB_CreatePipe(&TST_PROD_APP_Data.TestPipe, TST_PROD_APP_Data.PipeDepth, TST_PROD_APP_Data.PipeName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error creating pipe, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_TRAFFIC_0002_MID), TST_PROD_APP_Data.TestPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Subscribe to Housekeeping request commands
    */
    status = CFE_SB_Subscribe(CFE_SB_ValueToMsgId(TST_TRAFFIC_0003_MID), TST_PROD_APP_Data.TestPipe);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Subscribing to HK request, RC = 0x%08lX\n", (unsigned long)status);
        return status;
    }

    /*
    ** Register Table(s)
    */
    status = CFE_TBL_Register(&TST_PROD_APP_Data.TblHandles[0], "TstProdAppTable", sizeof(TST_PROD_APP_Table_t),
                              CFE_TBL_OPT_DEFAULT, TST_PROD_APP_TblValidationFunc);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Registering Table, RC = 0x%08lX\n", (unsigned long)status);

        return status;
    }
    else
    {
        status = CFE_TBL_Load(TST_PROD_APP_Data.TblHandles[0], CFE_TBL_SRC_FILE, TST_PROD_APP_TABLE_FILE);
    }

    CFE_EVS_SendEvent(TST_PROD_APP_STARTUP_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_PROD_APP Initialized.%s",
                      TST_PROD_APP_VERSION_STRING);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*     This routine will process any packet that is received on the TST_PROD_APP */
/*     command pipe.                                                          */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
void TST_PROD_APP_ProcessCommandPacket(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_SB_MsgId_t MsgId = CFE_SB_INVALID_MSG_ID;

    CFE_MSG_GetMsgId(&SBBufPtr->Msg, &MsgId);

    switch (CFE_SB_MsgIdToValue(MsgId))
    {
        case TST_PROD_APP_CMD_MID:
            TST_PROD_APP_ProcessGroundCommand(SBBufPtr);
            break;

        case TST_PROD_APP_SEND_HK_MID:
            TST_PROD_APP_ReportHousekeeping((CFE_MSG_CommandHeader_t *)SBBufPtr);
            break;

        default:
            CFE_EVS_SendEvent(TST_PROD_APP_INVALID_MSGID_ERR_EID, CFE_EVS_EventType_ERROR,
                              "TST_PROD_APP: invalid command packet,MID = 0x%x", (unsigned int)CFE_SB_MsgIdToValue(MsgId));
            break;
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_PROD_APP ground commands                                                 */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
void TST_PROD_APP_ProcessGroundCommand(CFE_SB_Buffer_t *SBBufPtr)
{
    CFE_MSG_FcnCode_t CommandCode = 0;

    CFE_MSG_GetFcnCode(&SBBufPtr->Msg, &CommandCode);

    /*
    ** Process "known" TST_PROD_APP ground commands
    */
    switch (CommandCode)
    {
        case TST_PROD_APP_NOOP_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_NoopCmd_t)))
            {
                TST_PROD_APP_Noop((TST_PROD_APP_NoopCmd_t *)SBBufPtr);
            }

            break;

        case TST_PROD_APP_RESET_COUNTERS_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_ResetCountersCmd_t)))
            {
                TST_PROD_APP_ResetCounters((TST_PROD_APP_ResetCountersCmd_t *)SBBufPtr);
            }

            break;

        case TST_PROD_APP_PROCESS_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_ProcessCmd_t)))
            {
                TST_PROD_APP_Process((TST_PROD_APP_ProcessCmd_t *)SBBufPtr);
            }

            break;

        case TST_PROD_APP_START_TEST_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_StartCmd_t)))
            {
                TST_PROD_APP_Start((TST_PROD_APP_StartCmd_t *)SBBufPtr);
            }
            
            break;

        case TST_PROD_APP_COMPLETE_TEST_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_CompleteCmd_t)))
            {
                TST_PROD_APP_Complete((TST_PROD_APP_CompleteCmd_t *)SBBufPtr);
            }
            break;

        case TST_PROD_APP_TST_MSG_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_TestCmd_t)))
            {
                TST_PROD_APP_TestMsg((TST_PROD_APP_TestCmd_t *)SBBufPtr);
            }
            break;

        case TST_PROD_APP_CHANGE_NUM_MSGS_CC:
            if (TST_PROD_APP_VerifyCmdLength(&SBBufPtr->Msg, sizeof(TST_PROD_APP_NumMessagesCmd_t)))
            {
                TST_PROD_APP_ChangeNumMsgs((TST_PROD_APP_NumMessagesCmd_t *)SBBufPtr);
            }
            break;

        /* default case already found during FC vs length test */
        default:
            CFE_EVS_SendEvent(TST_PROD_APP_COMMAND_ERR_EID, CFE_EVS_EventType_ERROR,
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
int32 TST_PROD_APP_ReportHousekeeping(const CFE_MSG_CommandHeader_t *Msg)
{
    int i;

    /*
    ** Get command execution counters...
    */
    TST_PROD_APP_Data.HkTlm.Payload.CommandErrorCounter = TST_PROD_APP_Data.ErrCounter;
    TST_PROD_APP_Data.HkTlm.Payload.CommandCounter      = TST_PROD_APP_Data.CmdCounter;

    /*
    ** Send housekeeping telemetry packet...
    */
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_PROD_APP_Data.HkTlm.TelemetryHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_PROD_APP_Data.HkTlm.TelemetryHeader), true);

    /*
    ** Manage any pending table loads, validations, etc.
    */
    for (i = 0; i < TST_PROD_APP_NUMBER_OF_TABLES; i++)
    {
        CFE_TBL_Manage(TST_PROD_APP_Data.TblHandles[i]);
    }

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_PROD_APP NOOP commands                                                   */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_PROD_APP_Noop(const TST_PROD_APP_NoopCmd_t *Msg)
{
    TST_PROD_APP_Data.CmdCounter++;

    CFE_EVS_SendEvent(TST_PROD_APP_COMMANDNOP_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_PROD_APP: NOOP command %s",
                      TST_PROD_APP_VERSION);

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function resets all the global counter variables that are     */
/*         part of the task telemetry.                                        */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 TST_PROD_APP_ResetCounters(const TST_PROD_APP_ResetCountersCmd_t *Msg)
{
    TST_PROD_APP_Data.CmdCounter = 0;
    TST_PROD_APP_Data.ErrCounter = 0;

    CFE_EVS_SendEvent(TST_PROD_APP_COMMANDRST_INF_EID, CFE_EVS_EventType_INFORMATION, "TST_PROD_APP: RESET command");

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_PROD_APP START commands                                                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_PROD_APP_Start(const TST_PROD_APP_NoopCmd_t *Msg)
{
    TST_PROD_APP_Data.totMsgCount = 0;
    TST_PROD_APP_Data.CmdCounter++;
    
    if (TST_PROD_APP_Data.TestInProgress == 1)
    {
        CFE_EVS_SendEvent(TST_PROD_APP_START_ERR_EID, CFE_EVS_EventType_INFORMATION, 
                          "TST_PROD_APP: Test already in progress...");
        goto TST_PROD_APP_Start_Exit_Tag;
    }

    TST_PROD_APP_Data.TestInProgress = 1;
    TST_PROD_APP_Data.ProcessedTestMsgs = 0;

    CFE_EVS_SendEvent(TST_PROD_APP_START_INF_EID, CFE_EVS_EventType_INFORMATION, 
                      "TST_PROD_APP: Let's start testing!");

    // for (int i = 0; i < 100; ++i)
    // {
    //     if (TST_PROD_APP_Data.LogData[i].nextNode == NULL)
    //     {
    //         TST_PROD_APP_Data.LogData[i].nextNode = malloc(sizeof(TST_PROD_LoggingNode_t));
    //         (TST_PROD_LoggingNode_t *)(TST_PROD_APP_Data.LogData[i].nextNode)->timePtr = 
    //             malloc(sizeof(CFE_TIME_SysTime_t) * TST_PROD_APP_Data.NumMsgsPerTest);
    //     }
    // }
    CFE_PSP_GetTime(&TST_PROD_APP_Data.StartTime);

    /* First message will have 0 for start */
    TST_PROD_APP_Data.TestMsg.Payload = TST_START_MSG;
    for (int i = 0; i < TST_PROD_APP_Data.NumMsgsPerTest-1; ++i)
    {
        TST_PROD_APP_Data.totMsgCount++;
        OS_printf("[PROD] Sent (%d)\n", TST_PROD_APP_Data.totMsgCount);
        // OS_printf("Sending test message from Producer (#%d)\n", i+1);
        CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_PROD_APP_Data.TestMsg.CmdHeader));
        CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_PROD_APP_Data.TestMsg.CmdHeader), true);

        /* Second message -> Second to last message will have 1 for middle */
        TST_PROD_APP_Data.TestMsg.Payload = TST_MIDDLE_MSG;
    }
    
    /* Last message will have 2 for end */
    TST_PROD_APP_Data.totMsgCount++;
    TST_PROD_APP_Data.TestMsg.Payload = TST_END_MSG;
    OS_printf("[PROD] Sent (%d)\n", TST_PROD_APP_Data.totMsgCount);
    // OS_printf("Sending test message from Producer (#%d)\n", TST_PROD_APP_Data.NumMsgsPerTest);
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_PROD_APP_Data.TestMsg.CmdHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_PROD_APP_Data.TestMsg.CmdHeader), true);

    OS_printf("[PROD] Sending wakeup MID for Consumer (0x%07X)\n", TST_CSMR_WAKEUP_CMD_MID);
    CFE_SB_TimeStampMsg(CFE_MSG_PTR(TST_PROD_APP_Data.WakeupMsg.CmdHeader));
    CFE_SB_TransmitMsg(CFE_MSG_PTR(TST_PROD_APP_Data.WakeupMsg.CmdHeader), true);

TST_PROD_APP_Start_Exit_Tag:
    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* TST_PROD_APP COMPLETE commands                                                */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
int32 TST_PROD_APP_Complete(const TST_PROD_APP_NoopCmd_t *Msg)
{
    /* Producers and consumers running in parallel?? */
    TST_PROD_APP_Data.CmdCounter++;
    
    if (TST_PROD_APP_Data.TestInProgress == 0)
    {
        CFE_EVS_SendEvent(TST_PROD_APP_COMPLETE_ERR_EID, CFE_EVS_EventType_INFORMATION, 
                          "TST_PROD_APP: NO test in progress...");
        goto TST_PROD_APP_Start_Exit_Tag;
    }

    TST_PROD_APP_Data.TestInProgress = 0;

    CFE_PSP_GetTime(&TST_PROD_APP_Data.StopTime);
    OS_time_t TestDelta = OS_TimeSubtract(TST_PROD_APP_Data.StopTime, TST_PROD_APP_Data.StartTime);
    int64 TimeTaken = OS_TimeGetTotalMicroseconds(TestDelta);

    if (TST_PROD_APP_Data.ProcessedTestMsgs != TST_PROD_APP_Data.NumMsgsPerTest)
    {
        CFE_EVS_SendEvent(TST_PROD_APP_COMPLETE_INF_EID, CFE_EVS_EventType_INFORMATION, 
                          "TST_PROD_APP: Incomplete test finished! Received %d / %d messages in %d μs\n", 
                          TST_PROD_APP_Data.ProcessedTestMsgs, TST_PROD_APP_Data.NumMsgsPerTest, (int)TimeTaken);

    }
    else
    {
        CFE_EVS_SendEvent(TST_PROD_APP_COMPLETE_INF_EID, CFE_EVS_EventType_INFORMATION, 
                          "TST_PROD_APP: Test finished! Completed in %d μs\n", (int)TimeTaken);
    }

TST_PROD_APP_Start_Exit_Tag:
    return CFE_SUCCESS;
}

int32 TST_PROD_APP_TestMsg(const TST_PROD_APP_TestCmd_t *Msg)
{
    // OS_printf("Received test message (%d)\n", TST_PROD_APP_Data.ProcessedTestMsgs);
    if (TST_PROD_APP_Data.TestInProgress == 0)
    {
        OS_printf("NO TEST IN PROGRESS...\n");
        // CFE_EVS_SendEvent(TST_PROD_APP_TEST_ERR_EID, CFE_EVS_EventType_ERROR, 
        //                  "TST_PROD_APP: No test running.. that's weird");
        goto TST_PROD_APP_TestMsg;
    }

    TST_PROD_APP_Data.ProcessedTestMsgs++;
    OS_printf("[PROD] Received (%d)\n", TST_PROD_APP_Data.ProcessedTestMsgs);

TST_PROD_APP_TestMsg:
    return CFE_SUCCESS;
}

int32 TST_PROD_APP_ChangeNumMsgs(const TST_PROD_APP_NumMessagesCmd_t *Msg)
{

    if (TST_PROD_APP_Data.TestInProgress != 0)
    {
        CFE_EVS_SendEvent(TST_PROD_APP_UPDATE_NUM_MSGS_EID, CFE_EVS_EventType_INFORMATION, 
                         "TST_PROD_APP: Cannot update number of messages per test while a test is in progress");
        goto TST_PROD_APP_ChangeNumMsgs_Exit_Tag;
    }

    /* Producers and consumers running in parallel?? */
    TST_PROD_APP_Data.CmdCounter++;

    TST_PROD_APP_Data.NumMsgsPerTest = Msg->NumMessages;
    CFE_EVS_SendEvent(TST_PROD_APP_UPDATE_NUM_MSGS_EID, CFE_EVS_EventType_INFORMATION, 
                      "TST_PROD_APP: Updating number of messages per test to %d", (int)Msg->NumMessages);

TST_PROD_APP_ChangeNumMsgs_Exit_Tag:
    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/*  Purpose:                                                                  */
/*         This function Process Ground Station Command                       */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * *  * *  * * * * */
int32 TST_PROD_APP_Process(const TST_PROD_APP_ProcessCmd_t *Msg)
{
    int32               status;
    TST_PROD_APP_Table_t *TblPtr;
    const char *        TableName = "TST_PROD_APP.TstProdAppTable";

    /* Test Producer Use of Table */

    status = CFE_TBL_GetAddress((void *)&TblPtr, TST_PROD_APP_Data.TblHandles[0]);

    if (status < CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Fail to get table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    CFE_ES_WriteToSysLog("Test Producer App: Table Value 1: %d  Value 2: %d", TblPtr->Int1, TblPtr->Int2);

    TST_PROD_APP_GetCrc(TableName);

    status = CFE_TBL_ReleaseAddress(TST_PROD_APP_Data.TblHandles[0]);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Fail to release table address: 0x%08lx", (unsigned long)status);
        return status;
    }

    

    return CFE_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
/*                                                                            */
/* Verify command packet length                                               */
/*                                                                            */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
bool TST_PROD_APP_VerifyCmdLength(CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength)
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

        CFE_EVS_SendEvent(TST_PROD_APP_LEN_ERR_EID, CFE_EVS_EventType_ERROR,
                          "Invalid Msg length: ID = 0x%X,  CC = %u, Len = %u, Expected = %u",
                          (unsigned int)CFE_SB_MsgIdToValue(MsgId), (unsigned int)FcnCode, (unsigned int)ActualLength,
                          (unsigned int)ExpectedLength);

        result = false;

        TST_PROD_APP_Data.ErrCounter++;
    }

    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Verify contents of First Table buffer contents                  */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32 TST_PROD_APP_TblValidationFunc(void *TblData)
{
    int32               ReturnCode = CFE_SUCCESS;
    TST_PROD_APP_Table_t *TblDataPtr = (TST_PROD_APP_Table_t *)TblData;

    /*
    ** Test Producer Table Validation
    */
    if (TblDataPtr->Int1 > TST_PROD_APP_TBL_ELEMENT_1_MAX)
    {
        /* First element is out of range, return an appropriate error code */
        ReturnCode = TST_PROD_APP_TABLE_OUT_OF_RANGE_ERR_CODE;
    }

    return ReturnCode;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                 */
/* Output CRC                                                      */
/*                                                                 */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void TST_PROD_APP_GetCrc(const char *TableName)
{
    int32          status;
    uint32         Crc;
    CFE_TBL_Info_t TblInfoPtr;

    status = CFE_TBL_GetInfo(&TblInfoPtr, TableName);
    if (status != CFE_SUCCESS)
    {
        CFE_ES_WriteToSysLog("Test Producer App: Error Getting Table Info");
    }
    else
    {
        Crc = TblInfoPtr.Crc;
        CFE_ES_WriteToSysLog("Test Producer App: CRC: 0x%08lX\n\n", (unsigned long)Crc);
    }
}
