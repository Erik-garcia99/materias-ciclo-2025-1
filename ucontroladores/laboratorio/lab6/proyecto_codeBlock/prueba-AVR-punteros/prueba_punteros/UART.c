

#include <avr/io.h>
#include "UART.h"

// Prototypes
// Initialization

#define FOSC 16000000 //velocidad del clock


// es una macro

UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop)
{

	//hacer una condicion para saber si escojer doble veolcidad o velocidad normal

    UART_reg_t *myUART = UART_offset[com]; // eligo a mi UAR


	//sacamos el valor de boud
	uint16_t myUBRR_VN = (FOSC/16/baudrate-1);

	/**************************************/
	//debemos pensar en una manera de como poder escoger si velocidad normal o doble velocidad
	//unit16_t myUBRR_2X = (FOSC/8/baudrate-1);


	//if(myUBRR_VN )

	//myUART->UBRR = myUBRR;
	/***************************************/


    // para inicalr el UART este debemos de especificar la velocidad de comunicacion, si tendra bit de paridad o no,
    // los bits de stop el tamanio del frame del mismo

	if(parity ==1){
		partity=2;
	}
	else{
		partity=3;
	}




    myUART->UCSRC = (parity << UPM00) | (((stop==1)?0:1;) << USBS0); // este no importa cual sea al final seran la misma gata, estamos indicando la paridad
                                                       // indicando el/los stop bits

	if(size!=9 && size <= 8){

		//estos no son necesario
		switch(size){

			case 6:
				myUART->UCSRC = 1 << UCSZ00;
				break;

			case 7:
				myUART->UCSRC = 2 << UCSZ00;
				break;

			case 8:
				myUART->UCSRC = 3<< UCSZ00;
				break;

		}

	}
	else{
		//en este caso del tamanio sera de 9 y terndremso que activar el bit del otro registro

		myUART->UCSRC = 3 << UCSZ00;
		myUART->UCSRB = 1<< UCSZ02; //habilitamos los 9 bits
	}

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
