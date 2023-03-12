#ifndef LDR_H
#define LDR_H

#include "mbed.h"

class LDR{
    public:
        void LDRSetup();        // Setup the LDR
        void ReadLDR();         // Read the LDR
        int LDR;
};

#endif