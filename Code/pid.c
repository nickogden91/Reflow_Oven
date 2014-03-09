/*
 * File:   pid.c
 * Author: nick
 *
 *
 */


 /******************************************************************************
 *                          Include Files
 ******************************************************************************/

#include "pid.h"


 /******************************************************************************
 *                          Global Variables
 ******************************************************************************/

int Kp;
int Ki;
int Kd;

int PVal;
int IVal;
int DVal;

int previousVal;
int setPoint;


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

void initPID(int p, int i, int d)
{
    Kp = p;
    Ki = i;
    Kd = d;

    PVal = 0;
    IVal = 0;
    DVal = 0;
    
    previousVal = 0;
}

void setPIDVal(int s)
{
    setPoint = s;
}

int pidStep(int dt ,int currentVal)
{
    int error;
    error = setPoint - currentVal;
    PVal = Kp * error;
    IVal += Ki * error * dt;

    // integrator windup prevention
    if (IVal > 2)
        IVal = 2;
    else if (IVal < -2)
        IVal = -2;

    DVal = -1 * Kd * (currentVal - previousVal)/dt;
    previousVal = currentVal;
    return PVal + IVal + DVal;
}