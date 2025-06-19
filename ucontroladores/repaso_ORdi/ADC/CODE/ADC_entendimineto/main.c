/*
 */

#include <avr/io.h>
#include<avr/interrupt.h>


/*

creo que ya le ando hagarrando la onda a todo esto del ADC, por lo que haremos el mismo
programa que estaba pero ahora de una mejor manera que entendamos.


*/

//en este ejemplo solo mostraremos por puerto lo que hay en nuestro ADC

void init_GPIO(void){

    //en este vamos a seleccionar un puerto de salida, el PUERTO A es buena opcion

    //para poner un pin en salida es con 1
    DDRB = 0xFF; //contandrra ADL 8 bits de la parte baja
    //PC7-PC6 son los seran para la parte baja con estos 10 bits del ADC
    DDRC = (1<<PC7) | (1<<PC6); //contendra ADCH parte alta del ADC
    //DDRC = 0xFF;

}

void init_ADC(){

    //seleccionaremos el ADC0
    //ALAR=0; ACOMODADO A LA DERECHA
    //como referencia AVCC = 5V
    ADMUX = (1<<REFS0) ;
    //EL TIMER DEBDE DE ESTAR en una Frecuencia de 50kHz - 200kHz
    //F_timer = 16MHz / 128 = 125,000
    ADCSRA = (7<<ADPS0) | (1<<ADEN) | (1<<ADATE) | (1<<ADSC);
    //como estamos en el modo de conversion susecviam debemos escoger el evento
    //que va a decir

    ADCSRB = 0;
    //en este modo es encesario decir cual sera el eventro de disparo, en este caso
    //vamos a decir que es "freee runnig mode" por lo que los registros
    //ASRS2:0 = 0.

}











int main(void)
{

    init_GPIO();

    init_ADC();
    // Insert code

    while(1){

        //vamos a checar si se ha completado la conversion, cuando esto pase imprimimos el valor
        //por medio de los lEDS

        //en este while le estoy diciendo que espere hasta que la bandera se setie, cunado
        //la bandera esta en 1 quiere decir que la conversion se completo, por lo que miestras
        //que no este esta en 0, por lo que negamos la condicion para, miestras que no se haya
        /*
        completado la conversion permanecera en el while, por lo que estara en 1, cunado la conversion
        termine este pasara a 1 real pero por la negacion pasara a 0 condicion falsa para el while






        */


        while(!(ADCSRA & (1<<ADIF)));

        //una vez que este se active debemos limpiarlo por software, si se hiicera por interrupciones
        //ña interrupcion misma lo limpia, pero con poner 1 se limpia

        //pasamos el valor alto al registro de puerto

        PORTB = ADCL;
        PORTC = (ADCH & 0x03);

        //limpiamos la badera de interrupciones, vuelve a inicar a esperar a la sigueitne conversion

        ADCSRA |= (1<<ADIF);



    }

    return 0;
}
