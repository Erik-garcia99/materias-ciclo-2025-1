#include <avr/io.h>
#include "UART.h"

// Prototypes
// Initialization

#define FOSC 16000000 //velocidad del clock <- creo que esto va a valer verga, vamos a probar
//como se comporta si no debemos volver a reajustar para los 12M


// es una macro

/*****************************************************************************************/
//iniciamos nuestro UART

UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){

    UART_reg_t *myUART = UART_offset[com]; // eligo a mi UART

    myUART->UCSRB = 3<< TXC0;


    //establecer las paridades
	uint8_t parity_mode= (parity == 1) ? 2 : 3;  // 2: Paridad par, 3: Paridad impar


    uint8_t stop_mode= (stop == 1)? 0:1;

    myUART->UCSRC = (parity_mode << UPM00) | (stop_mode <<USBS0);


    //tamanio del frame
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

    uint16_t v_UBRR = (16000000 / (8 * baudrate)) - 1 ;

    myUART->UBRR = v_UBRR;
}


// Send
void UART_puts(uint8_t com, char *str){

	//TXn trasmitir el contenido 

	while(*str){
	
		//mientras haya contenido en el apuntador, que sea diferente a NULL 


		while()
	}

	

}



void UART_putchar(uint8_t com, char data){

	//este lo que hace es trasmitiri un caracter 

	//perimro debemos de mostrar a cual UART es 

	//que nos dice la teoria, la teroia nos dice que su UDR esta en 1 quiere decir
	//que el registro esta vacio, 

	//entonces el ciclo va a espear para cunado este se vacie por completo
	//lo que hace es esperar mientras haya datos dentro del buffer

	UART_reg_t *myUART = UART_offset[com]; 

	while(!(myUART->UCSRA &(1<<myUART->UDR)))
		; //espera a que el periferico este vacio 


	myUART->UDR= data;
}



// Received



//aguanta mientras no haya dato en para trasmitir
/*uint8_t UART_available(uint8_t com){

	//si hay 1 queire decir que el buffer esta vacia 

	//nosotros queremos esperar siempre y cunado este vacio, si hay un dato en el perferico
	//estara en 0 y nos sacara porque ya hay algo en el periferico

	 UART_reg_t *myUART = UART_offset[com]; //con esta varuable me desplajo al UART a usar
	 //si no existe dado en el periferico aqui se queda cunado exista arroja 1

	while(myUART->UCSRA & (1<<UDRE0))
		;

		return 1;  //regresa 1 cunado esta disponible 
}
*/

uint8_t UART_available(uint8_t com){

	//RXC0 sta en 1 cuando hay un dato sin leer en RXC 
	//y esta en 0 cunado este no tiene nada 

	UART_reg_t *myUART = UART_offset[com]; 

	return (myUART->UCSRA & (1 << RXC0)) ; // Hay dato disponible
	//creo que va a asi pero si hay errores podemos invertirlo

}



/*char UART_getchar(uint8_t com );
void UART_gets(uint8_t com, char *str);

// Escape sequences
UART_clrscr( uint8_t com );
UART_setColor(uint8_t com, uint8_t color);
UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);

// Utils
void itoa(uint16_t number, char* str, uint8_t base);
uint16_t atoi(char *str);*/
