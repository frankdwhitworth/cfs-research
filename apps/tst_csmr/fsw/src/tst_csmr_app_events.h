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
 * Define Test Consumer App Events IDs
 */

#ifndef TST_CSMR_APP_EVENTS_H
#define TST_CSMR_APP_EVENTS_H

#define TST_CSMR_APP_RESERVED_EID          0
#define TST_CSMR_APP_STARTUP_INF_EID       1
#define TST_CSMR_APP_COMMAND_ERR_EID       2
#define TST_CSMR_APP_COMMANDNOP_INF_EID    3
#define TST_CSMR_APP_COMMANDRST_INF_EID    4
#define TST_CSMR_APP_INVALID_MSGID_ERR_EID 5
#define TST_CSMR_APP_LEN_ERR_EID           6
#define TST_CSMR_APP_PIPE_ERR_EID          7

#define TST_CSMR_APP_RCVD_TST_MID_INF_EID  8
#define TST_CSMR_APP_SENT_COMPLETE_INF_EID 9

#define TST_CSMR_APP_BAD_TEST_MSG_ERR_EID 10

#endif /* TST_CSMR_APP_EVENTS_H */
