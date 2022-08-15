#pragma once
#include "delay.h"
#include "memoire_24.h"
#include "minuterie.h"
#include "moteur.h"
#include "uart.h"

void modeReprise() {
    Memoire24CXXX memoire;
    Moteur moteur;
    Delay delay;
    uint8_t outputData[3];

    memoire.lecture(0, outputData, 2);

    uint16_t longueurProgramme = ((outputData[0] << 8) | (outputData[1] & 0xFF));

    for (uint8_t positionLecture = 2; positionLecture <= longueurProgramme; positionLecture += 3) {
        memoire.lecture(positionLecture, outputData, 3);
        moteur.ajustementPwmPrecis(outputData[0], outputData[1], false, false);
        PORTB = outputData[2];

        delay.delayDynamique(2);
    }

    moteur.ajustementPwmPrecis(0, 0, false, false);
}
