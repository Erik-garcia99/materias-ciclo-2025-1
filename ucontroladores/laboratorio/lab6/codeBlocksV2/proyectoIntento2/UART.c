

#include <avr/io.h>

// Prototypes
// Initialization
#include "UART.h"


//es una macro

UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){

    uint8_t UART_reg_t *myUART = UART_offset[com]; //eligo a mi UART

    //para inicalr el UART este debemos de especificar la velocidad de comunicacion, si tendra bit de paridad o no,
    //los bits de stop el tamanio del frame del mismo
    myUART->

    myUART->UCSRC = (parity << UMP0) | (stop << USB0); //este no importa cual sea al final seran la misma gata, estamos indicando la paridad
                                        //indicando los stopbits



}
/*
// Send
void UART_puts(uint8_t com, char *str);
void UART_putchar(uint8_t com, char data);

// Received
uint8_t UART_available(uint8_t com);
char UART_getchar(uint8_t com );
void UART_gets(uint8_t com, char *str);

// Escape sequences
UART_clrscr( uint8_t com );
UART_setColor(uint8_t com, uint8_t color);
UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);

// Utils
void itoa(uint16_t number, char* str, uint8_t base);
uint16_t atoi(char *str);*/






