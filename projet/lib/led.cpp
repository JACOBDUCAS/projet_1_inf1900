#include "led.h"

Led::Led() {
    port_ = &PORTD;
    pinRouge_ = PD6;
    pinVert_ = PD7;
}

void Led::changerCouleur(Couleur couleur){
    switch (couleur){
        case Couleur::VERT:
            *port_ |= (1 << pinVert_);
            *port_ &= ~(1 << pinRouge_);
            break;

        case Couleur::ROUGE:
            *port_ |= (1 << pinRouge_);
            *port_ &= ~(1 << pinVert_);
            break;

        case Couleur::AMBREE:
            minuteurLed.partirMinuterie2();
            break;

        case Couleur::ETEINT:
            minuteurLed.arreterMinuterie2();
            *port_ &= ~(1 << pinVert_);
            *port_ &= ~(1 << pinRouge_);
            break;
    }
}

void Led::clignoterCouleur(Couleur couleur){
    Delay delay;
    switch (couleur){
        case Couleur::VERT:
            for (uint8_t i = 0; i < 15; i++) {
                changerCouleur(couleur);
                delay.delayDynamique(TEMPS_CLIGNOTEMENT);
                changerCouleur(Couleur::ETEINT);
                delay.delayDynamique(TEMPS_CLIGNOTEMENT);
            }
            break;

        case Couleur::ROUGE:
            for (uint8_t i = 0; i < 15; i++) {
                changerCouleur(couleur);
                delay.delayDynamique(TEMPS_CLIGNOTEMENT);
                changerCouleur(Couleur::ETEINT);
                delay.delayDynamique(TEMPS_CLIGNOTEMENT);
            }
            break;

        case Couleur::AMBREE:
            changerCouleur(couleur);
            break;

        case Couleur::ETEINT:
           changerCouleur(couleur);
            break;
    }
}