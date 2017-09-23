#include <Arduino.h>

#include "pins.h"
#include "wifi.h"
#include "config.h"

#include <FastLED.h>

const uint8_t NUM_LEDS = 1;

enum class Status {
    STARTING,
    CONNECTING_WIFI,
    CONNECTED,
    CONNECTING_FAILED,
};

CRGB leds[NUM_LEDS];
Status status = Status::STARTING;

void update_led(Status status) {
    switch(status) {
    case Status::STARTING:
        leds[0] = CRGB::White;
        break;
    case Status::CONNECTING_WIFI:
        leds[0] = CRGB::Blue;
        break;
    case Status::CONNECTED:
        leds[0] = CRGB::Green;
        break;
    case Status::CONNECTING_FAILED:
        leds[0] = CRGB::Red;
        break;
    }

    FastLED.show();
}



void connect_to_wifi() {
    auto command = wifi_connect_cmd(WIFI_SSID, WIFI_PASS);

    Serial.println(command.get());
    Serial2.println(command.get());

    status = Status::CONNECTING_WIFI;
    update_led(status);

    AtStatusListener sl;
    while(sl.current_status() == AtStatus::WAITING) {
        while(Serial2.available() == 0) {}

        char received = Serial2.read();
        sl.receive_char(received);

        Serial.write(received);
    }

    if(sl.current_status() == AtStatus::OK) {
        status = Status::CONNECTED;
    }
    else {
        status = Status::CONNECTING_FAILED;
    }
}

float messure_distance() {
    digitalWrite(ULTRASONIC_PIN, HIGH);

    delayMicroseconds(5);
    digitalWrite(ULTRASONIC_PIN, LOW);

    pinMode(ULTRASONIC_PIN, INPUT);

    auto start = micros();
    while (digitalRead(ULTRASONIC_PIN) == LOW) {
        if(micros() - start > 1000) {
            return -1;
        }
    }

    start = micros();

    while(digitalRead(ULTRASONIC_PIN) == HIGH) {
        if(micros() - start > 20000) {
            return -1;
        }
    }

    auto travel_time = micros() - start;

    return travel_time;
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(ULTRASONIC_PIN, OUTPUT);
    digitalWrite(ULTRASONIC_PIN, LOW);

    FastLED.addLeds<NEOPIXEL, LED_DATA_PIN>(leds, NUM_LEDS);

    update_led(status);


    Serial.begin(9600);

    Serial2.begin(115200);

    connect_to_wifi();

    update_led(status);
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

    Serial.println(messure_distance());
}

