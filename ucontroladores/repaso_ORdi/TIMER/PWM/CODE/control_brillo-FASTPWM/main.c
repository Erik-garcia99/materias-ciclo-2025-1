


#include <avr/io.h>       // Librería para registros de E/S del AVR
#include <util/delay.h>   // Librería para _delay_ms()

// --- Definiciones ---
#define F_CPU 16000000UL  // Frecuencia del CPU: 16 MHz
/*
aqui el valor que se agregge a OCR0A o OCE0B es directamente porpocional por ejemplo
a la intencidad del PWM es decri si queremos que lede brille al 50% OCR0A = 128

porque el 50% de 256 -> 128

*/



//variable global

volatile uint8_t cycle_PWM = 0;

extern uint16_t delay(uint16_t time);




void FAST_PWM_init(void){


    TCCR0A |= (1<<WGM00) | (1<<WGM01) | (1<<COM0A1); //fast pwm->0xFF top y como no invertido

    TCCR0A &= ~(1<<COM0A0);
    /*el prescaladsor con PWM hace referencia a la frecueicna del PWM que tan rapido
    seran esos cambios

    en FAST PWM - OCR0A dica el ciclo de trabajo


    formula calcular la frecueicnia <- definida por el prescldor

    F_PWM = Fosc / PS *256 = 16MHz / 1024 * 256 = 61.03Hz


    -----------

    OCR0A define el ciclo de trabajo cunado el TOP es 0xFF, a que quiere decri esto
    el ciclo de trabajo es el tiempo que dura entre que esta arriba y despues baja

    podemos calcular ese ciclo mediante la sigueitne formula

    OCR0A = ((cycle% / 100)*256)-1

    si queremos lograr el 50% del ciclo es decri mitad del tiempo arriba mitad abajo

    OCR0A = ((50/100) * 256 ) -1 = 127;

    */



   // OCR0A = 127; //50% del ciclo de trabajo


    /*prescalador
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

    */

    //EL EJERCICO pide controlar el burillo dde un LED como el que hicmos con
    //CTC y PORTx pero ahora si con lo que es PWM

    TCCR0B |= (1<<CS01) | (1<<CS00);
    TCNT0=0;
    //TIMSK0 |= (1<<TOIE0);
    //sei();

    OCR0A=0;



}




int main(void)
{

    //para que el valor del PWM se vea reflejado en la salida es necesario ponera OC0A o OC0B
    //como salida, OC0A -> PB7 ya integrago

    DDRB |=1<<PB7;
    FAST_PWM_init();


    // Insert code

    while(1){

        //OCR0A =0;

        for(uint8_t ctn = 0 ; ctn < 255 ; ctn++){

            OCR0A=ctn;
            _delay_ms(10);

        }

        _delay_ms(500);

        //OCR0A=0xFF;

        for(uint8_t ctn = 255; ctn >0 ; ctn-- ){
            OCR0A=ctn;
            _delay_ms(10);
        }

        _delay_ms(500);

    }

    return 0;
}
