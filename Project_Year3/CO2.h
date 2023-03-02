#ifndef CO2_H
#define CO2_H

#include "mbed.h"

class CO2{
    public:
        void ReadCO2();
    private:
        uint16_t adc_rd;

};

#endif
