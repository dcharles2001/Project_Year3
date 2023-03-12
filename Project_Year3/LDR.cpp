#include "LDR.h"

AnalogIn LDRIN(A1);

void LDR::ReadLDR(){
    LDR = LDRIN.read_u16();
}