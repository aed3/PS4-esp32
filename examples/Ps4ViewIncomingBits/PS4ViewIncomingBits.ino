#include <PS4Controller.h>

String byteToBinary(uint8_t byte) {
  String byteString(byte, BIN);

  for (int i = byteString.length(); i < 8; i++) {
    byteString = '0' + byteString;
  }

  return byteString;
}

/* Print bytes and bits in decreasing order where
*  each byte is displayed in its group of 8 bits
*  and 4 bytes are printed per line
*
*  Example: Print 4 bytes
*
*           Bit 8  Bit 1 Bit 8  Bit 1 Bit 8  Bit 1 Bit 8  Bit 1
*           ˅      ˅     ˅      ˅     ˅      ˅     ˅      ˅
*  BYTE 3 : 10101110     11010011     10101110     11010011 : BYTE 0
*              ˄            ˄            ˄            ˄
*           4th Byte     3rd Byte     2nd Byte     1st Byte
*/
void printBits(uint8_t* packet, int byteCount) {
  for (int byte = byteCount - 4; byte >= 0; byte -= 4) {
    Serial.printf("BYTE %d :\t%s %s %s %s\t: BYTE %d\n",
      (byte + 3),
      byteToBinary(packet[byte + 3]),
      byteToBinary(packet[byte + 2]),
      byteToBinary(packet[byte + 1]),
      byteToBinary(packet[byte]),
      byte);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  // Replace the "03:03:03:03:03:03" with the MAC address
  // the controller wants to pair to
  PS4.begin("03:03:03:03:03:03");
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    // Prints the 64 bytes of data the controller sends
    // to the ESP32 every second
    delay(1000);
    printBits(PS4.LatestPacket(), 64);
  }
}
