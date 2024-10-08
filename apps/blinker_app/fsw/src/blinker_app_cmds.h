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
 *   This file contains the prototypes for the Blinker App Ground Command-handling functions
 */

#ifndef BLINKER_APP_CMDS_H
#define BLINKER_APP_CMDS_H

/*
** Required header files.
*/
#include "cfe_error.h"
#include "blinker_app_msg.h"

CFE_Status_t BLINKER_APP_SendHkCmd(const BLINKER_APP_SendHkCmd_t *Msg);
CFE_Status_t BLINKER_APP_ResetCountersCmd(const BLINKER_APP_ResetCountersCmd_t *Msg);
CFE_Status_t BLINKER_APP_ProcessCmd(const BLINKER_APP_ProcessCmd_t *Msg);
CFE_Status_t BLINKER_APP_NoopCmd(const BLINKER_APP_NoopCmd_t *Msg);
CFE_Status_t BLINKER_APP_DisplayParamCmd(const BLINKER_APP_DisplayParamCmd_t *Msg);
CFE_Status_t BLINKER_APP_ResetAllBlinkersCmd(const BLINKER_APP_ResetAllBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_EnableAllBlinkersCmd(const BLINKER_APP_EnableAllBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_DisableAllBlinkersCmd(const BLINKER_APP_DisableAllBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_ResetSpecBlinkersCmd(const BLINKER_APP_ResetSpecBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_ReportSpecBlinkersCmd(const BLINKER_APP_ReportSpecBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_EnableSpecBlinkersCmd(const BLINKER_APP_EnableSpecBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_DisableSpecBlinkersCmd(const BLINKER_APP_DisableSpecBlinkersCmd_t *Msg);
CFE_Status_t BLINKER_APP_SwapBlinkerCountsCmd(const BLINKER_APP_SwapBlinkerCountsCmd_t *Msg);

#endif /* BLINKER_APP_CMDS_H */
