 /*
 * main.c
 *
 * Created: 10/22/2026 10:50:XX AM
 *  Author: Gabriel Demossi P. Machado
 */ 

#include <avr/io.h>
#include "avr/interrupt.h"

//Objetivo: Obter uma interrupção com período de 10ms

// ISR do Timer0 no modo CTC, disparada quando TCNT0 == OCR0A.
// A cada ~9,984 ms, esta função é executada.
ISR(TIMER0_COMPA_vect) { //Interrupção de comparação A do timer 0
    // Alterna o estado de PB0 (toggle).
    // Como a ISR ocorre a cada ~10 ms, PB0 alterna a cada ~10 ms,
    // gerando uma onda quadrada de período ~20 ms (~50 Hz) no pino.
    PORTB ^= (1<<PORTB0); //Alterna portb0
}

void TMR0_config() {
    // TCCR0A: WGM01=1 e WGM00=0 → modo CTC.
    // (No modo CTC, o Timer0 é zerado quando TCNT0 == OCR0A.)
    TCCR0A = (1<<WGM01)|(0<<WGM00); //Configuração no modo CTC

    // TCCR0B: WGM02=0, CS02=1, CS01=0, CS00=1 → prescaler = 1024.
    // Clock do timer = 16 MHz / 1024 = 15625 Hz → 64 µs por tick.
    TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00);

    // OCR0A = 155 → (155 + 1) × 64 µs = 9984 µs ≈ 9,984 ms.
    // A interrupção dispara quando TCNT0 atinge OCR0A (modo CTC).
    OCR0A  = 155; //Corresponde a 9,984ms

    // Habilita a interrupção de comparação do canal A do Timer0 (OCIE0A).
    // (Não habilita interrupção de overflow nem de comparação B.)
    TIMSK0 = (1<<OCIE0A); //Habilita interrupção COMPA
}

int main(void) {
    // Configura PB0 como saída (pino do LED/sinal).
    DDRB |= (1<<DDB0); //PORTB0 como saída

    // Configura o Timer0 (CTC, prescaler 1024, OCR0A=155, interrupção habilitada).
    TMR0_config();

    // Habilita interrupções globais.
    sei();

    // Loop principal vazio: tudo é feito na ISR do Timer0.
    while(1) {
        //TODO:: Please write your application code 
    }
}