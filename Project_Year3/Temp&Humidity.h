#ifndef DHT11_H
#define DHT11_H

#include "mbed.h"

// DHT returns
#define DHT_OK 0
#define DHT_ERROR_CHECKSUM   -1
#define DHT_ERROR_TIMEOUT    -2

class Dht11{
    public:
        void DHT11setup();              // function to setup Sensor
        int readDHT11();                // read the sensor data
        float getFahrenheit();          // calculate the Fahrenheit temp
        int getCelsius();               // return the Celsius temp
        int getHumidity();              // return the Humidity
    private:
        int humidity;
        int temperature;
        Timer DHT11Startup;             // Timer to give correct start

};

#endif