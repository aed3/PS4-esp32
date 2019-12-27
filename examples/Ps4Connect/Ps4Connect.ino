#include <PS4Controller.h>

void setup()
{
    Serial.begin(9600);
    PS4.begin("03:03:03:03:03:03");
    Serial.println("Ready.");
}

void loop()
{
  if (PS4.isConnected()){
    Serial.println("Connected!");
  }

  delay(3000);
}
