#ifndef PING_SENSOR_H
#define PING_SENSOR_H

/* * * * * * * * * * * * * * *
 * ping_sensor.h
 * 
 * Pins: GP6 (Trigger), GP7 (Echo)
 * 
 * * * * * * * * * * * * * * * */

// Function declarations for ping sensor operations
void ping_sensor_init(void);
int ping_sensor_read(void);

#endif // PING_SENSOR_H