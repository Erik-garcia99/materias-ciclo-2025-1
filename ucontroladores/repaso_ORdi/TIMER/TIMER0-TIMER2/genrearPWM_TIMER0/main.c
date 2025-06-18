/*

    ele ejercico pide una frecueicna de 62.5kHz

    gradualmente invrementar el ciclo de trabajo de 0 a 100%

 */

#include <avr/io.h>
#include<avr/interrupt.h>


volatile uint8_t milis=0;

void PWM0_init(void){


    /*
    okay, en el modo de PWM donde 0xFF es el tope, el prescalador dictara la frecueicnia

    y OCR0A o OCR0B dicara el ciclo de r¿trabajo

    haremos perimro con OCR0A y despues con OCR0B

    para esto solo nos tendremos que ajustar a los PS que tiene el TIMER0 o TIMER2



    */

    TCCR0A = (3<<WGM00) | (1<<COM0A1); //fast-pwm (TOP-0xFF), modo no invertido

    /*

    calculo del PS y ocr0a
     que me define el prescaldor, el prescalador me esta definiendo la frecueicna
    del PWM que tan rapido vamos a poder ver esos cambios, sin osciloscopio podremos
    hacerlo medinate LEDs pero el ojo humano solo es capza de vez a 50Hz - 60Hz, puede ver cambios

    mas alla o menos no notara diferencia

    cunado el TOP para PWM es 0xFF solo podremos tener los siguenites valores: cquqe podemos calcar ocm

    F_PWM = Fosc / PS * TOP

    F_PWM = 16MHZ / 8 * 256 = 7812.5 Hz

    F_PWM = 16MHZ / 64 * 256 = 976.56 Hz -> VAMOS A ELEIR ESTE

    F_PWM = 16MHZ / 256 * 256 = 244.14 Hz

    F_PWM = 16MHZ / 1024 * 256 = 61.03 Hz



    para calcular el ciclo de trabajo usamos la sigueitne fomrula

    duty_cycle = (OCR0A + 1) / 256 * 100% -> OCRxn = (Duty_cycle *256/100) -1


    75% de cilce duty

     OCR0A = (((75 * 256) /100))) - 1=

     OCR0A = (19200 / 100) -1 -> 192 -1 = 191//
     OCR0A = 192 para 75% de Duty_cycle con una freuencia de 61

    */

    TCCR0B = (1<<CS00); //sin PS sera el mismo que el reloj interno

    OCR0A = 0; //inicmaos el ciclo de trabajo en la parte baja
    sei();

    TCNT0=0;


}

//usaremos TIMER2 como un cnotador para incremnetar el ciclo de trabajo

void TIMER2_init(void){



    TCCR2A = (1<<WGM21); //modo CTC

    /*
        PS para 1 MS

        F_timer = 16MHz / ps = 16MHz / 64 = 250000

        F_tikc = 1/F_timer -> 1/250000 = 4uS

        #tikc = 1mS / 4 uS = 250 -> cada 250 ticks 1 ms

    */

    TCCR2B = (1<<CS22);
    OCR2A = 250 -1;

    TIMSK2 = (1<<OCIE2A); //hbailitar la interrupcion
    TCNT2 =0;
    sei();



}

//en esua ISR aumentamos
ISR(TIMER2_COMPA_vect){

    milis++;
}


/**/
void duty_cicle_update(void){


    for(uint8_t ctn = 0; ctn <= 255 ;){


        if(milis == 50){
            milis =0; //reinciamos el conteo
            OCR0A = ctn++;
        }
    }

    for(uint8_t ctn = 256 ; ctn > 0 ; ){


        if(milis == 50){
            milis =0;
            OCR0A=ctn++;
        }
    }



}





int main(void)
{
    //tenog uqe hbailitar PB7 que es la aslaida del PWM -TIMER0

    DDRB |=1<<PB7;

    PWM0_init();
    TIMER2_init();

    while(1){

        duty_cicle_update();
    }

    return 0;
}
