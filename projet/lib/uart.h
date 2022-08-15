/*
 * Classe permettant l'envoie de message qui seront vus dans un terminal.
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
#ifndef UART
#define UART

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
class Uart {
   public:
    Uart();
    void initialisationUart();
    void transmissionUart(unsigned char donnee);
    void transmissionUartPhrase(const char* donnee);
    void transmissionUartVariable(const char* donnee, uint16_t variable);
    
    unsigned char receptionUart(void);
};
#endif