/*
haremos casi los mismo ejercicios para TIMER0 y TIMER2 (no se como podramos hacer para simular TIMER2 de 32Khz)

haremos un ejercio con PWM con TIMER0 y el que lleve el timepo TIMER2
solo eso, un PWM-fastPWM - 50% cycle duty

 */

 //este ejercicio lo hare con el TIMER0 donde OC0A esta con un led ya en la board



 //--------------------------------------------------------------------



#include <avr/io.h>
//sin interrupciones

//hace parpadear un led, PB7 cada 100ms, el ejercio pide que se haga con INT pero por ahora no
//queremos INT

//100 cabe en 8 bits (255 - 0xFF)
volatile uint8_t milis=0;

void TIMER_init(void){


    //inicmaos nuestri timer de modo nomral con nuetro top=MAX = 0xFF

    TCCR0A = 0;

    //ahora necesitamos elegir nutro PS que va de la mano con el timepo que queremos
    //que se logre en TIMER

    /*

    como elegir el PS con el tiempo que queremos que se logre y este valor que ingresarmeos
     ya sea como offset a TCNT0

    F_timer = 16MHz / ps = 16MHz / 64 = 250000

    F_Tick = 1/250000 = 4uS

    #tick = 1ms / 4uS = 250 -> cada 250 ticks se hace 1ms, ocupamos un offset de 6



    */

    TCCR0B |= (3<<CS00);

    TCNT0 = 6;

    //no estamos con interrupciones

    //limpiamos la bandera

    TIFR0 |= (1<<TOV0);
}


int main(void)
{

    TIMER_init();
    //IMPORTANTE SI RECORDAR PARA TIMER EN SU FORMA PWM CONFIGRUAR EL PIN DONDE ESTE
    //SALDFRA COMO ALSIDA

    DDRB |= 1<<PB7; //configurar PB7 como salida

    while(1){


        //ahora quiero matear cunado se active la bandera de sobre fljp que de llego al final
        //se completo 1 MS

        //quiero esprar mientras la comparacion no se complete
        while(!(TIFR0 &(1<<TOV0)));
        //se completo

        milis++;
        TCNT0=6; //volemos a cargar el offset
        TIFR0 |= 1<<TOV0;

        if(milis == 100){

            PINB |=1<<PB7;
            milis =0; //reinicamos los milis

        }

        //limpamos la bandera


    }

    return 0;
}
