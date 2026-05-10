#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

#define PI 3.14159265358979323846

// Station Structure
typedef struct {
    char name[50];
    double lat;
    double lon;
} Station;

// Route Map
Station route[] = {
    {"Piassa", 9.0312, 38.7525},
    {"Stadium", 9.0118, 38.7615},
    {"Bole", 8.9882, 38.7891}
};
int totalStations = 3;
int currentStationIndex = 0;

// Simulated system state
int passengerCount = 0;
int maxCapacity = 30;
double latitude = 9.0312; // Starting at Piassa
double longitude = 38.7525;
double currentSpeedKmH = 40.0; // Simulated speed in km/h

// Convert degrees to radians
double toRadians(double degree) {
    return degree * PI / 180.0;
}

// Calculate distance between two points in km (Haversine formula)
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371.0; // Earth radius in km
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);
    
    double a = sin(dLat/2) * sin(dLat/2) +
               cos(toRadians(lat1)) * cos(toRadians(lat2)) *
               sin(dLon/2) * sin(dLon/2);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));
    return R * c;
}

// Calculate ETA in minutes
double calculateETA(double distanceKm, double speedKmH) {
    if (speedKmH <= 0) return 0;
    return (distanceKm / speedKmH) * 60.0; // Return in minutes
}

// Update mock GPS location to move towards the next station
void update_gps() {
    if (currentStationIndex < totalStations - 1) {
        Station next = route[currentStationIndex + 1];
        
        // Very basic movement interpolation
        if (latitude < next.lat) latitude += 0.0002;
        else if (latitude > next.lat) latitude -= 0.0002;
        if (longitude < next.lon) longitude += 0.0002;
        else if (longitude > next.lon) longitude -= 0.0002;
        
        // If close enough, increment station
        if (fabs(latitude - next.lat) < 0.0005 && fabs(longitude - next.lon) < 0.0005) {
            currentStationIndex++;
        }
    }
}

// Display visualization
void render_dashboard() {
    // Clear terminal (Unix/Windows)
    system("clear || cls");
    
    printf("==========================================\n");
    printf(" SMART BUS TRACKING & CROWDING PREDICTION \n");
    printf("==========================================\n\n");
    
    printf("  [ GPS & ROUTE INFO ]\n");
    printf("  -> Latitude : %f\n", latitude);
    printf("  -> Longitude: %f\n\n", longitude);
    
    if (currentStationIndex < totalStations - 1) {
        Station next = route[currentStationIndex + 1];
        double dist = calculateDistance(latitude, longitude, next.lat, next.lon);
        double eta = calculateETA(dist, currentSpeedKmH);
        
        printf("  -> Next Station: %s\n", next.name);
        printf("  -> Remaining Distance: %.2f km\n", dist);
        printf("  -> Current Speed: %.1f km/h\n", currentSpeedKmH);
        printf("  -> \033[0;36mEstimated Time of Arrival (ETA): %.1f mins\033[0m\n\n", eta);
    } else {
        printf("  -> \033[0;32mArrived at Final Station: %s\033[0m\n\n", route[currentStationIndex].name);
    }
    
    printf("  [ PASSENGER DATA ]\n");
    printf("  -> Current Count: %d / %d\n", passengerCount, maxCapacity);
    
    printf("  -> Crowding Level: ");
    if (passengerCount < maxCapacity * 0.5) {
        printf("\033[0;32mLOW (GREEN LED)\033[0m\n"); // Green
    } else if (passengerCount < maxCapacity * 0.8) {
        printf("\033[0;33mMEDIUM (YELLOW LED)\033[0m\n"); // Yellow
    } else {
        printf("\033[0;31mHIGH (RED LED)\033[0m\n"); // Red
    }
    
    printf("\n==========================================\n");
    printf("Controls: \n[e] Passenger Enters \n[x] Passenger Exits \n[q] Quit\n");
    printf("Choice: ");
}

int main()
{
    char input;

    while (1)
    {
        render_dashboard();
        update_gps();

        // Wait for user input to simulate interrupt sensor triggers
        if (scanf(" %c", &input) == 1)
        {
            if (input == 'e' || input == 'E')
            {
                if (passengerCount < maxCapacity)
                    passengerCount++;
            }
            else if (input == 'x' || input == 'X')
            {
                if (passengerCount > 0)
                    passengerCount--;
            }
            else if (input == 'q' || input == 'Q')
            {
                printf("Exiting terminal simulation.\n");
                break;
            }
        }
    }

    return 0;
}
