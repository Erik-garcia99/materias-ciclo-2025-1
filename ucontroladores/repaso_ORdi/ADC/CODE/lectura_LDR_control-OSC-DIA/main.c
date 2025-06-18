/*}

 LA  histéresis viene siendo una variable con la cual evitamos un cambio bursco entre
 condiciones, por ejemplo en el ADC con un potenciomentro o un LDR
 en el cual al hacer cambios entre valores si los conectamos con LEDS estos tienden a
 parpadiar por ruido en la respuesta, para evitar esto se usa la < histéresis - HYSTERESIS>

 con la cual si pasa de X umbral cambia su estado-

 */


/*
ejercicio:
    Ejercicio 2: Detección de Umbral para Encender un LED
Enunciado:
Lee un sensor de luz (LDR) o temperatura (LM35) con el ADC. Si el valor supera un umbral predefinido, enciende un LED.
Partes Críticas:

Calibrar el umbral correctamente (depende del sensor y condiciones ambientales).

Implementar histéresis para evitar oscilaciones al borde del umbral.

Filtrar ruido eléctrico para evitar falsos positivos.


*/


#include <avr/io.h>


//el ejercicio pide que si se supera X umbral un led debe de prender -> puede que sea de
//noche

/*

que seria oscruto, que pase del 40% del ADC no crees?,

cual es el 40% del ADC, este es por dar un ejemplo


100% - 1023
40%

VALOR_PCTJ = (cantidad total * porcentaje) / 100 -> (1023 * 40) / 100

HYSTERESIS = 409.2 -> si pasa de este umbral, que sea menor quiere decri que esta oscuro
 por lo que deberia de prender el LED



 la configuracion de como esta el LDR por ejemplo tambine importa, no me salia porque
 la cofiguracion no era la adeciada

 lo tenia peimero como si fuera un "pull up" -> con una reisitenica(10Kohms)
 a Vcc y de ahi salia a ADC9, pero este siempre me daba lo maximo

 por lo que lo cambia un "pull down" -> ahora con una resistencia a GND

-Mucha luz -> Resistencia LDR baja -> Voltaje en ADC9 alto -> resultado_ADC alto.

-Poca luz/Oscuridad -> Resistencia LDR alta -> Voltaje en ADC9 bajo ->
 resultado_ADC bajo. Esto concuerda con tu lógica de que si resultado_ADC es bajo,
  enciendes el LED.


*/

static uint16_t HYSTERESIS = 204;
//variable que guarda el valor del ADC completo de 10 bits


//inicamos el ADC

void init_ADC(void){

    //ajustado a la drecha, VCC como voltaje de referencia, ADC9
    ADMUX = (1<<REFS0) | (1<<MUX0);
    //  ADC0 y FREE RUNNER
    ADCSRB |= (1<<MUX5);
    ADCSRB &=~(7<<ADTS0);
    //activamos ADC , comenzamos la conversion (esto porque lo haremos de manera continua
    //si fuera de 1 a 1 seria diferente, despues haremos 1 con ese proposito),
    //ADATE - ACTIVA LA CONVERSION AUTOMATICA, RELAIZA UNA CNONVERSION EN EL PRIMER FRANCO
    /*
    DE SUBIDA.

    PS de 128 es el unico que esta dentro del rango que puede operar el ADC

    habilitamos interrupciones
    */
    ADCSRA = (1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (7<<ADPS0) | (1<<ADIF);

    //ADC9 -> PK1
    DIDR2 = (1<<ADC9D); //deshabilita la funcion digital del PIN del ADC9

}







int main(void)
{

    init_ADC();

    //habilitamos los puertos para ver el ADC

    DDRA = 0xFF;
    DDRC = (1<<PC6) | (1<<PC7);
    DDRB = (1<<PB7); //el led que va a prender cuando disminuya el umbral

    // Insert code

    while(1){

        if(ADCSRA & (1<<ADIF)){

            uint8_t low = ADCL;
            uint8_t high = ADCH;

            uint16_t resultado_ADC = (high << 8) | low;


            //solo me lo muestra con BITS
            PORTA = low;
            PORTC = (high << PC6);

            if(resultado_ADC > HYSTERESIS){

                PORTB &=~(1<<PB7);
            }
            else{
                //mantenerlo apagado
                PORTB |= 1<<PB7;

            }


            ADCSRA |= (1<<ADIF); //limpiamos la bandera
        }



    }

    return 0;
}
