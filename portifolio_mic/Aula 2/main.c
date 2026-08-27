/*
*main.c
*
*Create: 27/08/2026 11:28:56 AM
* Author: Gabriel Demossi P. Machado
*/
// #define F_CPU 16000000 || O Microship studio pede isso porém o compilador no Tails OS já está configurado para definir isso

#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio #include <xc.h>
#include "util/delay.h"

#define BIT0_MASK 0b00000001
#define BIT1_MASK 0b00000010
#define BIT2_MASK 0b00000100
#define BIT5_MASK 0b00100000

int main(void)
{
        // Três forma diferente de setar a direção de PB2 e PB5
	DDRB = (1<<DDB5); // Mesma coisa que escreve DDRB = 0b00100000;
	DDRB = 0b00100000 | 0b00000100; //Resultado 0b00100100
	DDRB = BIT5_MASK | BIT2_MASK;
	while(1)
	{
	}
}
