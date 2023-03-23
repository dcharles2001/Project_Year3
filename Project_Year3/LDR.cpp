#include "LDR.h"

AnalogIn LDRIN(A0);

void LDR::LDRSetup(){
    LDR = 0;                    // Set LDR to 0;
}

void LDR::ReadLDR(){
    LDR = LDRIN.read_u16();     // Read the LDR pin
}