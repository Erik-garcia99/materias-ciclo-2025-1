/*
 */

 //genrear una frecuncia de 1kHz sin PWM con el timer0
#include <avr/io.h>
#include<avr/interrupt.h>
/*
la fomrula para conocer OCR0A cunado sabemos la frecueicna es de:

esta formula solo va a funcionar para el ciclo de trabajo de 50%
OCR0A = (F_cpu / 2 * PS * F_deseasa ) - 1


si queremos simular el PWM con un ciclo de trabajo ajustado con el que prendemos
y apagos un PIN la formula para OCR0A seria de


OCRxn = (F_cpu / PS * f_INT ) -1

donde f_INT -> la frecuencia que quiero llegar


*/



void TIMER_init(void){

    //con este usamos el modo CTC, el PWM sera de 50% de trabajo, dado por el toggleo
    //toggle cada mathc, como CTC (OCR0A -> TOP)

    TCCR0A = (1<<COM0A0) | (1<<WGM01);

    //si el PWM sera de 1KHZ con PS de 64

    /*
        OCR0A =(16Mhz / 2 * 64 * 1000)-1;

    */

    OCR0A = 124;
    TCCR0B = (3<<CS00);
    TCNT0=0;
}


int main(void)
{

    // Insert code
    TIMER_init();
    DDRB |=1<<PB7; //OC0A como salida


    while(1)
    ;

    return 0;
}
