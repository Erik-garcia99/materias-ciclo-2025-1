
#include<avr/io.h>
#include<avr/interrupt.h>




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

//este funciona para la actualizacion de leds


//ubral para la ilumincacion
static uint16_t HYSTERESIS_H = 870;

static uint16_t HYSTERESIS_L = 306;



static uint16_t inflexion = 450;

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
    milis_delay++;
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

    OCR0A = 0; //ciclo trabaji abajo

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
/*
uint8_t READ_BTN(void){

    //en este caso estamos leyendo 3 botones

    static uint8_t current_State = None;
    static uint8_t estable = 1;
    static uint8_t last_state = None;


    if(milis_delay > 50){

        milis = 0;
        if(!(PINF & (1<<PF2))){
            last_state = current_State;
            current_State=hazard;
            return current_State;
        }
        if(!(PINF &(1<<PF1))){
            //izq
            last_state = current_State;
            current_State = left;
            return current_State;
        }
        if(!(PINF & (1<<PF3))){

            last_state = current_State;
            current_State = rigth;
            return current_State;
        }

    }
    /*
    //vamos a leer el PIN
    //digamos que - verde izq -> amarrilo - drch -> azul hazard
    if(!(PINF & (7<<PF1))){

        //si lee algun boton, no se cual se activa

        if(milis_delay > 50 && estable == 1){
            milis_delay = 0;

            //presiono izquierda
            if( !(PINF & (1<<PF2))){
                last_state = current_State;
                current_State=hazard;
                return current_State;
            }
            else if( !(PINF & (1<<PF1)) ){
                last_state = current_State;
                current_State = left;
                return current_State;
            }
            else if(!(PINF & (1<<PF3))){
                last_state = current_State;
                current_State = rigth;
                return current_State;
            }
        }
        //estaa varibale lo que se encarga es de mantener ese boton presionado
        estable = 0;
    }
    else{
        estable =1;
    }

    return current_State;


}
*/

uint8_t READ_BTN(void) {
    static uint8_t last_state = None;
    static uint8_t current_state = None;
    static uint8_t estable = 1;


    if (milis_delay > 50) {
        milis_delay = 0;
        if (!(PINF & (1 << PF2))) {
            current_state = hazard;
        } else if (!(PINF & (1 << PF1))) {
            current_state = left;
        } else if (!(PINF & (1 << PF3))) {
            current_state = rigth;
        } else {
            current_state = None;
        }

        if (current_state != last_state) {

            last_state = current_state;
            return current_state;
        }
    }

    return None; // si no hubo cambio o no presionado
}



//actualizacoin de los LEDS
//lo que recibe sera lo que se mande de los botones
void Update_Leds(uint8_t state){


    //aqui en la lectura debermos dar prioridad a hazard, pero
    //perimo debemos de leer
    static uint8_t estado_actual = None;
    estado_actual = state;
    static uint8_t estado_anterior = None;


    static uint8_t LD1=0,LD2=0,LD3 = 0;

    static uint8_t hz_on = 0;



    if(estado_actual != estado_anterior){

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
                PORTB |=(7<<PB0);
                hz_on=1;

            }
            else{
                PORTK &=~(7<<PK0);
                PORTB &=~(7<<PB0);
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

    }


}




//UMBRAL DEL ADC




int main(void){

    GPIO_init();
    TIMER2_init();
    PWM_init();

    ADC_init();

    uint8_t btn;
    static uint8_t last_btn = None;

    while(1){


        uint16_t lectura_ADC = READ_ADC();


        if(lectura_ADC >inflexion){

            OCR0A = 204;
        }

        if(lectura_ADC < inflexion){
            OCR0A = 77;
        }



        btn=READ_BTN();



        if(btn != None){

            if(btn == last_btn){
                last_btn = None;
            }
            else{
                last_btn = btn;
            }
        }




        Update_Leds(last_btn);


    }

}
