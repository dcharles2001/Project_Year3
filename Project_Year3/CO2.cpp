#include "CO2.h"
AnalogIn Co2(A1);                              // Sensor Pin

void CO2::CO2setup(){
    ppm = 0;                                   // Set the PPm to 0
}

void CO2::ReadCO2(){
    CO2data = Co2.read_u16();                  // Read the sensor
    wait_us(10000);
}

void CO2::CalculatePartsPerMinute()
{
    Vrl = (double)CO2data * Vadc;              // Calculate output voltage 
    Rs = Rl * (5 - Vrl)/Vrl;                   // Calculate sensor resistance
    ratio = Rs/Rl;                             // Calculate ratio
    lgPPM = (log10(ratio) * -0.8) + 0.9;       // Calculate ppm
    ppm = 6* pow(10,lgPPM);                    // Calculate ppm
}
