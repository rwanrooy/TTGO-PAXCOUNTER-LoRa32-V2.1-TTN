# TTGO-LoRa32-V2.1-TTN (version T3_v1.6 20180606)

TTGO LoRa32 V2.1 Sketch for TTN (The Things Network)

![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/ttgo%20lora32%20board.png)

This sketch has al the needed components to connect the board to The Things Network. I found a few sketches online based on the paxcounter but I didn't find them lean and mean. This sketch has also support for the oled display wich you can adjust to your needs.


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

The sketch is configured to receive downlink messages. You can use the TTN console to send a payload to the board. The payload will be scheduled and send when the board sends the first new uplink message to TTN. The sketch is configured to blink the green led when it receives a payload of 01. If you send another payload than 01 the led will turn off. You can assign another DIO pin as an output to a relays to turn on a motor or another device of choice.

You can use the sensor tab to fill the payload with data of a sensor of your own. The payload is now set to 11 for testing purposes.

Work is still in progress and testing has to be done.

![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/pin%20schematic.png)
![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/board%20versions.png)
![alt text](https://github.com/rwanrooy/TTGO-LoRa32-V2.1-TTN/blob/master/img/pin%20schematic%20v1.5%20and%201.6.png)
