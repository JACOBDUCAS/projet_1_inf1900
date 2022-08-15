/*
 * Classe permettant le déverminage en utilisant le Uart où les messages
 ne sont pas obligatoirement vus.
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
#ifndef DEBUGAGE
#define DEBUGAGE

#define F_CPU 8000000UL
#include <uart.h>
#include <avr/io.h>

//#define DEBUG  // à mettre en commentaire ou non au besoin
#ifdef DEBUG
//#define DEBUG_PRINT(message) printf(message)  // ou par RS-232
#define DEBUG_PRINT(message) debug::transmissionDebug(message)
#else
#define DEBUG_PRINT(message) do {} while (0)  // code mort
#endif

namespace debug{
void transmissionDebug(char message[]);
};
#endif
