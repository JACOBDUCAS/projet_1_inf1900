#include "capteurDistance.h"

#include "led.h"
#include "moteur.h"
#include "uart.h"

CapteurDistance::CapteurDistance() : convertisseurDistance_() {}

// int CapteurDistance::detectionDistanceVoulue()
// {
//     uint8_t valeur = convertisseurDistance_.lecture(PORT_DISTANCE) >> BIT_SHIFT_DEUX;
//     return valeur;
// }

uint8_t CapteurDistance::distance() {
    can convertisseur;
    uint8_t valeur = convertisseur.lecture(PORT_DISTANCE) >> BIT_SHIFT_DEUX;
    return valeur;
}

bool CapteurDistance::mesureDistance(uint8_t vingtCm) {
    Moteur moteur;
    Uart uart;

    uint8_t valeur = distance();
    if (valeur < vingtCm / 2) {
        uart.transmissionUartPhrase("PasDeMur");
        moteur.pasDeMur();
        return false;

    } 
    else if (valeur >= vingtCm - 30 && valeur < vingtCm - 15) {
        uart.transmissionUartPhrase("TropLoin");
        moteur.tropLoin();
        return true;
    } 
    else if (valeur > vingtCm + 15) {
        uart.transmissionUartPhrase("tropProche");
        moteur.tropProche();
        return true;
    }
    else {
        uart.transmissionUartPhrase("normal");
        moteur.ajustementPwm(150, 150, false, false);
        return true;
    } 
}

bool CapteurDistance::retourVirageU() {
    if (distance() > 50) {
        return false;
    }
    return true;
}

bool CapteurDistance::detecterMur(uint8_t vingtCm){
    return distance() < vingtCm / 2;
}