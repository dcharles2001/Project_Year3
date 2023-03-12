#ifndef LDR_H
#define LDR_H

#include "mbed.h"

class LDR{
    public:
        void ReadLDR();
        int LDR;
};

#endif