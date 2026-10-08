/*
 * SPI.c
 *
 * Created: 10/8/2026 10:35:64 AM
 *  Author: Gabriel Demossi P. Machado
 */ 

 #include <avr/io.h>

 void SPI_master_config(){
	SPCR = (1<<SPE) | (0<<DORD) | (1<<MSTR) //habilita SPI, MSB primeiro(padrão)
	| (0<<CPOL) | (0<<CPHA) //modo 0 de operação
	|(0<<SPR1)| (0<<SPR0); // velocidade de fosc/2 , SPI->8MHz
	SPSR = (1<<SPI2X); //operação de velocidade
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