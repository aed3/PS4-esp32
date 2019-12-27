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
 *  this file contains the L2CAP API definitions
 *
 ******************************************************************************/
#ifndef L2C_API_H
#define L2C_API_H

#include <stdbool.h>

#include "stack/hcidefs.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Define the minimum offset that L2CAP needs in a buffer. This is made up of
** HCI type(1), len(2), handle(2), L2CAP len(2) and CID(2) => 9
*/
#define L2CAP_MIN_OFFSET    13     /* plus control(2), SDU length(2) */


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

typedef struct {
#define L2CAP_FCR_BASIC_MODE    0x00
#define L2CAP_FCR_ERTM_MODE     0x03
#define L2CAP_FCR_STREAM_MODE   0x04

    UINT8  mode;

    UINT8  tx_win_sz;
    UINT8  max_transmit;
    UINT16 rtrans_tout;
    UINT16 mon_tout;
    UINT16 mps;
} tL2CAP_FCR_OPTS;

/* Define a structure to hold the configuration parameters. Since the
** parameters are optional, for each parameter there is a boolean to
** use to signify its presence or absence.
*/
typedef struct {
    UINT16      result;                 /* Only used in confirm messages */
    BOOLEAN     mtu_present;
    UINT16      mtu;
    BOOLEAN     qos_present;
    FLOW_SPEC   qos;
    BOOLEAN     flush_to_present;
    UINT16      flush_to;
    BOOLEAN     fcr_present;
    tL2CAP_FCR_OPTS fcr;
    BOOLEAN     fcs_present;            /* Optionally bypasses FCS checks */
    UINT8       fcs;                    /* '0' if desire is to bypass FCS, otherwise '1' */
    BOOLEAN               ext_flow_spec_present;
    tHCI_EXT_FLOW_SPEC    ext_flow_spec;
    UINT16      flags;                  /* bit 0: 0-no continuation, 1-continuation */
} tL2CAP_CFG_INFO;

/* Define the structure that applications use to create or accept
** connections with enhanced retransmission mode.
*/
typedef struct {
    UINT8       preferred_mode;
    UINT8       allowed_modes;
    UINT16      user_rx_buf_size;
    UINT16      user_tx_buf_size;
    UINT16      fcr_rx_buf_size;
    UINT16      fcr_tx_buf_size;

} tL2CAP_ERTM_INFO;

#endif  /* L2C_API_H */
