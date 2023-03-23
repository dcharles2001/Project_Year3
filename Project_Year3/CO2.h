#ifndef CO2_H
#define CO2_H

#include "mbed.h"

class CO2{
    public:
        void CO2setup();                             // Setup the Environmental sensor
        void ReadCO2();                              // Function to read the sensor
        void CalculatePartsPerMinute();              // Function to calculate PPM
        int ppm;                                // Parts Per minute
    private:
        uint16_t CO2data;                            // Sesnor Data
        const double Rl = 5000.0;                    // Load resistance
        const double Vadc = 0.000076295;             // ADC 5V/65535 0.00762mV
        double Vrl;                                  // Output voltage
        double Rs;                                   // Sensor resistance
        double ratio;                                // Rs/Rl ratio
        double lgPPM;                                // logPPM

};

#endif
