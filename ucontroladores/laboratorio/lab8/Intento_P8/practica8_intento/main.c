#include <avr/io.h>
#include "Timer0.h"
/* incluir lo necesario para hacer uso de UART0 */

void UART0_AutoBaudRate(void)
{
    /* Implementar */
}

void Clock_Ini(uint64_t millis)
{
    /* Implementar */
}

void Clock_Date_Display()
{
    /* Implementar */
    UART_puts(0,'+1s\n\r');
}

uint64_t millis(void){


}

int main()
{
    /* llamar a función para inicializar puertos E/S */
    DDRB |= 1 << PB7;
    /* llamar a función para inicializar UART0 */

    UART0_AutoBaudRate();

    UART_clrscr(0);
    UART_gotoxy(0,5,1);
    UART_puts(0,"Autobauding done. UBRR0=");
    itoa(UBRR0,str,10);
    UART_puts(0,str);
    UART_puts(0,'\n\r');

    Timer0_Ini(); /* Inicializar contador de millis.*/
    Clock_Ini(1744376390000);

    while(1)
    { /* == main loop == */
        if( Timer0_SecFlag() )
        { /* ¿ha pasado un Segundo? */
            UART_gotoxy(0,5,2);
            Clock_Date_Display();
            UART_gotoxy(0,5,3);
            /* Parpadea LED L */
            PINB = 1 << PB7;
        }
    } /* fin del loop principal */
    return 0; /* <-- no se llega aquí */
}
