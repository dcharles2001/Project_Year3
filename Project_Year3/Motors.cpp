#include "Motors.h"

//////Line Finder Pins
DigitalIn LeftLF(D6);
DigitalIn RightLF(D5);

//////Motor Pins
DigitalOut motorLA(D13);
DigitalOut motorLB(D12);
DigitalOut EnableLeft(D11);

DigitalOut motorRA(D8);
DigitalOut motorRB(D9);
DigitalOut EnableRight(D10);    //////// Note to self: Black dot

void Motors::motorSetup(){      //Set motor driver enable pins to 1
    EnableRight = 1;
    EnableLeft = 1;
    printf("Motor Setup\n\r");
}
void Motors::motorsForward(){   // Set both sides to forward
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 1;
    //printf("Forward\n\r");
}
void Motors::motorsBackward(){  // Set both sides to Backward
    motorLA = 0;
    motorLB = 1;
    motorRA = 1;
    motorRB = 0;
    //printf("Backward\n\r");
}
void Motors::motorsStop(){      // Stop the motors
    motorLA = 0;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
    //printf("Stop\n\r");
}
void Motors::motorsLeft(){      // Stop the left side motors and go forward on the right side (Turn Left)
    motorLA = 0;
    motorLB = 0;
    motorRA = 1;
    motorRB = 0;
    //printf("Left\n\r");
}
void Motors::motorsRight(){    // Stop the right side motors and go forward on the left side (Turn Right)
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
    //printf("Right\r\n");
}



void Motors::LineFollowing(){   // Depending on the status of the line finders, do a differnt action with the motors
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