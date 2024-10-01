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
 * Main header file for the BLINKER application
 */

#ifndef BLINKER_APP_DISPATCH_H
#define BLINKER_APP_DISPATCH_H

/*
** Required header files.
*/
#include "cfe.h"
#include "blinker_app_msg.h"

void BLINKER_APP_TaskPipe(const CFE_SB_Buffer_t *SBBufPtr);
void BLINKER_APP_ProcessGroundCommand(const CFE_SB_Buffer_t *SBBufPtr);
void BLINKER_APP_Blink(void);
bool BLINKER_APP_VerifyCmdLength(const CFE_MSG_Message_t *MsgPtr, size_t ExpectedLength);

#endif /* BLINKER_APP_DISPATCH_H */
