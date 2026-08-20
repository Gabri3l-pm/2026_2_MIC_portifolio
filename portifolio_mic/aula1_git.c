/*
*main.c
*
*Create: 20/08/2026 12:00:15 AM
* Author: Gabriel Demossi P. Machado
*/
// #define F_CPU 16000000 || O Microship studio pede isso porém o compilador no Tails OS já está configurado para definir isso

#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio #include <xc.h>
#include "util/delay.h"

int main(void){
	DDRB |= 0b00100000; //Define pino PB5 como saída
	while(1) 	// Manter loop para microcontrolador funcionar repetidamente
	{
		PORTB |= 0b00100000; //Atribui nível  lógico alto no pino PB5
		_delay_ms(100);
		PORTB &= 0b11011111; //Atríbui nivel lógico baixo ao pino PB5
		_delay_ms(900);
	}
}
