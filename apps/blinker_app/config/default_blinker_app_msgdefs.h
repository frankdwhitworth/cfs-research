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
 *   message constant definitions.
 *
 *  For BLINKER_APP this is only the function/command code definitions
 */
#ifndef BLINKER_APP_MSGDEFS_H
#define BLINKER_APP_MSGDEFS_H

#include "common_types.h"
#include "blinker_app_fcncodes.h"

typedef struct BLINKER_APP_DisplayParam_Payload
{
    uint32 ValU32;                            /**< 32 bit unsigned integer value */
    int16  ValI16;                            /**< 16 bit signed integer value */
    char   ValStr[BLINKER_APP_STRING_VAL_LEN]; /**< An example string */
} BLINKER_APP_DisplayParam_Payload_t;


#endif
