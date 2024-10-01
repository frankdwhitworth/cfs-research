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
 * This is a compatibility header for the "blinker_app_msg.h" file that has
 * traditionally provided the message definitions for cFS apps.
 *
 * @note This file may be overridden/superceded by mission-provided defintions
 * either by overriding this header or by generating definitions from a command/data
 * dictionary tool.
 */
#ifndef BLINKER_APP_MSG_H
#define BLINKER_APP_MSG_H

#include "blinker_app_mission_cfg.h"
#include "blinker_app_platform_cfg.h"
#include "blinker_app_msgdefs.h"
#include "blinker_app_msgstruct.h"


typedef BLINKER_APP_NoArgs_t         BLINKER_APP_NoopCmd_t;
typedef BLINKER_APP_NoArgs_t         BLINKER_APP_ResetCountersCmd_t;
typedef BLINKER_APP_NoArgs_t         BLINKER_APP_ProcessCmd_t;
typedef BLINKER_APP_NoArgs_t         BLINKER_APP_ResetAllBlinkersCmd_t;
typedef BLINKER_APP_NoArgs_t         BLINKER_APP_EnableAllBlinkersCmd_t;
typedef BLINKER_APP_NoArgs_t         BLINKER_APP_DisableAllBlinkersCmd_t;
typedef BLINKER_APP_SpecBlinker_t    BLINKER_APP_ResetSpecBlinkersCmd_t;
typedef BLINKER_APP_SpecBlinker_t    BLINKER_APP_ReportSpecBlinkersCmd_t;
typedef BLINKER_APP_SpecBlinker_t    BLINKER_APP_EnableSpecBlinkersCmd_t;
typedef BLINKER_APP_SpecBlinker_t    BLINKER_APP_DisableSpecBlinkersCmd_t;
typedef BLINKER_APP_SwapBlinkers_t   BLINKER_APP_SwapBlinkerCountsCmd_t;


#endif
