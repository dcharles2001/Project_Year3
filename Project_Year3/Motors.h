#ifndef MOTORS_H
#define MOTORS_H

#include "mbed.h"

class Motors{
    public:
        void motorSetup();          // Setup for the motor drivers
        void LineFollowing();       // Function to decide the movement of the motors
        // Functions to set the motors moving in directions
        void motorsForward();
        void motorsBackward();
        void motorsStop();
        void motorsLeft();
        void motorsRight();
        
};

#endif