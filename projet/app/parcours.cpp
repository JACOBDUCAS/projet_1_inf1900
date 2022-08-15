/*
 * Code permettant de faire fonctionner le robot en determiant quoi faire et a quel moment du parcours
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
#include <avr/interrupt.h>
#include <avr/io.h>

#include "can.h"
//#include "capteurDistance.h"
#include "capteurDistance2.h"
#include "delay.h"
#include "interruption.cpp"
#include "led.h"
#include "minuterie.h"
#include "moteur.h"
#include "sauvegarde.h"
#include "sauvegarderVitesse.cpp"
#include "suivreLumiere.cpp"
#include "virageU.cpp"
#include "uart.h"

// +----------------------+----------------+-------------+--------------------------------------+----------------+
// |     EtatPresent      | boutonIterrupt | boutonBlanc |             EtatSuivant              | Couleurlumiere |
// +----------------------+----------------+-------------+--------------------------------------+----------------+
// | Etat::INIT           |              0 |           0 | Etat::INIT                           | eteint         |
// | Etat::INIT           |              1 |           0 | Etat::SUIVRE_MUR                     | vert clignote  |
// | Etat::INIT           |              0 |           1 | Etat::REPRISE                        | rouge clignote |
// | Etat::SUIVRE_MUR     |              - |           - | Etat::ATTENTE                        | eteinte        |
// | Etat::ATTENTE        |              0 |           1 | Etat::TOURNER                        | ambree         |
// | Etat::ATTENTE        |              1 |           0 | Etat::FIN_PARCOURS                   | eteinte        |
// | Etat::ATTENTE        |              0 |           0 | etat::SUIVRE_LUMIERE                 | eteinte        |
// | ETAT::SUIVRE_LUMIERE |              - |           - | etat::SUIVRE_MUR                     | eteinte        |
// | Etat::TOURNER        |              - |           - | etat::SUIVRE_MUR_SANS_ENREGISTREMENT | eteinte        |
// +----------------------+----------------+-------------+--------------------------------------+----------------+

const int BOUTONINTERRUPT = PD2;
const uint8_t BOUTONBLANC = PD3;
const uint8_t TEMPS_SAUVEGARDE = 2;
const int TEMPS_DEBOUNCE = 10;

bool debounce(volatile uint8_t* pin, uint8_t position) {
    if (*pin & (1 << position)) {
        _delay_ms(TEMPS_DEBOUNCE);
        return (*pin & (1 << position));
    }
    return false;
}

enum class Etat { INIT,
                  DEBUT_PARCOURS,
                  SUIVRE_MUR,
                  ATTENTE,
                  SUIVRE_LUMIERE,
                  TOURNER,
                  REPRISE,
                  FIN_PARCOURS,
                  SUIVRE_MUR_SANS_ENREGISTREMENT };
volatile Etat etat = Etat::INIT;

int main() {
    DDRA = 0x00;
    DDRD = 0xF0;
    DDRB = 0xFF;
    Uart uart;
    Moteur moteur;
    Delay delay;
    Led led;
    Sauvegarde sauvegarde;
    CapteurDistance2 capteurDistance(moteur);


    uint8_t luminositeAmbiante[3];
    detecterLuminositeAmbiante(luminositeAmbiante);
    uint8_t vingtCm = capteurDistance.getVingtCm();

    while (true) {
        switch (etat) {
            case Etat::INIT:
                uart.transmissionUartPhrase("Init");
                while (true) {
                    if ((debounce(&PIND, BOUTONINTERRUPT))) {
                        etat = Etat::DEBUT_PARCOURS;
                        uart.transmissionUartPhrase("versDebutParcours");
                        break;

                    } else if ((debounce(&PIND, BOUTONBLANC))) {
                        uart.transmissionUartPhrase("versDebutReprise");
                        etat = Etat::REPRISE;
                        break;
                    }
                }
                break;

            case Etat::DEBUT_PARCOURS:
                uart.transmissionUartPhrase("DebutParcours");
                led.clignoterCouleur(Couleur::VERT);
                uart.transmissionUartPhrase("versSuivreMur");
                etat = Etat::SUIVRE_MUR;
                break;

            case Etat::REPRISE:
                uart.transmissionUartPhrase("reprise");
                led.clignoterCouleur(Couleur::ROUGE);
                modeReprise();
                break;

            case Etat::SUIVRE_MUR: {
                CapteurDistance2 capteurDistanceMur(moteur, capteurDistance.getVingtCm());
                uart.transmissionUartPhrase("suivreMur");
                // delay.delayDynamique(10);
                while (capteurDistanceMur.suivreMur()) {
                    sauvegarde.sauvegarderEtat();
                    delay.delayDynamique(TEMPS_SAUVEGARDE);
                }
                etat = Etat::ATTENTE;
            } break;

            case Etat::ATTENTE:
                uart.transmissionUartPhrase("ATTENTE");
                while (true) {
                    if ((debounce(&PIND, BOUTONINTERRUPT))) {
                        etat = Etat::FIN_PARCOURS;
                        uart.transmissionUartPhrase("versFIN");
                        break;
                    }
                    if ((debounce(&PIND, BOUTONBLANC))) {
                        uart.transmissionUartPhrase("versTourner");
                        etat = Etat::TOURNER;
                        break;
                    }
                    if (seuilLumiere(luminositeAmbiante)) {
                        uart.transmissionUartPhrase("versSuivreLumiere");
                        etat = Etat::SUIVRE_LUMIERE;
                        break;
                    }
                }
                break;

            case Etat::TOURNER:
                uart.transmissionUartPhrase("tourner");
                led.changerCouleur(Couleur::AMBREE);
                virageU(capteurDistance, moteur, delay);
                led.changerCouleur(Couleur::ETEINT);
                etat = Etat::SUIVRE_MUR_SANS_ENREGISTREMENT;
                break;

            case Etat::FIN_PARCOURS:
                uart.transmissionUartPhrase("FINParcours");
                led.changerCouleur(Couleur::ROUGE);
                sauvegarde.ecrireLongueurprogramme();
                led.changerCouleur(Couleur::VERT);
                return 0;
                break;

            case Etat::SUIVRE_LUMIERE: {
                uart.transmissionUartPhrase("suivreLumiere");
                // CapteurDistance2 capteurDistance(moteur, vingtCm);
                while (!capteurDistance.detecterMur()) {
                    suivreLumiere(moteur, luminositeAmbiante);
                    sauvegarde.sauvegarderEtat();
                    delay.delayDynamique(TEMPS_SAUVEGARDE);
                }
                etat = Etat::SUIVRE_MUR;
            } break;

            case Etat::SUIVRE_MUR_SANS_ENREGISTREMENT: {
                uart.transmissionUartPhrase("suivreMurNonEnregistre");
                CapteurDistance2 capteurDistance(moteur, vingtCm);
                while (capteurDistance.suivreMur()) {
                    delay.delayDynamique(1);
                }
                etat = Etat::ATTENTE;
            } break;
        }
    }
}