#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

/* * * * * * * * * * * * * *
 * motor_driver.h
 * 
 * Pins: GP18, GP19, GP20, GP21
 * 
 * * * * * * * * * * * * * * */



// Define motor pins
#define Motor1 18
#define Motor2 19
#define Motor3 20
#define Motor4 21

void motor_driver_init();
void motor_driver_set_speed(int motor_id, float speed);

#endif // MOTOR_DRIVER_H