//este documento sera una inicalizacion simple, asi como mandar y recibir datos, por UART, tenemos que avanzar hasta las interrupciones rapidito



#include <avr/io.h>
#include "UART.h"



UART_init(void){

    //primero debemos saber la velocidad a la cual nos queremos comunicar, (utilizaremos UART 0 en esta coacion para comnicarnos con la terminal)

    /*
        formula para calcular esto, tenemos

        UBBR = Fosc / (16 * BOUD) + 1 -> veocidad normal

        UBBR = Fosc / (8 * BOUD) + 1 -> velocidad doble
    */
    UBRR0 =


}




int main(void)
{

    // Insert code

    while(1)
    ;

    return 0;
}
