#include <esp_system.h>

#include "ps4.h"
#include "ps4_int.h"

/********************************************************************************/
/*                            L O C A L    T Y P E S */
/********************************************************************************/

enum ps4_packet_index {
  packet_index_analog_stick_lx = 13,
  packet_index_analog_stick_ly = 14,
  packet_index_analog_stick_rx = 15,
  packet_index_analog_stick_ry = 16,

  packet_index_button_standard = 17,
  packet_index_button_extra = 18,
  packet_index_button_ps = 19,

  packet_index_analog_l2 = 20,
  packet_index_analog_r2 = 21,

  packet_index_status = 42
};

enum ps4_button_mask {
  button_mask_up = 0,
  button_mask_right = 0b00000010,
  button_mask_down = 0b00000100,
  button_mask_left = 0b00000110,

  button_mask_upright = 0b00000001,
  button_mask_downright = 0b00000011,
  button_mask_upleft = 0b00000111,
  button_mask_downleft = 0b00000101,

  button_mask_direction = 0b00001111,

  button_mask_square = 0b00010000,
  button_mask_cross = 0b00100000,
  button_mask_circle = 0b01000000,
  button_mask_triangle = 0b10000000,

  button_mask_l1 = 0b00000001,
  button_mask_r1 = 0b00000010,
  button_mask_l2 = 0b00000100,
  button_mask_r2 = 0b00001000,

  button_mask_share = 0b00010000,
  button_mask_options = 0b00100000,

  button_mask_l3 = 0b01000000,
  button_mask_r3 = 0b10000000,

  button_mask_ps = 0b01,
  button_mask_touchpad = 0b10
};

enum ps4_status_mask {
  ps4_status_mask_battery = 0b00001111,
  ps4_status_mask_charging = 0b00010000,
  ps4_status_mask_audio = 0b00100000,
  ps4_status_mask_mic = 0b01000000,
};

/********************************************************************************/
/*              L O C A L    F U N C T I O N     P R O T O T Y P E S */
/********************************************************************************/

ps4_sensor_t parsePacketSensor(uint8_t* packet);
ps4_status_t parsePacketStatus(uint8_t* packet);
ps4_analog_stick_t parsePacketAnalogStick(uint8_t* packet);
ps4_analog_button_t parsePacketAnalogButton(uint8_t* packet);
ps4_button_t parsePacketButtons(uint8_t* packet);
ps4_event_t parseEvent(ps4_t prev, ps4_t cur);

/********************************************************************************/
/*                         L O C A L    V A R I A B L E S */
/********************************************************************************/

static ps4_t ps4;
static ps4_event_callback_t ps4_event_cb = NULL;

/********************************************************************************/
/*                      P U B L I C    F U N C T I O N S */
/********************************************************************************/
void parserSetEventCb(ps4_event_callback_t cb) { ps4_event_cb = cb; }

void parsePacket(uint8_t* packet) {
  ps4_t prev_ps4 = ps4;

  ps4.button = parsePacketButtons(packet);
  ps4.analog.stick = parsePacketAnalogStick(packet);
  ps4.analog.button = parsePacketAnalogButton(packet);
  // ps4.sensor = parsePacketSensor(packet);
  ps4.status = parsePacketStatus(packet);
  ps4.latestPacket = packet;

  ps4_event_t ps4Event = parseEvent(prev_ps4, ps4);

  ps4PacketEvent(ps4, ps4Event, packet);
}

/********************************************************************************/
/*                      L O C A L    F U N C T I O N S */
/********************************************************************************/

