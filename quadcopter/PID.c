#include "PID.h"


void PID_Init(PIDController *PID, float p, float i, float d, float t){
    PID->Kp = p;
    PID->Ki = i;
    PID->Kd = d;
    PID->T = t;
    PID->integral = 0;
    PID->prevError = 0;
}

int PID_CalculatePWM(PIDController *PID, float desired, float measured){
    // Calcualte Current Error
    double error = desired - measured;

    // Apply Proportional term
    double motorPWM = PID->Kp * error;

    // Apply Integral term with anti-windup clamp
    PID->integral += error * PID->T;
    // Clamp integral so its contribution stays within ±50% duty
    float integral_max = 50.0f / PID->Ki;
    if (PID->integral > integral_max)  PID->integral = integral_max;
    if (PID->integral < -integral_max) PID->integral = -integral_max;
    motorPWM += PID->Ki * PID->integral;

    // Apply Derivative term
    motorPWM += (error - PID->prevError) * PID->Kd;

    // Bind to [-100, 100] 
    if (motorPWM > 100){
        motorPWM = 100;
    }else if(motorPWM < -100){
        motorPWM = -100 ;
    }
    
    // Save error for next derivative calculation
    PID->prevError = error;

    return motorPWM;
}
