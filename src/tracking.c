#include "bustracking.h"
#include <stdio.h>
#include "driver/uart.h"
#include "driver/gpio.h"

int passengerCount = 0;
bool lastEntranceState = HIGH;
bool lastExitState = HIGH;

uint32_t get_millis(void)
{
    return 0;
}

void system_setup(void)
{
    printf("System Initializing...\n");
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        // .source_clk = UART_SCLK_DEFAULT,
    };
}

void system_loop(void)
{
    static uint32_t lastCheck = 0;
    uint32_t currentMillis = get_millis();

    if ((currentMillis - lastCheck) > 5000)
    {
        printf("Checkup: System is running. Waiting for sensors...\n");
        lastCheck = currentMillis;
    }
}

void handleSensors(void)
{
    bool currentEntrance = LOW;
    bool currentExit = LOW;
    // 

}

int main(void) {
    system_setup();
    while (1) {
        system_loop();
        handleSensors();
    }
    return 0;
}