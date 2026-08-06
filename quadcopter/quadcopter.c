#include <stdio.h>
#include "pico/stdlib.h"

#include "ping_sensor.h"
#include "motor_driver.h"
#include "PID.h"


//#define PING_TEST
//#define MOTOR_TEST
//#define IMU_TEST

// Main funciton
#if !defined(PING_TEST) && !defined(IMU_TEST) && !defined(MOTOR_TEST)

int main()
{
    stdio_init_all();
    ping_sensor_init();
    motor_driver_init();

    // Initialize PID controller
    PIDController Motor1PID;
    PID_Init(&Motor1PID, 1.0f, 0.5f, 0.1f, 0.001f); // Initialize PID controller with example gains

    while (true) {
        // flight control loop
        int distance = ping_sensor_read();

        // Example desired distance (setpoint) for the PID controller
        float desired_distance = 20.0f; // Desired distance in cm
        int motor_pwm = PID_CalculatePWM(&Motor1PID, desired_distance, (float)distance);
        if (motor_pwm < 0) {
            motor_pwm = 0; // Ensure PWM is not negative
        }
        if (motor_pwm > 80) {
            motor_pwm = 80; // Ensure PWM does not exceed 80% duty cycle (motor safety limit)
        }
        motor_driver_set_speed(Motor1, (float)motor_pwm);
        // printf("Distance: %d cm, Motor PWM: %d\n", distance, motor_pwm);
        sleep_ms(1); // Wait for 1 ms before the next loop iteration
    }
}
#endif // Main


#ifdef MOTOR_TEST
int main()
{
    stdio_init_all();
    motor_driver_init();

    while (true) {
        motor_driver_set_speed(Motor1, 10.0f); // Set motor speed to 10%
        sleep_ms(5000); // Wait for 5 seconds   
        motor_driver_set_speed(Motor1, 0.0f); // Stop the motor
        sleep_ms(5000); // Wait for 5 seconds
    }
}
#endif // MOTOR_TEST


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
