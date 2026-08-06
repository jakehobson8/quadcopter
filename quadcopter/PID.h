#ifndef PID_H
#define PID_H

typedef struct {
    float Kp;        // Proportional gain
    float Ki;        // Integral gain
    float Kd;        // Derivative gain
    float T;         // Sampling period (seconds)
    float integral;  // Accumulated error
    float prevError; // Error from the previous step
} PIDController;


//---------------------------------------------------
// PID Initialization
//   Input: PID struct, desired PID gains, sampling period
//   Output: None
//---------------------------------------------------
void PID_Init(PIDController *PID, float p, float i, float d, float t);

//---------------------------------------------------
// PID Computation
//   Input: PID struct, desired force, measured force
//   Output: New PWM value (0-100)
//---------------------------------------------------
int PID_CalculatePWM(PIDController *PID, float desired, float measured);


#endif // PID_H