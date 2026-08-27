/*
*main.c
*
*Create: 27/08/2026 12:02:45 AM
* Author: Gabriel Demossi P. Machado
*/

// #define F_CPU 16000000 || O Microship studio pede isso porém o compilador no Tails OS já está configurado para definir isso

#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio #include <xc.h>
#include <stdint.h> //boa prática a adição dessa biblioteca
#include "util/delay.h"

// Controle de vários LEDs com operações de Shift

void GPIO_config()
{
  DDRB = (1<<DDB6)|(1<<DDB4)|(1<<DDB2)|(1<<DDB0); 
  //BITS pares estão config com shift || DDRB = 0b01010101;
}

int main(void)
{
  GPIO_config();
  uint8_t tData = 0x03; //0b0000 0011;
  while(1)
  {
    PORTB = tData;
    _delay_ms(1000);
    tData = tData << 1; //0b0000 0110, 0b0000 1100, 0b0001 1000, ...
    if(tData == 0)
    {
      tData = 0x03;
    }
  }
}
