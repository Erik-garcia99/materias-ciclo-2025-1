/*
en este rograma haremos un delay con timer
asi como prender los led en diferenes frecuencias

//lo haremos con TIMER2 para practica TIMER2

cada uno a diferente LED para ver si es que realmene esta cambiando
1S

500 ms

250 ms
 */

#include <avr/io.h>
#include<avr/interrupt.h>


volatile uint16_t milis = 0;

//delay con 10 ms
volatile uint8_t delay = 0;

/*
para el boton es checar en que estado esta antes y cambiarlo

*/

enum estado{

    ST1=0,
    ST2,
    ST3
};

volatile uint8_t _state= ST1; //inicia con el estado inical

void TIMER2_init(void){


    TCCR2A = (1<<WGM21); //como CTC

    /*
    haremos una interrupcioens cada 1ms


    F_timer = 16MHz / ps = 250000

    F_tick = 1/F_timer = 1/250000 = 4uS

    #tick = 1mS / 4 uS =  250 ticks = 1mS

    ESTE timer TIENE MAS PS por lo que hay mas opciones
    */
    TCCR2B = (1<<CS22);

    OCR2A = 250 -1;
    sei();
    //habilita la bandera de interrupciones
    TIMSK2 = (1<<OCIE2A);

    TCNT2 = 0;


}


ISR(TIMER2_COMPA_vect){
    delay++;
    milis++;

    /*
    tenemos el puerto F - PF0-PF1-FP2 como salida

    PF0 -1S

    PF1-500ms

    PF2 - 250 ms
    */

    if(_state == ST1){

        if(milis == 1000){
             PINF |= 1<<PF0;
             milis = 0;
        }
    }

    if(_state == ST2){

        if(milis == 500){
            PINF |= 1<<PF1;
            milis = 0;
        }
    }

    if(_state == ST3){

        if(milis == 250){

            PINF =| 1<<PF2;
            milis = 0;
        }
    }

}


void __delay(void){

    while(delay < 10){
        delay = 0:
    }
}



void BTN_read(void){

    ///usaremos el PIN PF3 como entrada
    if(!(PINF & (1<<PF3))){

        __delay();




    }


}



int main(void)
{

    DDRF &= ~(1<<PF3);
    PORF|= (1<<PF3); //hbailitar PULL UP

    DDRF |= (7<<PF0);

    // Insert code
    TIMER2_init();

    while(1){

        BTN_read();
    }

    return 0;
}
