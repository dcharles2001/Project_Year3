#include "Motors.h"

//////Line Finder Pins
DigitalIn LeftLF(D15);
DigitalIn RightLF(D14);

//////Motor Pins
PwmOut motorLA(D10);
PwmOut motorLB(PB_3);
DigitalOut EnableLeft(D11);

PwmOut motorRA(D6);
PwmOut motorRB(D5);
DigitalOut EnableRight(D7);   

static float DutyCycle = 0.2;       // 0.2 second duty cycle
static float ActiveCycle = 0.4;    // period of activeness

void Motors::motorSetup(){          //Set motor driver enable pins to 1
    EnableRight = 1;
    EnableLeft = 1;
    printf("Motor Setup\n\r");
    motorLA.period(DutyCycle);
    motorLB.period(DutyCycle);
    motorRA.period(DutyCycle);
    motorRB.period(DutyCycle);
}

void Motors::motorsForward(){       // Set both sides to forward
    motorLA.write(ActiveCycle);
    motorLB.write(0.0f);
    motorRA.write(0.0f);
    motorRB.write(ActiveCycle);
    printf("Forward\n\r");
}
void Motors::motorsBackward(){      // Set both sides to Backward
    motorLA.write(0.0f);
    motorLB.write(ActiveCycle);
    motorRA.write(ActiveCycle);
    motorRB.write(0.0f);
    //printf("Backward\n\r");
}
void Motors::motorsStop(){          // Stop the motors
    motorLA.write(0.0f);
    motorLB.write(0.0f);
    motorRA.write(0.0f);
    motorRB.write(0.0f);
    printf("Stop\n\r");
}
void Motors::motorsLeft(){          // Stop the left side motors and go forward on the right side (Turn Left)
    motorLA.write(ActiveCycle);
    motorLB.write(0.0f);
    motorRA.write(ActiveCycle);
    motorRB.write(0.0f);
    printf("Left\n\r");
}
void Motors::motorsRight(){         // Stop the right side motors and go forward on the left side (Turn Right)
    motorLA.write(0.0f);
    motorLB.write(ActiveCycle);
    motorRA.write(0.0f);
    motorRB.write(ActiveCycle);
    printf("Right\r\n");
}



void Motors::LineFollowing(){       // Depending on the status of the line finders, do a differnt action with the motors
    if (LeftLF && RightLF){
        motorsStop();
    }
    if (!LeftLF && !RightLF){
        motorsForward();
    }
    if (!LeftLF && RightLF){
        motorsRight();
    }
    if (LeftLF && !RightLF){
        motorsLeft();
    }
}