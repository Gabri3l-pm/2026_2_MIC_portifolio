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
	DDRB = (1<<DDB3)|(1<<DDB5);	// Config dos pinos MOSI e SCK como saídas
}

int main(void) {
	SPI_master_config();
    while(1) {
        SPDR = 0xC7;
		_delay_ms(1);
    }
}