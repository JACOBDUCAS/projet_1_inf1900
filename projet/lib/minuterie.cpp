#include "minuterie.h"

uint8_t Minuterie::minuterieExpiree = 0;  // utiliser cette syntaxe pour y acceder dans le main
Minuterie::Minuterie(){}

void Minuterie::partirMinuterieCTC(uint16_t duree) {
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    TCNT1 = 0;
    OCR1A = duree;
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TCCR1C = 0;
    TIMSK1 = (1 << ICIE1) | (1 << OCIE1A);
}

void Minuterie::partirMinuterie2() {
    minuterieExpiree = 0;
    // mode PWM phase correct du timer 2 avec horloge divisée par 2
    // sans intteruption
    TCNT2 = 0;
    OCR2A = 0xFF >> 1; 
    OCR2B = 0xFF >> 1;
    TCCR2A = (1 << WGM20) | (1 << COM2B1)| (1 << COM2B0)| (1 << COM2A1)| (0 << COM2A0);
    TCCR2B = (0 << CS22) | (0 << CS21)| (1 << CS20);
    TIMSK2 = (0 << OCIE2B) | (0 << OCIE2A);
}

void Minuterie::arreterMinuterie2() {
    TCCR2A = 0;
}

uint16_t Minuterie::calculerTempsMinuterie() {
    return OCR1A * PRESCALER_MINUTERIE / F_CPU;
}

void Minuterie::ajusterSon(uint8_t note) {

    //page 120 de la documentation du atmega pour le calcul de frequence
    uint16_t frequenceNote = powf(2, (note-69) / 12) * FREQUENCE_NOTE_A4;

    TCNT1 = 0;
    OCR1A = (F_CPU / (2 * frequenceNote)) - 1;
    TCCR1A = (1 << COM1A0);
    TCCR1B = (1 << WGM12) | (1 << CS10);
    TCCR1C = 0;
    TIMSK1 = (0 << ICIE1) | (0 << OCIE1A);      //pas d'interuption pour jouer un son
}

void Minuterie::arreterSon(){
    //le pwm ne produira pas d'onde
    OCR1A = 0;
}