/******************/
/*    E V E N T   */
/******************/
ps4_event_t parseEvent(ps4_t prev, ps4_t cur) {
  ps4_event_t ps4Event;

  /* Button down events */
  ps4Event.button_down.right = !prev.button.right && cur.button.right;
  ps4Event.button_down.down = !prev.button.down && cur.button.down;
  ps4Event.button_down.up = !prev.button.up && cur.button.up;
  ps4Event.button_down.left = !prev.button.left && cur.button.left;

  ps4Event.button_down.square = !prev.button.square && cur.button.square;
  ps4Event.button_down.cross = !prev.button.cross && cur.button.cross;
  ps4Event.button_down.circle = !prev.button.circle && cur.button.circle;
  ps4Event.button_down.triangle = !prev.button.triangle && cur.button.triangle;

  ps4Event.button_down.upright = !prev.button.upright && cur.button.upright;
  ps4Event.button_down.downright = !prev.button.downright && cur.button.downright;
  ps4Event.button_down.upleft = !prev.button.upleft && cur.button.upleft;
  ps4Event.button_down.downleft = !prev.button.downleft && cur.button.downleft;

  ps4Event.button_down.l1 = !prev.button.l1 && cur.button.l1;
  ps4Event.button_down.r1 = !prev.button.r1 && cur.button.r1;
  ps4Event.button_down.l2 = !prev.button.l2 && cur.button.l2;
  ps4Event.button_down.r2 = !prev.button.r2 && cur.button.r2;

  ps4Event.button_down.share = !prev.button.share && cur.button.share;
  ps4Event.button_down.options = !prev.button.options && cur.button.options;
  ps4Event.button_down.l3 = !prev.button.l3 && cur.button.l3;
  ps4Event.button_down.r3 = !prev.button.r3 && cur.button.r3;

  ps4Event.button_down.ps = !prev.button.ps && cur.button.ps;
  ps4Event.button_down.touchpad = !prev.button.touchpad && cur.button.touchpad;

  /* Button up events */
  ps4Event.button_down.right = prev.button.right && !cur.button.right;
  ps4Event.button_down.down = prev.button.down && !cur.button.down;
  ps4Event.button_down.up = prev.button.up && !cur.button.up;
  ps4Event.button_down.left = prev.button.left && !cur.button.left;

  ps4Event.button_down.square = prev.button.square && !cur.button.square;
  ps4Event.button_down.cross = prev.button.cross && !cur.button.cross;
  ps4Event.button_down.circle = prev.button.circle && !cur.button.circle;
  ps4Event.button_down.triangle = prev.button.triangle && !cur.button.triangle;

  ps4Event.button_down.upright = prev.button.upright && !cur.button.upright;
  ps4Event.button_down.downright = prev.button.downright && !cur.button.downright;
  ps4Event.button_down.upleft = prev.button.upleft && !cur.button.upleft;
  ps4Event.button_down.downleft = prev.button.downleft && !cur.button.downleft;

  ps4Event.button_down.l1 = prev.button.l1 && !cur.button.l1;
  ps4Event.button_down.r1 = prev.button.r1 && !cur.button.r1;
  ps4Event.button_down.l2 = prev.button.l2 && !cur.button.l2;
  ps4Event.button_down.r2 = prev.button.r2 && !cur.button.r2;

  ps4Event.button_down.share = prev.button.share && !cur.button.share;
  ps4Event.button_down.options = prev.button.options && !cur.button.options;
  ps4Event.button_down.l3 = prev.button.l3 && !cur.button.l3;
  ps4Event.button_down.r3 = prev.button.r3 && !cur.button.r3;

  ps4Event.button_down.ps = prev.button.ps && !cur.button.ps;
  ps4Event.button_down.touchpad = prev.button.touchpad && !cur.button.touchpad;

  ps4Event.analog_move.stick.lx = cur.analog.stick.lx != 0;
  ps4Event.analog_move.stick.ly = cur.analog.stick.ly != 0;
  ps4Event.analog_move.stick.rx = cur.analog.stick.rx != 0;
  ps4Event.analog_move.stick.ry = cur.analog.stick.ry != 0;

  return ps4Event;
}

/********************/
/*    A N A L O G   */
/********************/
ps4_analog_stick_t parsePacketAnalogStick(uint8_t* packet) {
  ps4_analog_stick_t ps4AnalogStick;

  const uint8_t offset = 128;

  ps4AnalogStick.lx = packet[packet_index_analog_stick_lx] - offset;
  ps4AnalogStick.ly = -packet[packet_index_analog_stick_ly] + offset - 1;
  ps4AnalogStick.rx = packet[packet_index_analog_stick_rx] - offset;
  ps4AnalogStick.ry = -packet[packet_index_analog_stick_ry] + offset - 1;

  return ps4AnalogStick;
}

