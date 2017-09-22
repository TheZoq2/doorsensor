#include <Arduino.h>

#include "pins.h"
#include "wifi.h"
#include "config.h"

void connect_to_wifi() {
    auto command = wifi_connect_cmd(WIFI_SSID, WIFI_PASS);

    Serial.println(command.get());
    Serial2.println(command.get());
}

void setup() {
    pinMode(LED_PIN, INPUT);

    Serial.begin(9600);

    Serial2.begin(115200);

    while(Serial.available() == 0) {

    }
    connect_to_wifi();

    digitalWrite(LED_PIN, HIGH);
}

void loop() {
    while(Serial.available() != 0) {
        auto incomming = Serial.read();

        if (incomming == '\r') {
            Serial2.write('\r');
            Serial2.write('\n');
        }
        else {
            Serial2.write(incomming);
        }
    }

    while(Serial2.available() != 0) {
        auto incomming = Serial2.read();

        Serial.write(incomming);
    }
}

