#include "moteur.h"
#include "capteurDistance.h"

const uint8_t SECONDE = 40;
const uint8_t DEUX_SECONDES = 80;
const uint8_t TROIS_SECONDES = 120;
const uint8_t DEMIE_SECONDE = 20;
const uint8_t VITESSE_ARRET = 0;
Moteur::Moteur() {
    DDRB |= (1 << PORT_DIRECTION_DROITE) | (1 << PORT_DIRECTION_GAUCHE) | (1 << PORT_VITESSE_DROITE) | (1 << PORT_VITESSE_GAUCHE);
}


void Moteur::garderMemoire(uint8_t ratioGauche, uint8_t ratioDroit, bool reculer) {
    ratioGauche_ = ratioGauche;
    ratioDroit_ = ratioDroit;
    reculer_ = reculer;
};

void Moteur::ajustementPwm(uint8_t ratioGauche, uint8_t ratioDroit, bool reculer, bool tourner) {
    // mise à un des sorties OC1A et OC1B sur comparaison
    // réussie en mode PWM 8 bits, phase correcte
    // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
    // page 130 de la description technique du ATmega324PA)
    OCR0A = ratioDroit;
    OCR0B = ratioGauche;

    if (reculer == true) {
        PORTB |= (1 << PB2) | (1 << PB5);
    }
    else {
        PORTB &= ~((1 << PB2) | (1 << PB5));
    }

    if(!tourner){
        garderMemoire(ratioGauche, ratioDroit, reculer);
    }

    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR0A = (1 << WGM00) | (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0);
    TCCR0B = (1 << CS01);
}

void Moteur::ajustementPwmPrecis(uint8_t ratioGauche, uint8_t ratioDroit, bool reculerG, bool reculerD) {
    OCR0A = ratioDroit;
    OCR0B = ratioGauche;

    if (reculerG == true) {
        PORTB |= (1 << PB2);
    }
    else {
        PORTB &= ~(1 << PB2);
    }

    if (reculerD == true) {
        PORTB |= (1 << PB5);
    }
    else {
        PORTB &= ~(1 << PB5);
    }

    // sauvegarde des vitesses dans les attributs
    ratioGauche_ = ratioGauche;
    ratioDroit_ = ratioDroit;
    //directionGauche_ = reculerG;
    //directionDroit_ = reculerD;
    // division d'horloge par 8 - implique une fréquence de PWM fixe
    TCCR0A = (1 << WGM00) | (1 << COM0A1) | (0 << COM0A0) | (1 << COM0B1) | (0 << COM0B0);
    TCCR0B = (1 << CS01);
}

void Moteur::tournerGauche(){
    ajustementPwm(0,ratioDroit_, false, true);
    Delay delay;
    delay.delayDynamique(TEMPS_VIRAGE_VITESSE_MAX *100 / (ratioDroit_ *100 / VITESSE_MAX));
    ajustementPwm(ratioGauche_, ratioDroit_, reculer_, true);
}

void Moteur::tournerDroite(){
    ajustementPwm(200,0, false, true);
    Delay delay;
    delay.delayDynamique(SECONDE);
    ajustementPwm(ratioGauche_, ratioDroit_, reculer_, true);
}

void Moteur::virageU()
{
    Delay delay;
    CapteurDistance capteur;
    //Attendre 1 seconde
    ajustementPwm(200, 200, false, false);
    delay.delayDynamique(DEUX_SECONDES);

    //Mettre une led ambrée

    tournerDroite();
    delay.delayDynamique(TROIS_SECONDES);
    tournerDroite();
    
    
    bool pasMur = true;
    while(capteur.retourVirageU())
    {

        delay.delayDynamique(1);
    }
    ajustementPwm(VITESSE_ARRET, VITESSE_ARRET, false, false);
}

void Moteur::tropLoin()
{
    //Delay delay;
    ajustementPwm(150, 130, false, false);
    //delay.delayDynamique(SECONDE);

}

void Moteur::tropProche()
{
    //Delay delay;
    ajustementPwm(130, 150, false, false);
    //delay.delayDynamique(SECONDE);

}

void Moteur::pasDeMur()
{
    ajustementPwm(VITESSE_ARRET, VITESSE_ARRET, false, false);
}

uint8_t Moteur::getRatioDroit() const {return ratioDroit_;}

uint8_t Moteur::getRatioGauche() const {return ratioGauche_;}

//bool Moteur::getDirectionGauche() {return directionGauche_;}

//bool Moteur::getDirectionDroit() {return directionDroit_;}
