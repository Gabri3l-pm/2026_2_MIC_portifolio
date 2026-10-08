/*
 * SM28VLT32.c
 *
 * Created: 10/8/2026 10:36:12 AM
 *  Author: Gabriel Demossi P. Machado
 */ 

#include <avr/io.h>
#include "SPI.h"

void SM28VLT32_config() {
	DDRC = (1<<DDC0);			// Usando PC0 como Slave Select (Saída)
	PORTC |= (1<<PORTC0);		// Slave select em nível alto
}

uint16_t SM28VLT32_readWord(uint32_t pAddress) {
	uint8_t tAddressByte2 = (pAddress & 0x00FF0000) >> 16;
	uint8_t tAddressByte1 = (pAddress & 0x0000FF00) >> 8;
	uint8_t tAddressByte0 = (pAddress & 0x000000FF) >> 0;
	uint8_t tDataByte1;
	uint8_t tDataByte0;
	uint16_t tDataWord;
	
	PORTC &= ~(1<<PORTC0);				// Slave select em nível baixo
	SPI_transceive(0x15);				//Comando 'Read Word'
	SPI_transceive(tAddressByte2);
	SPI_transceive(tAddressByte1);
	SPI_transceive(tAddressByte0);
	tDataByte1 = SPI_transceive(0x00);
	tDataByte0 = SPI_transceive(0x00);
	SPI_transceive(0x00);				//Dummy
	PORTC |= (1<<PORTC0);				// Slave select em nível alto
	tDataWord = ((uint16_t) tDataByte1) << 8 //0xAB
			  | ((uint16_t) tDataByte0) << 0;//0xCD
	return tDataWord;
}