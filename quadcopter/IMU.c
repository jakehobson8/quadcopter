#include "IMU.h"
#include <stdio.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"


#define I2C_PORT i2c0
#define BNO055_ADDR 0x28 // Change to 0x29 if ADDR pin is pulled high

#define BNO055_CHIP_ID_REG 0x00
#define BNO055_OPR_MODE_REG 0x3D
#define BNO055_EULER_H_LSB_REG 0x1A

// Helper function to write a single byte to a register
void bno055_write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2] = {reg, data};
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, buf, 2, false);
}

// Helper function to read multiple bytes from a starting register
void bno055_read_registers(uint8_t reg, uint8_t *dest, size_t len) {
    i2c_write_blocking(I2C_PORT, BNO055_ADDR, &reg, 1, true); // true to keep master control of bus
    i2c_read_blocking(I2C_PORT, BNO055_ADDR, dest, len, false);
}


void IMU_init() {
    // Initialization code for IMU

     // Initialize I2C at 400kHz
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(4, GPIO_FUNC_I2C); // GP4 as SDA
    gpio_set_function(5, GPIO_FUNC_I2C); // GP5 as SCL

    // Verify Chip ID (Expected: 0xA0)
    uint8_t chip_id = 0;
    bno055_read_registers(BNO055_CHIP_ID_REG, &chip_id, 1);
    if (chip_id != 0xA0) {
        printf("BNO055 not found! Chip ID: 0x%X\n", chip_id);
        while (1) { sleep_ms(100); }
    }

    // Switch to Config Mode before changing settings (safe practice)
    bno055_write_register(BNO055_OPR_MODE_REG, 0x00);
    sleep_ms(25);

    // Set Operating Mode to NDOF (9 Degrees of Freedom Fusion)
    // NDOF mode value = 0x0C
    bno055_write_register(BNO055_OPR_MODE_REG, 0x0C);
    sleep_ms(20); // Startup time for fusion mode
}

IMUData IMU_read_orientation() {
    // Code to read IMU orientation data
    int16_t euler_data[3]; // Buffer to hold the Euler angles data (6 bytes: 2 bytes each for roll, pitch, yaw)
    uint8_t buffer[6]; // Buffer to hold the raw data (6 bytes: 2 bytes each for roll, pitch, yaw)

    // Read 6 bytes: Heading (2 bytes), Roll (2 bytes), Pitch (2 bytes) starting at 0x1A
    bno055_read_registers(BNO055_EULER_H_LSB_REG, buffer, 6);

    // Reassemble 16-bit signed integers from LSB/MSB
    euler_data[0] = (int16_t)((buffer[1] << 8) | buffer[0]); // Yaw
    euler_data[1] = (int16_t)((buffer[3] << 8) | buffer[2]); // Roll
    euler_data[2] = (int16_t)((buffer[5] << 8) | buffer[4]); // Pitch

    // Convert raw register values to degrees (1 degree = 16 LSB in default scaling)
    IMUData data;
    data.roll = (float)euler_data[1] / 16.0f; // Convert to degrees
    data.pitch = (float)euler_data[2] / 16.0f; // Convert to degrees
    data.yaw = (float)euler_data[0] / 16.0f; // Convert to degrees
    return data;
}
