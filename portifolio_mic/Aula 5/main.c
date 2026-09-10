/*
*main.c
*
*Create: 09/10/2026 11:12:21 AM
* Author: Gabriel Demossi P. Machado
*/
//#define F_CPU 16000000
#include <avr/io.h> //Mapeamento dos registradores || No microship studio #include <xc.h>
#include "util/delay.h"

void GPIO_config () {
  DDRB |= (1<<DDB5) ; // Pino PB5 (13) como saida
}
int main(void)
{
  GPIO_config();
  while(1)//Nunca retira esse while, pois o programa depende dele para funcionar
  {
    PORTB ^= (1<<PORTB5); //Alterna o LED
    _delay_ms(500);
  }
}
