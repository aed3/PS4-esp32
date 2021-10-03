#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);

  // Replace the "1a:2b:3c:01:01:01" with the MAC address
  // the controller wants to pair to
  // Note: MAC address must be unicast
  PS4.begin("1a:2b:3c:01:01:01");
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    Serial.println("Connected!");
  }

  delay(3000);
}
