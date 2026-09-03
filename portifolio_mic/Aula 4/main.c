/*
*main.c
*
*Create: 08/06/2026 11:57:67 AM
* Author: Gabriel Demossi P. Machado
*/
//O Microship studio pede isso porém o compilador já foi previamente configurado
#define F_CPU 16000000
#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio #include <xc.h>
#include "avr/interrupt.h"

ISR(PCINT0_vect){

}

void GPIO_config(){
  //Seleciona os bits e os inverte para nível lógico baixo (Boa prática de código), 0b1111 0000
  DDRB &= ~((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)); 
  DDRB = 0xff; //Seta todos os pinos como saída
  //DDRB |= (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3)(1<<DDD4)(1<<DDD5)(1<<DDD6)|(1<<DDD7);                    
}
int main(void)
{
	while(1) 	// Nunca retira esse while, pois o programa depende dele para funcionar
	{
		//TODO
	}
}
