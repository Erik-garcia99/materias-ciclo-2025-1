/*
ejercicio :

Ejercicio 2: Luz de Emergencia Automática (Hazard inteligente)
Objetivo: Diseñar un sistema de luz de emergencia para un vehículo que
se active automáticamente en función de la detección de una posible frenada
repentina (simulada con un sensor), y que también permita activación manual.

Requerimientos
Entradas:

Botón conectado a PD2 (modo manual de emergencia).

Potenciómetro conectado a ADC0 (simula sensor de aceleración/frenado).

Salidas:

LEDs conectados a PORTB (PB0–PB5) que simulan intermitentes de emergencia.

LED PWM en PB6 (OC0A) que parpadea con brillo variable (advertencia adicional).

Condiciones de Activación:

Si el valor del ADC es menor a un umbral (ej. < 200), se interpreta como una
frenada brusca → Se activa modo emergencia automática.

Si el botón en PD2 se presiona, se activa modo emergencia manual.

Secuencia de Salida:

En modo emergencia (manual o automática):

Los LEDs PB0 a PB5 deben parpadear todos juntos cada cierto tiempo (~250 ms).

El LED en PB6 (OC0A) debe parpadear con brillo proporcional al ADC,
si es emergencia automática. Si es manual, el brillo es constante (ej. 128).

Si no hay emergencia, todas las salidas deben apagarse.


------------------------------------------

el ejercicia solicita

TIMER// - PWM// - GPIO// - ADC -> GOOD


-> al parecer por lo que veo, si es de forma manual, el frenado por el BTN
los led deben de permanecer prendidos siempre y cunado este precionado el BTN (creo)


 */

#include <avr/io.h>
#include<avr/interrupt.h>
//facil - cofiguracion de GPIO - TIMER2

//umbral de cambio para el ADC
static uint16_t HYSTERESIS = 800; //20% del ADC, si el ADC es menor esta umbral se activa

//varibale de timer para el parpadero de leds



volatile uint8_t milis=0;

//para la lectrua del PIN
volatile uint8_t milis_delay=0;

volatile uint16_t sec_1mS = 0;


//este enum sera para el boton,
enum EMG{
    ON = 0,
    OFF,
    ON_ADC,
    OFF_ADC

};
void GPIO_init(void){


    //boton conectado a PD2 -> GPIO IN - PULL UP

    DDRD &=~(1<<PD2);
    PORTD |=(1<<PD2);

    //leds de salida PUERTO B PB0 - PB5
    // 0011 1111
    DDRB |=(0x3F <<PB0); //led que simila las luces de emergencia

    //salida del PWM -> el PWM lo haremos con el TIMER0 - 0C0A -> salida -> PB7

    DDRB |=(1<<PB7); //salida del PWM
}


//inciamos TIMER

void TIMER_init(void){


    //timer que contara cada mS

    TCCR2A = (1<<WGM21); //modo CTC

    /*
    calcular cuantos ticks para OCR2A

    F_TIME = 16MHz / PS -> 16MHz / 64 = 250000

    F_ticks = 1/F_TIMER = 1/250000 = 4uS

    #ticks = 1mS / 4 uS = 250 ticks = 1mS

    */
    //PS -> 64
    TCCR2B = (1<<CS22);
    OCR2A = 250 -1;
    TIMSK2 = (1<<OCIE2A); //habilitamos interrupciones

    sei();
    TCNT2 = 0;


}


ISR(TIMER2_COMPA_vect){

    //aumentamos la varibale global que cuenta para los 250mS de parpadeto de los led
    milis++;
    //usamos lo mismo para el delay del BTN
    milis_delay++;

    sec_1mS++;
}

//AHORA PB7 que es la salida del PWM la ocupamos para generar un PWM con un ciclo de trabajo definido por PWM

void PWM_init(void){
    //PWM con TIMER0
    //FAST PWM TOP = 0xFF - modo no invertido
    TCCR0A = (3<<WGM00) | (1<<COM0A1);
    /*
    prescalador creo que se usa el de 64

    F_TIMER = 16MHz / 64 * 256 = 976.56 ~ 1kHz

    */

    TCCR0B = (3<<CS00);

    OCR0A = 0; //inicmaos sin ciclo de trabajo
    TCNT0 =0;

}


