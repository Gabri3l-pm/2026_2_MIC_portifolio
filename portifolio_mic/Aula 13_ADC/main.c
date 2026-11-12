/*
*main.c 
*
*Created: 11/12/2026 10:15:18
* Author: Gabriel Demossi Pereira Machado
*/

//#define F_CPU 16000000 compilador já adiciona essa definição
#include <avr/io.h>

void ADC_config(){
    ADMUX = (1<<MUX3) | (1<<MUX1) | (1<<MUX0) //Canal de conversão pino adc7
            | (1<<REFS0) | (1<<REFS1); //Canal de referência internade 1.1V
    ADCSRA = (1<<ADEN) | (0<<ADATE) // Habilita o modo manual do ADC
            | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);  //Fator de preescaler 128(125Khz)
}



int main(void){
    DDRB |= (1<<DDB0);
    uint16_t tADCData = ADC;
    ADC_config();
    while(1){
        ADCSRA |= (1<<ADSC); // inicia uma conversão
        while((ADCSRA & (1<<ADIF)) == 0) // Segura o processameto aé que o processo seja concluido
        //Acesso como um único registrador 16 bits ao ADCH e ADCl (Faixa de 0 a 1023) ADLAR = 0;
        tADCData = ADC;
        //Acende o LED, a 728mV 728/1100 = 65,4% * 1024 -> 670
        if(tADCData < 670){
            PORTB |= (1<<PORTB0);
        }
        else{
            PORTB &= ~(1<<PORTB0);
        }
    }
}

//Pré escaler é definido no regitrador ADCSRA e para subdividir o clock do sistema. O clock deve ficar sempre entre 50 KHz a 200Khz