#include "uart.h"

#include <avr/io.h>
Uart::Uart() {
    initialisationUart();
}
void Uart::initialisationUart() {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (0 << UMSEL00) | (1 << UCSZ01) | (1 << UCSZ00);
}

void Uart::transmissionUart(unsigned char donnee) {
    while (!(UCSR0A & (1 << UDRE0))) {
    }
    UDR0 = donnee;
}

void Uart::transmissionUartPhrase(const char* donnee) {
    uint8_t i = 0;
    while (donnee[i] != '\0') {
        while (!(UCSR0A & (1 << UDRE0))){}
        UDR0 = donnee[i];
        i++;
    }
    while (!(UCSR0A & (1 << UDRE0))){}
    UDR0 = '\n';
}

void Uart::transmissionUartVariable(const char* donnee, uint16_t variable){
    char buffer[50];
    sprintf(buffer, donnee, variable);
    transmissionUartPhrase((char*)buffer);
}

unsigned char Uart::receptionUart(void){
    while (!(UCSR0A & (1<<RXC0))){}

    return UDR0;
}