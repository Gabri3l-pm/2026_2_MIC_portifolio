/*
*main.c
*
*Create: 09/10/2026 11:12:21 AM
* Author: Gabriel Demossi P. Machado
*/
//#define F_CPU 16000000
#include <avr/io.h> //Mapeamento dos registradores || No microship studio #include <xc.h>
#include "util/delay.h"

const uint8_t sequencia[] = {
    0b00011000, // Acende os 2 LEDs do centro
    0b00100100, // Expande para fora
    0b01000010, 
    0b10000001, // Chega nas pontas
    0b01000010, // Retorna para o centro
    0b00100100,
    0b01010101, // Alternado 1
    0b10101010  // Alternado 2
};

int main() {
    DDRD = 0xFF; // Configura PORTD como saída [passage 928, 948]

    while (1) {
        for (uint8_t i = 0; i < 8; i++) {
            PORTD = (uint8_t)~sequencia[i]; // Aplica a máscara invertida [passage 884, 949]
            _delay_ms(150);
        }
    }
}


