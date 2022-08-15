/*
 * Classe permettant le déplacement du robot.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Équipe 4546
 * Alexis Nicolas
 * Fatima Mellata
 * Jacob Ducas
 * Guek lang lim
 * 2022
 *
 * Code qui n'est sous aucune license.
 *
 */
#ifndef MOTEUR
#define MOTEUR

#define F_CPU 8000000UL
#include <avr/io.h>
#include "delay.h"

const uint8_t TEMPS_VIRAGE_VITESSE_MAX = 28;        //valeur calculee qui donne un virage de 90 degree
const uint8_t VITESSE_MAX = 0xFF;                   //vitesse maximale permise par 8 bits
const uint8_t PORT_VITESSE_GAUCHE = PB4, PORT_VITESSE_DROITE = PB3, PORT_DIRECTION_GAUCHE = PB2, PORT_DIRECTION_DROITE = PB5;

class Moteur {
   public:
    Moteur();
    /*Moteur(uint16_t ratioGauche, uint16_t ratioDroit, bool reculer);*/

    void ajustementPwm(uint8_t ratioGauche, uint8_t ratioDroit, bool reculer, bool tourner);      //1 a reculer pour reculer
    void ajustementPwmPrecis(uint8_t ratioGauche, uint8_t ratioDroit, bool reculerG, bool reculerD);
    uint16_t pourcentage(uint16_t& ratio);
    void garderMemoire(uint8_t ratioGauche, uint8_t ratioDroit, bool reculer);
    void tournerGauche();
    void tournerDroite();
    void virageU();
    void tropLoin();
    void tropProche();
    void pasDeMur();
    uint8_t getRatioDroit() const;
    uint8_t getRatioGauche() const;
    //bool getDirectionGauche();
    //bool getDirectionDroit();
    
   private:
    uint8_t ratioGauche_;
    uint8_t ratioDroit_;
    bool reculer_;
    //bool directionGauche_;
    //bool directionDroit_;
    uint8_t max_ = 0xFF;  // le max du timer 0 est a 0xFF
};
#endif