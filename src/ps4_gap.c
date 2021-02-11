
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_log.h"
#include "osi/allocator.h"
#include "ps4.h"
#include "ps4_int.h"
#include "stack/bt_types.h"
#include "stack/gap_api.h"

#define PS4_TAG "PS4_GAP"
#define BTM_SEC_SERVICE_FIRST_EMPTY 51
#define BT_DEFAULT_BUFFER_SIZE (4096 + 16)

/********************************************************************************/
/*              L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/********************************************************************************/

static uint16_t gapInitService(char* name, uint16_t psm, uint8_t securityID);
static void gapEventHandle(uint16_t gapHandle, uint16_t event);
static void gapUpdateConnected();

/********************************************************************************/
/*                         L O C A L    V A R I A B L E S                       */
/********************************************************************************/
static tL2CAP_ERTM_INFO ertmInfo = {0};
static tL2CAP_CFG_INFO cfgInfo = {0};

uint16_t gapHandleHIDControl = GAP_INVALID_HANDLE;
uint16_t gapHandleHIDInterrupt = GAP_INVALID_HANDLE;

static bool isConnected = false;

/********************************************************************************/
/*                      P U B L I C    F U N C T I O N S                        */
/********************************************************************************/
/*******************************************************************************
**
** Function         gapIsConnected
**
** Description      This returns whether a PS4 controller is connected, based
**                  on whether a successful handshake has taken place.
**
** Returns          void
**
*******************************************************************************/
bool gapIsConnected() { return isConnected; }

/*******************************************************************************
**
** Function         gapInitServices
**
** Description      This function initialises the required GAP services.
**
** Returns          void
**
*******************************************************************************/
void gapInitServices() {
  gapHandleHIDControl = gapInitService("PS4-HIDC", BT_PSM_HID_CONTROL, BTM_SEC_SERVICE_FIRST_EMPTY);
  gapHandleHIDInterrupt = gapInitService("PS4-HIDI", BT_PSM_HID_INTERRUPT, BTM_SEC_SERVICE_FIRST_EMPTY + 1);
}

/*******************************************************************************
**
** Function         gapSendHid
**
** Description      This function sends the HID command using the GAP service.
**
** Returns          void
**
*******************************************************************************/
void gapSendHid(hid_cmd_t* hidCommand, uint8_t length) {
  uint8_t result;
  BT_HDR* buffer;

  buffer = (BT_HDR*)osi_malloc(BT_DEFAULT_BUFFER_SIZE);

  if (!buffer) {
    ESP_LOGE(PS4_TAG, "[%s] allocating buffer for sending the command failed", __func__);
  }

  buffer->length = length + (sizeof(*hidCommand) - sizeof(hidCommand->data));
  buffer->offset = L2CAP_MIN_OFFSET;

  memcpy((uint8_t*)(buffer + 1) + buffer->offset, (uint8_t*)hidCommand, buffer->length);

  result = GAP_ConnBTWrite(gapHandleHIDControl, buffer);

  if (result == BT_PASS) {
    ESP_LOGI(PS4_TAG, "[%s] sending command: success\n", __func__);
  }
  else {
    ESP_LOGE(PS4_TAG, "[%s] sending command: failed\n", __func__);
  }
}

/********************************************************************************/
/*                      L O C A L    F U N C T I O N S                          */
/********************************************************************************/

/*******************************************************************************
**
** Function         gapInitService
**
** Description      This registers the specified bluetooth service in order
**                  to listen for incoming connections.
**
** Returns          The created GAP handle
**
*******************************************************************************/
static uint16_t gapInitService(char* name, uint16_t psm, uint8_t securityID) {
  uint16_t handle = GAP_ConnOpen(name, securityID, /*is_server=*/true, /*p_rem_bda=*/NULL, psm, &cfgInfo,
    &ertmInfo, /*security=*/0, /*chan_mode_mask=*/0, gapEventHandle);

  if (handle == GAP_INVALID_HANDLE) {
    ESP_LOGE(PS4_TAG, "%s Registering GAP service %s failed", __func__, name);
  }
  else {
    ESP_LOGI(PS4_TAG, "[%s] GAP Service %s Initialized: %d", __func__, name, handle);
  }

  return handle;
}

/*******************************************************************************
**
** Function         gapEventHandle
**
** Description      Callback for GAP events, currently handling the connection
**                  opened, connection closed, and data available events.
**
** Returns          void
**
*******************************************************************************/
static void gapEventHandle(uint16_t gapHandle, uint16_t event) {
  switch (event) {
    case GAP_EVT_CONN_OPENED:
    case GAP_EVT_CONN_CLOSED: {
      bool wasConnected = isConnected;
      gapUpdateConnected();

      if (wasConnected != isConnected) {
        ps4ConnectEvent(isConnected);
      }

      break;
    }
    case GAP_EVT_CONN_DATA_AVAIL: {
      BT_HDR* buffer;
      GAP_ConnBTRead(gapHandle, &buffer);

      if (buffer->length > 2) {
        parsePacket(buffer->data);
      }

      osi_free(buffer);
      break;
    }
    default:
      break;
  }
}

/*******************************************************************************
**
** Function         gapUpdateConnected
**
** Description      This updates the isConnected flag by checking if the
**                  GAP handles return valid L2CAP channel IDs.
**
** Returns          void
**
*******************************************************************************/
static void gapUpdateConnected() {
  isConnected = GAP_ConnGetL2CAPCid(gapHandleHIDControl) != 0 && GAP_ConnGetL2CAPCid(gapHandleHIDInterrupt) != 0;
}
