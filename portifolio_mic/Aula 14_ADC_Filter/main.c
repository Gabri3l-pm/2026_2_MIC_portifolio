/*
 * main.c
 *
 * Created: 11/19/2026 08:20:66
 * Author: Gabriel Demossi Pereira Machado
 */

//#define F_CPU 16000000  // compilador já adiciona essa definição
#include <avr/io.h>
#include <avr/interrupt.h>          // necessário para ISR() e sei()

volatile uint16_t tADCData;         // precisa ser global e volatile (usada na ISR)
volatile uint16_t x[4] = {0, 0, 0, 0};   // declarado e inicializado

ISR(TIMER1_COMPB_vect){
    PORTB ^= (1<<PORTB1);           // Debug: visualizar o tempo do TMR1
}

ISR(ADC_vect){ // Interrupção para  visualizar o tempo TIME1
    PORTB |= (1<<PORTB1);
    tADCData = ADC;
    // Acende o LED: 728 mV -> 728/1100 = 66,2% * 1024 ≈ 678
    x[3] = x[2]; //Linha de atraso do sinal X (fila FIFO)
    x[2] = x[1];
    x[1] = x[0];
    uint16_t y = (x[0] + x[1] + x[2] + x[3]) / 4; //Filtro média movel de ordem 4
    if(y < 678){
        PORTB |= (1<<PORTB0);
    }
    else{
        PORTB &= ~(1<<PORTB0);
    }
    PORTB ^= ~(1<<PORTB1);
}

void ADC_config(){
    ADMUX = (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0) // Canal ADC0
            | (1<<REFS1) | (1<<REFS0);                    // Ref interna 1.1 V
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADIE)           // CORREÇÃO: ADATE=1 (auto-trigger) e ADIE=1
            | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);       // Prescaler 128 (125 kHz)
    DIDR0 = (1<<ADC0D);                                   // Desabilita digital do PC0
    ADCSRB = (1<<ADTS2) | (0<<ADTS1) | (1<<ADTS0);        // Trigger: Timer1 Compare Match B 
}

// Configuração do Timer1 como fonte de disparo
void TMR1_config(){
    TCCR1A = (0<<WGM11) | (0<<WGM10);
    TCCR1B = (0<<WGM13) | (1<<WGM12)                      // Modo 4: CTC com top em OCR1A
            | (0<<CS12) | (0<<CS11) | (1<<CS10);          // CORREÇÃO: CS12/CS11/CS10 (era CS11 duplicado) e prescaler 1
    OCR1A = 15999;                                        // 16 MHz / 16000 = 1 kHz → 1 ms
    OCR1B = 0;                                            // Compare B em 0 → dispara 1x por período
    TIMSK1 = (1<<OCIE1B);
}

int main(void){
    DDRB |= (1<<DDB0) | (1<<DDB1);   // PB1 (debug) também precisa ser saída
    ADC_config();
    TMR1_config();
    sei();                           // habilita interrupções globais
    while(1){
        // Conversões disparadas automaticamente pelo Timer1 (ADATE=1)
    }
}