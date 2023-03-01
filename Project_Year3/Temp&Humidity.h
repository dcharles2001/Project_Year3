#ifndef DHT11_H
#define DHT11_H

#include "mbed.h"

class Dht11{
public:
    Dht11(PinName const &p);
    int read();
    float getFahrenheight();
    int getCelsius();
    int getHumidity();
private:
    int humidity;
    int temperature;
    DigitalInOut DHT11pin;
    Timer DHT11Startup;
};
#endif