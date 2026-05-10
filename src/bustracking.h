#ifndef BUSTRACKING_H
#define BUSTRACKING_H

#include <stdint.h>
#include <stdbool.h>

// PIN DEFINITIONS
#define IR_SENSOR_ENTRANCE 13
#define IR_SENSOR_EXIT 12
#define GPS_RX 16
#define GPS_TX 17

#define HIGH 1
#define LOW  0

extern int passengerCount;
extern bool lastEntranceState;
extern bool lastExitState;

void system_setup(void);
void system_loop(void);
void handleSensors(void);

#endif // BUSTRACKING_H