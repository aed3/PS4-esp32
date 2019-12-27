/******************************************************************************
 *
 *  Copyright (C) 2009-2013 Broadcom Corporation
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

#ifndef GAP_API_H
#define GAP_API_H

#include "stack/profiles_api.h"
#include "stack/btm_api.h"
#include "stack/l2c_api.h"

/*****************************************************************************
**  Constants
*****************************************************************************/
/*** GAP Error and Status Codes ***/
#define GAP_EVT_CONN_OPENED         0x0100
#define GAP_EVT_CONN_CLOSED         0x0101
#define GAP_EVT_CONN_DATA_AVAIL     0x0102
#define GAP_EVT_CONN_CONGESTED      0x0103
#define GAP_EVT_CONN_UNCONGESTED    0x01043

/*** used in connection variables and functions ***/
#define GAP_INVALID_HANDLE      0xFFFF

/*****************************************************************************
**  Type Definitions
*****************************************************************************/
/*
** Callback function for connection services
*/
typedef void (tGAP_CONN_CALLBACK) (UINT16 gap_handle, UINT16 event);


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/

/*** Functions for L2CAP connection interface ***/

/*******************************************************************************
**
** Function         GAP_ConnOpen
**
** Description      This function is called to open a generic L2CAP connection.
**
** Returns          handle of the connection if successful, else GAP_INVALID_HANDLE
**
*******************************************************************************/
extern UINT16 GAP_ConnOpen (const char *p_serv_name, UINT8 service_id, BOOLEAN is_server,
                            BD_ADDR p_rem_bda, UINT16 psm, tL2CAP_CFG_INFO *p_cfg,
                            tL2CAP_ERTM_INFO *ertm_info,
                            UINT16 security, UINT8 chan_mode_mask, tGAP_CONN_CALLBACK *p_cb);

/*******************************************************************************
**
** Function         GAP_ConnClose
**
** Description      This function is called to close a connection.
**
** Returns          BT_PASS             - closed OK
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**
*******************************************************************************/
extern UINT16 GAP_ConnClose (UINT16 gap_handle);

/*******************************************************************************
**
** Function         GAP_ConnBTRead
**
** Description      GKI buffer aware applications will call this function after
**                  receiving an GAP_EVT_RXDATA event to process the incoming
**                  data buffer.
**
** Returns          BT_PASS             - data read
**                  GAP_ERR_BAD_HANDLE  - invalid handle
**                  GAP_NO_DATA_AVAIL   - no data available
**
*******************************************************************************/
extern UINT16 GAP_ConnBTRead (UINT16 gap_handle, BT_HDR **pp_buf);

/*******************************************************************************
**
** Function         GAP_ConnBTWrite
**
** Description      GKI buffer aware applications can call this function to write data
**                  by passing a pointer to the GKI buffer of data.
**
** Returns          BT_PASS                 - data read
**                  GAP_ERR_BAD_HANDLE      - invalid handle
**                  GAP_ERR_BAD_STATE       - connection not established
**                  GAP_INVALID_BUF_OFFSET  - buffer offset is invalid
*******************************************************************************/
extern UINT16 GAP_ConnBTWrite (UINT16 gap_handle, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         GAP_ConnGetL2CAPCid
**
** Description      Returns the L2CAP channel id
**
** Parameters:      handle      - Handle of the connection
**
** Returns          UINT16      - The L2CAP channel id
**                  0, if error
**
*******************************************************************************/
extern UINT16 GAP_ConnGetL2CAPCid (UINT16 gap_handle);

#endif  /* GAP_API_H */