ps4_analog_button_t parsePacketAnalogButton(uint8_t* packet) {
  ps4_analog_button_t ps4AnalogButton;

  ps4AnalogButton.l2 = packet[packet_index_analog_l2];
  ps4AnalogButton.r2 = packet[packet_index_analog_r2];

  return ps4AnalogButton;
}

/*********************/
/*   B U T T O N S   */
/*********************/

ps4_button_t parsePacketButtons(uint8_t* packet) {
  ps4_button_t ps4_button;
  uint8_t frontBtnData = packet[packet_index_button_standard];
  uint8_t extraBtnData = packet[packet_index_button_extra];
  uint8_t psBtnData = packet[packet_index_button_ps];
  uint8_t directionBtnsOnly = button_mask_direction & frontBtnData;

  ps4_button.up = directionBtnsOnly == button_mask_up;
  ps4_button.right = directionBtnsOnly == button_mask_right;
  ps4_button.down = directionBtnsOnly == button_mask_down;
  ps4_button.left = directionBtnsOnly == button_mask_left;

  ps4_button.upright = directionBtnsOnly == button_mask_upright;
  ps4_button.upleft = directionBtnsOnly == button_mask_upleft;
  ps4_button.downright = directionBtnsOnly == button_mask_downright;
  ps4_button.downleft = directionBtnsOnly == button_mask_downleft;

  ps4_button.triangle = (frontBtnData & button_mask_triangle) ? true : false;
  ps4_button.circle = (frontBtnData & button_mask_circle) ? true : false;
  ps4_button.cross = (frontBtnData & button_mask_cross) ? true : false;
  ps4_button.square = (frontBtnData & button_mask_square) ? true : false;

  ps4_button.l1 = (extraBtnData & button_mask_l1) ? true : false;
  ps4_button.r1 = (extraBtnData & button_mask_r1) ? true : false;
  ps4_button.l2 = (extraBtnData & button_mask_l2) ? true : false;
  ps4_button.r2 = (extraBtnData & button_mask_r2) ? true : false;

  ps4_button.share = (extraBtnData & button_mask_share) ? true : false;
  ps4_button.options = (extraBtnData & button_mask_options) ? true : false;
  ps4_button.l3 = (extraBtnData & button_mask_l3) ? true : false;
  ps4_button.r3 = (extraBtnData & button_mask_r3) ? true : false;

  ps4_button.ps = (psBtnData & button_mask_ps) ? true : false;
  ps4_button.touchpad = (psBtnData & button_mask_touchpad) ? true : false;

  return ps4_button;
}

/*******************************/
/*   S T A T U S   F L A G S   */
/*******************************/
ps4_status_t parsePacketStatus(uint8_t* packet) {
  ps4_status_t ps4Status;

  ps4Status.battery = packet[packet_index_status] & ps4_status_mask_battery;
  ps4Status.charging = packet[packet_index_status] & ps4_status_mask_charging ? true : false;
  ps4Status.audio = packet[packet_index_status] & ps4_status_mask_audio ? true : false;
  ps4Status.mic = packet[packet_index_status] & ps4_status_mask_mic ? true : false;

  return ps4Status;
}

/********************/
/*   S E N S O R S  */
/********************/
ps4_sensor_t parsePacketSensor(uint8_t* packet) {
  ps4_sensor_t ps4Sensor;
  /*
      const uint16_t offset = 0x200;

      ps4Sensor.accelerometer.x = (packet[packet_index_sensor_accelerometer_x] << 8) +
     packet[packet_index_sensor_accelerometer_x+1] - offset;
      ps4Sensor.accelerometer.y = (packet[packet_index_sensor_accelerometer_y] << 8) +
     packet[packet_index_sensor_accelerometer_y+1] - offset;
      ps4Sensor.accelerometer.z = (packet[packet_index_sensor_accelerometer_z] << 8) +
     packet[packet_index_sensor_accelerometer_z+1] - offset;
      ps4Sensor.gyroscope.z     = (packet[packet_index_sensor_gyroscope_z]
     << 8) + packet[packet_index_sensor_gyroscope_z+1]     - offset;
  */
  return ps4Sensor;
}
