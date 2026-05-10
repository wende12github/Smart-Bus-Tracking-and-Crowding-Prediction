# Hardware Components Integration Guide

This document provides a comprehensive list of all hardware materials required for the Smart Bus Tracking and Crowding Prediction System. It includes their properties, functions, and exactly how they integrate with the main ESP32 controller.

---

### 1. ESP32 Microcontroller (ESP-WROOM-32)

- **Name**: ESP32 Development Board
- **Properties**: 32-bit dual-core processor, operates at 3.3V logic level. Has built-in Wi-Fi and Bluetooth capabilities, multiple hardware UARTs, and I2C buses.
- **Function**: The central "brain" of the project. It runs the C-logic state machine, calculates Haversine distances, manages the hardware interrupts, and prepares data for transmission.
- **Integration**: Acts as the main hub. All other sensors and modules connect directly to its GPIO pins.

### 2. NEO-6M GPS Module

- **Name**: U-blox NEO-6M GPS Receiver
- **Properties**: 9600 baud rate (default), requires 3.3V-5V power, outputs raw NMEA text strings over UART.
- **Function**: Communicates with satellites to provide real-time latitude, longitude, and speed of the bus.
- **Integration**: Connects to the ESP32 via Hardware UART2.
  - **TX Pin (GPS)** ➔ **RX Pin 16 (ESP32)**
  - **RX Pin (GPS)** ➔ **TX Pin 17 (ESP32)**

### 3. Infrared (IR) Proximity Sensors (x2)

- **Name**: Standard IR Obstacle Avoidance Modules
- **Properties**: Emits an IR beam and reads the reflection. Outputs a digital logic signal (HIGH when clear, LOW when blocked). Includes an onboard potentiometer to adjust range.
- **Function**: Placed at the bus doors. One acts as the "Entrance" trigger and the other as the "Exit" trigger to maintain an accurate internal tally of passengers.
- **Integration**: Connects to ESP32 GPIO pins configured as digital inputs.
  - **Entrance Sensor OUT** ➔ **Pin 13 (ESP32)**
  - **Exit Sensor OUT** ➔ **Pin 12 (ESP32)**

### 4. 16x2 I2C Display (LCD)

- **Name**: 16x2 Character LCD with I2C Backpack
- **Properties**: Displays 16 characters across 2 rows. The I2C backpack reduces the required wiring from 16 parallel pins down to just 4. Operates typically on the address `0x27`.
- **Function**: Visually displays the Passenger Count out of the Max Capacity, current ETA, remaining distance, and crowding level directly to the driver and boarding passengers.
- **Integration**: Connects to the standard I2C bus of the ESP32.
  - **SDA (Data)** ➔ **Pin 21 (ESP32)**
  - **SCL (Clock)** ➔ **Pin 22 (ESP32)**

### 5. Status Indicator LEDs (x3)

- **Name**: 5mm LEDs (Green, Yellow, Red)
- **Properties**: Standard light-emitting diodes. Require current-limiting resistors (e.g., 220Ω or 330Ω) to prevent burning out.
- **Function**: Provide a fast, at-a-glance visualization of the current passenger crowding index.
  - _Green_: Bus is mostly empty (Low crowding).
  - _Yellow_: Bus is getting full (Medium crowding).
  - _Red_: Bus is at maximum capacity (High crowding).
- **Integration**: Driven by standard ESP32 digital output pins.
  - **Green LED (+)** ➔ **Pin 25 (ESP32)**
  - **Yellow LED (+)** ➔ **Pin 26 (ESP32)**
  - **Red LED (+)** ➔ **Pin 27 (ESP32)**

### 6. Supporting Materials

- **Breadboard**: Used for temporary prototyping and testing without soldering.
- **Jumper Wires**: Male-to-Male and Male-to-Female ribbons for routing signals.
- **Resistors (330 Ohms)**: Essential for current-limiting on the LEDs to protect the ESP32 output pins.
- **Power Supply**: Standard 5V USB Power Bank or Li-Po battery pack to keep the system mobile inside the test bus.

---

### Overall Data Flow (Hardware level)

1. **[Satellites]** ➔ NEO-6M GPS ➔ (UART) ➔ **[ESP32]**
2. **[Human Boarding]** ➔ IR Sensors ➔ (Digital Pullups) ➔ **[ESP32]**
3. **[ESP32 Processing]** ➔ (I2C) ➔ **[16x2 Display]**
4. **[ESP32 Processing]** ➔ (Voltage HIGH/LOW) ➔ **[LEDs]**
