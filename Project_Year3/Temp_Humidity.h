#ifndef TEMP_HUMIDITY_H
#define TEMP_HUMIDITY_H

#include "mbed.h"

#define DHT_OK 0
#define DHT_ERROR_CHECKSUM   -1
#define DHT_ERROR_TIMEOUT    -2

class Temp_Humidity{
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
        Timer bitread;                  // Timer to give the corect time for reading bits

};
#endif