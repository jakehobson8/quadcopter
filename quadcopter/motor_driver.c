#include "motor_driver.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define wrap_value 15000 // Define the wrap value for PWM

void motor_driver_init() {
    // Initialization code for motor driver
    gpio_set_function(Motor1, GPIO_FUNC_PWM); // Set GPIO 18 to PWM functionq

    uint slice_num = pwm_gpio_to_slice_num(Motor1);
    pwm_set_wrap(slice_num, wrap_value); // Set the PWM wrap value

    pwm_set_enabled(slice_num, true); // Enable the PWM slice

    pwm_set_gpio_level(Motor1, 0); // Set initial PWM level to 0
}

void motor_driver_set_speed(int motor_id, float speed) {
    // Code to set motor speed
    // Clamps speed to [0, 100]
    if (speed < 0.0f) {
        speed = 0.0f;
    } else if (speed > 100.0f) {
        speed = 100.0f;
    }

    // Set the PWM level based on the speed
    uint16_t level = (uint16_t)(speed * (wrap_value / 100.0f)); // Scale speed to PWM level (0-wrap_value)

    switch (motor_id) {
        case Motor1:
            pwm_set_gpio_level(Motor1, level);
            break;
        case Motor2:
            pwm_set_gpio_level(Motor2, level);
            break;
        case Motor3:
            pwm_set_gpio_level(Motor3, level);
            break;
        case Motor4:
            pwm_set_gpio_level(Motor4, level);
            break;
    }
}