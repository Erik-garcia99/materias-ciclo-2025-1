/*
 */

 //vamos a togglear un pin cada que TCNT0 haga match con OCR0A

#include <avr/io.h>

volatile uint16_t milis =0;




void TIMER0_CTC_init(void){

    //va a tooglear, modo CTC
    TCCR0A |=(1<<WGM01);
    /*
    calculamos que el tiempo del TIMER

    f_timer = Fosc/PS = 16MHz / 64 = 250000

    f_TICK = 1/f_timer = 1 / 250000 = 4 uS

    #tick = 1ms/4us = 250 ticks = 1mS

    tiempo = (OCR0A +1 ) * (PS/FOSC)

    */

    TCCR0B = (3<<CS00); //ps 64
    OCR0A = 250 -1 ; //TCNT0 hara matcth con OCR0
    TCNT0= 0;

    //la bandera que nos dice el match es OCF0A en TIFR0 la interrupcion la limpia
    //pero sin interrupciones se hace escribiendole 1

    TIFR0 |= 1<<OCF0A;


}



int main(void)
{

    // Insert code

    TIMER0_CTC_init();
    DDRB |=1<<PB7;

    while(1){

        while(!(TIFR0 & (1<<OCF0A)));

        milis++;
        TCNT0=0;


        TIFR0 |= 1<<OCF0A; //LIMPIAMOS L ABANDERA

        if(milis == 500){
            PINB |=1<<PB7;
            milis=0;

        }


    }

    return 0;
}
