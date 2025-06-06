/*
 */

#include <avr/io.h>
#include<avr/interrupt.h>

#define BTN PB0; //boton para leer.


volatile uint16_t milis =0;

/*
como vamos a hacer que parpadee mas rapido,

*/


//bamos a ocupar un delay como nuestro antirebote por software
extern void delay_103us(void);

void read_BTN(void);


//variable global que me representa el estado




enum estado{

    velocidad_normal=0,
    velocidad_doble,
    velocidad_triple

};

volatile uint8_t _state=velocidad_normal;



void Timer0_init_CTC(void){

    //hay que configurar el timer
    sei();

    TCNT0= 0;

    TCCR0A |= (2<<WGM00);

    /*el CTC sera de 1ms

    F_Timer = FOSC / PS - 16MHz / 256 -> 62500

    T_ticks = 1 / F_Timer = 1/62500 = 16 uS

    #Ticks = tiempo / T_ticks -> 1mS / 16 uS = 62. 5 -> OCR0A= 62

    */


    //prescalador  -> 256

    TCCR0B |= (1<<CS02);

    //habilitamos interrupcionde OCR0A

    TIMSK0 |=1<<OCIE0A;

    OCR0A = 62;

}



//rutina de manera deinterrupcion



ISR(TIMER0_COMPA_vect){



    milis++;

    switch(_state){


        case velocidad_normal:

                if(milis == 1000){

                    PINB |= 1<<PB7;
                    milis=0;
                }break;

        case velocidad_doble:

                if(milis == 500){

                    PINB |= 1<<PB7;
                    milis=0;
                }break;

        case velocidad_triple:

                if(milis == 250){

                    PINB |= 1<<PB7;
                    milis=0;
                }break;

    }


}



void read_BTN(void){



    uint8_t read = 0;

    /*
    PINx me devuelve el estaso fisico del PIN

    como el pin en entrada esta configurado con PULL-UP

    por lo que en su estado normal el pin envia 1 logico -> 3 a 5.5V

    el estado por defecto es 0 -> velocidad normal


    */



    if(!(PINB & (1<<PB0))){

        delay_103us();


        if(_state == velocidad_normal){

            _state= velocidad_doble;
            milis=0;
        }

        else if(_state == velocidad_doble){

            _state= velocidad_triple;
            milis=0;
        }

        else{
            _state= velocidad_normal;
            milis=0;
        }



    }



}




int main(void)
{






    DDRB |= (1<<PB7); //salida
    DDRB &= ~(1<<PB0); //entrada
    PORTB |=(1<<PB0); //activamos su pullup

    Timer0_init_CTC();


    // Insert code

    while(1){

        //haora si va algo aquie, constantemente debemos de estar leyecnod el puerto

        read_BTN();
    }

    return 0;
}
