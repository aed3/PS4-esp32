#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "include/ps4.h"
#include "ps4_int.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "time.h"
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

#define  PS4_TAG "PS4_PARSER"


/********************************************************************************/
/*                            L O C A L    T Y P E S                            */
/********************************************************************************/

time_t prevTime = 0;

enum ps4_packet_index {
    ps4_packet_index_buttons = 17,

    ps4_packet_index_analog_stick_lx = 13,
    ps4_packet_index_analog_stick_ly = 14,
    ps4_packet_index_analog_stick_rx = 15,
    ps4_packet_index_analog_stick_ry = 16,

    ps4_packet_index_analog_button_l2 = 20,
    ps4_packet_index_analog_button_r2 = 21,

    ps4_packet_index_status = 42
};

enum ps4_button_mask {
    ps4_button_mask_up       = 0,
    ps4_button_mask_right    = 1 << 1,
    ps4_button_mask_down     = 1 << 2,
    ps4_button_mask_left     = (1 << 1) + (1 << 2),

    ps4_button_mask_upright  = 1,
    ps4_button_mask_upleft   = 1 + (1 << 1) + (1 << 2),
    ps4_button_mask_downright= 1 + (1 << 1),
    ps4_button_mask_downleft = 1 + (1 << 2),

    ps4_button_mask_arrows   = 0xf,

    ps4_button_mask_square   = 1 << 4,
    ps4_button_mask_cross    = 1 << 5,
    ps4_button_mask_circle   = 1 << 6,
    ps4_button_mask_triangle = 1 << 7,

    ps4_button_mask_l1       = 1 << 8,
    ps4_button_mask_r1       = 1 << 9,
	ps4_button_mask_l2       = 1 << 10,
    ps4_button_mask_r2       = 1 << 11,
		
    ps4_button_mask_share    = 1 << 12,
    ps4_button_mask_options  = 1 << 13,
		
    ps4_button_mask_l3       = 1 << 14,
    ps4_button_mask_r3       = 1 << 15,

    ps4_button_mask_ps       = 1 << 16,
    ps4_button_mask_touchpad = 1 << 17
};

enum ps4_status_mask {
    ps4_status_mask_battery  = 0xf,
    ps4_status_mask_charging = 1 << 4,
    ps4_status_mask_audio    = 1 << 5,
    ps4_status_mask_mic      = 1 << 6,
};


/********************************************************************************/
/*              L O C A L    F U N C T I O N     P R O T O T Y P E S            */
/********************************************************************************/

ps4_sensor_t ps4_parse_packet_sensor( uint8_t *packet );
ps4_status_t ps4_parse_packet_status( uint8_t *packet );
ps4_analog_stick_t ps4_parse_packet_analog_stick( uint8_t *packet );
ps4_analog_button_t ps4_parse_packet_analog_button( uint8_t *packet );
ps4_button_t ps4_parse_packet_buttons( uint8_t *packet );
ps4_event_t ps4_parse_event( ps4_t prev, ps4_t cur );


/********************************************************************************/
/*                         L O C A L    V A R I A B L E S                       */
/********************************************************************************/

static ps4_t ps4;
static ps4_event_callback_t ps4_event_cb = NULL;

/********************************************************************************/
/*                      P U B L I C    F U N C T I O N S                        */
/********************************************************************************/
void ps4_parser_set_event_cb( ps4_event_callback_t cb )
{
    ps4_event_cb = cb;
}

void printBytes2Binary(uint8_t *packet, int byteCount) {
    packet += 12;
	int byte = byteCount-4;
	for (; byte >= 0; byte-=4) {
		uint32_t toBinary = *((uint32_t*)&packet[byte]);
        //printf("%d, %d, %d, %d, ", 0xff & (toBinary >> 24), 0xff & (toBinary >> 16), 0xff & (toBinary >> 8), 0xff & toBinary);
		printf("%d : "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN" : %d\n", (byte+3), BYTE_TO_BINARY(toBinary>>24), BYTE_TO_BINARY(toBinary>>16), BYTE_TO_BINARY(toBinary>>8), BYTE_TO_BINARY(toBinary), byte);
	}
	printf("\n");
}

void ps4_parse_packet( uint8_t *packet )
{
    ps4_t prev_ps4 = ps4;

    //time_t newTime = clock();
    //if (newTime - prevTime > 1000) {
    //    printf("%c", 255);
    //    printBytes2Binary(packet, 44);
    //    printf("Battery = %d\n", ps4.status.battery);
    //    prevTime = newTime;
    //}

    ps4.button        = ps4_parse_packet_buttons(packet);
    ps4.analog.stick  = ps4_parse_packet_analog_stick(packet);
    ps4.analog.button = ps4_parse_packet_analog_button(packet);
    //ps4.sensor        = ps4_parse_packet_sensor(packet);
    ps4.status        = ps4_parse_packet_status(packet);

    ps4_event_t ps4_event = ps4_parse_event( prev_ps4, ps4 );

    ps4_packet_event( ps4, ps4_event );

}


