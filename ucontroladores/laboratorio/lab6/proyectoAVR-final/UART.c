
#include <avr/io.h>
#include "UART.h"

// Prototypes
// Initialization

#define FOSC 16000000 //velocidad del clock <- creo que esto va a valer verga, vamos a probar 
//como se comporta si no debemos volver a reajustar para los 12M


// es una macro

/*****************************************************************************************/
//iniciamos nuestro UART

UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop)
{

	//hacer una condicion para saber si escojer doble veolcidad o velocidad normal

    UART_reg_t *myUART = UART_offset[com]; // eligo a mi UART

	
	//sacamos el valor de boud 
	uint16_t myUBRR_VN = set_UBRR(boudrate);

	/**************************************/
	//debemos pensar en una manera de como poder escoger si velocidad normal o doble velocidad
	//unit16_t myUBRR_2X = (FOSC/8/baudrate-1);

	//habilitamos RXC y TXC 

	myUART->UCSRB = 3 << TXC0;

	//porque con A no, porque al parecer A las podemos escirbir o leer pero estas se setean cunado
	//hay un dato por ejemplo con el de trasmition trasmitio todo el frame O Rreception recibio 
	//todo el frame

	//if(myUBRR_VN )

	//myUART->UBRR = myUBRR;
	/***************************************/


    // para inicalr el UART este debemos de especificar la velocidad de comunicacion, si tendra bit de paridad o no,
    // los bits de stop el tamanio del frame del mismo
	
	/*if(parity ==1){
		partity=2;
	}
	else{
		partity=3;
	}*/
	
	//establecer las variuables
	parity_mode= (parity == 1) ? 2 : 3;  // 2: Paridad par, 3: Paridad impar
	
	stop_mode= (stop==1)? 0: 1;


	myUART->UCSRC = (parity_mode << UPM00) | (stop_mode << USBS0);


    //myUART->UCSRC |= (parity << UPM00) | (((stop==1)?0:1;) << USBS0); // este no importa cual sea al final seran la misma gata, estamos indicando la paridad
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

/*el registro cunado se hace devicion con el atmega este guarda el resultado en registro de 12 bits
por lo que el rango en el cual es aceptable es de 0 a 4095 si vemos en la pagina 222 encontramos
la tabla en donde se ve el registro de la UBRR el cual es de 12 bits UBRRL = 1 byte UBRR - 4 bits*/

uint16_t set_UBRR(uint32_t baudrate){

	uint16_t ubrr_final;
	uint8_t use_doble;
	
	uint16_t UBRR_Simp= (FOSC/16/baudrate-1);
	unit16_t UBRR_2V = (FOSC/8/baudrate-1);


	uint8_t valid_Simp = (UBRR_Simp <= 0xFFF) && (baudrate <= (FOSC / 16));
	uint8_t valid_2V = (UBRR_2V <= 0xFFF) && (baudrate <= (FOSC / 8));

	if(valid_Simp && valid_2V ){

		// Calcular error para modo normal: |FOSC - 16*baud*(ubrr_vn+1)|

		uint32_t error_Simp= (FOSC > 16 * boudrate*(UBRR_Simp + 1))? 
							(FOSC - 16 * baudrate * (ubrr_vn + 1)):
							(16 * baudrate * (ubrr_vn + 1) - FOSC);

		uint32_t error_2V = (FOSC > 8 * boudrate*(UBRR_Simp + 1))? 
							(FOSC - 8 * baudrate * (ubrr_vn + 1)):
							(8 * baudrate * (ubrr_vn + 1) - FOSC)
	
		
		use_doble = (error_2V < error_Simp);
		ubrr_final= use_doble? UBBR_2V: UBRR_Simp;
	
	}
	else if(valid_Simp){
		ubrr_final=UBRR_Simp;
	}
	else if(valid_2V){
		use_doble=1;
		ubrr_final=UBRR_2V;
	}
	else{
		//boudrate no soportado
		return ;
	}

	return ubrr_final
	
}



/****************************************************************************/

/*
// Send
void UART_puts(uint8_t com, char *str);
void UART_putchar(uint8_t com, char data);

// Received*/

//aguanta mientras no haya dato en para trasmitir
uint8_t UART_available(uint8_t com){

	 UART_reg_t *myUART = UART_offset[com]; //con esta varuable me desplajo al UART a usar
	 //si no existe dado en el periferico aqui se queda cunado exista arroja 1

	while(!(myUART->UCSR0A & (1<<UDRE0)))
		;

		return 1;

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
