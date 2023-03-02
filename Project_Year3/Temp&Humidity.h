#ifndef DHT11_H
#define DHT11_H

#include "mbed.h"

#define DHTLIB_OK 0
#define DHTLIB_ERROR_CHECKSUM   -1
#define DHTLIB_ERROR_TIMEOUT    -2

class Dht11{
public:
    Dht11(PinName const &p);
    int readDHT11();
    float getFahrenheit();
    int getCelsius();
    int getHumidity();
private:
    int humidity;
    int temperature;
    DigitalInOut DHT11pin;
    Timer DHT11Startup;
};
#endif