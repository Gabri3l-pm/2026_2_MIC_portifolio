
/*
 * main.c
 *
 * Created: 9/24/2026 10:22:07 AM
 *  Author: Gabriel Demossi Pereira Machado
 */ 

//#define F_CPU 16000000 
#include <avr/io.h> 	//Mapeamento dos registradores 
#include <string.h>
#include "util/delay.h"	
#include "avr/interrupt.h"

#define	RX_BUFFER_SIZE 16

uint8_t		gMessage[RX_BUFFER_SIZE]; //Buffer global da UART
uint16_t	gRxCounter = 0; //Contagem de bytes recebidos
uint8_t		gMessageReady = 0;

/**
 * Configuração do módulo USART0 para modo assíncrono, frame de 8 bits, paridade par
**/
void UART_config_rx(uint16_t pBAUD) {
	UCSR0A = (0<<U2X0);							//Modo "double speed" desativado
	UCSR0B = (1<<RXEN0)|(0<<TXEN0)|(0<<UCSZ02)  // Habilita o receptor UART0, frame de 8 bits
		   | (1<<RXCIE0);						// Habilita Iterrupção de recepção da UART0
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)			// Modo assíncrono
		   | (1<<UPM01)|(0<<UPM00)				// Habilita paridade par
		   | (1<<USBS0)							// 2 bits de stop
		   | (1<<UCSZ01)|(1<<UCSZ00)			// Frame de 8 bits
		   | (0<<UCPOL0);						// Polaridade do clock: ignorada
	UBRR0 = (1000000 / pBAUD) - 1;
	
}

void GPIO_config() {
	DDRC = (1<<DDC0)|(1<<DDC1); // PC0 e PC1 como saída
}

ISR(USART_RX_vect) {
	uint8_t tReceivedByte = UDR0;	//Leitura do buffer UART
	gMessage[gRxCounter] = tReceivedByte; //Armazena bytes recebidos on buffer
	gRxCounter++;
	if(gRxCounter == RX_BUFFER_SIZE) { //Proteção contra estouro do buffer
		gRxCounter = 0;
	}
	if(tReceivedByte == '\n') {
		gMessageReady = 1; //Flag de mensagem completa
	}
}

int main(void) {
	GPIO_config();
	UART_config_rx(9600);
	sei(); //Habilita interrupções globalmente
    while(1) {
		if(gMessageReady) { //Aguarda mensagem completa
			gMessageReady = 0;
			//Tratamento da mensagem recebida
			if(strcmp(gMessage,"Message") == 0) {
				PORTC |= (1<<PORTC0); //Led Verde
				} else {
				PORTC |= (1<<PORTC1); //Led Vermelho
			}
			_delay_ms(10);
			PORTC = 0; //Apaga todos os LEDs
		}
    }
}