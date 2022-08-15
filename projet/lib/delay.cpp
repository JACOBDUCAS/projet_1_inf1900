#include "delay.h"
#include <util/delay.h>
Delay::Delay() {}

void Delay::delayDynamique(uint8_t duree)
{
    for(int i = 0; i < duree*25; i++)
    {
        _delay_ms(1);
    }
}
