/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/******************************************************************************
 *
 *  This file contains the Bluetooth Manager (BTM) API function external
 *  definitions.
 *
 ******************************************************************************/
#ifndef BTM_API_H
#define BTM_API_H

#include "common/bt_target.h"

/* Security service definitions (BTM_SetSecurityLevel)
** Used for Authorization APIs
*/

/* Update these as services are added */
#define BTM_SEC_SERVICE_FIRST_EMPTY     51

#ifndef BTM_SEC_MAX_SERVICES
#define BTM_SEC_MAX_SERVICES            65
#endif

#endif /* BTM_API_H */
