#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#include "ping_sensor.h"


int main()
{
    stdio_init_all();
    ping_sensor_init();

    while (true) {
        int distance = ping_sensor_read();
        printf("Distance: %d cm\n", distance);
        sleep_ms(1000);
    }
}
