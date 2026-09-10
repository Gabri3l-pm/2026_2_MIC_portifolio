/*
*main.c
*
*Create: 09/10/2026 11:12:21 AM
* Author: Gabriel Demossi P. Machado
*/
//#define F_CPU 16000000
#include <avr/io.h> //Mapeamento dos registradores || No microship studio #include <xc.h>
#include "util/delay.h"

void GPIO_incBar(){
  PORTD = PORTD >> 1;
  PORTD |= 0b10000000;
}

void GPIO_decBar(){
  PORTD = PORTD << 1;
}

void GPIO_config () {
  DDRD = 0xFF; //Pinos de saída para os LEDs
  PORTD = 0x00; // Barra apaga
}
int main(void)
{
  GPIO_config();
  while(1)//Nunca retira esse while, pois o programa depende dele para funcionar
  {
    for ( uint8_t i = 0; i < 8; i ++){
      GPIO_incBar();
      _delay_ms (200);
    }
    for ( uint8_t i = 0; i < 8; i ++){
      GPIO_decBar();
      _delay_ms (200);
    }
    
  }
}
