/*
 * main.c
 *
 * Created: 10/8/2026 8:49:56 AM
 *  Author: Gabriel Demossi P. Machado
 */ 
//#define F_CPU 16000000
#include <avr/io.h>
#include "util/delay.h"
#include "spi.h"
#include "sm28vlt32.h"


int main(void){
	SPI_master_config();
	uint8_t dado;
	dado = 0x45;
    while(1){
        SPI_transceive(dado);
		_delay_ms(1);
    }
}