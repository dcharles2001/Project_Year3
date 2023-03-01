#ifndef MOTORS_H
#define MOTORS_H

#include "mbed.h"

class Motors{
    public:
        void motorSetup();
        void motorsForward();
        void motorsBackward();
        void motorsStop();
        void mototsLeft();
        void motorsRight();
        void LineFollowing();
};

#endif