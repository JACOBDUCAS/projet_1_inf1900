#ifndef MINUTERIE
#define MINUTERIE

/*
 * Classe permettant l'utilisation de la minuterie en initialisant les registres nécessaires.
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
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

const uint8_t TEMPS_MINUTERIE_MS = 100;

const uint16_t PRESCALER_MINUTERIE = 1024;

uint16_t const FREQUENCE_NOTE_A4 = 440;

class Minuterie {
   public:
    Minuterie();
    static uint8_t minuterieExpiree;
    void partirMinuterieCTC(uint16_t duree);
    void partirMinuterie2();
    void arreterMinuterie2();
    uint16_t calculerTempsMinuterie();
    void ajusterSon(uint8_t note);
    void arreterSon();
};
#endif