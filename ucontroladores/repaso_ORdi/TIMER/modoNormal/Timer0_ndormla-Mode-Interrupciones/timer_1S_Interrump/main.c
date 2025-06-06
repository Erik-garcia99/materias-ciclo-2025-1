
#include <avr/io.h>
#include <avr/interrupt.h>


//es el mismo proceso pero ahora manejaremos las interrupciones, con el modo normal.

volatile uint16_t milis = 0;


void TIMER0_init(void){


    sei();

    TCCR0A = 0;

    TCCR0B |= (1<<CS02) | (1<<CS00);

    TIMSK0 |= 1<<TOIE0; //habilitamos la interrupcion


    TCNT0 = 240;


}


ISR(TIMER0_OVF_vect){


    milis++;

    if(milis == 1000){

        PINB |= 1<< PB7;
        milis=0;

    }


    //limpaimos la vandera de overflow
    //TIFR0 |= (1 << TOV0);

    TCNT0 = 240; //volvemos a empezar
}




int main(void)
{

    DDRB |= 1<<PB7;

    TIMER0_init();

    while(1);


}
