/*
*main.c
*
*Create: 08/06/2026 11:57:67 AM
* Author: Gabriel Demossi P. Machado
*/
//O Microship studio pede isso porém o compilador já foi previamente configurado #define F_CPU 16000000
#include <avr/io.h> 	//Mapeamento dos registradores || No microship studio #include <xc.h>
#include "avr/interrupt.h"
#include "util/delay.h"

void GPIO_config(){
  //Seleciona os bits e os inverte para nível lógico baixo (Boa prática de código), 0b1111 0000
  DDRB &= ~((1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3)); 
  PORTB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2)|(1<<DDB3); 
  //DDRB = 0xff; Seta todos os pinos da porta D como saída 
  DDRD |= (1<<DDD0)|(1<<DDD1)|(1<<DDD2)|(1<<DDD3)|(1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);                    
  DDRC |= (1<<DDC0); //Pino PC0 como saída
}

void GPIO_incBar(){
  PORTD = PORTD >> 1;
  PORTD |= 0b10000000; //Aciona o bit mais significativo D7
}

void PCINT_config() {
  PCICR |= (1<<PCIE0); //Habilita grupo 0 de vetores
  PCMSK0 |= (1<<PCINT3)|(1<<PCINT2)|(1<<PCINT1)|(1<<PCINT0); //habilita interrupção PCINT-PCIN
}


ISR(PCINT0_vect){
  PORTC |= (1<<PORTC0); //seta pino PC0
  _delay_ms(100);
  PORTC &= ~(1<<PORTC0); //zera pino PC0
  GPIO_incBar();
}

int main(void)
{
  GPIO_config();
  PCINT_config();
  sei();
  
  while(1) 	// Nunca retira esse while, pois o programa depende dele para funcionar
  {
    //TODO
  }
}
