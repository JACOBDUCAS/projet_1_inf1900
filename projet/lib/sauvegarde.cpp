#include "sauvegarde.h"
#include "uart.h"

Sauvegarde::Sauvegarde(): memoire_() {positionEcritureMemoire_ = PREMIERE_INSTRUCTION;}


void Sauvegarde::sauvegarderEtat() {
    Uart uart;
    //uart.transmissionUartVariable("LProgramme: %d", positionEcritureMemoire_);
    uint8_t inputData[3] = {OCR0A, OCR0B, PORTB & ((1 << PORT_DIRECTION_DROITE) | (1 << PORT_DIRECTION_GAUCHE))};
    memoire_.ecriture(positionEcritureMemoire_, inputData, 3);
    positionEcritureMemoire_ += 3;
}

void Sauvegarde::ecrireLongueurprogramme() {
    uint8_t longueurProgramme[2] = {uint8_t(positionEcritureMemoire_ >> 8), uint8_t(positionEcritureMemoire_)};
    memoire_.ecriture(0, longueurProgramme, 2);
}

