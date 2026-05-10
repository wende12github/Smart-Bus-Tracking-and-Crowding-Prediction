# Smart Bus Tracking and Crowding Prediction

## Components and Materials

Based on the project proposal and methodology, here are the required components, their functionality, and properties.

### 1. ESP32 Microcontroller

- **Functionality**: Serves as the central processing unit. It collects data from sensors, processes algorithms for passenger counting, processes NMEA GPS data, and transmits data wirelessly to a monitoring platform.
- **Properties**: Features built-in Wi-Fi and Bluetooth suitable for IoT. Multi-core processor, multiple GPIOs supporting Interrupts, UART, I2C, SPI.

### 2. Arduino Uno (Simulation Alternative)

- **Functionality**: Primarily used in TinkerCad simulation as a drop-in replacement where ESP32 is not natively supported. Acts as the core microcontroller for testing logic.
- **Properties**: ATmega328P based, 5V logic, no built-in Wi-Fi, suitable for breadboard prototyping.

### 3. NEO-6M GPS Module

- **Functionality**: Responsible for continuously tracking the geographical location (latitude and longitude) of the bus in real-time.
- **Properties**: High sensitivity, 9600 baud rate default, supports UART interface, outputs NMEA strings.

### 4. Infrared (IR) Sensors

- **Functionality**: Placed at entry and exit doors of the bus to detect passengers continuously. Used to implement bi-directional passenger counting logic.
- **Properties**: Emits and detects IR radiation, adjustable detection range, provides digital output (HIGH/LOW) when an obstacle is detected.

### 5. LCD / OLED Display

- **Functionality**: Installed inside the bus or driver's dashboard to display system status, current coordinate readings, and passenger count / crowding level.
- **Properties**: Uses I2C communication (SDA/SCL pins), typical size 16x2 for LCD or 0.96 inch for OLED.

### 6. LED Status Indicators

- **Functionality**: Used to provide immediate visual feedback regarding crowd levels (e.g., Green for Empty, Yellow for Getting Full, Red for Overcrowded).
- **Properties**: Low power consumption, operated via standard GPIO pins with current-limiting resistors.

### 7. Supplementary Components (Push Buttons, Breadboard, Potentiometer)

- **Functionality**: Prototyping materials. Buttons simulate IR sensors triggering in soft-simulations; breadboard allows wiring without soldering; potentiometer adjusts LCD contrast.
