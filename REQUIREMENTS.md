# System Requirements

## 1. General Objective

To develop a functional prototype embedded system for tracking public buses in real-time and predicting passenger crowding to improve urban transport efficiency.

## 2. Functional Requirements

- **Location Tracking:** The system must actively read GPS coordinates (Latitude and Longitude) at a regular interval.
- **Passenger Counting:** The system must detect passengers boarding and distinctively detect passengers disembarking.
- **Crowd Level Calculation:** The system must calculate the live occupancy and classify it (e.g., Low, Medium, High crowding).
- **Data Display:** The system must display the location and crowding level on an I2C LCD/OLED display.
- **Visual Status:** The system must visually show the crowding status using LEDs.
- **Transmission (Future):** The system must be capable of transmitting data via Wi-Fi to a remote server.

## 3. Non-Functional Requirements

- **Real-time Accuracy:** Passenger counts should update with minimum latency (less than 100ms).
- **Reliability:** Firmware must accurately parse NMEA strings without crashing on malformed serial data.
- **Portability:** System should be compact and capable of running off a 5V/3.3V battery bank.
- **Fault Tolerance:** Software must include debouncing for IR sensors so that a person pausing near the door isn't counted multiple times.

## 4. Hardware Requirements

- Microcontroller: ESP32-WROOM-32 (or similar)
- Positioning: NEO-6M GPS Module
- Sensors: 2x IR Proximity Sensors
- Output: 16x2 I2C Characters LCD or 0.96" OLED
- Indicators: Green, Yellow, and Red LEDs.

## 5. Software Requirements

- Environment: VS Code with PlatformIO (or Arduino IDE)
- Language: C / C++ standard
- Libraries: `TinyGPS++`, `LiquidCrystal_I2C`, Board manager for ESP32.
- Simulation: Proteus Design Suite / TinkerCad.
