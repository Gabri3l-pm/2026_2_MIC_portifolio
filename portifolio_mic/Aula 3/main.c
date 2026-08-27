/*
*main.c
*
*Create: 08/27/2026 13:26:17 AM
* Author: Gabriel Demossi P. Machado
*/
// #define F_CPU 16000000 || O Microship studio pede isso porém o compilador no Tails OS já configura

#include <avr/io.h> //Mapeamento dos registradores
#include "avr/interrupt.h" //Habilita interrupção global
#include "util/delay.h"

ISR(INT0_vect){ //Função no vetor de interrupção INT0
  PORTB |= (1<<PORTB1);// Acende LED em PB1
  _delay_ms(50);
  PORTB &= ~(1<<PORTB1); // Apaga LED em PB1
}

ISR(INT1_vect){ //Função no vetor de interrupção INT0
  PORTB |= (1<<PORTB1);// Acende LED em PB1
  _delay_ms(100);
  PORTB &= ~(1<<PORTB1); // Apaga LED em PB1
}

void INT_config(){
  EICRA = (1<<ISC11)|(1<<ISC10)  //configura INT1 na borda de subida
        | (1<<ISC01)|(0<<ISC00); //configura o INT0 na borda de decida 
  EIMSK = (1<<INT1)|(1<<INT0); //Habilita INT0
}

void GPIO_config(){
  DDRB |= (1<<DDB0); // Pino PB0 como saída
  DDRB |= (1<<DDB1); //Habilita PB1
}

int main(void){
  GPIO_config();
  INT_config();  
  sei(); //Habilita interrupção global
  while(1) { // loop do microprocessador
    cli(); //Desabilita interrupção temporariamente
    PORTB |= (1<<PORTB0);
    _delay_ms(100);
    sei(); //Habilita interrupção global
    PORTB &= ~(1<<PORTB0);
    _delay_ms(100);
  }
} 

