#ifndef BUSTRACKING_H
#define BUSTRACKING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Station definition
typedef struct {
    const char* name;
    double latitude;
    double longitude;
} Station;

// PASSENGER LOGIC
void init_bus_state(int max_cap);
void process_sensor_input(bool currentEntrance, bool currentExit);
int get_passenger_count(void);
int get_crowding_level(void); // 0=Green, 1=Yellow, 2=Red

// ROUTE & ETA LOGIC
void init_route_system(void);
void process_gps_update(double lat, double lon, double speed);

// Position Tracking
const char* get_current_location_name(void);
double get_current_lat(void);
double get_current_lon(void);

// Target Tracking
const char* get_next_station_name(void);
double get_remaining_distance(void);
double get_current_eta(void);
bool is_route_completed(void);

// Math calculations
double calculate_distance(double lat1, double lon1, double lat2, double lon2);
double calculate_eta(double distanceKm, double speedKmH);

#ifdef __cplusplus
}
#endif

#endif // BUSTRACKING_H