/********************************************************************************/
/*                      L O C A L    F U N C T I O N S                          */
/********************************************************************************/

/******************/
/*    E V E N T   */
/******************/
ps4_event_t ps4_parse_event( ps4_t prev, ps4_t cur )
{
    ps4_event_t ps4_event;

    /* Button down events */
    ps4_event.button_down.options  = !prev.button.options  && cur.button.options;
    ps4_event.button_down.l3       = !prev.button.l3       && cur.button.l3;
    ps4_event.button_down.r3       = !prev.button.r3       && cur.button.r3;
    ps4_event.button_down.share    = !prev.button.share    && cur.button.share;

    ps4_event.button_down.up       = !prev.button.up       && cur.button.up;
    ps4_event.button_down.right    = !prev.button.right    && cur.button.right;
    ps4_event.button_down.down     = !prev.button.down     && cur.button.down;
    ps4_event.button_down.left     = !prev.button.left     && cur.button.left;

    ps4_event.button_down.upright  = !prev.button.upright  && cur.button.upright;
    ps4_event.button_down.upleft   = !prev.button.upleft   && cur.button.upleft;
    ps4_event.button_down.downright= !prev.button.downright&& cur.button.downright;
    ps4_event.button_down.downleft = !prev.button.downleft && cur.button.downleft;

    ps4_event.button_down.l2       = !prev.button.l2       && cur.button.l2;
    ps4_event.button_down.r2       = !prev.button.r2       && cur.button.r2;
    ps4_event.button_down.l1       = !prev.button.l1       && cur.button.l1;
    ps4_event.button_down.r1       = !prev.button.r1       && cur.button.r1;

    ps4_event.button_down.triangle = !prev.button.triangle && cur.button.triangle;
    ps4_event.button_down.circle   = !prev.button.circle   && cur.button.circle;
    ps4_event.button_down.cross    = !prev.button.cross    && cur.button.cross;
    ps4_event.button_down.square   = !prev.button.square   && cur.button.square;

    ps4_event.button_down.ps       = !prev.button.ps       && cur.button.ps;
    ps4_event.button_down.touchpad = !prev.button.touchpad && cur.button.touchpad;

    /* Button up events */
    ps4_event.button_up.options  = !prev.button.options  && cur.button.options;
    ps4_event.button_up.l3       = !prev.button.l3       && cur.button.l3;
    ps4_event.button_up.r3       = !prev.button.r3       && cur.button.r3;
    ps4_event.button_up.share    = !prev.button.share    && cur.button.share;

    ps4_event.button_up.up       = !prev.button.up       && cur.button.up;
    ps4_event.button_up.right    = !prev.button.right    && cur.button.right;
    ps4_event.button_up.down     = !prev.button.down     && cur.button.down;
    ps4_event.button_up.left     = !prev.button.left     && cur.button.left;

    ps4_event.button_up.upright  = !prev.button.upright  && cur.button.upright;
    ps4_event.button_up.upleft   = !prev.button.upleft   && cur.button.upleft;
    ps4_event.button_up.downright= !prev.button.downright&& cur.button.downright;
    ps4_event.button_up.downleft = !prev.button.downleft && cur.button.downleft;

    ps4_event.button_up.l2       = !prev.button.l2       && cur.button.l2;
    ps4_event.button_up.r2       = !prev.button.r2       && cur.button.r2;
    ps4_event.button_up.l1       = !prev.button.l1       && cur.button.l1;
    ps4_event.button_up.r1       = !prev.button.r1       && cur.button.r1;

    ps4_event.button_up.triangle = !prev.button.triangle && cur.button.triangle;
    ps4_event.button_up.circle   = !prev.button.circle   && cur.button.circle;
    ps4_event.button_up.cross    = !prev.button.cross    && cur.button.cross;
    ps4_event.button_up.square   = !prev.button.square   && cur.button.square;

    ps4_event.button_up.ps       = !prev.button.ps       && cur.button.ps;
    ps4_event.button_up.touchpad = !prev.button.touchpad && cur.button.touchpad;

	ps4_event.analog_move.stick.lx = cur.analog.stick.lx != 0;
	ps4_event.analog_move.stick.ly = cur.analog.stick.ly != 0;
	ps4_event.analog_move.stick.rx = cur.analog.stick.rx != 0;
	ps4_event.analog_move.stick.ry = cur.analog.stick.ry != 0;

    return ps4_event;
}

/********************/
/*    A N A L O G   */
/********************/
ps4_analog_stick_t ps4_parse_packet_analog_stick( uint8_t *packet )
{
    ps4_analog_stick_t ps4_analog_stick;

    const uint8_t int_offset = 0x80;

    ps4_analog_stick.lx = (uint8_t)packet[ps4_packet_index_analog_stick_lx] - int_offset;
    ps4_analog_stick.ly = -(uint8_t)packet[ps4_packet_index_analog_stick_ly] + int_offset - 1;
    ps4_analog_stick.rx = (uint8_t)packet[ps4_packet_index_analog_stick_rx] - int_offset;
    ps4_analog_stick.ry = -(uint8_t)packet[ps4_packet_index_analog_stick_ry] + int_offset - 1;

    return ps4_analog_stick;
}

