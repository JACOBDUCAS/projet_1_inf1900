/*
 * Classe permettant la sauvegarde des vitesses des roues dans la memoire externe
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
#ifndef SAUVEGARDE
#define SAUVEGARDE

#include <avr/io.h>

#include "memoire_24.h"
#include "moteur.h"

const uint8_t PREMIERE_INSTRUCTION = 2;

class Sauvegarde {
   public:
    Sauvegarde();
    void sauvegarderEtat();
    void ecrireLongueurprogramme();

   private:
    uint16_t positionEcritureMemoire_ = PREMIERE_INSTRUCTION;
    Memoire24CXXX memoire_;
};

#endif