//haremos un programa sencillo con time en su modo normal el cual quiero que solo
//pasado X tiempo se prenda un led.

#include<avr/io.h>

extern void delay_103us();

volatile uint16_t milis=0;

void Timer0_init(void){

    //con el modo normal quiero que cunado se llegue al segundo se prenda el led

    //habilitamos el modo normal del TIMER (contdor asendente hasta MAX 0xFF)

    TCCR0A = 0; //modo normal

    //seleccionamos el prescalador
    //como seleccionamos el prescalador

    //tenemos que

    /*

    quiero 1 ms
    quiero lograr X numeros de ticks que conforman X tiempo

    debemos de calcular la frecuecnia del reloj del timer

    F_Timer = Fosc / PS -> 16MHz / 1024 = 15625 - 250000

    tiempo por tick del timer (el tiempo que darta en dar un cambio por el prescaldor del timer)

    T_ticks = 1/F_Timer = 1/ 15625 = 64.00 uS - 4uS

    por lo que los ticks que deben de ser para que sea el segundo es de :

    TicksDeseados = Time_deseado / T_ticks = 1mS / 64uS = 15.65 -> 16

    -> con 16 ticks con prescalador de 1024 el timer dara 1S



    */


    //entonces como seria en el modo normal?, debemos asignr un OFFSET para que cunado llegue a 255 - 0xFF
    //se actvie la bander de OVERFLOW.

    //PRECLACALOR

    TCCR0B |= (1<<CS02) | (1<<CS00);

    TCNT0 = 240;
    TIFR0 |= 1<<TOV0;


}








int main(void){


    //configurar PB7 como salida

    DDRB |= 1<< PB7;
    Timer0_init();


    while(1){

            if(TIFR0 & (1<<TOV0)){

                milis++;
                //limpiamos la bandera

                TIFR0 |= 1<<TOV0;
                TCNT0 = 240;

                if(milis == 1000){
                    PINB |= (1<<PB7); // Enciende el LED
                    // delay_custom(algún_tiempo_visible_en_ms); // Podrías crear otro delay o usar un contador de milis para apagarlo
                    // PORTB &= ~(1<<PB7); // Apaga el LED

                    /*
                    PORTB |= 1<<PB7;

                    delay_103us();

                    PORTB &= ~(1<<PB7);*/

                    milis = 0; // Reiniciar contador de milis

                }
            }



    }


}


