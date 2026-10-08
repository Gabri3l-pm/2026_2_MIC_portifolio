/*
 * main.c
 *
 * Created: 10/8/2026 8:49:56 AM
 *  Author: Gabriel Demossi P. Machado
 */ 
//#define F_CPU 16000000
#include <avr/io.h>
#include "util/delay.h"

void SPI_master_config() {
	SPCR = (1<<SPE)|(0<<DORD)	// Habilita SPI, Ordem MSB primeiro (padrão)
		 | (1<<MSTR)			// Modo Mestre
		 | (0<<CPOL)|(0<<CPHA)	// SPI modo 0
		 | (0<<SPR1)|(0<<SPR0);	// Divisor fosc/2, SCK->8MHz
	SPSR = (1<<SPI2X);			// Velocidade dobrada
	DDRB = (1<<DDB3) | (1<<DDB5); //// MOSI e SCK como saída; MISO permanece como entrada
	DDRC =	(1<<DDC0); //usando PC0 como Slave Selectd (saída)
	PORTC |= (1<<PORTC0); //define nivel alto no PC0
}

uint8_t SPI_transceive(uint8_t pTxByte){
	uint8_t tReceivedByte;
	PORTC &= ~(1<<PORTC0); //Sleve select em nivel baixo
	SPDR = pTxByte; // escrita no SPDR dispara a transação
	while((SPSR & (1<<SPIF)) == 0); //isola a flag, espera a flag SPIF subir
	tReceivedByte = SPDR; //leitura de dados
	PORTC |= (1<<PORTC0); //sleve selectd em nivel alto
	return tReceivedByte;
	
}
int main(void){
	SPI_master_config();
	uint8_t dado;
	dado = 0x45;
    while(1){
        SPI_transceive(dado);
		_delay_ms(1);
    }
}