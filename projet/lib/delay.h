#ifndef DELAY
#define DELAY
/*
 * Classe permettant une attente de duree variable  sur le robot.
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Équipe 4546
 * Alexis Nicolas
 * Fatima Mellata
 * Jacob Ducas
 * Guek Lang Lim
 * 2022
 *
 * Code qui n'est sous aucune license.
 *
 */
#define F_CPU 8000000UL
#include <avr/io.h>
class Delay{
    public:
    Delay();
    void delayDynamique(uint8_t duree);
};
#endif 