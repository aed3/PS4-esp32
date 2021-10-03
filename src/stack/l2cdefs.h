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

#ifndef L2CDEFS_H
#define L2CDEFS_H

/* Define the L2CAP connection result codes
*/
#define L2CAP_CONN_OK                0
#define L2CAP_CONN_PENDING           1
#define L2CAP_CONN_NO_PSM            2
#define L2CAP_CONN_SECURITY_BLOCK    3
#define L2CAP_CONN_NO_RESOURCES      4
#define L2CAP_CONN_BAD_CTLR_ID       5              /* AMP related */
#define L2CAP_CONN_TIMEOUT           0xEEEE
#define L2CAP_CONN_AMP_FAILED        254
#define L2CAP_CONN_NO_LINK           255        /* Add a couple of our own for internal use */
#define L2CAP_CONN_CANCEL            256        /* L2CAP connection cancelled */


/* Define the L2CAP configuration result codes
*/
#define L2CAP_CFG_OK                    0
#define L2CAP_CFG_UNACCEPTABLE_PARAMS   1
#define L2CAP_CFG_FAILED_NO_REASON      2
#define L2CAP_CFG_UNKNOWN_OPTIONS       3
#define L2CAP_CFG_PENDING               4
#define L2CAP_CFG_FLOW_SPEC_REJECTED    5

#endif