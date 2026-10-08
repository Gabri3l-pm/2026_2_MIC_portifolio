/*
 * SPI.c
 *
 * Created: 10/8/2026 10:35:64 AM
 *  Author: Gabriel Demossi P. Machado
 */ 

#include <avr/io.h>

void SPI_master_config() {
	SPCR = (1<<SPE)|(0<<DORD)	// Habilita SPI, Ordem MSB primeiro (padrão)
	| (1<<MSTR)			// Modo Mestre
	| (0<<CPOL)|(0<<CPHA)	// SPI modo 0
	| (0<<SPR1)|(0<<SPR0);	// Divisor fosc/2, SCK->8MHz
	SPSR = (1<<SPI2X);			// Velocidade dobrada
	DDRB = (1<<DDB3)|(1<<DDB5);	// Config dos pinos MOSI e SCK como saídas
}

uint8_t SPI_transceive(uint8_t pTxByte) {
	uint8_t tReceivedByte;
	SPDR = pTxByte;					// Escrita no SPDR dispara a transação
	while((SPSR & (1<<SPIF)) == 0); // Espera a flag SPIF subir
	tReceivedByte = SPDR;			// Leitura do registrador de dados
	return tReceivedByte;
}