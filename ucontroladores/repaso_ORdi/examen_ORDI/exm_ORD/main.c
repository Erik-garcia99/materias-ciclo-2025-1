
#include<avr/io.h>
#include<avr/interrupt.h>

/*
probelam del ORDI


-> PWM  (PB7)-> con TIMER0 -> ciclo de trabajo definido por el potenciometro

-> el PWM les da la señal a los LEDS

botones en PF1-PF3


LEDS ->PK0-PK2 <-> PB0-PB2




*/

/*
parte muy importante


*/


//variable que indcia el estado en que esta la secueicnia

enum estado{

    None=0,
    left,
    rigth,
    hazard
};



//inicamos los puertos

volatile uint16_t milis=0; //variable que contara los mS
volatile uint8_t milis_delay=0;

volatile uint8_t estado_actual;
volatile uint8_t estado_anterior;

//ubral para la ilumincacion
static uint16_t HYSTERESIS_H = 870;

static uint16_t HYSTERESIS_L = 306;


/*
80% de intensidad de OCR0A -> 8 bits es 204

30% de intensidad de OCR0A -> 8 bits es 77
*/


void GPIO_init(void){


    //puertos de salida, el de los LED

    DDRB |= (7<<PB0); //LEDS LADO DRCH
    DDRB |=(1<<PB7); //SALIDA DEL pwm

    DDRK |= (7<<PK0); //los leds para el lado IZQ

    //puertos de entrada
    DDRF &=~(7<<PF1); //PF1-PF2-PF3 como entrada
    PORTF |=(7<<PF1); //acitvamos pull ups

    //prueba

    DDRA |=(7<<PA0);

}


//inicamos el TIMER, con TIMER2


void TIMER2_init(void){


    TCCR2A = (1<<WGM21); //moco CTC

    /*
    cada 1mS

    PS = 64

    OCR0A = 250-1
    */

    TCCR2B = (1<<CS22);

    TIMSK2 = (1<<OCIE2A); //INT
    OCR2A = 250-1;
    sei();
    TCNT2 = 0;
}

ISR(TIMER2_COMPA_vect){

    milis++;

}


//configruar el PWM
void PWM_init(void){

    //PWM con el TIMER0
    //fastPWM TOP=MAX, modo invetido
    TCCR0A = (3<<WGM00) | (1<<COM0A1);

    /*
    F_PMW = 16MHz / 64 * 256 = ~1kHz
    */

    TCCR0B = (3<<CS00);

    OCR0A = 128; //ciclo trabaji abajo

    sei();
    TCNT0 =0;

}

//inciar ADC

/*
para el ciclode trabajo debemos tener varibales que indican si el ADC, potenciometro
LDR sobre pasaa y disminuye de X y Y umbral la potencia de los leds es diferente

*/


void ADC_init(void){

    //AVCC como voltaje de referencia
    //ajustado a la derecha, ADC0
    ADMUX = (1<<REFS0);

    /*
    habilitar ADC, limpiar bandera, PS de 128 = 125kHz
    */
    ADCSRA = (1<<ADEN) | (1<<ADIF) | (7<<ADPS0);

    //para ADC0 y free runnig
    ADCSRB = 0;

    DIDR0 = (1<<ADC0D); //deshabilitar la parte digital del PIN

}

//lectura del ADC
uint16_t READ_ADC(void){


    ADCSRA |= (1<<ADSC);


    while(!(ADCSRA &(1<<ADIF))); //esperamos a que se termine la conversion

    uint8_t low = ADCL;
    uint8_t high = ADCH;


    uint16_t resultado = (high << 8) | low;

    ADCSRA |=(1<<ADIF);//limpiamos bandera

    return resultado;

}


//lecturade los botones



//actualizacoin de los LEDS
//lo que recibe sera lo que se mande de los botones
void Update_Leds(uint8_t state){


    //aqui en la lectura debermos dar prioridad a hazard, pero
    //perimo debemos de leer
    estado_actual = state;
    estado_anterior = None;

    static uint8_t LD1=0,LD2=0,LD3 = 0;

    static uint8_t hz_on = 0;


    if((estado_actual == left && estado_anterior == rigth) || (estado_actual == rigth || estado_anterior == left)){

        //en este paso pasamos a apagar todas las luces

        PORTK &=~(7<<PK0);
        PORTB &=~(7<<PB0);
    }



    if(estado_actual == hazard){

        estado_anterior = estado_actual;
        if(milis >= 500){

            milis=0;

            if(hz_on == 0){

                PORTK |=(7<<PK0);
                PORTA |=(7<<PA0);
                hz_on=1;

            }
            else{
                PORTK &=~(7<<PK0);
                PORTA &=~(7<<PA0);
                hz_on = 0;
            }

        }

    }


    //LEFT PORTK
    if(estado_actual == left){

        estado_anterior = estado_actual;

        if(milis >= 250){

            milis =0;

            if(LD1 == 0 && LD2 == 0 && LD3 == 0){

                PORTK |=(1<<PK0);
                LD1 = 1;

            }

            else if(LD1 == 1 && LD2 == 0 && LD3 == 0){
                PORTK |=(1<<PK1);
                LD2=1;

            }
            else if(LD1 == 1 && LD2 == 1 && LD3 == 0){

                PORTK |=(1<<PK2);
                LD3=1;
            }

            else if(LD1 == 1 && LD2 == 1 && LD3 == 1){

                PORTK &=~(7<<PK0);
                LD1 = LD2 = LD3 =0;
            }

        }


    }

    //este es el que mas mas va costar porque el puerto B esta asociado con PWM

    /*
    if(estado_actual == rigth){


        estado_anterior = estado_actual;

        if(milis >= 250){

            milis = 0;

            if(LD1 == 0 && LD2 == 0 && LD3 == 0){

                //podriamos apagar por un momento el PWM


                PORTB |=(1<<PB0);
                LD1 = 1;

            }

            else if(LD1 == 1 && LD2 == 0 && LD3 == 0){
                PORTB |=(1<<PB1);
                LD2=1;

            }
            else if(LD1 == 1 && LD2 == 1 && LD3 == 0){

                PORTB |=(1<<PB2);
                LD3=1;
            }

            else if(LD1 == 1 && LD2 == 1 && LD3 == 1){

                PORTB &=~(7<<PB0);
                LD1 = LD2 = LD3 =0;
            }

        }

    }*/

    if(estado_actual == rigth){


        estado_anterior = estado_actual;

        if(milis >= 250){

            milis = 0;

            if(LD1 == 0 && LD2 == 0 && LD3 == 0){

                //podriamos apagar por un momento el PWM


                PORTA |=(1<<PA0);
                LD1 = 1;

            }

            else if(LD1 == 1 && LD2 == 0 && LD3 == 0){
                PORTA |=(1<<PA1);
                LD2=1;

            }
            else if(LD1 == 1 && LD2 == 1 && LD3 == 0){

                PORTA |=(1<<PA2);
                LD3=1;
            }

            else if(LD1 == 1 && LD2 == 1 && LD3 == 1){

                PORTA &=~(7<<PA0);
                LD1 = LD2 = LD3 =0;
            }

        }

    }


}




//




int main(void){

    GPIO_init();
    TIMER2_init();
    PWM_init();

    ADC_init();



    while(1){


        uint16_t lectura_ADC = READ_ADC();

        //OCR0A =128;
        Update_Leds(hazard);


    }

}
