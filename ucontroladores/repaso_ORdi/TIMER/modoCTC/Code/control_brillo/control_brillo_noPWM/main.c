/*
 */

#include <avr/io.h>
#include<avr/interrupt.h>


volatile uint16_t micros = 0;

volatile uint8_t cycle_simulado=0;

volatile uint8_t cycle_pwm =0;


extern void delay_103us();

extern uint16_t delay(uint16_t time);



void Timer_init_no_PWM(void){



    TCCR0A |= (2<<WGM00);

    /*
        debemos hacer interrupciones muy rapdias - 100uS


        Ftimer = Fos / PS -> 16MHz / 64 = 250000

        T_tikc = 1/F_timer = 1/250000 = 4uS

        #ticks = tiempo_deseado / t_ticks -> 100uS / 4uS = OCR0A = 25

    */

    TCCR0B |= (1<<CS00) | (1<<CS01);
    TIMSK0 |= (1<<OCIE0A);
    sei();


    TCNT0=0;
    OCR0A =25;
}


ISR(TIMER0_COMPA_vect){



    cycle_pwm++;

    if(cycle_pwm < cycle_simulado){
        PORTB |= 1<<PB7; //prendemos
    }
    else{
        PORTB &=~(1<<PB7);

    }



    if(cycle_pwm >= 255){
        cycle_pwm=0; // se reincial
    }

}






int main(void)
{



    DDRB |= 1<<PB7;
    Timer_init_no_PWM();


    // Insert code

    while(1){


        for(cycle_simulado =0; cycle_simulado<256 ;cycle_simulado++){

            delay(10);

        }

        for(cycle_simulado =255 ; cycle_simulado>=0; cycle_simulado--){
            delay(10);
        }


    }

    return 0;
}
