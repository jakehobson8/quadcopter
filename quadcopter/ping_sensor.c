#include <stdio.h>
#include "ping_sensor.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Global variables to track the state of the ping sensor
volatile static int stateTimer = 0; 
volatile static int pulseHigh, pulseLow, pulseWidth;
volatile static uint8_t state = 0; 

struct repeating_timer timer;

#define TIMER_IRQ_0 0

// Interrupt Service Routines for the ping sensor
bool ping_sensor_timer_callback(struct repeating_timer *timer) {
    // Handle the interrupt for the ping sensor
    if (state == 0) {
        // Trigger the ping sensor by sending a pulse
        gpio_put(0, 1); // Set trigger pin high

        // Wait for 10 microseconds
        if (stateTimer >= 1) {
            state = 1; // Change state to waiting for echo
            stateTimer = 0; // Reset the timer
        } else {
            stateTimer++; // Increment the timer
        }


    } else if (state == 1) {
        gpio_put(0, 0); // Set trigger pin low
        
        // Wait 60ms to send another pulse
        if (stateTimer >= 6000) {
            state = 0; // Change state back to idle
            stateTimer = 0; // Reset the timer
        } else {
            stateTimer++; // Increment the timer
        }
    }
}

void ping_sensor_echo_callback(uint gpio, uint32_t events) {
    // Handle the echo pin interrupt
    if (gpio_get(1) == 1) {
        // Echo pin went high, start timing
        pulseHigh = time_us_32();
    } else {
        // Echo pin went low, stop timing
        pulseLow = time_us_32();
        pulseWidth = pulseLow - pulseHigh; // Calculate pulse width
    }
}



void ping_sensor_init(void) {
    // Initialize the ping sensor (e.g., set up GPIO pins)
    gpio_init(0); // Trigger pin initialization
    gpio_set_dir(0, GPIO_OUT); // Set GPIO as output for trigger
    gpio_init(1); // Echo pin initialization    
    gpio_set_dir(1, GPIO_IN); // Set GPIO as input for echo
    gpio_pull_up(1); // Enable pull-up resistor for echo pin

    // Set up a timer interrupt for the ping sensor
    add_repeating_timer_us(-10, ping_sensor_timer_callback, NULL,  &timer); // 1us timer for triggering the sensor

    // Set up an interrupt for the echo pin
    gpio_set_irq_enabled_with_callback(1, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &ping_sensor_echo_callback);
}


int ping_sensor_read(void) {
    // Convert duration to distance (in cm)
    int distance = (int)(pulseWidth / 58); // Speed of sound is approximately 340 m/s, so 340 / 2 = 170 cm/ms, and 170 * 1000 = 170000 cm/s
    return distance; // Return the calculated distance
}