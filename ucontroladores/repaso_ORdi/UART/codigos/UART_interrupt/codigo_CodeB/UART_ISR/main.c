
#include <avr/io.h>
#include "UART.h"




int main( void )
{
    char cad[cantidad];
    char cadUart3[cantidad];
    uint16_t num;

    //iniclaizamos los buffers
    /*tx_buffer= {{0}};
    rx_buffer= {{0}};*/



    UART_Ini(0,12345,8,1,2); //comunicacion con teraterm
    UART_Ini(2,115200,8,0,1);
    UART_Ini(3,115200,8,0,1);
    while(1)
    {
        UART_getchar(0);//getchar funciona OK
        //UART_clrscr(0);

        //UART_gotoxy(0,2,2);
        //UART_setColor(0,YELLOW);
        UART_puts(0,"Introduce un n?mero:"); //terminamos con un caracter nulo, funciona bien puts
        //UART_gotoxy(0,22,2);
        //UART_setColor(0,GREEN);
        UART_gets(0,cad); //vreremos si este funciona correcto
// -------------------------------------------
        // Cycle through UART2->UART3
        UART_puts(2,cad);
        UART_puts(2,"\r");
        UART_gets(3,cadUart3);
        //UART_gotoxy(0,5,3);
        UART_puts(0,cadUart3);
// -------------------------------------------
        //num = atoi(cad);
        //itoa(num,cad,16);

        //UART_gotoxy(0,5,4);
        //UART_setColor(0,BLUE);
        UART_puts(0,"Hex: ");
        UART_puts(0,cad);
        //itoa(num,cad,2);

        //UART_gotoxy(0,5,5);
        UART_puts(0,"Bin: ");
        UART_puts(0,cad);
    }
}




