#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "can.h"
#include "led.h"
#include "minuterie.h"
#include "moteur.h"
#include "interruption.cpp"
#include "uart.h"
#include "delay.h"
#include "sauvegarde.h"
#include "capteurDistance2.h"
#include "suivreLumiere.cpp"


int test() 
{
    DDRA = 0x00;
    DDRD = 0xFF;
    DDRB = 0xFF;
    Delay delay;
    Moteur moteur;
    Uart uart;
    delay.delayDynamique(10);
    CapteurDistance2 capteur(moteur);
    // while (true){
    //     if (!capteur.detecterMur()){
    //         break;
    //     }
    //     capteur.suivreMur();
    //     delay.delayDynamique(2);
    // }

    uint8_t luminosite[3];
    detecterLuminositeAmbiante(luminosite);

    // while (capteur.suivreMur()){
    //     delay.delayDynamique(2);
    // }
    //uint8_t luminosite[3] = { 10, 20, 30};
    delay.delayDynamique(100);
    while (!capteur.detecterMur()) {
        uart.transmissionUartPhrase("suitLumiere");
        suivreLumiere(moteur, luminosite);
        delay.delayDynamique(2);
    }

    return 0;
}

int main() {
    DDRA = 0x00;
    DDRD = 0xFF & 0b11111011;
    DDRB = 0xFF;
    Moteur moteur;
    Uart uart;
    Delay delay;
    can capteurLumiere;
    Sauvegarde sauvegarde;

    uint8_t luminositeAmbiante[3];
    detecterLuminositeAmbiante(luminositeAmbiante);

    for (uint16_t i = 0; i < 100; i++) {
        suivreLumiere(moteur, luminositeAmbiante);
        sauvegarde.sauvegarderEtat();
        delay.delayDynamique(2);
    }

    sauvegarde.ecrireLongueurprogramme();
    modeReprise();
    uart.transmissionUartPhrase("mode relecture de suivre lumiere fini");
    moteur.ajustementPwmPrecis(0,0, false, false);

    return 0;
    }