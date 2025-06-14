/*
 */

#include <avr/io.h>
#include "UART.h"






int main(void)
{

    char cad[cantidad];
    char cadUART3[cantidad];

    //comunicacion con teraterm
    UART_init(0,12345,1,2,8);
    UART_init(2,115200,0,1,8);
    UART_init(3,115200,0,1,8);

    while(1){

        UART_getchar(0);

        UART_puts(0,"introduce un numero:");

        UART_gets(0,cad);

        UART_puts(2,cad);
        UART_puts(2,"\r");
        UART_gets(3,cadUART3);

        UART_puts(0,cadUART3);

    }




    return 0;
}
