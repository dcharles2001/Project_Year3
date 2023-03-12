#ifndef CO2_H
#define CO2_H

#include "mbed.h"

class CO2{
    public:
        void ReadCO2();
        void CalculatePartsPerMinute();
        uint16_t ppm;
    private:
        uint16_t adc_rd;
        const double Rl = 5000.0;                    // Rl (Ohm) - Load resistance
        const double Vadc_33 = 0.0000503548;         // ADC step 3,3V/65535 0.00503mV (16bit ADC)
        double Vrl;                                  // Output voltage
        double Rs;                                   // Rs (Ohm) - Sensor resistance
        double ratio;                                // Rs/Rl ratio
        double lgPPM;

};

#endif
