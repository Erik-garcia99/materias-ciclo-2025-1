

#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 16000000UL


/*
tneemos 2 modos del PWM

cunado este su TOP es 0xFF

cunado su TOP es OCR0A en este modo podemos modficar la frecuencia del PWM

cunado su TOP esta dicado por 0xFF nos debemos adeucar a la frecuencias que nos da
con los PS establecido en TCCR0B

cunado su TOP es 0CR0A este dica ahora la frecuencia del PWM


como podemos calcular el valor que va a ir en OCR0A en funcion a que frecuencia queremos
lograr

la formula nos dice

F_PWM = Fosc / (PS * (OCSR0A +1))

por lo que:

OCSR0A = (Fosc / (F_pwm * PS) ) - 1;

con ese podremo saber que asignar a OCSR0A para llegar a la frecueicina deseada

ahora como OCSR0A responde a la frecuencia del PWM, quien responde al ciclo de trabajo?

en este caso OCSR0B responde a ese valor. por lo que ahora debemos habilitar
el pin asociado a OC0B que es PG7 como salida para poder observar el PWM


---------------------------------------
la formula se modifica cunado se trata del phase correct PWM esto porque?
poruqe este PWM no baja de golpe como FAST PWM, si no que sube, llega a OCSR0A y despues
baja hasta 0.

por lo que:

F_pwm =Fosc / (PS * 2 * OCR0A) -> este nos dara la frecuenia del PWM

para obtener que OCR0A debemos tener para lograr X frecuencia debemos despejar la formula


por lo que:

OCR0A = Focs / F_pwm * PS * 2




*/

void FAST_PWM_MOD(void){


    TCNT0= 0;

    //como ahora COM0A1:0 no sera la senial de salida, ahora tenemos a COM0B1:0 que
    //responderas a eso.
    TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0B1); // modo no inverdio

    TCCR0B |= (1<<WGM02);

    //SELECCIONAR EL PS

    /*

        OCR0A = (Fosc / (F_PWM * PS)) -1; ps =1024 - OCR0A = 155 - F_pwm = 100Hz
    */

    TCCR0B |= (1<<CS02) | (1<<CS00);

    OCR0A = 155; //para una frecuencia de 100Hz

    OCR0B = 128-1; //50% cycle duty - 25
}



int main(void)
{


    DDRG |=1<<PG5;
    // Insert code

    FAST_PWM_MOD();

    while(1)
    ;

    return 0;
}
