#include <Arduino.h>
#include <TinyGPS++.h>

// PIN DEFINITIONS
#define IR_SENSOR_ENTRANCE 13
#define IR_SENSOR_EXIT 12
#define GPS_RX 16
#define GPS_TX 17

// OBJECTS
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);

// VARIABLES
int passengerCount = 0;
bool lastEntranceState = HIGH;
bool lastExitState = HIGH;

void setup() {
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
    
    pinMode(IR_SENSOR_ENTRANCE, INPUT_PULLUP);
    pinMode(IR_SENSOR_EXIT, INPUT_PULLUP);

    Serial.println("System Initializing...");
}

void loop() {
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 5000) {
        Serial.println("Checkup: System is running. Waiting for sensors...");
        lastCheck = millis();
    }
}

void handleSensors() {
    bool currentEntrance = digitalRead(IR_SENSOR_ENTRANCE);
    bool currentExit = digitalRead(IR_SENSOR_EXIT);
    
}