/*
 * main.c
 *
 * Created: 10/22/2026 8:20:XX AM
 *  Author: Gabriel Demossi Pereira Machado
 */ 
#include <avr/io.h>
#include "avr/interrupt.h"
#include "util/delay.h"

// Interrupção do TWI/I2C.
// Sempre que ocorre um evento no barramento, o código entra aqui.
ISR(TWI_vect) {
    // Acende PB0 para indicar que entrou na ISR do TWI.
    PORTB |= (1<<PORTB0);
    // Lê o status do TWI e mascara os bits menos significativos.
    // TWSR & 0b11111000 pega apenas o código de status.
    uint8_t tStatusCode = (TWSR & 0b11111000);
    switch(tStatusCode) {
        case (0x60): {
            // 0x60 = próprio endereço SLA+W recebido, ACK retornado.
            // Acende PB1.
            PORTB |= (1<<PORTB1);
            // Limpa TWINT, mantém TWI e interrupção habilitados
            // e habilita ACK para continuar recebendo.
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        } break;
        case (0x80): {
            // 0x80 = byte de dados recebido, ACK retornado.
            // Acende PB2.
            PORTB |= (1<<PORTB2);
            // Lê o byte recebido.
            // Neste código o byte é lido, mas não é armazenado nem usado.
            uint8_t tReceivedByte = TWDR;
            // Prepara para receber o próximo byte com ACK.
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT)|(1<<TWEA);
        } break;

        case (0xA0): {
            // 0xA0 = STOP ou repeated START recebido enquanto escravo.
            // Acende PB3.
            PORTB |= (1<<PORTB3);

            // Limpa TWINT.
            // Aqui não coloca TWEA, então não gera ACK nesse evento.
            TWCR = (1<<TWEN)|(1<<TWIE)|(1<<TWINT);
        } break;

        default: {
            // Qualquer outro status: acende PB4.
            PORTB |= (1<<PORTB4);
        }
    }

    // Espera 1 ms para o LED ficar visível.
    // Observação: usar _delay_ms dentro de ISR não é boa prática,
    // pois bloqueia a CPU e atrasa outras interrupções.
    _delay_ms(1);

    // Apaga todos os LEDs conectados ao PORTB.
    PORTB = 0;
}

// Configura o TWI como escravo.
void TWI_slave_config(uint8_t pAddress) {
    // Configura PC4 e PC5 como entradas.
    // No ATmega328, PC4 = SDA e PC5 = SCL.
    DDRC &= ~((1<<DDC5)|(1<<DDC4));

    // TWBR só tem efeito prático no modo mestre.
    // Como aqui é escravo, esse valor normalmente não importa.
    TWBR = 64;

    // Configura o endereço do escravo.
    // O endereço é deslocado para a esquerda: 10<<1 = 0x14.
    TWAR = pAddress<<1;

    // Habilita o TWI e a interrupção do TWI.
    // Observação: não habilita TWEA aqui.
    TWCR = (1<<TWEN)|(1<<TWIE);
}

int main(void) {
    // Configura PB0 até PB4 como saídas para os LEDs de sinalização.
    DDRB |= (1<<DDB4)|(1<<DDB3)|(1<<DDB2)|(1<<DDB1)|(1<<DDB0);

    // Configura o TWI como escravo no endereço 10.
    TWI_slave_config(10);

    // Habilita interrupções globais.
    sei();

    // Loop principal vazio: todo o trabalho ocorre na ISR do TWI.
    while(1) {
        //
    }
}