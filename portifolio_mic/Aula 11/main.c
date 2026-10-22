 /*
 * main.c
 *
 * Created: 10/22/2026 10:50:XX AM
 *  Author: Gabriel Demossi P. Machado
 */ 

/*
 * main.c
 *
 * Created: 10/22/2026 10:50:XX AM
 *  Author: Gabriel Demossi P. Machado
 */

#include <avr/io.h>
#include "avr/interrupt.h"
#include <util/atomic.h>   // Para ATOMIC_BLOCK (acesso atômico a gMilliseconds)

// ---------------------------------------------------------------
// Variáveis globais compartilhadas entre ISR e main.
// "volatile" é OBRIGATÓRIO em gMilliseconds porque ela é
// modificada na ISR (TIMER0_COMPA_vect) e lida no main.
// Sem volatile, o compilador pode otimizar/eliminar as leituras
// no main, quebrando a lógica do LED e do RTC em -O1 ou superior.
// ---------------------------------------------------------------
volatile uint16_t gMilliseconds = 0;

// As variáveis do RTC só são modificadas no main,
// então não precisam de volatile. São uint8_t (0–255), suficiente
// para segundos (0–59), minutos (0–59) e horas (0–23).
uint8_t gClockSeconds = 0;
uint8_t gClockMinutes = 0;
uint8_t gClockHours   = 0;

// ---------------------------------------------------------------
// ISR do Timer0 no modo CTC, disparada quando TCNT0 == OCR0A.
// A cada ~9,984 ms, esta função é executada.
//
// Aqui NÃO se alterna PB0 (o comentário anterior estava errado).
// A ISR apenas acumula ~10 ms no contador global de milissegundos.
// ---------------------------------------------------------------
ISR(TIMER0_COMPA_vect) {
    gMilliseconds += 10;
}

void TMR0_config() {
    // TCCR0A: WGM01=1, WGM00=0 → modo CTC.
    // (No modo CTC, o Timer0 é zerado quando TCNT0 == OCR0A.)
    TCCR0A = (1<<WGM01);

    // TCCR0B: WGM02=0, CS02=1, CS01=0, CS00=1 → prescaler = 1024.
    // Clock do timer = 16 MHz / 1024 = 15625 Hz → 64 µs por tick.
    TCCR0B = (1<<CS02)|(1<<CS00);

    // OCR0A = 155 → (155 + 1) × 64 µs = 9984 µs ≈ 9,984 ms.
    // A interrupção dispara quando TCNT0 atinge OCR0A (modo CTC).
    // Observação: como a ISR soma 10 ms, o "segundo" real fica ~0,16%
    // mais rápido (100 ticks ≈ 998,4 ms). Para uso real, ajustar OCR0A
    // para 156 (10,048 ms) ou usar outro prescaler.
    OCR0A = 155;

    // Habilita a interrupção de comparação do canal A do Timer0 (OCIE0A).
    // Usa "|=" para não sobrescrever outros bits de TIMSK0 que possam
    // ser habilitados no futuro (ex.: overflow).
    TIMSK0 |= (1<<OCIE0A);
}

int main(void) {
    // Configura PB0 como saída (pino do LED/sinal).
    DDRB |= (1<<DDB0);

    // Configura o Timer0 (CTC, prescaler 1024, OCR0A=155, OCIE0A).
    TMR0_config();

    // Habilita interrupções globais.
    sei();

    // Loop principal: lógica de LED e RTC baseada em gMilliseconds.
    while(1) {
        // -----------------------------------------------------------
        // Leitura atômica de gMilliseconds.
        // gMilliseconds é uint16_t e é modificada na ISR; no AVR (8 bits)
        // a leitura de 2 bytes NÃO é atômica. Se a ISR disparar entre a
        // leitura do low e do high byte, o valor fica corrompido.
        // ATOMIC_BLOCK garante que a cópia seja feita sem interrupção.
        // -----------------------------------------------------------
        uint16_t ms;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            ms = gMilliseconds;
        }

        // -----------------------------------------------------------
        // LED blink NÃO bloqueante.
        // Enquanto ms estiver entre 0 e 99, PB0 fica setado.
        // Como o contador é consumido em passos de 1000 (RTC abaixo),
        // o LED fica:
        //   - aceso   por 100 ms  (ms = 0..99)
        //   - apagado por ~900 ms (ms = 100..999)
        // Duty cycle ≈ 10%, período ≈ 1 s.
        //
        // Observação: como esta checagem ocorre ANTES do "-= 1000"
        // do RTC, no instante em que ms chega a 1000 o LED fica
        // apagado por ~10 ms extras até o RTC zerar o contador.
        // Comportamento real: ~100 ms aceso / ~910 ms apagado.
        // -----------------------------------------------------------
        if(ms < 100) {
            PORTB |= (1<<PORTB0);    // Seta PB0
        } else {
            PORTB &= ~(1<<PORTB0);   // Zera PB0
        }

        // -----------------------------------------------------------
        // Lógica do RTC (Real Time Clock).
        // Quando o contador de milissegundos chega a 1000, subtrai
        // 1000 (rollover) e incrementa os segundos. Em cascata, faz
        // o "carry" para minutos e horas.
        //
        // Usa "while" em vez de "if" para consumir TODOS os ticks
        // pendentes caso o loop atrase e gMilliseconds pule mais de
        // 1000 em uma iteração (evita atraso/perda de segundos).
        // -----------------------------------------------------------
        while(ms >= 1000) {
            ms -= 1000;

            // Consome 1 segundo do contador global.
            // O -= é read-modify-write de 16 bits → precisa de atomicidade.
            ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
                gMilliseconds -= 1000;
            }

            // Incrementa segundos e faz carry para minutos/horas.
            gClockSeconds++;
            if(gClockSeconds >= 60) {
                gClockSeconds = 0;              // Zera segundos
                gClockMinutes++;                // Incrementa minutos
                if(gClockMinutes >= 60) {
                    gClockMinutes = 0;          // Zera minutos
                    gClockHours++;              // Incrementa horas
                    if(gClockHours >= 24) {
                        gClockHours = 0;        // Zera horas (meia-noite)
                    }
                }
            }
        }
    }
}