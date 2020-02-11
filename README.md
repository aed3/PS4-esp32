# PS4-esp32
Use a PS4 controller with an esp32

This is heavily based on the work of Jeffery Pernis to connect a PS3 controller to an esp32.
You can find that here: https://github.com/jvpernis/esp32-ps3

This repo can be downloaded as a zip file and imported into the Arduino IDE as a library.
The instructions on how to do this and the library for PS3 controllers can be found here: https://github.com/jvpernis/esp32-ps3/issues/3#issuecomment-517141523

## Pairing the PS4 Controller:
When a PS4 controller is 'paired' to a PS4 console, it just means that it has stored the console's Bluetooth MAC address, which is the only device the controller will connect to. Usually, this pairing happens when you connect the controller to the PS4 console using a USB cable, and press the PS button. This initiates writing the console's MAC address to the controller.

Therefore, if you want to connect your PS4 controller to the ESP32, you either need to figure out what the Bluetooth MAC address of your PS4 console is and set the ESP32's address to it, or change the MAC address stored in the PS4 controller.

Whichever path you choose, you're going to need a tool to read and/or write the currently paired MAC address from the PS4 controller. I used [SixaxisPairTool](https://dancingpixelstudios.com/sixaxis-controller/sixaxispairtool/) for this, but you can try using [sixaxispairer](https://github.com/user-none/sixaxispairer) as well, if open source is important to you.

If you opted to change the ESP32's MAC address, you'll need to include the ip address in the ```PS4.begin()``` function during within the ```setup()``` Arduino function like below where ```03:03:03:03:03:03``` is the MAC address:
```
void setup()
{
    Serial.begin(9600);
    PS4.begin("03:03:03:03:03:03");
    Serial.println("Ready.");
}
```
