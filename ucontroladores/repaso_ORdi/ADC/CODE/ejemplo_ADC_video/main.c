/*
ejemplo 1

mostrar la digitalizacion del voltaje que se aplica en un
divisor de voltaje montado con un potenciometro por
ADC0 en una tira con 10 led


conversion susesiva

 */

/*
#include <avr/io.h>

int main(void){


    //acomodamos el resultado del ADC hacia la izquierda, tomara el ADC0
    ADMUX = (1<<ADLAR) | (1<<REFS0);

    //hbailitacion del ADC,
    //habilita el auto desparador, para la conversion sucesiva, este se escribe
    //ADSC se comienza la conversion
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADSC);

    //todo el puerto B como salida
    DDRB = 0xFF;
    DDRC = 0x00; //puerto C como entrada sin pull up
    DDRA = 0xFF; //puerto D como salida


    while(1){
        PORTB = ADCL;
        PORTA = ADCH;

    }


}

*/

/*
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 5000000UL

ralizar la sigitalizacion anterior pero ahora con interrupciones






int main(void){


    //acomodamos el resultado del ADC hacia la izquierda, tomara el ADC0
    ADMUX = (1<<ADLAR) | (1<<REFS0);

    //hbailitacion del ADC,
    //habilita el auto desparador, para la conversion sucesiva, este se escribe
    //ADSC se comienza la conversion
    //habiñlitar las interrupcoines del ADC
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADSC) | (1<<ADIE);

    //todo el puerto B como salida
    DDRB = 0xFF;
    DDRC = (1<<3) |~ (1<<0); //puerto C como entrada sin pull up
    DDRA = 0xFF; //puerto D como salida

    sei();


    while(1){
        PORTC |=(1<<3);
        _delay_ms(500);
        PORTC = ~(1<<3);
        _delay_ms(500);
    }


}




ISR(ADC_vect){


    PORTB = ADCL;
    PORTA =ADCH;
}


*/

/*
realizar con el ADC controlar el brillo de un led con PWM, este led sera el OC1B
*/


#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
#define F_CPU 5000000UL

//ralizar la sigitalizacion anterior pero ahora con interrupciones






int main(void){


    SPL =0xFF;
    SPH = 0x08;

    cli();

    //acomodamos el resultado del ADC hacia la izquierda, tomara el ADC0
    ADMUX = (1<<ADLAR) | (1<<REFS0);

    TCCR1A =0x81;
    TCCR1B = 0x09;
    //hbailitacion del ADC,
    //habilita el auto desparador, para la conversion sucesiva, este se escribe
    //ADSC se comienza la conversion
    //habiñlitar las interrupcoines del ADC
    ADCSRA = (1<<ADEN) | (1<<ADATE) | (1<<ADSC) | (1<<ADIE);

    //todo el puerto B como salida
    DDRB = 0xFF;
    DDRC = (1<<3) |~ (1<<0); //puerto C como entrada sin pull up
    DDRA = 0xFF; //puerto D como salida

    sei();


    while(1){
        PORTC |=(1<<3);
        _delay_ms(500);
        PORTC = ~(1<<3);
        _delay_ms(500);
    }


}




ISR(ADC_vect){

    //para controlar el brullo del led lo ponemos en OCR1A este cunado 0xFF es el tope
    OCR1A = ADCH;
    PORTA =ADCH;
}






 /*
hacer variar el burllo del led por OC1A con un potenciometro








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

    }

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

}*/
