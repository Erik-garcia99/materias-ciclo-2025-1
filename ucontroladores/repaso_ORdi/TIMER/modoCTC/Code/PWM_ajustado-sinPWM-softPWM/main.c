/*
realizar un PWM por un PIN, el PINX
 */

#include <avr/io.h>
#include<avr/interrupt.h>

volatile uint8_t pwm_setep = 0;
volatile uint8_t duty_ciicle = 70;


void softPWM_TM0(void){

    /*
        queremos un PWM de 10kHz por ejemplo
        por lo que la formula para darle a OCR0A que dicta el ciclo
        de trabajo es el siguiente

        OCR0A = (F_cpu / PS * fs_deseada) -1

        OCRxn = 16MHz / 64 * 10kHz = 25 -1 = 24 - cunado el TIMER
        TCNT0  haga match con 10
    */
    //modo CTC
    //a dferentcia con el otro que es siempre 50% ciclo de trabajo
    //aqui no togleamos porque aqui el ciclo de trabajo sera modificado
    TCCR0A = (1<<WGM01);
    sei(); //habilitamos las interrupciones

    /**/
    TCCR0B = (3<<CS00);
    OCR0A = 24;
    TCNT0= 0;
    TIMSK0 = (1<<OCIE0A);


}


ISR(TIMER0_COMPA_vect){
    pwm_setep++;

    if(pwm_setep >= 100){
        pwm_setep = 0;
    }
    //PF0 como el pin de salida
    /*
    lo que hace aqui es que estamos diviendo el 100%
    de la resolucion del PWM en sus partes correspondientes
    a que me refiere

    un PWM dura 100%, el ciclo de trabajo inica que tanto de ese porcentaje estara arriba y cunado aabajo,
    entonces, para cunado este arriba es el porcentaje de nosotros le damos, en este cao 30%, 30% arriba, 70% abajo.

    entonces los que hace es contar, como un barrido discreto de la senial, entoncec cunado los pasos sean menores
    al ciclo de trabajo esta "ON" por lo que sebe ser una senial de salida,
    si el PWM_step es mayor entonces debe de estar encendidos

    */
    if(duty_ciicle > pwm_setep){
        PORTF |= (1<<PF0); //poner ese PIN en alto lo que hace es gradualmente incrementa
    }
    else{

        PORTF ^= (1<<PF0); //bajamos el PIN

    }


}

/*
como conoer el ciclo de trabajo, como ajustar ese ciclo de trabajo

frecueicnia del PWM

frecueicnia_INT / numero de pasos:

comoc: 10kHz / 100 = 100Hz

*/




int main(void)
{

    softPWM_TM0();
    DDRF |= 1<<PF0; //ponemos el PIN como salid

    while(1)
    ;

    return 0;
}
