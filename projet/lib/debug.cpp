#include <debug.h>

namespace debug{
    void transmissionDebug(char message[]){
        Uart uart = Uart();
        uart.transmissionUartPhrase(message);
    }
}