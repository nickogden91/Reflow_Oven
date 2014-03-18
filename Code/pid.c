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

// coefficients for P,I, and D
int Kp;
int Ki;
int Kd;

// current values for P,I, and D
int PVal;
int IVal;
int DVal;

int previousVal; // previous value of PID input
int setPoint;    // set point for PID controller


/*******************************************************************************
 *                             Other Functions
 ******************************************************************************/

/**
 *  \brief   PID controller initialization
 *
 *  This function initializes the variables used in the PID controller
 *
 *  \param  (int) p: proportional coefficient (Kp)
 *          (int) i: integral coefficient (Ki)
 *          (int) d: derivative coefficient (Kd)
 *
 *  \return None
 *
 *  \note   None
 */
void initPID(int p, int i, int d)
{
    // initialize the coeficcients Kp, Ki, and Kd
    Kp = p;
    Ki = i;
    Kd = d;

    // resets the P,I, and D values
    PVal = 0;
    IVal = 0;
    DVal = 0;
    
    previousVal = 0;
}


/**
 *  \brief   Update set point for PID controller
 *
 *  This function changes the set point for the PID controller
 *
 *  \param  (int) s: the new set point for the PID controller
 *
 *  \return None
 *
 *  \note   None
 */
void setPIDVal(int s)
{
    setPoint = s;
}


/**
 *  \brief   PID step
 *
 *  This function computes the next value for PID controller given the current
 *  value and the elapsed time since the previous step
 *
 *  \param  (int) dt: elapsed time since last PID step
 *          (int) currentVal: current value of system
 *
 *  \return returns the sum of the P, I, and D terms
 *
 *  \note   None
 */
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