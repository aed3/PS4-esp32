#include <PS4Controller.h>

void setup() {
  Serial.begin(115200);

  // Replace the "03:03:03:03:03:03" with the MAC address
  // the controller wants to pair to
  PS4.begin("03:03:03:03:03:03");
  Serial.println("Ready.");
}

void loop() {
  if (PS4.isConnected()) {
    Serial.println("Connected!");
  }

  delay(3000);
}
