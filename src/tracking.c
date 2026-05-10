#include "bustracking.h"
#include <math.h>

#define PI 3.14159265358979323846
#define EARTH_RADIUS_KM 6371.0

// STATE VARIABLES
static int current_passengers = 0;
static int max_capacity = 30;
static bool last_entrance_state = true;
static bool last_exit_state = true;

#define TOTAL_STATIONS 3
static Station route[TOTAL_STATIONS] = {
    {"Piassa", 9.0312, 38.7525},
    {"Stadium", 9.0118, 38.7615},
    {"Bole", 8.9882, 38.7891}
};

static int current_station_index = 0;
static double current_distance = 0.0;
static double current_eta = 0.0;
static double current_lat = 0.0;
static double current_lon = 0.0;


// PASSENGER LOGIC
void init_bus_state(int max_cap) {
    max_capacity = max_cap;
    current_passengers = 0;
}

void process_sensor_input(bool currentEntrance, bool currentExit) {
    if (last_entrance_state == true && currentEntrance == false) {
        if (current_passengers < max_capacity) current_passengers++;
    }
    last_entrance_state = currentEntrance;

    if (last_exit_state == true && currentExit == false) {
        if (current_passengers > 0) current_passengers--;
    }
    last_exit_state = currentExit;
}

int get_passenger_count(void) { return current_passengers; }

int get_crowding_level(void) {
    if (current_passengers < (max_capacity * 0.5)) return 0;
    if (current_passengers < (max_capacity * 0.8)) return 1;
    return 2;
}

// MATH CALCS
static double to_radians(double degree) {
    return degree * PI / 180.0;
}

double calculate_distance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = to_radians(lat2 - lat1);
    double dLon = to_radians(lon2 - lon1);
    
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(to_radians(lat1)) * cos(to_radians(lat2)) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return EARTH_RADIUS_KM * c;
}

double calculate_eta(double distanceKm, double speedKmH) {
    if (speedKmH <= 0) return 0;
    return (distanceKm / speedKmH) * 60.0; 
}


// ROUTE LOGIC
void init_route_system(void) {
    current_station_index = 0;
    current_distance = 0.0;
    current_eta = 0.0;
    current_lat = 0.0;
    current_lon = 0.0;
}

void process_gps_update(double lat, double lon, double speed) {
    current_lat = lat;
    current_lon = lon;
    
    if (current_station_index < TOTAL_STATIONS - 1) {
        Station next = route[current_station_index + 1];
        
        current_distance = calculate_distance(lat, lon, next.latitude, next.longitude);
        current_eta = calculate_eta(current_distance, speed);
        
        if (current_distance < 0.05) { 
            current_station_index++;
        }
    }
}

double get_current_lat(void) { return current_lat; }
double get_current_lon(void) { return current_lon; }

const char* get_current_location_name(void) {
    // If the bus is within 200m (0.2km) of ANY station, it assumes it is "At [Station]"
    for (int i = 0; i < TOTAL_STATIONS; i++) {
        if (calculate_distance(current_lat, current_lon, route[i].latitude, route[i].longitude) <= 0.2) {
            return route[i].name; 
        }
    }
    return "In Transit";
}

const char* get_next_station_name(void) {
    if (current_station_index < TOTAL_STATIONS - 1) {
        return route[current_station_index + 1].name;
    }
    return "Final Destination";
}

double get_remaining_distance(void) { return current_distance; }
double get_current_eta(void) { return current_eta; }
bool is_route_completed(void) { return current_station_index >= TOTAL_STATIONS - 1; }
