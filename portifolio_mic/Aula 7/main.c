
/*
 * main.c
 *
 * Created: 9/24/2026 10:22:07 AM
 *  Author: Gabriel Demossi Pereira Machado
 */ 

//#define F_CPU 16000000 
#include <avr/io.h> 	//Mapeamento dos registradores 
#include "util/delay.h"	
#include "avr/interrupt.h"

/**
 * Configuração do módulo USART0 para modo assíncrono, frame de 8 bits, paridade par, BAUD Variável
**/
void UART_config_rx(uint16_t pBAUND) {
	UCSR0A = (0<<U2X0);							//Modo "double speed" desativado
	UCSR0B = (1<<RXEN0)|(0<<TXEN0)|(0<<UCSZ02) // Habilita o receptor UART0, frame de 8 bits
        | (1<<RXCIE0);                          // Habilita interrupção do UART0
	UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)			// Modo assíncrono
		   | (1<<UPM01)|(0<<UPM00)				// Habilita paridade par
		   | (1<<USBS0)							// 2 bits de stop
		   | (1<<UCSZ01)|(1<<UCSZ00)			// Frame de 8 bits
		   | (0<<UCPOL0);						// Polaridade do clock: ignorada
	UBRR0 = (1000000 / pBAUND) - 1; //BAUD de 9600, erro de 0,16%
}

void GPIO_config(){
    DDRC = (1<<DDC0)|(1<<DDC1); //PC0 e PC1 como saída
}

ISR(USART_RX_vect) {
        uint8_t tReceivedByte = UDR0; //Leitura do Buffer
        //Tratamento de mmensagem recebida
        if(tReceivedByte == 'M') {
            PORTC |= (1<<PORTC0); //LED Verde
        } else {
            PORTC |= (1<<PORTC1); //LED vermelho
        }
        _delay_ms(1);
        PORTC = 0; //Apaga todos os LEDs
}
int main(void){
    GPIO_config();
    UART_config_rx(9600);
    sei(); //Habilita interrupção globais
    while(1){
       
    }
}