/*
 * main.c
 *
 * Created: 11/19/2026 08:20:66
 * Author: Gabriel Demossi Pereira Machado
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

volatile uint16_t tADCData;
volatile uint16_t x[4] = {0, 0, 0, 0};
volatile int32_t  y[4] = {0, 0, 0, 0};

const int16_t b[3] = {1372, 2744, 1372};
const int16_t a[2] = {-16899, 6002};

ISR(ADC_vect)
{
    // Marca entrada da ISR para debug no PB1
    PORTB |= (1 << PORTB1);

    tADCData = ADC;

    // Linha de atraso do sinal x
    x[2] = x[1];
    x[1] = x[0];
    x[0] = tADCData;

    // Linha de atraso do sinal y
    y[2] = y[1];
    y[1] = y[0];

    // Filtro IIR com casts para evitar overflow em 16 bits
    int32_t y_new = (int32_t)b[0] * (int32_t)x[0]
                  + (int32_t)b[1] * (int32_t)x[1]
                  + (int32_t)b[2] * (int32_t)x[2]
                  - (int32_t)a[0] * y[1]
                  - (int32_t)a[1] * y[2];

    y[0] = y_new;

    // Compara a saída do filtro, não o array
    if (y[0] < 678) {
        PORTB |= (1 << PORTB0);
    } else {
        PORTB &= ~(1 << PORTB0);
    }

    // Marca saída da ISR
    PORTB &= ~(1 << PORTB1);
}

void ADC_config(void)
{
    ADMUX = (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0) // ADC0
          | (1 << REFS1) | (1 << REFS0);                          // Ref. interna 1,1 V

    // Trigger por Timer1 Compare Match B
    ADCSRB = (1 << ADTS2) | (0 << ADTS1) | (1 << ADTS0);

    ADCSRA = (1 << ADEN)  | (1 << ADATE) | (1 << ADIE)            // habilita ADC, auto-trigger e interrupção
           | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);          // prescaler 128

    DIDR0 = (1 << ADC0D); // desabilita entrada digital do PC0
}

void TMR1_config(void)
{
    TCCR1A = 0;

    // Modo CTC com top em OCR1A, prescaler 1
    TCCR1B = (1 << WGM12) | (1 << CS10);

    OCR1A = 15999; // 16 MHz / 16000 = 1 kHz -> 1 ms
    OCR1B = 0;     // Compare B em 0 -> dispara 1x por período
}

int main(void)
{
    DDRB |= (1 << DDB0) | (1 << DDB1);

    ADC_config();
    TMR1_config();

    // Inicia a primeira conversão no modo auto-trigger
    ADCSRA |= (1 << ADSC);

    sei();

    while (1) {
        // Conversões disparadas automaticamente pelo Timer1
    }
}