
#include <avr/io.h>       // Librería para registros de E/S del AVR
#include <util/delay.h>   // Librería para _delay_ms()
#include<avr/interrupt.h>
// --- Definiciones ---
#define F_CPU 16000000UL  // Frecuencia del CPU: 16 MHz


enum PS{

    sin_PS =0,
    PS_8,
    PS_64,
    PS_256,
    PS_1024


};


volatile uint8_t PS = sin_PS;


void Fast_PWM_init(void){




    TCCR0A |= (3<<WGM00) | (1<<COM0A1);

    //INICUALMENTE inicamos sin prescalador

    TCCR0B |= (1<<CS00);

    //50% el ciclo de trabajo

    OCR0A = 127;

    TCNT0 = 0;

    //TIMSK0 |= (1<<TOIE0);
    //sei();


}

/*
ISR(TIMER0_OVF_vect){

    _delay_ms(200);

    if(PS == sin_PS){

        PS=PS_8;

        TCCR0B = (2<<CS00);
        _delay_ms(100);
    }

    else if(PS == PS_8){

        PS= PS_64;
        TCCR0B = (3<<CS00);
        _delay_ms(100);

    }
    else if(PS == PS_64){

        PS=PS_256;
        TCCR0B =(4<<CS00);
        _delay_ms(100);
    }

    else if(PS == PS_256){

        PS=PS_1024;
        TCCR0B =(5<<CS00);
        _delay_ms(100);
    }
}

*/

int main(void)
{

    // Insert code

    DDRB |= 1<<PB7;

    Fast_PWM_init();


    while(1){


        TCCR0B = (1<<CS00);

        for(uint8_t ctn = 0 ; ctn < 100 ; ctn ++){

            _delay_ms(10);
        }

        TCCR0B = (2<<CS00); //PS 8

        for(uint8_t ctn = 0 ; ctn < 100 ; ctn ++){

            _delay_ms(10);
        }

        TCCR0B = (3<<CS00); //PS 64

        for(uint8_t ctn = 0 ; ctn < 100 ; ctn ++){

            _delay_ms(10);
        }

        TCCR0B =(4<<CS00); //PS256

        for(uint8_t ctn = 0 ; ctn < 100 ; ctn ++){

            _delay_ms(10);
        }

        TCCR0B =(5<<CS00); //PS 1024

        for(uint8_t ctn = 0 ; ctn < 100 ; ctn ++){

            _delay_ms(10);
        }



    }

    return 0;
}
