#include <Arduino.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "bustracking.h"

// PIN DEFINITIONS (ESP32)
#define IR_SENSOR_ENTRANCE 13
#define IR_SENSOR_EXIT 12
#define GPS_RX 16
#define GPS_TX 17

#define LED_GREEN 25
#define LED_YELLOW 26
#define LED_RED 27

// HARDWARE OBJECTS
TinyGPSPlus gps;
HardwareSerial SerialGPS(2);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
    
    pinMode(IR_SENSOR_ENTRANCE, INPUT_PULLUP);
    pinMode(IR_SENSOR_EXIT, INPUT_PULLUP);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_RED, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Smart Bus System");
    delay(2000);
    lcd.clear();

    // Initialize backend logic components
    init_bus_state(30); 
    init_route_system(); 

    Serial.println("System Initialized for ESP32.");
}

void update_hardware_displays() {
    // Fetch Backend Data
    int count = get_passenger_count();
    int level = get_crowding_level();
    double dist = get_remaining_distance();
    double eta = get_current_eta();
    bool completed = is_route_completed();

    // Hardware LEDs
    digitalWrite(LED_GREEN, level == 0 ? HIGH : LOW);
    digitalWrite(LED_YELLOW, level == 1 ? HIGH : LOW);
    digitalWrite(LED_RED, level == 2 ? HIGH : LOW);

    // LCD Screen
    lcd.setCursor(0, 0);
    lcd.printf("Pax:%02d/30 ", count);
    if (level == 0) lcd.print("LOW ");
    else if (level == 1) lcd.print("MED ");
    else lcd.print("HIGH");

    lcd.setCursor(0, 1);
    if (!completed) {
        lcd.printf("ETA: %02.0fmin %.1fkm", eta, dist);
    } else {
        lcd.print("Final Dest reach");
    }
}

void loop() {
    // INPUT: Feed Hardware Sensors to C-Logic Module
    bool currentEntrance = digitalRead(IR_SENSOR_ENTRANCE);
    bool currentExit = digitalRead(IR_SENSOR_EXIT);
    process_sensor_input(currentEntrance, currentExit);

    // INPUT: Feed GPS string directly
    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }

    // PROCESSING: Periodic Checks (1 second loops)
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 1000) {
        lastCheck = millis();

        // Feed GPS Data mapping into the tracking backend
        if (gps.location.isValid()) {
            process_gps_update(gps.location.lat(), gps.location.lng(), gps.speed.kmph());
        }

        // Apply physical updates to User Interfaces
        update_hardware_displays();
        
        // Debugging prints every 5 seconds
        if ((millis() / 5000) % 2 == 0) {
            Serial.printf("Loc[%s] Tgt: %s | Dist: %.2f km | ETA: %.1f mins | Pax: %d\n", 
                gps.location.isValid() ? "LOCKED" : "WAIT",
                get_next_station_name(), 
                get_remaining_distance(), 
                get_current_eta(),
                get_passenger_count()
            );
        }
    }
}