ps4_analog_button_t ps4_parse_packet_analog_button( uint8_t *packet )
{
    ps4_analog_button_t ps4_analog_button;

    ps4_analog_button.l2 = packet[ps4_packet_index_analog_button_l2];
    ps4_analog_button.r2 = packet[ps4_packet_index_analog_button_r2];

    return ps4_analog_button;
}

/*********************/
/*   B U T T O N S   */
/*********************/

ps4_button_t ps4_parse_packet_buttons( uint8_t *packet )
{
    ps4_button_t ps4_button;
    uint32_t ps4_buttons_raw = *((uint32_t*)&packet[ps4_packet_index_buttons]);
	uint8_t arrow_buttons_only = ps4_button_mask_arrows & ps4_buttons_raw;

    ps4_button.options  = (ps4_buttons_raw & ps4_button_mask_options)  ? true : false;
    ps4_button.l3       = (ps4_buttons_raw & ps4_button_mask_l3)       ? true : false;
    ps4_button.r3       = (ps4_buttons_raw & ps4_button_mask_r3)       ? true : false;
    ps4_button.share    = (ps4_buttons_raw & ps4_button_mask_share)    ? true : false;

    ps4_button.up        = arrow_buttons_only == ps4_button_mask_up;
    ps4_button.right     = arrow_buttons_only == ps4_button_mask_right;
    ps4_button.down      = arrow_buttons_only == ps4_button_mask_down;
    ps4_button.left      = arrow_buttons_only == ps4_button_mask_left;

    ps4_button.upright   = arrow_buttons_only == ps4_button_mask_upright;
    ps4_button.upleft    = arrow_buttons_only == ps4_button_mask_upleft;
    ps4_button.downright = arrow_buttons_only == ps4_button_mask_downright;
    ps4_button.downleft  = arrow_buttons_only == ps4_button_mask_downleft;

    ps4_button.l2       = (ps4_buttons_raw & ps4_button_mask_l2)       ? true : false;
    ps4_button.r2       = (ps4_buttons_raw & ps4_button_mask_r2)       ? true : false;
    ps4_button.l1       = (ps4_buttons_raw & ps4_button_mask_l1)       ? true : false;
    ps4_button.r1       = (ps4_buttons_raw & ps4_button_mask_r1)       ? true : false;

    ps4_button.triangle = (ps4_buttons_raw & ps4_button_mask_triangle) ? true : false;
    ps4_button.circle   = (ps4_buttons_raw & ps4_button_mask_circle)   ? true : false;
    ps4_button.cross    = (ps4_buttons_raw & ps4_button_mask_cross)    ? true : false;
    ps4_button.square   = (ps4_buttons_raw & ps4_button_mask_square)   ? true : false;

    ps4_button.ps       = (ps4_buttons_raw & ps4_button_mask_ps)       ? true : false;
    ps4_button.touchpad = (ps4_buttons_raw & ps4_button_mask_touchpad) ? true : false;

    return ps4_button;
}

/*******************************/
/*   S T A T U S   F L A G S   */
/*******************************/
ps4_status_t ps4_parse_packet_status( uint8_t *packet )
{
    ps4_status_t ps4_status;

    ps4_status.battery  = packet[ps4_packet_index_status] & ps4_status_mask_battery;
    ps4_status.charging = packet[ps4_packet_index_status] & ps4_status_mask_charging ? true : false;
    ps4_status.audio    = packet[ps4_packet_index_status] & ps4_status_mask_audio    ? true : false;
    ps4_status.mic      = packet[ps4_packet_index_status] & ps4_status_mask_mic      ? true : false;

    return ps4_status;
}

/********************/
/*   S E N S O R S  */
/********************/
ps4_sensor_t ps4_parse_packet_sensor( uint8_t *packet )
{
    ps4_sensor_t ps4_sensor;

/*     const uint16_t int_offset = 0x200;

    ps4_sensor.accelerometer.x = (packet[ps4_packet_index_sensor_accelerometer_x] << 8) + packet[ps4_packet_index_sensor_accelerometer_x+1] - int_offset;
    ps4_sensor.accelerometer.y = (packet[ps4_packet_index_sensor_accelerometer_y] << 8) + packet[ps4_packet_index_sensor_accelerometer_y+1] - int_offset;
    ps4_sensor.accelerometer.z = (packet[ps4_packet_index_sensor_accelerometer_z] << 8) + packet[ps4_packet_index_sensor_accelerometer_z+1] - int_offset;
    ps4_sensor.gyroscope.z     = (packet[ps4_packet_index_sensor_gyroscope_z]     << 8) + packet[ps4_packet_index_sensor_gyroscope_z+1]     - int_offset; */

    return ps4_sensor;

}
