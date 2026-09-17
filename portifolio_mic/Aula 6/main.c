/*
*main.c
*
*Create: 09/17/2026 10:13:34 AM
* Author: Gabriel Demossi P. Machado
*/

//#define F_CPU 16000000 
#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio 
#include "util/delay.h"

// Configuração de modulo de paridade
void UART_config() {
	UCSR0A = (0<<U2X0);  // Ignorado  por enquanto 
	UCSR0B = (0<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02); // Habilita o transmissor UART0, frame de 8 bits
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00) // Modo assincrono
				|(1<<UPM01)|(0<<UPM00) //Habilita paridade
				|(0<<USBS0) //1 Bit de stop
				|(1<<UCSZ01)|(1<<UCSZ00) //Frame de 8 bits
				|(1<<UCPOL0); // Polaridade do clock: ignorada
	UBRR0 = (103); //BAUD DE 9600(freq. de bits por seg.)ERRO DE 0.16%
}
// Envia um byte na USATR

void UART_send(){
	UDR0 = 0x53; //Envia umm byte exemplo
	_delay_ms(2);
}
int main(void)
{
	UART_config();
	while(1) 	// Nunca retira esse while, pois o programa depende dele para funcionar
	{
		UART_send();
	}
}