#include "mbed.h"
#include <iostream>


using namespace std;

DigitalOut motorLA(D8);
DigitalOut motorLB(D9);
DigitalOut EnableLeft(D10);

DigitalOut motorRA(D6);
DigitalOut motorRB(D5);
DigitalOut EnableRight(D7);

AnalogIn LDR(A1);

void motorSetup(){
    EnableRight = 1;
    EnableLeft = 1;
}
void motorsForward(){
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 1;
}
void motorsBackward(){
    motorLA = 0;
    motorLB = 1;
    motorRA = 1;
    motorRB = 0;
}

int main()
{
    float lightValue = 0;
    motorSetup();
    while (true) {
        motorsForward();
        lightValue = LDR.read();
        cout<<"LDR value: "<<lightValue<<endl;

    }

}