#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <TinyGPS++.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "bustracking.h"

// FIREBASE & WIFI CREDENTIALS
#define WIFI_SSID "nonamecode"
#define WIFI_PASSWORD "0000000000000"

#define FIREBASE_HOST "your-project-id.firebaseio.com"
#define FIREBASE_AUTH " your-firebase-database-secret-or-legacy-token "

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

// Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

void push_to_firebase()
{
    int level = get_crowding_level();
    String crowdStr = (level == 0) ? "Low" : (level == 1) ? "Medium"
                                                        : "High";

    // Set integer, float, and string values directly to Firebase Realtime Database
    Firebase.setInt(fbdo, "/bus/passengers", get_passenger_count());
    Firebase.setString(fbdo, "/bus/current_location", get_current_location_name());
    Firebase.setString(fbdo, "/bus/next_station", get_next_station_name());
    Firebase.setDouble(fbdo, "/bus/remaining_distance", get_remaining_distance());
    Firebase.setDouble(fbdo, "/bus/eta_minutes", get_current_eta());
    Firebase.setString(fbdo, "/bus/crowding_state", crowdStr);

    Firebase.setDouble(fbdo, "/bus/gps/lat", get_current_lat());
    Firebase.setDouble(fbdo, "/bus/gps/lng", get_current_lon());
}

void setup()
{
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
    lcd.setCursor(0, 1);
    lcd.print("Connecting WiFi.");

    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected.");

    // Initialize Firebase
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Configure Firebase timeout and retries
    Firebase.setReadTimeout(fbdo, 1000 * 60);
    Firebase.setwriteSizeLimit(fbdo, "tiny");

    lcd.clear();

    // Initialize backend logic components
    init_bus_state(30);
    init_route_system();

    Serial.println("System Initialized for ESP32 and Firebase.");
}

void update_hardware_displays()
{
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
    if (level == 0)
        lcd.print("LOW ");
    else if (level == 1)
        lcd.print("MED ");
    else
        lcd.print("HIGH");

    lcd.setCursor(0, 1);
    if (!completed)
    {
        lcd.printf("ETA: %02.0fmin %.1fkm", eta, dist);
    }
    else
    {
        lcd.print("Final Dest reach");
    }
}

void loop()
{
    // 1. INPUT: Feed Hardware Sensors to C-Logic Module
    bool currentEntrance = digitalRead(IR_SENSOR_ENTRANCE);
    bool currentExit = digitalRead(IR_SENSOR_EXIT);
    process_sensor_input(currentEntrance, currentExit);

    // 2. INPUT: Feed GPS string directly
    while (SerialGPS.available() > 0)
    {
        gps.encode(SerialGPS.read());
    }

    // 3. PROCESSING: Periodic Checks (1 second loops)
    static unsigned long lastCheck = 0;
    if (millis() - lastCheck > 1000)
    {
        lastCheck = millis();

        // Feed GPS map data into tracking module
        if (gps.location.isValid())
        {
            process_gps_update(gps.location.lat(), gps.location.lng(), gps.speed.kmph());
        }

        // Apply visual updates directly to hardware
        update_hardware_displays();

        // Push exact live data to Firebase
        if (Firebase.ready())
        {
            push_to_firebase();
        }
    }
}
