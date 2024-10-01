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
 * Define Blinker App Events IDs
 */

#ifndef BLINKER_APP_EVENTS_H
#define BLINKER_APP_EVENTS_H

#define BLINKER_APP_RESERVED_EID      0
#define BLINKER_APP_INIT_INF_EID      1
#define BLINKER_APP_CC_ERR_EID        2
#define BLINKER_APP_NOOP_INF_EID      3
#define BLINKER_APP_RESET_INF_EID     4
#define BLINKER_APP_MID_ERR_EID       5
#define BLINKER_APP_CMD_LEN_ERR_EID   6
#define BLINKER_APP_PIPE_ERR_EID      7
#define BLINKER_APP_VALUE_INF_EID     8
#define BLINKER_APP_CR_PIPE_ERR_EID   9
#define BLINKER_APP_SUB_HK_ERR_EID    10
#define BLINKER_APP_SUB_CMD_ERR_EID   11
#define BLINKER_APP_TABLE_REG_ERR_EID 12

#define reserved                                     13
#define BLINKER_APP_RESET_ALL_BLINKERS_INF_EID       14
#define BLINKER_APP_RESET_SPEC_BLINKER_INF_EID       15
#define BLINKER_APP_REPORT_SPEC_BLINKER_INF_EID      16
#define BLINKER_APP_ENABLE_SPEC_BLINKER_INF_EID      17
#define BLINKER_APP_DISABLE_SPEC_BLINKER_INF_EID     18
#define BLINKER_APP_ENABLE_ALL_BLINKERS_INF_EID      19
#define BLINKER_APP_DISABLE_ALL_BLINKERS_INF_EID     20
#define BLINKER_APP_SWAP_TWO_BLINKERS_INF_EID        21

#endif /* BLINKER_APP_EVENTS_H */
