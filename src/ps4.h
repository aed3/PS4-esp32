#ifndef PS4_H
#define PS4_H

#include <stdbool.h>
#include <stdint.h>

/********************************************************************************/
/*                                  T Y P E S */
/********************************************************************************/

/********************/
/*    A N A L O G   */
/********************/

typedef struct {
  int8_t lx;
  int8_t ly;
  int8_t rx;
  int8_t ry;
} ps4_analog_stick_t;

typedef struct {
  uint8_t l2;
  uint8_t r2;
} ps4_analog_button_t;

typedef struct {
  ps4_analog_stick_t stick;
  ps4_analog_button_t button;
} ps4_analog_t;

/*********************/
/*   B U T T O N S   */
/*********************/

typedef struct {
  uint8_t right : 1;
  uint8_t down : 1;
  uint8_t up : 1;
  uint8_t left : 1;

  uint8_t square : 1;
  uint8_t cross : 1;
  uint8_t circle : 1;
  uint8_t triangle : 1;

  uint8_t upright : 1;
  uint8_t downright : 1;
  uint8_t upleft : 1;
  uint8_t downleft : 1;

  uint8_t l1 : 1;
  uint8_t r1 : 1;
  uint8_t l2 : 1;
  uint8_t r2 : 1;

  uint8_t share : 1;
  uint8_t options : 1;
  uint8_t l3 : 1;
  uint8_t r3 : 1;

  uint8_t ps : 1;
  uint8_t touchpad : 1;
} ps4_button_t;

/*******************************/
/*   S T A T U S   F L A G S   */
/*******************************/

typedef struct {
  uint8_t battery;
  uint8_t charging : 1;
  uint8_t audio : 1;
  uint8_t mic : 1;
} ps4_status_t;

/********************/
/*   S E N S O R S  */
/********************/

typedef struct {
  int16_t z;
} ps4_sensor_gyroscope_t;

typedef struct {
  int16_t x;
  int16_t y;
  int16_t z;
} ps4_sensor_accelerometer_t;

typedef struct {
  ps4_sensor_accelerometer_t accelerometer;
  ps4_sensor_gyroscope_t gyroscope;
} ps4_sensor_t;

/*******************/
/*    O T H E R    */
/*******************/

typedef struct {
  uint8_t smallRumble;
  uint8_t largeRumble;
  uint8_t r, g, b;
  uint8_t flashOn;
  uint8_t flashOff;  // Time to flash bright/dark (255 = 2.5 seconds)
} ps4_cmd_t;

typedef struct {
  ps4_button_t button_down;
  ps4_button_t button_up;
  ps4_analog_t analog_move;
} ps4_event_t;

typedef struct {
  ps4_analog_t analog;
  ps4_button_t button;
  ps4_status_t status;
  ps4_sensor_t sensor;
  uint8_t* latestPacket;
} ps4_t;

/***************************/
/*    C A L L B A C K S    */
/***************************/

typedef void (*ps4_connection_callback_t)(uint8_t isConnected);
typedef void (*ps4_connection_object_callback_t)(void* object, uint8_t isConnected);

typedef void (*ps4_event_callback_t)(ps4_t ps4, ps4_event_t event);
typedef void (*ps4_event_object_callback_t)(void* object, ps4_t ps4, ps4_event_t event);

/********************************************************************************/
/*                             F U N C T I O N S */
/********************************************************************************/

bool ps4IsConnected();
void ps4Init();
void ps4Enable();
void ps4Cmd(ps4_cmd_t ps4_cmd);
void ps4SetConnectionCallback(ps4_connection_callback_t cb);
void ps4SetConnectionObjectCallback(void* object, ps4_connection_object_callback_t cb);
void ps4SetEventCallback(ps4_event_callback_t cb);
void ps4SetEventObjectCallback(void* object, ps4_event_object_callback_t cb);
void ps4SetLed(uint8_t r, uint8_t g, uint8_t b);
void ps4SetOutput(ps4_cmd_t prev_cmd);
void ps4SetBluetoothMacAddress(const uint8_t* mac);

#endif
