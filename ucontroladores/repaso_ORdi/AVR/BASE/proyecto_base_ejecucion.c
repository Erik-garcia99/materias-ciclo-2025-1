/*

ejercicio en donde cambiemos la frecueinca en timpo real
va a cambiar el tiempo que pasa, actualizamos

som INT
 */

#include <avr/io.h>
#include<avr/interrumpt.h>
//usaremos el mismo TIMER para la lecttrua

/*
normal seria

1 S

500 ms

250 ms


*/


volatile uint16_t milis=0;

enum _state{
    normal=0,
    mitad,
    tercio

};


volatile uint8_t estado = normal;

//contador para el retardo
volatile uint8_t delay=0;

void TIMER_CTC_init(void){

    TCCR0A = (1<<WGM01); //modo tcnt
    //para 1ms

    TCCR0B =(3<<CS00);
    //usaremos A para el parpaadeo del led
    OCR0A = 250-1;
    TCNT0 = 0;
    //usaremos B para la lectura del pin
    TIMSK0 = (1<<OCIE0A); //habilitamos la intrrupcion del match


    sei();
}


ISR(TIMER0_COMPA_vect){

    milis++;
    delay++;

    switch(estado){

        case normal:

            if(milis==1000){
                milis=0;
                PINB |= 1<<PB7;
            }

        case mitad:
            if(milis==200){
                milis=0;
                PINB|=1<<PB7;


            }

        case tercio:

            if(milis ==150){
                milis=0;
                PINB |=1<<PB7;
            }
    }

}


//devolvera 1 cunado se cumpla un retardo de 10 ms
uint8_t retardo(){

    if(delay<20){
        delay=0;
        return 1;
    }

    return 0;
}



void lectura_BTN(void) {


    if(!(PORTF & (1<<PF0))){

        while(!(retardo()));

        if(estado == normal || estado == tercio){
            milis=0;
            estado = normal;

        }

        if(estado == normal){

            milis=0;
            estado= mitad;
        }

        if(estado == mitad){
            milis=0;
            estado= tercio;
        }

    }

}

int main(void)
{

    DDRB |=1<<PB7; //pin 7 como salida

    DDRF &=~(1<<PF0);
    PORTF |= 1<<PF0;

    while(1){

        lectura_BTN();

    }


    return 0;
}
