#define F_CPU 8000000UL
#include <avr/io.h>
#include "can.h"
#include "moteur.h"
#include "uart.h"

const uint8_t PORT_DISTANCE = PA4;
const uint8_t BIT_SHIFT_DEUX = 2;
const uint8_t PAS_MUR = 50;
const uint8_t TROP_LOIN = 70;
const uint8_t TROP_PROCHE = 100;
const uint8_t MUR =  140;

class CapteurDistance2{
    public:
    CapteurDistance2(Moteur& moteur);
    CapteurDistance2(Moteur& moteur, uint8_t vingtCm);
    uint8_t mesurerDistance();
    bool suivreMur();
    bool detecterMur();
    uint8_t getVingtCm() const {return vingtCm_;};

    private:
    can convertisseurDistance_;
    uint8_t vingtCm_;
    uint8_t tropLoin_;
    uint8_t tropProche_;
    Moteur moteur_;
    Uart uart;
};