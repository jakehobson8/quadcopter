#include <stdio.h>
#include "pico/stdlib.h"

#include "ping_sensor.h"


//#define PING_TEST
//#define IMU_TEST

// Main funciton
#if !defined(PING_TEST) && !defined(IMU_TEST)

int main()
{
    stdio_init_all();
    ping_sensor_init();

    while (true) {
        ;
    }
}
#endif // Main





// Ping Sensor Test Harness
#ifdef PING_TEST
int main()
{
    stdio_init_all();
    ping_sensor_init();

    while (true) {
        int distance = ping_sensor_read();
        printf("Distance: %d cm\n", distance);
        sleep_ms(100); // Wait for 1 second before the next reading
    }
}
#endif // PING_TEST
