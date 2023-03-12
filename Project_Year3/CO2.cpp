#include "CO2.h"
AnalogIn Co2(A0);

void CO2::ReadCO2(){
    adc_rd = Co2.read_u16();
    wait_us(10000);
}

void CO2::CalculatePartsPerMinute()
{
    Vrl = (double)adc_rd * Vadc_33;            // For 3.3V Vcc use Vadc_33
    Rs = Rl * (3.3 - Vrl)/Vrl;                 // Calculate sensor resistance
    ratio = Rs/Rl;                             // Calculate ratio
    lgPPM = (log10(ratio) * -0.8) + 0.9;       // Calculate ppm
    ppm = 6* pow(10,lgPPM);                    // Calculate ppm
}
