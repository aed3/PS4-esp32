#include <PS4Controller.h>

int r = 255;
int g = 0;
int b = 0;

// Calculates the next value in a rainbow sequence
void nextRainbowColor() {
  if (r > 0 && b == 0) {
    r--;
    g++;
  }
  if (g > 0 && r == 0) {
    g--;
    b++;
  }
  if (b > 0 && g == 0) {
    r++;
    b--;
  }
}

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
    // Sets the color of the controller's front light
    // Params: Red, Green,and Blue
    // See here for details: https://www.w3schools.com/colors/colors_rgb.asp
    PS4.setLed(r, g, b);
    nextRainbowColor();

    // Sets how fast the controller's front light flashes
    // Params: How long the light is on in ms, how long the light is off in ms
    // Range: 0->2550 ms, Set to 0, 0 for the light to remain on
    PS4.setFlashRate(PS4.LStickY() * 10, PS4.RStickY() * 10);

    // Sets the rumble of the controllers
    // Params: Weak rumble intensity, Strong rumble intensity
    // Range: 0->255
    PS4.setRumble(PS4.L2Value(), PS4.R2Value());

    // Sends data set in the above three instructions to the controller
    PS4.sendToController();

    // Don't send data to the controller immediately, will cause buffer overflow
    delay(10);
  }
}
