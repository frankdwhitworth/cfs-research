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
 *  The Test Consumer App header file containing version information
 */

#ifndef TST_CSMR_APP_VERSION_H
#define TST_CSMR_APP_VERSION_H

/* Development Build Macro Definitions */

#define TST_CSMR_APP_BUILD_NUMBER 1 /*!< Development Build: Number of commits since baseline */
#define TST_CSMR_APP_BUILD_BASELINE \
    "v0.1.99" /*!< Development Build: git tag that is the base for the current development */

/*
 * Version Macros, see \ref cfsversions for definitions.
 */
#define TST_CSMR_APP_MAJOR_VERSION 0  /*!< @brief Major version number. */
#define TST_CSMR_APP_MINOR_VERSION 1  /*!< @brief Minor version number. */
#define TST_CSMR_APP_REVISION      99 /*!< @brief Revision version number. Value of 99 indicates a development version.*/

/*!
 * @brief Mission revision.
 *
 * Reserved for mission use to denote patches/customizations as needed.
 * Values 1-254 are reserved for mission use to denote patches/customizations as needed. NOTE: Reserving 0 and 0xFF for
 * cFS open-source development use (pending resolution of nasa/cFS#440)
 */
#define TST_CSMR_APP_MISSION_REV 0xFF

#define TST_CSMR_APP_STR_HELPER(x) #x /*!< @brief Helper function to concatenate strings from integer macros */
#define TST_CSMR_APP_STR(x) \
    TST_CSMR_APP_STR_HELPER(x) /*!< @brief Helper function to concatenate strings from integer macros */

/*! @brief Development Build Version Number.
 * @details Baseline git tag + Number of commits since baseline. @n
 * See @ref cfsversions for format differences between development and release versions.
 */
#define TST_CSMR_APP_VERSION TST_CSMR_APP_BUILD_BASELINE "+dev" TST_CSMR_APP_STR(TST_CSMR_APP_BUILD_NUMBER)

/*! @brief Development Build Version String.
 * @details Reports the current development build's baseline, number, and name. Also includes a note about the latest
 * official version. @n See @ref cfsversions for format differences between development and release versions.
 */
#define TST_CSMR_APP_VERSION_STRING                       \
    " Test Consumer App DEVELOPMENT BUILD " TST_CSMR_APP_VERSION \
    ", Last Official Release: v0.1.99" /* For full support please use this version */

#endif /* TST_CSMR_APP_VERSION_H */
