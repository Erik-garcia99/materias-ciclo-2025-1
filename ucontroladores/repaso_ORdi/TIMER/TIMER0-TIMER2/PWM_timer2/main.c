/*

genrear un phase correct PWM con el TIMER2

Fec_PWM ~ 31kHz

PS = 8
 que vaya de 25 a 75% de cycle duty de ida y regreso

 cada 500 ms





---------------------------------------------------------


el ejercico pide realizar un PWM con una frecucnia cerda de 31kHz

-exiten 2 modos de TOP en el PWM

-cunado TOP = MAX (0xFF - 255) en este modo la frecueinca del PWM solo puede variar entre

-cunado TOP = OCR0A -> OCR0A indica la frecucina del PWM, entonces quien se encarga
    del ciclo de trabajo, ese se encarga OCR0B,

->para el ejercicio necesitamos relaizar con TOP = OCR2A


->frecueincia


    OCR2A = F_osc / 2 * PS * F_PWM = OCR2A = 16MHz / 2 * 8 * 31.4kHz ~ 32
    OCR2A = 32

    ->ciclode trabajo de 25 a 75

->ciclo de trabajo

    duty_cycle = (OCRxB / OCRxA) * 100%

    ->OCRxB = (duty_cycle * OCRxA) / 100 =

    ->OCR2B = (25 * 32) / 100 = 8 -> 25%

    ->OCR2B = (75 * 32) / 100 = 24 -> 75%

 */

#include <avr/io.h>
#include<avr/interrupt.h>


volatile uint16_t milis=0;

void PWM_init(void){

    //TIMER2

    TCCR2A = (1<<WGM20) | (1<<COM2B1); //phase correct PWM , modo "no invertido"

    TCCR2B = (1<<CS21) | (1<<WGM22); //PS 8

    OCR2A = 32; //F_PWM ~ 31.4 kHz
    OCR2B = 8;// CD inical 25%

    TCNT2 =0;

}

void TIMER0_init(void){

    //timer de 500ms
    TCCR0A = (1<<WGM01); //modo CTC

    /*
        timer de 1 Ms
        f_timer = 16MHz / 64 = 250000

        F_tick = 1 /250000 = 4uS

        #ticks = 1mS / 4 uS = 250
    */
    TCCR0B = (3<<CS00);

    OCR0A = 250 -1;
    TIMSK0 = 1<<OCIE0A;

    sei();
    TCNT0=0;

}

ISR(TIMER0_COMPA_vect){

    milis++;

}


void INC_DUTY_CYCLE(void){


    //va a ir desde 25% a 75%
    for(uint8_t ctn = 8 ; ctn <= 24 ; ){

        if(milis == 500){

            milis =0;
            OCR2B = ctn++;
        }

    }

    for(uint8_t ctn = 24 ; ctn >= 8 ;){

        if(milis == 500){
            milis = 0;
            OCR2B = ctn--;
        }

    }

}





int main(void)
{

    PWM_init();
    TIMER0_init();

    //HABILITMAOS OC2B -> PH6
    DDRH |= 1<<PH6;


    while(1){

        INC_DUTY_CYCLE();

    }

    return 0;
}
