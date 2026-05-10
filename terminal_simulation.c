#include <stdio.h>
#include <stdlib.h>
#include "src/bustracking.h"

// Mock GPS coordinates (starting slightly before Piassa)
double sim_lat = 9.0305;
double sim_lon = 38.7510;
double sim_speed = 40.0; // 40 km/h

void render_dashboard() {
    system("clear || cls");
    
    printf("==========================================\n");
    printf(" SMART BUS TRACKING LOGIC TERMINAL SIMULATOR \n");
    printf("==========================================\n\n");
    
    printf("  [ CURRENT POSITION ]\n");
    printf("  -> Zone / Location: %s\n", get_current_location_name());
    printf("  -> Raw GPS Lat    : %.5f\n", get_current_lat());
    printf("  -> Raw GPS Lon    : %.5f\n\n", get_current_lon());

    printf("  [ TARGET ROUTE ]\n");
    printf("  -> Next Station: %s\n", get_next_station_name());
    printf("  -> Distance remaining: %.2f km\n", get_remaining_distance());
    printf("  -> ETA: %.1f minutes\n", get_current_eta());
    if (is_route_completed()) {
        printf("  -> \033[0;32mArrived at Final Destination!\033[0m\n");
    }
    
    printf("\n  [ PASSENGER SYSTEM ]\n");
    int count = get_passenger_count();
    int level = get_crowding_level();
    
    printf("  -> Current Count: %d / 30\n", count);
    printf("  -> Crowding Level: ");
    if (level == 0) printf("\033[0;32mLOW (GREEN LED)\033[0m\n");
    else if (level == 1) printf("\033[0;33mMEDIUM (YELLOW LED)\033[0m\n");
    else printf("\033[0;31mHIGH (RED LED)\033[0m\n");
    
    printf("\n==========================================\n");
    printf("Controls: \n");
    printf("[e] Simulate Passenger Entry\n[x] Simulate Passenger Exit\n");
    printf("[w] Drive Bus Forward (Update GPS)\n[q] Quit\n");
    printf("Choice: ");
}

int main() {
    init_bus_state(30); 
    init_route_system();
    process_gps_update(sim_lat, sim_lon, sim_speed);

    char input;
    while (1) {
        render_dashboard();
        
        if (scanf(" %c", &input) == 1) {
            if (input == 'e' || input == 'E') {
                process_sensor_input(false, true); 
                process_sensor_input(true, true);  
            } 
            else if (input == 'x' || input == 'X') {
                process_sensor_input(true, false); 
                process_sensor_input(true, true);  
            } 
            else if (input == 'w' || input == 'W') {
                // Move coords towards Stadium/Bole
                sim_lat -= 0.002; 
                sim_lon += 0.001;
                process_gps_update(sim_lat, sim_lon, sim_speed);
            } 
            else if (input == 'q' || input == 'Q') break;
        }
    }

    return 0;
}
