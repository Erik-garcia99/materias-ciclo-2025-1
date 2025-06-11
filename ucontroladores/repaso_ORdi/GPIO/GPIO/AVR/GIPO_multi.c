/*
vamos a ver si como podemos leer mas de 1 pin, porque tenemos el ejemplo que lee 1 pin, y usar TIMER como un delay



 */




#include <avr/io.h>
#include<avr/interrupt.h>


//varibale para el dealy para la lectura del pin
volatile uint8_t delay_pin=0;
extern void delay_103us();

enum PORT_NUM{
    no_press=0,
    led1=1,
    led2=2,
    led3=3
};

//inicalizacion de los GPIO

void GPIO_init(void){



    //queremos con el puerto F
    //parte baja como entrada (PF0-PF1-FP2) - activando pull ups internos

    DDRF &=~(7<<PF0);
    PORTF |= (1<<PF0) | (1<<PF1) | (1<<PF2); //activanod pull ups


    //parte alta como salida (PF3 - PF4 - PF5)
    //para que in pin sea de  salida es con 1 en DDR

    /*

     xx00 0XXX
    */

    DDRF |= (1<<PF3) | (1<<PF4) | (1<<PF5);
    //los inicalizamos en bajo
    PORTF &= ~(7<<PF3);

    //PORTF |= (7<<PF3);




}






void TIMER_init(){


    sei();

    //como CTC

    TCCR0A |= (1<<WGM01);

    /*
    calcularemos el
    #ticks para el 1ms

    F_timer = Fosc / PS = 16MHz / 64 = 250000

    F_ticks = Time/F_timer = 1 / 250000 = 4uS

    #ticks =  time / F_Timer = 1ms / 4 uS = 250 -> 1 ms

    */

    TCCR0B |= (1<<CS00) | (1<<CS01);

    //activamos las interrupciones

    TIMSK0 |= (1<<OCIE0A);

    OCR0A = 250 -1;
    TCNT0 =0;


}


ISR(TIMER0_COMPA_vect){

    delay_pin++;
    delay_pin =0;
}


//vamos a tener que leer lo que entra
//usemos nomas para ver si mi idea es correcta el delay desde ASM


uint8_t read_btn(){
    //necesitamos una variabel que guarde el valor anterior


    static uint8_t last_value = no_press;

    uint8_t corrent_btn;


    /*
    el micro esta mandando :

    0000 0111
    xxxx x111


    */


    if((PINF & (7<<PF0)) != (7<<PF0)){

        delay_103us();

        PORTF |=(1<<PF3);
    }

    return 0;
}




int main(void){

    GPIO_init();

    uint8_t btn_in;
    while(1){

        btn_in = read_btn();



    }

    return 0;

}




