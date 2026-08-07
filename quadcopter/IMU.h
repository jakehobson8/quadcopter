#ifndef IMU_H
#define IMU_H

/* * * * * * * * * * * * * * * *
 * IMU.h
 * 
 * Pins: GP4 (SDA), GP5 (SCL)
 * 
 * * * * * * * * * * * * * * * * */

typedef struct {
    float roll;
    float pitch;
    float yaw;
} IMUData;

void IMU_init();

IMUData IMU_read_orientation();


#endif // IMU_H