#ifndef INCLUDES
#define INCLUDES

#define F_CPU 8000000UL
#include <avr/io.h>
#include "moteur.h"
#include "delay.h"
#include "capteurDistance2.h"
#include "uart.h"

#endif

void virageU(CapteurDistance2& capteurDistance, Moteur& moteur,Delay& delay) {
    Uart uart;
    moteur.ajustementPwmPrecis(255, 170, false, false);

    while (!capteurDistance.detecterMur()) {
        uart.transmissionUartPhrase("je tourne");
        delay.delayDynamique(1);
    }
    delay.delayDynamique(15);
    moteur.ajustementPwmPrecis(0, 0, false, false);
}