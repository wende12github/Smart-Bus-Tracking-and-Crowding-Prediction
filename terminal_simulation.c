#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP(ms) usleep((ms) * 1000)
#endif

// Simulated system state
int passengerCount = 0;
int maxCapacity = 30;
double latitude = 9.0300;
double longitude = 38.7400;

// Update mock GPS location
void update_gps()
{
    latitude += 0.0001;
    longitude += 0.0001;
}

// Display visualization
void render_dashboard()
{
    // Clear terminal (Unix/Windows)
    system("clear || cls");

    printf("==========================================\n");
    printf(" SMART BUS TRACKING & CROWDING PREDICTION \n");
    printf("==========================================\n\n");

    printf("  [ GPS LOCATION ]\n");
    printf("  -> Latitude : %f\n", latitude);
    printf("  -> Longitude: %f\n\n", longitude);

    printf("  [ PASSENGER DATA ]\n");
    printf("  -> Current Count: %d / %d\n", passengerCount, maxCapacity);

    printf("  -> Crowding Level: ");
    if (passengerCount < maxCapacity * 0.5)
    {
        printf("\033[0;32mLOW (GREEN LED)\033[0m\n"); // Green
    }
    else if (passengerCount < maxCapacity * 0.8)
    {
        printf("\033[0;33mMEDIUM (YELLOW LED)\033[0m\n"); // Yellow
    }
    else
    {
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