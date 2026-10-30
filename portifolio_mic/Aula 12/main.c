/*
 * main.c
 *
 * Created: 10/29/2026 19:40:24 AM
 *  Author: Gabriel Demossi Pereira Machado
 */

#define F_CPU 16000000
#include <xc.h>
#include "util/delay.h"

void PWM_config() {
	TCCR0A	= (1<<COM0A1)|(0<<COM0A0)	// Saída OCR0A não-inversora
			| (1<<COM0B1)|(1<<COM0B0)	// Saída OCR0B inversora
			| (1<<WGM01)|(1<<WGM00);	// Modo Fast PWM
	TCCR0B	= (0<<WGM02)|(0<<CS02)|(0<<CS01)|(1<<CS00);			//Habilita timer com Prescaler de 1x
	OCR0A	= 163;			//Razão de ciclo de 40% em OC0A
	OCR0B	= 76;			//Razão de ciclo de 30% em OC0B
	DDRD	= (1<<DDD6)|(1<<DDD5);	//Configura pinos de saída pwm OC0A e OC0B
}

void PWM_setVoltageA(uint16_t pMillivolts) {
	if(pMillivolts == 0) { //Proteção contra parâmetros fora da faixa útil
		OCR0A = 0;
	} else
	if(pMillivolts >= 5000) {
		OCR0A = 0xff;
	} else {
		//Ver: cast estático em linguagem C
		OCR0A = (uint8_t)(0x000000FF & (((uint32_t) pMillivolts * 256) / 5000));
	}
}

int main(void) {
	PWM_config();
	PWM_setVoltageA(2500);
    while(1) {
		PWM_setVoltageA(2500); //Fase 0º
		_delay_ms(1);
		PWM_setVoltageA(3750); //Fase 30º
		_delay_ms(1);
		PWM_setVoltageA(4665); //Fase 60º
		_delay_ms(1);
		PWM_setVoltageA(5000); //Fase 90º
		_delay_ms(1);
		PWM_setVoltageA(4665);
		_delay_ms(1);
		PWM_setVoltageA(3750);
		_delay_ms(1);
		PWM_setVoltageA(2500);
		_delay_ms(1);
		PWM_setVoltageA(1250);
		_delay_ms(1);
		PWM_setVoltageA(335);
		_delay_ms(1);
		PWM_setVoltageA(0);
		_delay_ms(1);
		PWM_setVoltageA(335);
		_delay_ms(1);
		PWM_setVoltageA(1250);
		_delay_ms(1);
    }
}
/*
feat(pwm): gera onda senoidal aproximada em OC0A via Fast PWM

Configura o Timer0 do ATmega328P (16 MHz) em modo Fast PWM com TOP=0xFF
e prescaler 1x, resultando em uma portadora de ~62,5 kHz. O canal OC0A
(PD6) opera em modo não-inversor e o OC0B (PD5) em modo inversor.

Adiciona a função PWM_setVoltageA(mV), que converte linearmente uma
tensão entre 0 e 5000 mV para o registrador OCR0A (0–255), com
saturação nos extremos. A conversão usa uint32_t intermediário para
evitar overflow em pMillivolts * 256.

O main() percorre 12 valores de tensão em intervalos de 1 ms,
reproduzindo uma senoide discretizada (offset 2,5 V, amplitude 2,5 V)
com frequência aproximada de 83 Hz na saída média do pino OC0A.

Notas:
- OCR0A = 163 corresponde a ~64% de duty (comentário original dizia 40%)
- O timing usa _delay_ms, portanto a frequência não é precisa
- OC0A e OC0B não possuem dead-time entre si
*/