/*
 * Classe permettant l'utilisation de la led libre sur le robot.
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
#ifndef LED
#define LED

#define F_CPU 8000000UL
#include <avr/io.h>
#include "minuterie.h"
#include "delay.h"

const uint8_t TEMPS_CLIGNOTEMENT = ((1000 / 25) / 5) / 2;


enum class Couleur {ROUGE, VERT, AMBREE, ETEINT};

class Led {
   public:
    Led();
    void changerCouleur(Couleur couleur);
    void clignoterCouleur(Couleur couleur);

   private:
    Minuterie minuteurLed;
    volatile uint8_t* port_;
    uint8_t pinRouge_;
    uint8_t pinVert_;
};
#endif