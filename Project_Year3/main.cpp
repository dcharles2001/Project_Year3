#include "mbed.h"
#include <iostream>

using namespace std;

DigitalOut motorLA(D6);
DigitalOut motorLB(D5);
DigitalOut EnableL293D(D7);
DigitalOut test(D4);

void motorInit()
{
    motorLA = 0;
    motorLB = 0;
    EnableL293D = 1;
}

void motorForward()
{
    motorLA = 1;
    motorLB = 0;
    EnableL293D = 1;
}
int main()
{
    void motorInit();
    while (true) {
        void motorForward();
        test = 1;
    }

}