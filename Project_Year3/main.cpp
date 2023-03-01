#include "mbed.h"
#include "Dht11.h"

using namespace std;

//////LDR
AnalogIn LDR (A0);
float lightValue;

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

//////DHT11
//DigitalIn DHT11(D2);
Dht11 sensor(D2);

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
void motorsStop(){
    motorLA = 0;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
}
void motorsLeft(){
    motorLA = 1;
    motorLB = 0;
    motorRA = 0;
    motorRB = 0;
}
void motorsRight(){
    motorLA = 0;
    motorLB = 0;
    motorRA = 1;
    motorRB = 0;
}



int main()
{
    motorSetup();
    while (true) {
        /*
        if (LeftLF && RightLF){
            motorsForward();
        }
        if (!LeftLF && !RightLF){
            motorsBackward();
        }
        if (!LeftLF && RightLF){
            motorsRight();
        }
        if (LeftLF && !RightLF){
            motorsLeft();
        }*/
        sensor.read();
        printf("T: %d, H: %d\r\n", sensor.getCelsius(), sensor.getHumidity());
    }
}