/*
haremos el progtam con TIMER2
debemos hacer para lectrua de PIN y cambio de led,

el timer hara una interrupcion cada milisegundo

PORF - PF0 - PF1 -PF2 - como salida

PORF  PF3 - entrad (por ahora solo por 1 boton)

 */

#include <avr/io.h>
#include<avr/interrupt.h>

//inicamos el TIMER que contara cada milisegundo

volatile uint16_t milis = 0;
volatile uint8_t milis_delay = 0;

//estado 1 - 1S
//ST2 - 500ms
//ST3 - 250ms
enum estado{

    ST1=0,
    ST2,
    ST3

};

volatile uint8_t _state= ST1; //estado incial

void TIMER2_init(void){

    TCCR2A = (1<<WGM21); //modmo CTC
    /*
    calcular PS y OCR2A para el conteo de 1 mS

    F_timer = Fosc / PS = 16MHz / 64 = 250000

    F_tick = 1/F_timer = 1/250000 = 4uS

    #ticks = 1mS /4uS = 250 - para un tick

    */

    TCCR2B = (1<<CS22);

    OCR2A = 250 -1;
    //mascara

    //la ISR limpa la bandera, y el moco CTC reinica TIMER
    TIMSK2 = (1<<OCIE2A);

    sei();
    TCNT2 = 0;

}

ISR(TIMER2_COMPA_vect){

    milis++;
    milis_delay ++;


    if(_state == ST1){
        if(milis == 1000){

            //cpn una frecueicni de cada seg
            PINF |= 1<<PF0;
            milis =0;
        }
    }
    if(_state == ST2){
        if(milis == 500){
            milis = 0;
            PINF |=1<<PF1;
        }
    }

    if(_state == ST3){

        if(milis == 250){
            milis = 0;
            PINF |= 1<<PF2;
        }
    }
}

uint8_t _delay(){

    if(milis_delay > 5){
        milis_delay=0;
        return 1;
    }
    //para hacerlo no bloqueante lo hacemos con un IF
    return 0;
}



//el que lee el boton va a cambiar a una varibale global

void READ_BTN(){

    //ohhh, entonces el ciclo while es el que
    static uint8_t bound = 0; //no esta precionado
    uint8_t var_delay=_delay();


    /*
        esta condicion es para que no se bloquee
        lo que esta haciendo es verificando si el delay se completo, cundao este se complete
        la funcion regresa 1 cuando esto pase la varibale

        "bound" es la que indica que el delay se completo se pone en 1 y el conteo se reinica
        si no se ha completado para eso sale de la funcion para seguir trabajano

    */


    if(!(PINF & (1<<PF3))){
        if(var_delay == 1){
            bound = 1;
        }
    }

    /*
    recordando que es lo que hace esta funcion, solo cambia las variables globales

    entonces si se presiono quiereo cambiar la frecuecnia con la que se tooglea el led y
    cambiar de led
    */
    if(bound == 1){

        switch(_state){

            case ST1 :
                _state = ST2;
                bound = 0;
                milis =0;
                PORTF &=~((1<<PF0) | (1<<PF1) | (1<<PF2));
                break;

            case ST2:
                _state = ST3;
                bound=0;
                milis=0;
                PORTF &=~((1<<PF0) | (1<<PF1) | (1<<PF2));
                break;

            case ST3:
                _state= ST1;
                bound = 0;
                milis =0;
                PORTF &=~((1<<PF0) | (1<<PF1) | (1<<PF2));
                break;
        }
    }
}


int main(void)
{
    //pnes de salida
     DDRF |= (1 << PF0) | (1 << PF1) | (1 << PF2); //PF0 PF1 PF3 - salida

    /*
        0000 0111
        1111 0111

    */

    DDRF &= ~(1 << PF3);
    PORTF |= (1 << PF3); //pulupp interno
    // Insert code

    TIMER2_init();

    while(1){
        READ_BTN();


    }

    return 0;
}
;
