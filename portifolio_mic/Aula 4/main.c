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
#include <stdint.h>

#define KEY_RELEASED 0
#define KEY_PRESSED 1

uint8_t gKeyState_w = 0;
uint8_t gKeyState_a = 0;
uint8_t gKeyState_s = 0;
uint8_t gKeyState_d = 0;

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

void GPIO_decBar(){
  PORTD = PORTD << 1;
}

void PCINT_config() {
  PCICR |= (1<<PCIE0); //Habilita grupo 0 de vetores
  PCMSK0 |= (1<<PCINT3)|(1<<PCINT2)|(1<<PCINT1)|(1<<PCINT0); //habilita interrupção PCINT-PCIN
}


ISR(PCINT0_vect){
  //Lógica tecla W
  uint8_t tCurrentkeyState_w = 0;
  if((PINB & (1<<PINB0)) != 0) {//Testa PB0
    //PB0 = 1, tecla 'w' solta 
    tCurrentkeyState_w = KEY_RELEASED;
  }
  else{
    //PB0 = 0, tecla 'w' pressionada
    tCurrentkeyState_w = KEY_PRESSED;
  }
  if(tCurrentkeyState_w == KEY_PRESSED && gKeyState_w == KEY_RELEASED){
    //Tecla W, acabou de ser pressionada
   // gKeyState_w = KEY_PRESSED;
    GPIO_incBar();
  }
  /*else if(tCurrentkeyState_w  == KEY_RELEASED && gKeyState_w == KEY_PRESSED){
    //Tecla W, acabou de ser solta
    gKeyState_w = KEY_RELEASED;
  }*/
  
  //Lógica tecla S
  uint8_t tCurrentkeyState_s = 0;
  if((PINB & (1<<PINB2)) != 0) {//Testa PB0
    //PB0 = 1, tecla 'S' solta 
    tCurrentkeyState_s = KEY_RELEASED;
  }
  else{
    //PB0 = 0, tecla 's' pressionada
    tCurrentkeyState_s = KEY_PRESSED;
  }
  if(tCurrentkeyState_s == KEY_PRESSED && gKeyState_s == KEY_RELEASED){
    //Tecla s, acabou de ser pressionada
   // gKeyState_s = KEY_PRESSED;
    GPIO_decBar();
  }
  /*else if(tCurrentkeyState_s  == KEY_RELEASED && gKeyState_s == KEY_PRESSED){
    //Tecla s, acabou de ser solta
    gKeyState_s = KEY_RELEASED;
  }*/
  
  PORTC ^= (1<<PORTC0); //seta pino PC0
  //_delay_ms(100);
  //PORTC &= ~(1<<PORTC0); //zera pino PC0
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
