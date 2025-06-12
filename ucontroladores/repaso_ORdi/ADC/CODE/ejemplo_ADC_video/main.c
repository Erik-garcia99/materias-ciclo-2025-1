/*
ejemplo 1

mostrar la digitalizacion del voltaje que se aplica en un
divisor de voltaje montado con un potenciometro por
ADC0 en una tira con 10 led


conversion susesiva

 */

 /*
hacer variar el burllo del led por OC1A con un potenciometro


 */





#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 16000000UL






int main(void)
{

    /*
    ADMUX = 1<< ADLAR;

    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADSC);

    DDRB = 0xFF;
    DDRC = 0x00;
    DDRD = 0xFF;

    while(1){

        PORTB = ADCL;
        PORTH= ADCH;

    }*/

    SPL = 0xFF;
    SPH = 0x00;

    cli();


    ADMUX = (1<<ADLAR);
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADSC) | ()1<<ADIE);
    TCC1A = 0x01;
    TCC1B = 0x09;

    DDRB = (1<<1);
    DDRC = (1<<3)|~(1<<0);
    DDRD = 0xFF;

    sei();

    while(1){

        PORTC |= (1<<3);
        _dealy_ms(500);
        PORTC = ~(1<<3);
        _delay_ms(500);

    }



    return 0;
}


ISR(ADC_vect){

    PORTD = ADCH;
    OCR1A = ADCH; //porque estamos usando un PWM variabel
    //donde el tope de OCRnA, no 0xFF

}
