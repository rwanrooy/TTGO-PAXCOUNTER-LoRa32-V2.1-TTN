# TTGO-LoRa32-V2.1-TTN (version T3_v1.6 20180606)

TTGO LoRa32 V2.1 Sketch for TTN (The Things Network) with downlink control for turning a relais off or on.

![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/ttgo%20lora32%20board.png)

This sketch has al the needed components to connect the board to The Things Network. I found a few sketches online based on the paxcounter but I didn't find them lean and mean. This sketch has also support for the oled display wich you can adjust to your needs.

You can turn a relais on or off by sending downlink messages in the TTN Console. I added some payload formats (Decider, Converter, Validator and Encoder) to make the payload more easy to read. This is also necessary if you want to control the relais with Node-Red.

Downlink confirmation:
The sketch confirms the state of the relais with uplink messages so you are always sure the message is transmitted to the node. I will add some node red instructions later when I have more time.


Setup
Follow the directions at espressif/arduino-esp32 to install the board to the Arduino IDE and use board 'TTGO LoRa32-OLED V1'.

Open this project file main/main.ino with the Arduino IDE and upload it to your TTGO LoRa 32 board. This sketch is made for the latest version. Older versions are not tested and probably will not work!

Install the Arduino IDE libraries:
- mcci-catena/arduino-lmic (for Rev0 and Rev1)
- ThingPulse/esp8266-oled-ssd1306 (for Rev0 and Rev1)

- Copy the contents of the project file main/lmic_project_config.h to the library file arduino-lmic/project_config/lmic_project_config.h and uncomment the proper frequency for your region. The sketch does always look at the library folder for the configured region!!!

- Edit this project file main/configuration.h
  #define SLEEP_BETWEEN_MESSAGES  0           // Do sleep between messages set this to 1 to enable deep sleep
  #define SEND_INTERVAL           60000        // Sleep for these many millis (60000 = 60 seconds)
  #define MESSAGE_TO_SLEEP_DELAY  5000        // Time after message before going to sleep (screen turns off in 5 seconds)

- Edit this project file main/credentials.h to use either USE_ABP or USE_OTAA and add the Keys/EUIs for your Application's Device from The Things Network. It has LoRaWAN support fot OTAA and ABP activation. OTAA is more secure but will join slower. ABP is fast and will join instantly.

The default configuration will upload the payload every 10 minutes to the TTN Application console. You can adjust the payload to work with a sensor of choice. I will update the sketch with a temperature sensor in the near future.

You can connect a relais to pin 14 (I can confirm it works). You have to connect the 5V pin and the ground on the right side of the board (opposit side of pin 14). I did not get the left side of the voltage and ground to work with pin 14.


Work is still in progress and testing has to be done.

![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/pin%20schematic.png)
![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/board%20versions.png)
![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/pin%20schematic%20v1.5%20and%201.6.png)
