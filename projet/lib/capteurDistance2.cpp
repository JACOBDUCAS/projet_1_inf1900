#include "capteurDistance2.h"
#include <util/delay.h>

CapteurDistance2::CapteurDistance2(Moteur& moteur) : convertisseurDistance_(), moteur_(moteur), uart() {
    _delay_ms(10);
    vingtCm_ = convertisseurDistance_.lecture(PORT_DISTANCE) >> BIT_SHIFT_DEUX;
    _delay_ms(10);
    tropLoin_ = vingtCm_ /6 * 5;
    _delay_ms(10);
    tropProche_ = vingtCm_ / 6 * 7;
    _delay_ms(10);
    uart.transmissionUartVariable("vingt: %d", vingtCm_);
    uart.transmissionUartVariable("tropLoin_: %d",tropLoin_);
    uart.transmissionUartVariable("tropProche_: %d", tropProche_);
}

CapteurDistance2::CapteurDistance2(Moteur& moteur, uint8_t vingtCm): convertisseurDistance_(), moteur_(moteur), uart(), vingtCm_(vingtCm) {
    tropLoin_ = vingtCm_ /6 * 5;
    tropProche_ = vingtCm_ / 6 * 7;
}

bool CapteurDistance2::detecterMur() {
    uart.transmissionUartPhrase("detecterMur");
    return mesurerDistance() > (vingtCm_ / 2);
}

uint8_t CapteurDistance2::mesurerDistance(){
    can convertisseurDistance2_;
    uart.transmissionUartPhrase("mesurerDistance");
    _delay_ms(10);
    uint8_t distance = convertisseurDistance2_.lecture(PORT_DISTANCE) >> BIT_SHIFT_DEUX;
    uart.transmissionUartPhrase("mesurerDistanceFin");
    return distance;
}

bool CapteurDistance2::suivreMur(){
    uint8_t distance = mesurerDistance();
    uart.transmissionUartVariable("distance: %d", distance);

    if (!detecterMur()){
        //uart.transmissionUartPhrase("PasDeMur");
        moteur_.ajustementPwmPrecis(0, 0, false, false);
        return false;
    }

    else if(distance >= tropLoin_ && distance <= tropProche_) {
        //uart.transmissionUartPhrase("normal");
        moteur_.ajustementPwmPrecis(150, 150, false, false);
    }
    
    else if (distance > tropProche_){
        //uart.transmissionUartPhrase("tropProche");
        uint8_t vitesseGauche = moteur_.getRatioGauche() >= 15 ? moteur_.getRatioGauche() - 15 : 0;
        moteur_.ajustementPwmPrecis(vitesseGauche, moteur_.getRatioDroit(), false, false);
    }

    else {
        //uart.transmissionUartPhrase("TropLoin");
        uint8_t vitesseDroite = moteur_.getRatioDroit() >= 15 ? moteur_.getRatioDroit() - 15 : 0;
        moteur_.ajustementPwmPrecis(moteur_.getRatioGauche(), vitesseDroite, false, false);
    }

    return true;
}