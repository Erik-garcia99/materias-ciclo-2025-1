#include <avr/io.h>
#include<avr/interrupt.h>

/*
garcia chavez erik 01275863
examen ordinario uC - 2025-1

*/
//botones de entrada
#define LEFT PF1
#define RIGTH PF2
#define HAZZARD PF3

//salida
//IZQ
#define LED1 PK0
#define LED2 PK1
#define LED3 PK2

//DRCH
#define LED4 PB0
#define LED5 PB1
#define LED6 PB2

//250 entra en 8 bits 
volatile uint8_t milis=0;

enum state{

  izq = 0,
  derh,
  hiz

};


void init_GPIO(void){

  //ENTREADAS SE DEFINEN con 0, activamos POULLUPS con 1 menos de PF0
  DDRF &=~(0xF<<PF0);
  PORTF |=(0xE<<PF0);

 
  //salida LED 1 
  DDRK |= (7<<LED1);
  DDRB |= (7<<LED4);
  //asigamos 0 en la salida
  /*PORTB |=(7<<LED4);
  PORTK |=(7<<LED1);*/


  //salida para PWM 

  DDRB |= (1<<PB7);

  //PF0 es una entreada 
}

//iniciamos nuestro PWM 
//al parecer es FAST PWM

//TIMER0 usaremos para hacer el PWM 

void FAST_PWM(void){

  //se tiene que usar TIMER0 para PWM 
  //donde OC0A es la que va a mandar la senial del PWM 

  TCCR0A |= (3<<WGM00) | (1<<COM0A1); //fast - modo no inverrido

  //frecueicnia con prescalador de 64 cercano al 1KHz

  TCCR0B |=(1<<CS01) | (1<<CS00);

  TCNT0 =0;

  OCR0A = 128-1; //50% DEL CICLO DE TRABAJO DEL PWM

}


//usare el TIMER2 para contabilizar los 
void TIMER2_init(){

  sei();
  /*
  usar modo NORMAL 

  F_timer = Fosc / PS = 16MHz / 128 = 125000

  T_tick = 1/ F_timer  = 1/ 125000 = 8uS

  #tick = 1mS/ 8uS = 125 -> OCR2A = 125
  
  */
  

  TCCR2B |= (1<<CS22) | (1<<CS20);
  TIMSK2 |=(1<<TOIE2);

  TCNT2 = 125-1;

}


ISR(TIMER0_OVF_vect){

/*
  milis++;

  if(milis==250){
    milis = 0;
    TCNT2 = 130-1;
  }
*/
  milis++;
  TCNT0 = 130-1;

  if(milis==250){

    PORTK|=(1<<LED1);
    milis=0;

  }


}


uint8_t read_BTN(void){

  //en el caso que cualquiera se presione, la condicion no sabe cual se preciono pero sabe que algun boton 
  //esta presionado
  if(!(PINF &(0xE << PF0))){
    //  va a tener rebote dado al delay

    //LEFT
    if(!(PINF & (0xE << PF0)) ==0x02){
      return izq;
    }

    else if(!(PINF & (0xE << PF0)) ==0x04){
      return derh;
    }

    else if(!(PINF & (0xE << PF0)) ==0x08){

      return hiz;
    }

  }

}




int main(void)
{
    //Inicializar perifericos

  init_GPIO();
  FAST_PWM();
  TIMER2_init();
  uint8_t actual_estado = 0;

  while(1)
  {

    uint8_t btn = read_BTN();

    if(btn == izq || actual_estado == izq){

      actual_estado = izq;

      while(milis < 250);

      PORTK |= (1<<LED1);
      milis=0; 
    }
  }
}
