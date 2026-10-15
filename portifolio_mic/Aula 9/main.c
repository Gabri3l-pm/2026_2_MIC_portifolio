/*
 * main.c
 *
 * Created: 10/15/2026 8:59:68 AM
 *  Author: Gabriel Demossi Pereira Machado
 */ 

//#define F_CPU 16000000 compilador já adiciona essa definição
#include <avr/io.h>
#include "util/delay.h"

void TWI_master_config() {
	TWBR = 64;						//Divisor 64, fosc=100kHz
	TWSR |= (0<<TWPS1)|(0<<TWPS0);	//Prescaler de 1
	TWCR |= (1<<TWEN);				//Habilita o periférico
}

int main(void) {
	TWI_master_config();
	_delay_ms(1000);
	uint8_t tSlaveAddress = 10;
    while(1) {
        //Iniciar uma transmissão como mestre
		TWCR |= (1<<TWSTA);						//Inicia condição de start
		while((TWCR & (1<<TWINT)) == 0);		//Aguarda flag de interrupção subir
		uint8_t tStatusCode = (TWSR & 0b11111000);
		if(tStatusCode == 0x08) {				//Condição de start ok
			//Iniciar escrita no escravo 10
			TWDR = (tSlaveAddress<<1)|0;		//Endereço + flag R/W (1 para leitura / 0 para escrita)
			TWCR = (1<<TWINT)|(1<<TWEN);		// Limpar flag de interrupção
			while((TWCR & (1<<TWINT)) == 0);	//Aguarda conclusão da transmissão
			tStatusCode = (TWSR & 0b11111000);	//Novo código de estado
			if(tStatusCode == 0x18) {
				//Ack recebido
			} else 
			if (tStatusCode == 0x28){
				//Nack recebidio (ou ack não recebido)
			} else {
				//outro erro
			}
		} else {
			//algum erro...
		}
		_delay_ms(100);
    }
}