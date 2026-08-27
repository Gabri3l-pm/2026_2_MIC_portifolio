/*
*main.c
*
*Create: 08/27/2026 13:26:17 AM
* Author: Gabriel Demossi P. Machado
*/
// #define F_CPU 16000000 || O Microship studio pede isso porém o compilador no Tails OS já configura


#include <avr/io.h>        // Mapeamento de registradores
#include <avr/interrupt.h> // Habilita o uso de ISR e registradores de interrupção
#include <util/delay.h>    // Biblioteca de temporização do AVR[cite: 1]

// ISR para o vetor de interrupção INT0 (Disparada no pino PD2)
ISR(INT0_vect) {
    PORTB ^= (1 << PORTB1); // Inverte o estado do LED no pino PB1 a cada pulso
}

void GPIO_config(void) {
    // Configura os pinos PB0 e PB1 do PORTB como Saídas (1)
    DDRB |= (1 << DDB0) | (1 << DDB1);
    // Configura o pino PD2 (INT0) do PORTD como Entrada (0)
    DDRD &= ~(1 << DDD2);
    // Ativa o resistor de Pull-up interno no pino PD2
    PORTD |= (1 << PORTD2);
}

void INT_config(void) {
    // Configura o acionamento do INT0 para Borda de Descida (Falling Edge)
    EICRA = (1 << ISC01)|(0 << ISC01);
    // Habilita a interrupção externa INT0 no registrador de máscara
    EIMSK = (1 << INT0);
}

int main(void) {
    GPIO_config(); // Configura direções e estados iniciais dos pinos
    INT_config();  // Configura os registradores da interrupção externa

    sei(); // Habilita globalmente as interrupções (SREG)

    while (1) {
        _delay_ms(100);          // Delay de 100 milissegundos
        PORTB ^= (1 << PORTB0);  // Inverte a saída PB0 (LED piscando continuamente)
    }
}

/*
 #include <avr/io.h> //Mapeamento dos registradores
#include "avr/interrupt.h" //Habilita interrupção global
#include "util/delay.h"

ISR(INT0_vect){ //Função no vetor de interrupção INT0
  PORTB |= (1<<PORTB1);// Acende LED em PB1
  _delay_ms(10);
  PORTB &= ~(1<<PORTB1); // Apaga LED em PB1
}

void GPIO_config(){
  DDRB |= (1<<DDB0); // Pino PB0 como saída
  DDRB |= (1<<INT0); //Habilita INT0
}

void INT_config(){
  EICRA = (1<<ISC01)|(0<<ISC00);//configura o INT0 na borda de decida 
  EIMSK = (1<<INT0); //Habilita INT0
}

int main(void){
  GPIO_config();
  INT_config();  
  sei(); //Habilita interrupção global
  
  while(1) { // loop do microprocessador
    _delay_ms(100);
    PORTB ^= (1<<PORTB0);
  }
} 
*/
