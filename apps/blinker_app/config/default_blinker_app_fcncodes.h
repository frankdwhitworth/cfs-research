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
 *   Specification for the BLINKER_APP command function codes
 *
 * @note
 *   This file should be strictly limited to the command/function code (CC)
 *   macro definitions.  Other definitions such as enums, typedefs, or other
 *   macros should be placed in the msgdefs.h or msg.h files.
 */
#ifndef BLINKER_APP_FCNCODES_H
#define BLINKER_APP_FCNCODES_H

/************************************************************************
 * Macro Definitions
 ************************************************************************/

/*
** Blinker App command codes
*/
#define BLINKER_APP_NOOP_CC                     0
#define BLINKER_APP_RESET_COUNTERS_CC           1
#define BLINKER_APP_PROCESS_CC                  2
#define BLINKER_APP_DISPLAY_PARAM_CC            3
#define BLINKER_APP_RESET_ALL_BLINKERS_CC       4
#define BLINKER_APP_RESET_SPEC_BLINKER_CC       5
#define BLINKER_APP_REPORT_SPEC_BLINKER_CC      6
#define BLINKER_APP_ENABLE_SPEC_BLINKER_CC      7
#define BLINKER_APP_DISABLE_SPEC_BLINKER_CC     8
#define BLINKER_APP_ENABLE_ALL_BLINKERS_CC      9
#define BLINKER_APP_DISABLE_ALL_BLINKERS_CC     10
#define BLINKER_APP_SWAP_TWO_BLINKERS_CC        11

#endif
