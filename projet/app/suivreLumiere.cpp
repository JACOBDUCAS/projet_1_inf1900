#ifndef INCLUDES
#define INCLUDES

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can.h"
#include "moteur.h"
#include "uart.h"
#include "delay.h"
#include "sauvegarderVitesse.cpp"
#include "sauvegarde.h"
#include "lecture.cpp"
#include "memoire_24.h"
#include "led.h"

#endif

const uint8_t PHOTORESISTANCE_GAUCHE = PA2;
const uint8_t PHOTORESISTANCE_DROITE = PA0;

bool seuilLumiere(uint8_t* luminositeAmbiante) {
    can capteurLumiere;

    if ((uint8_t(capteurLumiere.lecture(PHOTORESISTANCE_DROITE) >> 2)) > luminositeAmbiante[1] + 30 
        || (uint8_t(capteurLumiere.lecture(PHOTORESISTANCE_GAUCHE) >> 2)) > luminositeAmbiante[0] + 30)
    {
        return true;
    }
    return false;
}

void detecterLuminositeAmbiante(uint8_t* luminositeAmbiante) {
    can capteurLumiere;
    luminositeAmbiante[0] = uint8_t(capteurLumiere.lecture(PHOTORESISTANCE_GAUCHE) >> 2);
    luminositeAmbiante[1] = uint8_t(capteurLumiere.lecture(PHOTORESISTANCE_DROITE) >> 2);
    luminositeAmbiante[2] = uint8_t((luminositeAmbiante[1] + luminositeAmbiante[0]) / 2);
} 


void suivreLumiere(Moteur& moteur, uint8_t* luminositeAmbiante) {
    can capteurLumiere;
    uint8_t luminositeDroite = (capteurLumiere.lecture(PHOTORESISTANCE_DROITE) >> 2);
    uint8_t luminositeGauche = (capteurLumiere.lecture(PHOTORESISTANCE_GAUCHE) >> 2);

    /*uart.transmissionUartVariable("luminositeDroite: %d", luminositeDroite);
    uart.transmissionUartVariable("luminositeGauche: %d", luminositeGauche);*/

    luminositeDroite = luminositeDroite > luminositeAmbiante[1] + 5 ? luminositeDroite - luminositeAmbiante[1] + luminositeAmbiante[2] : 0;
    luminositeGauche = luminositeGauche > luminositeAmbiante[0] + 5 ? luminositeGauche - luminositeAmbiante[0] + luminositeAmbiante[2] : 0;
    uint8_t differenceDeLuminosite = 0;

    /*uart.transmissionUartVariable("luminositeDroite: %d", luminositeDroite);
    uart.transmissionUartVariable("luminositeGauche: %d", luminositeGauche);*/

    if (luminositeDroite != 0 || luminositeGauche != 0) {

        if (luminositeDroite > luminositeGauche) {
            differenceDeLuminosite = luminositeDroite - luminositeGauche;

            if (differenceDeLuminosite < 10) {
                moteur.ajustementPwmPrecis(luminositeDroite, luminositeDroite, false, false);
            }
            
            else if (differenceDeLuminosite > 25) {
                moteur.ajustementPwmPrecis(luminositeDroite, luminositeGauche, false, true);
            }

            else {
                moteur.ajustementPwmPrecis(luminositeDroite, luminositeGauche, false, false);
            }
        }

        else if (luminositeDroite < luminositeGauche) {
            differenceDeLuminosite = luminositeGauche - luminositeDroite;

            if (differenceDeLuminosite < 10) {
                moteur.ajustementPwmPrecis(luminositeGauche, luminositeGauche, false, false);
            }
            
            else if (differenceDeLuminosite > 25) {
                moteur.ajustementPwmPrecis(luminositeDroite, luminositeGauche, true, false);
            }

            else {
                moteur.ajustementPwmPrecis(luminositeDroite, luminositeGauche, false, false);
            }
        }

        else {
            moteur.ajustementPwmPrecis(luminositeDroite, luminositeGauche, false, false);
        }
    }

    else {
        moteur.ajustementPwmPrecis(0, 0, false, false);
    }
}
