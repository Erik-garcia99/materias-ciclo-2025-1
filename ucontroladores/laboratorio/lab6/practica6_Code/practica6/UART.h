

//estrucutra que define a los distintos uarts
typedef struct{

	uint8_t UCSRA;
	uint8_t UCSRB;
	uint8_t UCSRC;
	uint8_t resb;
	uint16_t UBRR;

}UART_reg_t;


/*
para poder acceder a ;ps registros

*/

uint8_t *UART_offset[]=
{
	(uint8_t*)&UCSR0A,
	(uint8_t*)&UCSR1A,
	(uint8_t*)&UCSR2A,
	(uint8_t*)&UCSR3A

};
