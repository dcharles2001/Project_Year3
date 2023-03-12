#include "Motors.h"

//////Line Finder
DigitalIn LeftLF(D4);
DigitalIn RightLF(D3);

//////Motor
DigitalOut motorLA(D8);
DigitalOut motorLB(D9);
DigitalOut EnableLeft(D10);
DigitalOut motorRA(D6);
DigitalOut motorRB(D5);
DigitalOut EnableRight(D7);

void Motors::motorSetup(){
    EnableRight = 1;
    EnableLeft = 1;
    printf("Motor Setup\n\r");
}
void Motors::motorsForward(){
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 1;
    printf("Forward\n\r");
}
void Motors::motorsBackward(){
    motorLA = 0;
    motorLB = 1;
    motorRA = 1;
    motorRB = 0;
    printf("Backward\n\r");
}
void Motors::motorsStop(){
    motorLA = 0;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
    printf("Stop\n\r");
}
void Motors::motorsLeft(){
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
    printf("Left\n\r");
}
void Motors::motorsRight(){
    motorLA = 0;
    motorLB = 0;
    motorRA = 1;
    motorRB = 0;
    printf("Right\r\n");
}



void Motors::LineFollowing(){
    if (LeftLF && RightLF){
        motorsBackward();
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