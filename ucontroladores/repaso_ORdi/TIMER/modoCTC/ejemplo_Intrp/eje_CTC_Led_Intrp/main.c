
#include <avr/io.h>
#include <avr/interrupt.h>


/*
ejercicio
Parpadeo Básico de un LED - 500ms

*/


//16 porque 8 bits no puede guardar 500 en decimal
volatile uint16_t milis = 0;



void TIMER0_CTC_init(void){


    sei(); //habilitamos interripciones globales

    //modo CTC tope OCR0A sin PWM

    TCNT0=0; //inicamos nuestro contador

    TCCR0A |= (1<<WGM00);
    /*
    puede ghacer 2 cosas

    que el resuldado no toggle OC0A o lo toggleamos con PIN


    seleccionamos nueso PS para asignarle para poder llegar

    la eucacion consta de cuantos ticks quiero lograr para X tiempo

    1- debemos calcular la freceicnia del timer, este nos dira que tan rapido
    va ese timer con respecto al socilador interno.

    F_Timer = Fosc / PS = 16MHZ / 64 = 250000

    2- el tiempo que tadrda en dar un tick bajo esa frecueicna

    T_ticks = 1 / F_Timer = 1 / 250000 = 4uS -> cada 4 uS es un tick de procesaro

    3- el tiempo deseado, el tiempo que quiero lograr con respecto a los ticks

    Ticks_lograr = TimeDeseado / T_ticks = 1mS / 4uS = 250

    OCR0A = 250 <- esto es correcto dado que este 250 es menos de 0xFF

    -> pero aqui pasa algo importante en tomar en cnuesta que debemos de restarle un
    1 poruqe el conteo espeiza desde 0 para que pueda lograr esos 250 ticks exactos


    -> entonces son 3 puntos para poder clauclar el numero de ticks

    1 - frecueicna del timer
    2 - el tiempo que toma es hacer 1 ticks en porporcion a la frecueicna del timer
    3 - calcular los ticks necesario par hacer X tiempo con respecto al tiempo
        que le toma en hacer 1 tick


    */

    //pero antes habilitamos nuestras interrupciones

    //habilitamos la interripcin cunado OCR0A haga el match, un OR porque no se si haya algo mas y no quieor afectarlo


    TIMSK0 |= 1<< OCIE0A;

    //prescalador = 64

    TCCR0B |= (1<<CS00) | (1<<CS01);

    OCR0A = 250-1; //cunado TCTNT haga match con OCR0A ocurrica la interupcion






}

ISR(TIMER0_COMPA_vect){


    //que va a hacer la ISR
    milis++;

    if(milis == 500){
        PINB |=1<<PB7;
        milis=0;


    }
}



int main(void)
{

    //habilitmso PB7 como salida

    DDRB |= 1<<PB7;
    TIMER0_CTC_init();


    while(1)
    ;

    return 0;
}

