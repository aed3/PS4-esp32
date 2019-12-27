#ifndef PS4_INT_H
#define PS4_INT_H

#include "sdkconfig.h"

/** Check if the project is configured properly */
#ifndef ARDUINO_ARCH_ESP32

#ifndef CONFIG_BT_ENABLED
#error "The ESP32-PS4 module requires the Bluetooth component to be enabled in the project's menuconfig"
#endif

#ifndef CONFIG_BLUEDROID_ENABLED
#error "The ESP32-PS4 module requires Bluedroid to be enabled in the project's menuconfig"
#endif

#ifndef CONFIG_CLASSIC_BT_ENABLED
#error "The ESP32-PS4 module requires Classic Bluetooth to be enabled in the project's menuconfig"
#endif

#ifndef CONFIG_BT_SPP_ENABLED
#error "The ESP32-PS4 module requires Classic Bluetooth's SPP to be enabled in the project's menuconfig"
#endif

/** Check the configured blueooth mode */
#ifdef CONFIG_BTDM_CONTROLLER_MODE_BTDM
#define BT_MODE ESP_BT_MODE_BTDM
#elif defined CONFIG_BTDM_CONTROLLER_MODE_BR_EDR_ONLY
#define BT_MODE ESP_BT_MODE_CLASSIC_BT
#else
#error "The selected Bluetooth controller mode is not supported by the ESP32-PS4 module"
#endif

#endif // ARDUINO_ARCH_ESP32

/** ESP-IDF compatibility configuration option choices */
#define IDF_COMPATIBILITY_MASTER_21165ED 3
#define IDF_COMPATIBILITY_MASTER_D9CE0BB 2
#define IDF_COMPATIBILITY_MASTER_21AF1D7 1

#ifndef CONFIG_IDF_COMPATIBILITY
#define CONFIG_IDF_COMPATIBILITY IDF_COMPATIBILITY_MASTER_21AF1D7
#endif

/** Size of the output report buffer for the Dualshock and Navigation controllers */
#define PS4_REPORT_BUFFER_SIZE 48
#define PS4_HID_BUFFER_SIZE    50

/********************************************************************************/
/*                         S H A R E D   T Y P E S                              */
/********************************************************************************/

enum hid_cmd_code {
    hid_cmd_code_set_report   = 0x50,
    hid_cmd_code_type_output  = 0x02,
    hid_cmd_code_type_feature = 0x03
};

enum hid_cmd_identifier {
    hid_cmd_identifier_ps4_enable  = 0xf4,
    hid_cmd_identifier_ps4_control = 0x01
};


typedef struct {
  uint8_t code;
  uint8_t identifier;
  uint8_t data[PS4_REPORT_BUFFER_SIZE];

} hid_cmd_t;

enum ps4_control_packet_index {
    ps4_control_packet_index_rumble_right_duration = 1,
    ps4_control_packet_index_rumble_right_intensity = 2,
    ps4_control_packet_index_rumble_left_duration = 3,
    ps4_control_packet_index_rumble_left_intensity = 4,

    ps4_control_packet_index_leds = 9,
    ps4_control_packet_index_led4_arguments = 10,
    ps4_control_packet_index_led3_arguments = 15,
    ps4_control_packet_index_led2_arguments = 20,
    ps4_control_packet_index_led1_arguments = 25
};

enum ps4_led_mask {
    ps4_led_mask_led1 = 1 << 1,
    ps4_led_mask_led2 = 1 << 2,
    ps4_led_mask_led3 = 1 << 3,
    ps4_led_mask_led4 = 1 << 4,
};


/********************************************************************************/
/*                     C A L L B A C K   F U N C T I O N S                      */
/********************************************************************************/

void ps4_connect_event(uint8_t is_connected);
void ps4_packet_event( ps4_t ps4, ps4_event_t event );


/********************************************************************************/
/*                      P A R S E R   F U N C T I O N S                         */
/********************************************************************************/

void ps4_parse_packet( uint8_t *packet );


/********************************************************************************/
/*                          S P P   F U N C T I O N S                           */
/********************************************************************************/

void ps4_spp_init();


/********************************************************************************/
/*                          G A P   F U N C T I O N S                           */
/********************************************************************************/

bool ps4_gap_is_connected();
void ps4_gap_init_services();
void ps4_gap_send_hid( hid_cmd_t *hid_cmd, uint8_t len );

#endif
