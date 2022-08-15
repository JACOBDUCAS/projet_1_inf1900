#pragma once

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "moteur.h"
#include "memoire_24.h"


void sauvegarderVitesse(Memoire24CXXX& memoire, uint16_t& positionEcritureMemoire) {
    Uart uart;
    uint8_t inputData[3] = {OCR0A, OCR0B, PORTB & ((1 << PORT_DIRECTION_DROITE) | (1 << PORT_DIRECTION_GAUCHE))};
    uint8_t outputData[3];
    /*uart.transmissionUartVariable("instructionEcriture: %d", positionEcritureMemoire);
    //ecriture des vitesses
    memoire.ecriture(positionEcritureMemoire, uint8_t(30));
    //memoire.lecture(positionEcritureMemoire, &outputData);
    //uart.transmissionUartVariable("sauvegardeG: %d", outputData);
    positionEcritureMemoire += 1;


    memoire.ecriture(positionEcritureMemoire, OCR0B);
    //memoire.lecture(positionEcritureMemoire, &outputData);
    //uart.transmissionUartVariable("sauvegardeD: %d", outputData);
    positionEcritureMemoire += 1;

    //ecriture des directions
    memoire.ecriture(positionEcritureMemoire, PORTB & ((1 << PORT_DIRECTION_DROITE) | (1 << PORT_DIRECTION_GAUCHE)));
    positionEcritureMemoire += 1;*/
    memoire.ecriture(positionEcritureMemoire, inputData, 3);
    positionEcritureMemoire += 3;
    memoire.lecture(positionEcritureMemoire, outputData);
    uart.transmissionUartVariable("instructionEcriture: %d", positionEcritureMemoire);
    uart.transmissionUartVariable("sauvegardeG: %d", outputData[0]);
    uart.transmissionUartVariable("sauvegardeD: %d", outputData[1]);
}