void ADC_init(void){


    //ahora lo haremos de lectrua a lectura
    //AVCC - Voltaje de referencia
    //ADC0
    //ajusdado a la derecha
    ADMUX = (1<<REFS0);
    /*
    prendemos el ADC, limpiamos bandera, PS de 128 = 125000 Hz -> rango aceptable
    */
    ADCSRA = (1<<ADEN) | (1<<ADIF) | (7<<ADPS0);

    ADCSRB= 0;

    DIDR0 = (1<<ADC0D);
}



uint16_t READ_ADC(void){


    uint16_t lectura;

    //inicia la conversion
    ADCSRA |= (1<<ADSC);

    while(!(ADCSRA & (1<<ADIF))); //espera a que el ADSC se limpie por hardware



    uint8_t low = ADCL;
    uint8_t high = ADCH;


    lectura = (high << 8) | low;



    ADCSRA |=(1<<ADIF); //limpiamos la bandera



    return lectura; //mientras no se termnie la cnovresion envia 0.
}


//lecutra del PIN
/*
mejor sera como las intermintentes creo, si se presiona se penden las luces,
para apagarlas se vuelve a precionar pero para esto debemos bloquea o limpiar lo del
ADC para que solo acepte el BOTON.
*/

/*
uint8_t READ_BTN(){

    static uint8_t bound = 0;
    static uint8_t state = OFF;

    if(!(PIND & (1<<PD2))){

        if(milis_delay > 50){

            milis_delay = 0;
            if(bound == 0){
                bound =1;
            }
            else{
                bound = 0;
            }
        }
    }


    if(bound == 1){

        if(state == OFF){

            state = ON;

        }
        else{
            state = OFF;
            bound =0;
        }


    }

    return state;
}*/

uint8_t READ_BTN(){

    static uint8_t lastState = 1;   // pull-up, estado alto inicial
    static uint8_t toggle = OFF;

    if (!(PIND & (1 << PD2))) { // Si se presiona (nivel bajo)
        if (milis_delay > 50 && lastState == 1) {
            milis_delay = 0;
            toggle = (toggle == OFF) ? ON : OFF;
        }
        lastState = 0;
    } else {
        lastState = 1; // Botón liberado
    }

    return toggle;
}




void LEDS_SECUECINA(uint8_t state){

    //recibimos el estado del BTN de la funcion.

    //si devuelve ON entonces actiavremos la secueicna
    /*
    harmeos un switch-case para poder pasar de apago a encendio en los 250 MS
    */
    static uint8_t estado_sec = 0; // 0 ON - 1 OF

    if(state == ON || state == ON_ADC){

        switch(estado_sec){

            case 0:
                PORTB |= (0x3F);//prendemos los led

                if(milis >= 250){
                    milis =0;
                    estado_sec = 1; //apagamos
                } break;

            case 1:
                PORTB &=~(0x3F);

                if(milis>= 250){
                    milis = 0;
                    estado_sec = 0; //volvemos a encender

                } break;


            }

        }



    else if (state == OFF || state == OFF_ADC){

        //si manda OFF apagsmoa todo
        PORTB &=~(0x3F);
        //aunque no cambiemos, no estamos receteanod el timepo en la ISR por lo que si no
        //lo hago se irea hasta el OVERFLOW
        if(milis>= 250){
            milis =0;
        }
    }
}





int main(void)
{

    // Insert code
    GPIO_init();

    TIMER_init();

    PWM_init();

    ADC_init();


    //una varibale que me diga


    while(1){

        //aqui se va a estar leyendo el pótenciometro o el BTN

        uint16_t lectura_ADC = READ_ADC();

        //necesimaos enviar lo que se lee en el BTN a la funcion que actualiza los leds
        //pero si esta presionado, debemos interrumpir el ADC y darle prioridad
        //al boton.
        //BTN me regresa ON o OF
        uint8_t BTN = READ_BTN();

        //le demo de dar prioriad a BTN
        if(BTN == ON){

            //si habia algo con ADC lo limpiamos los leds

            //PORTB &=~(0xBF <<PB0);

            LEDS_SECUECINA(ON);
            OCR0A = 128; //50% de ciclo de trabajo


        }
        else{

            //si no es el BTN sera con ADC




            if(lectura_ADC < HYSTERESIS){

                //SI EL adc baja del umbral activa PIN

                LEDS_SECUECINA(ON_ADC);
                OCR0A = lectura_ADC/4; //ciclo de trabajo adecuado para 8 bits 255
            }
            else{

                LEDS_SECUECINA(OFF_ADC);
                OCR0A = 0;
            }
        }

    }

    return 0;
}
