#ifndef CAPTEURDISTANCE
#define CAPTEURDISTANCE

#define F_CPU 8000000UL
#include <avr/io.h>
#include "can.h"

const uint8_t PORT_DISTANCE = PA4;
const uint8_t BIT_SHIFT_DEUX = 2;
const uint8_t PAS_MUR = 50;
const uint8_t TROP_LOIN = 70;
const uint8_t TROP_PROCHE = 100;
const uint8_t MUR =  140;
class CapteurDistance
{
    public:
    CapteurDistance();

    bool mesureDistance(uint8_t valeur);
    bool retourVirageU();
    uint8_t distance();
    bool detecterMur(uint8_t vingtCm);
    //int detectionDistanceVoulue();

    private:
    can convertisseurDistance_;
};
#endif