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

#include "stack/l2cdefs.h"
#include "stack/hcidefs.h"
#include "stack/btm_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/

/* Define the minimum offset that L2CAP needs in a buffer. This is made up of
** HCI type(1), len(2), handle(2), L2CAP len(2) and CID(2) => 9
*/
#define L2CAP_MIN_OFFSET 13 /* plus control(2), SDU length(2) */

/* result code for L2CA_DataWrite() */
#define L2CAP_DW_FAILED        false
#define L2CAP_DW_SUCCESS       true
#define L2CAP_DW_CONGESTED     2


/*****************************************************************************
**  Type Definitions
*****************************************************************************/

typedef struct {
#define L2CAP_FCR_BASIC_MODE    0x00
#define L2CAP_FCR_ERTM_MODE     0x03
#define L2CAP_FCR_STREAM_MODE   0x04

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


/*********************************
**  Callback Functions Prototypes
**********************************/

/* Connection indication callback prototype. Parameters are
**              BD Address of remote
**              Local CID assigned to the connection
**              PSM that the remote wants to connect to
**              Identifier that the remote sent
*/
typedef void (tL2CA_CONNECT_IND_CB) (BD_ADDR, uint16_t, uint16_t, uint8_t);


/* Connection confirmation callback prototype. Parameters are
**              Local CID
**              Result - 0 = connected, non-zero means failure reason
*/
typedef void (tL2CA_CONNECT_CFM_CB) (uint16_t, uint16_t);


/* Connection pending callback prototype. Parameters are
**              Local CID
*/
typedef void (tL2CA_CONNECT_PND_CB) (uint16_t);


/* Configuration indication callback prototype. Parameters are
**              Local CID assigned to the connection
**              Pointer to configuration info
*/
typedef void (tL2CA_CONFIG_IND_CB) (uint16_t, tL2CAP_CFG_INFO *);


/* Configuration confirm callback prototype. Parameters are
**              Local CID assigned to the connection
**              Pointer to configuration info
*/
typedef void (tL2CA_CONFIG_CFM_CB) (uint16_t, tL2CAP_CFG_INFO *);


/* Disconnect indication callback prototype. Parameters are
**              Local CID
**              bool whether upper layer should ack this
*/
typedef void (tL2CA_DISCONNECT_IND_CB) (uint16_t, bool);


/* Disconnect confirm callback prototype. Parameters are
**              Local CID
**              Result
*/
typedef void (tL2CA_DISCONNECT_CFM_CB) (uint16_t, uint16_t);


/* QOS Violation indication callback prototype. Parameters are
**              BD Address of violating device
*/
typedef void (tL2CA_QOS_VIOLATION_IND_CB) (BD_ADDR);


/* Data received indication callback prototype. Parameters are
**              Local CID
**              Address of buffer
*/
typedef void (tL2CA_DATA_IND_CB) (uint16_t, BT_HDR *);


/* Congestion status callback protype. This callback is optional. If
** an application tries to send data when the transmit queue is full,
** the data will anyways be dropped. The parameter is:
**              Local CID
**              TRUE if congested, FALSE if uncongested
*/
typedef void (tL2CA_CONGESTION_STATUS_CB) (uint16_t, bool);


/* Transmit complete callback protype. This callback is optional. If
** set, L2CAP will call it when packets are sent or flushed. If the
** count is 0xFFFF, it means all packets are sent for that CID (eRTM
** mode only). The parameters are:
**              Local CID
**              Number of SDUs sent or dropped
*/
typedef void (tL2CA_TX_COMPLETE_CB) (uint16_t, uint16_t);


/* Define the structure that applications use to register with
** L2CAP. This structure includes callback functions. All functions
** MUST be provided, with the exception of the "connect pending"
** callback and "congestion status" callback.
*/
typedef struct {
    tL2CA_CONNECT_IND_CB        *pL2CA_ConnectInd_Cb;
    tL2CA_CONNECT_CFM_CB        *pL2CA_ConnectCfm_Cb;
    tL2CA_CONNECT_PND_CB        *pL2CA_ConnectPnd_Cb;
    tL2CA_CONFIG_IND_CB         *pL2CA_ConfigInd_Cb;
    tL2CA_CONFIG_CFM_CB         *pL2CA_ConfigCfm_Cb;
    tL2CA_DISCONNECT_IND_CB     *pL2CA_DisconnectInd_Cb;
    tL2CA_DISCONNECT_CFM_CB     *pL2CA_DisconnectCfm_Cb;
    tL2CA_QOS_VIOLATION_IND_CB  *pL2CA_QoSViolationInd_Cb;
    tL2CA_DATA_IND_CB           *pL2CA_DataInd_Cb;
    tL2CA_CONGESTION_STATUS_CB  *pL2CA_CongestionStatus_Cb;
    tL2CA_TX_COMPLETE_CB        *pL2CA_TxComplete_Cb;

} tL2CAP_APPL_INFO;

/* Define the structure that applications use to create or accept
** connections with enhanced retransmission mode.
*/
typedef struct {
    uint8_t       preferred_mode;
    uint8_t       allowed_modes;
    uint16_t      user_rx_buf_size;
    uint16_t      user_tx_buf_size;
    uint16_t      fcr_rx_buf_size;
    uint16_t      fcr_tx_buf_size;

} tL2CAP_ERTM_INFO;

#define L2CA_REGISTER(a,b,c)        L2CA_Register(a,(tL2CAP_APPL_INFO *)b)
#define L2CA_DEREGISTER(a)          L2CA_Deregister(a)
#define L2CA_CONNECT_REQ(a,b,c,d)   L2CA_ErtmConnectReq(a,b,c)
#define L2CA_CONNECT_RSP(a,b,c,d,e,f,g) L2CA_ErtmConnectRsp(a,b,c,d,e,f)
#define L2CA_CONFIG_REQ(a,b)        L2CA_ConfigReq(a,b)
#define L2CA_CONFIG_RSP(a,b)        L2CA_ConfigRsp(a,b)
#define L2CA_DISCONNECT_REQ(a)      L2CA_DisconnectReq(a)
#define L2CA_DISCONNECT_RSP(a)      L2CA_DisconnectRsp(a)
#define L2CA_DATA_WRITE(a, b)       L2CA_DataWrite(a, b)

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
**
** Function         L2CA_Register
**
** Description      Other layers call this function to register for L2CAP
**                  services.
**
** Returns          PSM to use or zero if error. Typically, the PSM returned
**                  is the same as was passed in, but for an outgoing-only
**                  connection to a dynamic PSM, a "virtual" PSM is returned
**                  and should be used in the calls to L2CA_ConnectReq() and
**                  BTM_SetSecurityLevel().
**
*******************************************************************************/
extern uint16_t L2CA_Register (uint16_t psm, tL2CAP_APPL_INFO *p_cb_info);

/*******************************************************************************
**
** Function         L2CA_Deregister
**
** Description      Other layers call this function to deregister for L2CAP
**                  services.
**
** Returns          void
**
*******************************************************************************/
extern void L2CA_Deregister (uint16_t psm);

/*******************************************************************************
**
** Function         L2CA_ErtmConnectReq
**
** Description      Higher layers call this function to create an L2CAP connection
**                  that needs to use Enhanced Retransmission Mode.
**                  Note that the connection is not established at this time, but
**                  connection establishment gets started. The callback function
**                  will be invoked when connection establishes or fails.
**
** Returns          the CID of the connection, or 0 if it failed to start
**
*******************************************************************************/
extern uint16_t L2CA_ErtmConnectReq (uint16_t psm, BD_ADDR p_bd_addr,
                                   tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
** Function         L2CA_ErtmConnectRsp
**
** Description      Higher layers call this function to accept an incoming
**                  L2CAP connection, for which they had gotten an connect
**                  indication callback, and for which the higher layer wants
**                  to use Enhanced Retransmission Mode.
**
** Returns          TRUE for success, FALSE for failure
**
*******************************************************************************/
extern bool  L2CA_ErtmConnectRsp (BD_ADDR p_bd_addr, uint8_t id, uint16_t lcid,
                                     uint16_t result, uint16_t status,
                                     tL2CAP_ERTM_INFO *p_ertm_info);

/*******************************************************************************
**
** Function         L2CA_ConfigReq
**
** Description      Higher layers call this function to send configuration.
**
** Returns          TRUE if configuration sent, else FALSE
**
*******************************************************************************/
extern bool L2CA_ConfigReq (uint16_t cid, tL2CAP_CFG_INFO *p_cfg);

/*******************************************************************************
**
** Function         L2CA_ConfigRsp
**
** Description      Higher layers call this function to send a configuration
**                  response.
**
** Returns          TRUE if configuration response sent, else FALSE
**
*******************************************************************************/
extern bool L2CA_ConfigRsp (uint16_t cid, tL2CAP_CFG_INFO *p_cfg);

/*******************************************************************************
**
** Function         L2CA_DisconnectReq
**
** Description      Higher layers call this function to disconnect a channel.
**
** Returns          TRUE if disconnect sent, else FALSE
**
*******************************************************************************/
extern bool L2CA_DisconnectReq (uint16_t cid);

/*******************************************************************************
**
** Function         L2CA_DisconnectRsp
**
** Description      Higher layers call this function to acknowledge the
**                  disconnection of a channel.
**
** Returns          void
**
*******************************************************************************/
extern bool L2CA_DisconnectRsp (uint16_t cid);

/*******************************************************************************
**
** Function         L2CA_DataWrite
**
** Description      Higher layers call this function to write data.
**
** Returns          L2CAP_DW_SUCCESS, if data accepted, else FALSE
**                  L2CAP_DW_CONGESTED, if data accepted and the channel is congested
**                  L2CAP_DW_FAILED, if error
**
*******************************************************************************/
extern uint8_t L2CA_DataWrite (uint16_t cid, BT_HDR *p_data);


#ifdef __cplusplus
}
#endif

#endif  /* L2C_API_H */