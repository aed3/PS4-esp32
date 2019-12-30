
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "include/ps4.h"
#include "ps4_int.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "stack/gap_api.h"
#include "stack/bt_types.h"
#include "osi/allocator.h"


#define  PS4_TAG "PS4_GAP"


#define PS4_GAP_ID_HIDC 0x40
#define PS4_GAP_ID_HIDI 0x41


/********************************************************************************/
/*              L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/********************************************************************************/

static uint16_t ps4_gap_init_service( char *name, uint16_t psm, uint8_t security_id);
static void ps4_gap_event_handle(UINT16 gap_handle, UINT16 event);
static void ps4_gap_update_connected ();


/********************************************************************************/
/*                         L O C A L    V A R I A B L E S                       */
/********************************************************************************/
static tL2CAP_ERTM_INFO ps4_ertm_info = {0};
static tL2CAP_CFG_INFO ps4_cfg_info = {0};

uint16_t gap_handle_hidc = GAP_INVALID_HANDLE;
uint16_t gap_handle_hidi = GAP_INVALID_HANDLE;

static bool is_connected = false;


/********************************************************************************/
/*                      P U B L I C    F U N C T I O N S                        */
/********************************************************************************/
/*******************************************************************************
**
** Function         ps4_gap_is_connected
**
** Description      This returns whether a PS4 controller is connected, based
**                  on whether a successful handshake has taken place.
**
** Returns          void
**
*******************************************************************************/
bool ps4_gap_is_connected()
{
    return is_connected;
}

/*******************************************************************************
**
** Function         ps4_gap_init_services
**
** Description      This function initialises the required GAP services.
**
** Returns          void
**
*******************************************************************************/
void ps4_gap_init_services()
{
    gap_handle_hidc = ps4_gap_init_service( "PS4-HIDC", BT_PSM_HIDC, BTM_SEC_SERVICE_FIRST_EMPTY   );
    gap_handle_hidi = ps4_gap_init_service( "PS4-HIDI", BT_PSM_HIDI, BTM_SEC_SERVICE_FIRST_EMPTY+1 );
}


/*******************************************************************************
**
** Function         ps4_gap_send_hid
**
** Description      This function sends the HID command using the GAP service.
**
** Returns          void
**
*******************************************************************************/
void ps4_gap_send_hid( hid_cmd_t *hid_cmd, uint8_t len )
{
    uint8_t result;
    BT_HDR     *p_buf;

    p_buf = (BT_HDR *)osi_malloc(BT_DEFAULT_BUFFER_SIZE);

    if( !p_buf ){
        ESP_LOGE(PS4_TAG, "[%s] allocating buffer for sending the command failed", __func__);
    }

    p_buf->len = len + ( sizeof(*hid_cmd) - sizeof(hid_cmd->data) );
    p_buf->offset = L2CAP_MIN_OFFSET;

    memcpy ((uint8_t *)(p_buf + 1) + p_buf->offset, (uint8_t*)hid_cmd, p_buf->len);

    result = GAP_ConnBTWrite(gap_handle_hidc, p_buf);

    if (result == BT_PASS) {
        ESP_LOGI(PS4_TAG, "[%s] sending command: success\n", __func__);
        //printf("[%s] sending command: success", __func__);
    }
    else {
        ESP_LOGE(PS4_TAG, "[%s] sending command: failed\n", __func__);
        //printf("[%s] sending command: success", __func__);
    }
}


/********************************************************************************/
/*                      L O C A L    F U N C T I O N S                          */
/********************************************************************************/

/*******************************************************************************
**
** Function         ps4_gap_init_service
**
** Description      This registers the specified bluetooth service in order
**                  to listen for incoming connections.
**
** Returns          The created GAP handle
**
*******************************************************************************/
static uint16_t ps4_gap_init_service( char *name, uint16_t psm, uint8_t security_id)
{
    uint16_t handle = GAP_ConnOpen (name, security_id, /*is_server=*/true, /*p_rem_bda=*/NULL,
                     psm, &ps4_cfg_info, &ps4_ertm_info, /*security=*/0, /*chan_mode_mask=*/0,
                     ps4_gap_event_handle);

    if (handle == GAP_INVALID_HANDLE){
        ESP_LOGE(PS4_TAG, "%s Registering GAP service %s failed", __func__, name);
    }else{
        ESP_LOGI(PS4_TAG, "[%s] GAP Service %s Initialized: %d", __func__, name, handle);
    }

    return handle;
}


/*******************************************************************************
**
** Function         ps4_gap_event_handle
**
** Description      Callback for GAP events, currently handling the connection
**                  opened, connection closed, and data available events.
**
** Returns          void
**
*******************************************************************************/
static void ps4_gap_event_handle(UINT16 gap_handle, UINT16 event)
{
    switch(event){
        case GAP_EVT_CONN_OPENED:
        case GAP_EVT_CONN_CLOSED:{
            uint8_t was_connected = is_connected;
            ps4_gap_update_connected();

            if(was_connected != is_connected){
                ps4_connect_event(is_connected);
            }

            break;
        }

        case GAP_EVT_CONN_DATA_AVAIL: {
            BT_HDR *p_buf;

            GAP_ConnBTRead(gap_handle, &p_buf);

            if ( p_buf->len > 2 )
            {
                ps4_parse_packet( p_buf->data );
            }

            osi_free( p_buf );

            break;
        }

        default:
            break;
    }
}


/*******************************************************************************
**
** Function         ps4_gap_update_connected
**
** Description      This updates the is_connected flag by checking if the
**                  GAP handles return valid L2CAP channel IDs.
**
** Returns          void
**
*******************************************************************************/
static void ps4_gap_update_connected()
{
    is_connected = GAP_ConnGetL2CAPCid(gap_handle_hidc) != 0
                && GAP_ConnGetL2CAPCid(gap_handle_hidi) != 0;
}
