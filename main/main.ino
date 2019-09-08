/*

  Main module

  Copyright (C) 2019 by Roel van Wanrooy (www.connectix.nl)

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "configuration.h"
#include "rom/rtc.h"
#include <Wire.h>

bool ssd1306_found = false;
bool relais_on = false;

// Message counter, stored in RTC memory, survives deep sleep
RTC_DATA_ATTR uint32_t count = 0;

//enter the length of the payload in bytes (this has to be more than 3 if you want to receive downlinks)
static uint8_t txBuffer[3];

//downlink payload
static uint8_t rxBuffer[1];

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------

void send() {
  buildPacket(txBuffer);

#if LORAWAN_CONFIRMED_EVERY > 0
  bool confirmed = (count % LORAWAN_CONFIRMED_EVERY == 0);
#else
  bool confirmed = false;
#endif

  ttn_cnt(count);

// Blink led while sending  
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
  delay(100);
  digitalWrite(LED_PIN, HIGH);
  delay(50);
  digitalWrite(LED_PIN, LOW);
  
  ttn_send(txBuffer, sizeof(txBuffer), LORAWAN_PORT, confirmed);
  
// send count plus one 
  count++;
}

void sleep() {
#if SLEEP_BETWEEN_MESSAGES == 1

  // Show the going to sleep message on the screen
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "Sleeping in %3.1fs\n", (MESSAGE_TO_SLEEP_DELAY / 1000.0));
  screen_print(buffer);

  // Wait for MESSAGE_TO_SLEEP_DELAY millis to sleep
  delay(MESSAGE_TO_SLEEP_DELAY);

  // Turn off screen
  screen_off();

  // We sleep for the interval between messages minus the current millis
  // this way we distribute the messages evenly every SEND_INTERVAL millis
  uint32_t sleep_for = (millis() < SEND_INTERVAL) ? SEND_INTERVAL - millis() : SEND_INTERVAL;
  sleep_millis(sleep_for);

#endif
}

void callback(uint8_t message) {
  if (EV_JOINING == message) screen_print("Joining TTN...\n") , Serial.println("Joining TTN...\n");
  if (EV_JOINED == message) screen_print("TTN joined!\n") , Serial.println("TTN joined!\n");
  if (EV_JOIN_FAILED == message) screen_print("TTN join failed\n") , Serial.println("TTN join failed\n");
  if (EV_REJOIN_FAILED == message) screen_print("TTN rejoin failed\n") , Serial.println("TTN rejoin failed\n");
  if (EV_RESET == message) screen_print("Reset TTN connection\n") , Serial.println("Reset TTN connection\n");
  if (EV_LINK_DEAD == message) screen_print("TTN link dead\n") , Serial.println("TTN link dead\n");
  if (EV_ACK == message) screen_print("ACK received\n") , Serial.println("ACK received\n");
  if (EV_PENDING == message) screen_print("Message discarded\n") , Serial.println("Message discarded\n");
  if (EV_QUEUED == message) screen_print("Message queued\n") , Serial.println("Message queued\n");

  if (EV_TXCOMPLETE == message) {
    screen_print("Message sent\n") , Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
    if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
        for (int i = 0; i < LMIC.dataLen; i++) {
          if (LMIC.frame[LMIC.dataBeg + i] < 0x10) {
            Serial.print(F("0"));
        }
        Serial.print(F("Received payload: "));
        Serial.print(LMIC.frame[LMIC.dataBeg + i], HEX);
    }
    Serial.println();

// downlink (turn relais on when received payload = 1)
  if (LMIC.frame[LMIC.dataBeg] == 1)
  {
    digitalWrite(RELAIS_PIN, HIGH);
    Serial.println("RELAIS ON");
    relais_on = true;
  }
  else
  {
    digitalWrite(RELAIS_PIN, LOW);
    Serial.println("RELAIS OFF");
    relais_on = false;
  }
  }
    sleep();
  }

  if (EV_RESPONSE == message) {

    screen_print("[TTN] Response: ");

    size_t len = ttn_response_len();
    uint8_t data[len];
    ttn_response(data, len);

    char buffer[6];
    for (uint8_t i = 0; i < len; i++) {
      snprintf(buffer, sizeof(buffer), "%02X", data[i]);
      screen_print(buffer);
    }
    screen_print("\n");
  }
}

uint32_t get_count() {
  return count;
}

// scan I2C bus for devices like ssd1306 oled
void scanI2Cdevice(void)
{
    byte err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        err = Wire.endTransmission();
        if (err == 0) {
            Serial.print("I2C device found at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.print(addr, HEX);
            Serial.println(" !");
            nDevices++;

            if (addr == SSD1306_ADDRESS) {
                ssd1306_found = true;
                Serial.println("ssd1306 display found");
            }
        } else if (err == 4) {
            Serial.print("Unknow error at address 0x");
            if (addr < 16)
                Serial.print("0");
            Serial.println(addr, HEX);
        }
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

void setup() {
// Debug
  #ifdef DEBUG_PORT
  DEBUG_PORT.begin(SERIAL_BAUD);
  #endif

//  delay(1000);

// Scan for I2C devices
  Wire.begin(I2C_SDA, I2C_SCL);
  scanI2Cdevice();

// SET BUILT-IN LED TO OUTPUT
  pinMode(LED_PIN, OUTPUT);

// SET RELAIS_PIN TO OUTPUT
  pinMode(RELAIS_PIN, OUTPUT);

// Display
  screen_setup();

// Show logo on first boot
  if (0 == count) {
    screen_print(APP_NAME " " APP_VERSION, 0, 0 );
    screen_show_logo();
    screen_update();
    delay(LOGO_DELAY);
}

// TTN setup
   if (!ttn_setup()) {
   screen_print("[ERR] Radio module not found!\n");
   delay(MESSAGE_TO_SLEEP_DELAY);
   screen_off();
   sleep_forever();
}

  ttn_register(callback);
  ttn_join();
  ttn_sf(LORAWAN_SF);
  ttn_adr(LORAWAN_ADR);
}

void loop() {
  ttn_loop();
  screen_loop();

// Send every SEND_INTERVAL millis
  static uint32_t last = 0;
  static bool first = true;
  if (0 == last || millis() - last > SEND_INTERVAL) {
      last = millis();
      first = false;
      Serial.println("TRANSMITTING");
      send();
  }
}
