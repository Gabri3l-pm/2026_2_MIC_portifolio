
/*
 * main.c
 *
 * Created: 9/24/2026 08:15:07 AM
 *  Author: Gabriel Demossi Pereira Machado
 */ 

//#define F_CPU 16000000 
#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio 
#include "util/delay.h"	

char gMessage[16] = "Mensagem"; //"Null terminated strings" (8 bytes + 1 terminação nula)

/**
 * Configuração do módulo USART0 para modo assíncrono, frame de 8 bits, paridade par, BAUD de 9600
**/
void UART_config() {
	UCSR0A = (0<<U2X0);							//Modo "double speed" desativado
	UCSR0B = (0<<RXEN0)|(1<<TXEN0)|(0<<UCSZ02); // Habilita o transmissor UART0, frame de 8 bits
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)			// Modo assíncrono
		   | (1<<UPM01)|(0<<UPM00)				// Habilita paridade par
		   | (1<<USBS0)							// 2 bits de stop
		   | (1<<UCSZ01)|(1<<UCSZ00)			// Frame de 8 bits
		   | (0<<UCPOL0);						// Polaridade do clock: ignorada
	UBRR0 = 103; //BAUD de 9600, erro de 0,16%
}

/**
 *	Envia um byte na USART0
**/
void UART_send_byte(uint8_t pByte) {
	// Aguardar espaço no buffer (semáforo)
	while((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = pByte; //Envia o byte do parâmetro
}

void UART_send_string(char *pString) {
	char *tMessagePtr = pString;
	while(*tMessagePtr != 0) {
		UART_send_byte(*tMessagePtr);
		tMessagePtr++;
	}
}

int main(void) {
	UART_config();
    while(1) {
        UART_send_string(gMessage);
		_delay_ms(5); //Tempo de silêncio
    }
}