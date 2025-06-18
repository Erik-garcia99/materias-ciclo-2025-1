/*
realizamos una lectrua de un LDR con ADC

 vamos a mostrar los resultados por medio del PUERTO A(8 BITS) - PUERTO c (2 bits)

(EN ESTE LO HAREMOS CON los menos significativos en ADCL - ADLAR = 0)



-> HAREMOS UN PWM con el ciclo de trabajao definido por el voltaje del ADC

pero como metemos un valor de 16 bits en un rgistro de 8 bits

mas bien los 10 bits del ADC (1024) -> (255)

creo que el resultado seria dividido entre 4

 */

#include <avr/io.h>
//NO INTERRUPCIONES SOLO CEHCHAMOS LA BANDERA



void init_ADC(void){

    //referencia a  AVCC, ajustadoi a la drecha, con el ADC0
    ADMUX = (1<<REFS0);

    //ADC enable, limpiamos la bandera que indica la cnovresion del ADC.
    //necesuta una frecueicna de 50Khz a 200 kHz
    /*
        el unico que esta entre ese rango es con 128 -> 16MHz / 128 = 125000 Hz

        INICA EL ADC

        EL MODO DE AUTO TRIGGER
    */
    ADCSRA = (1<<ADEN) | (1<<ADIF) | (7<<ADPS0) | (1<<ADATE) | (1<<ADSC);

    //MODO FREE RUNNER
    ADCSRB = 0 ;


    DIDR0 = (1<<ADC0D); //DESHABILITAMOS LA CONFIGURACION DIGITAL DEL PIN
}


void PWM0_init(void){

    //fast OWM con 0XFF com TOP, modo no invertido
    TCCR0A = (3<<WGM00) | (1<<COM0A1);
    /*

    frecuencia de

    F_PWM = 16MHz / 64 * 256 = 976. 56 Hz
    */
    TCCR0B = (3<<CS00);

    OCR0A =0 ; //inicmaos sin ciclo de trabajo, el led estara "encendio"
    TCNT0 = 0;
}



int main(void)
{

    init_ADC();

    DDRA |= 0xFF;

    DDRC |= (1<<PC7) | (1<<PC6);

    DDRB |= 1<<PB7;
    PWM0_init();

    while(1){

        if(ADCSRA & (1<<ADIF)){

            /*
        !!!!!!!!!IMPORTANTE!!!!!!!!!!!!!!

        el ADC se debe de leer en un orden especifico,,

        en el ADC primero se debe de leer ADCL y despues ADCH, si se hace al revez este
        se bloqueara y no permitira leer de nuevo.



        */

        /*PORTA = ADCL;
        PORTC = (ADCH << PC6);*/

            uint8_t low = ADCL;
            uint8_t high = ADCH;


            PORTA = low;
            PORTC = (high << PC6);

            uint16_t lectura =( high << 8 ) | low;

            uint8_t DUTY_CYCLE = lectura /4; //porque entre 4,
        /*
        el ADC en su 100% es 1024, pero ese numero no lo podemos poner en un registro de 8 bits
        como lo es OCR0A, entonces si dividos entre 4 seria 256 el 100% en 8 bits.
        */

            OCR0A = lectura >> 2; //divicion entre 4

            ADCSRA |= 1<<ADIF;
        }
    }

    return 0;
}
