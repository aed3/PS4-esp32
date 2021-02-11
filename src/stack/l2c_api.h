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
#define L2CAP_MIN_OFFSET 13 /* plus control(2), SDU length(2) */

/*****************************************************************************
**  Type Definitions
*****************************************************************************/

typedef struct {
  uint8_t mode;

  uint8_t tx_win_sz;
  uint8_t max_transmit;
  uint16_t rtrans_tout;
  uint16_t mon_tout;
  uint16_t mps;
} tL2CAP_FCR_OPTS;

/* Define a structure to hold the configuration parameters. Since the
** parameters are optional, for each parameter there is a boolean to
** use to signify its presence or absence.
*/
typedef struct {
  uint16_t result; /* Only used in confirm messages */
  bool mtu_present;
  uint16_t mtu;
  bool qos_present;
  FLOW_SPEC qos;
  bool flush_to_present;
  uint16_t flush_to;
  bool fcr_present;
  tL2CAP_FCR_OPTS fcr;
  bool fcs_present; /* Optionally bypasses FCS checks */
  uint8_t fcs;      /* '0' if desire is to bypass FCS, otherwise '1' */
  bool ext_flow_spec_present;
  tHCI_EXT_FLOW_SPEC ext_flow_spec;
  uint16_t flags; /* bit 0: 0-no continuation, 1-continuation */
} tL2CAP_CFG_INFO;

/* Define the structure that applications use to create or accept
** connections with enhanced retransmission mode.
*/
typedef struct {
  uint8_t preferred_mode;
  uint8_t allowed_modes;
  uint16_t user_rx_buf_size;
  uint16_t user_tx_buf_size;
  uint16_t fcr_rx_buf_size;
  uint16_t fcr_tx_buf_size;
} tL2CAP_ERTM_INFO;

#endif /* L2C_API_H */
