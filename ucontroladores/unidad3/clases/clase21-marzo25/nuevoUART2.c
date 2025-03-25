//clase 21 de marzo del 2024

#include<avr/io.h>
//como podemos crear una incializacion de manera generica

//calculamos cual es el que tiene menos error, en la velocidad asincrona

//velcoidad simple - 


typedef struct{
			
	uint8_t UCSRA;
		
	
	uint8_t UCSRB;
	uint8_t UCSRC;
	uint8_t res; // brinco
	uint16_t UBRR;
	
}UART_Regs_t;


uint8_t *usart_offset[]=
{
	(uint8_t *)&UCSR0A,
	(uint8_t *)&UCSR1A,
	(uint8_t *)&UCSR2A,
	(uint8_t *)&UCSR3A
	
}

//U2X =0 - 65.78 // debemos calcular el procentaje de error para saber cual debemos usar. 
//U2X =1 - 16,000,000/(15200 * 8)  = 131 

 void uart_init(uint8_t com){
	 
	//uint8_t *regA= (&UCSR0A + (com*8)); //estamos haceindo referencia hacia la direccion en donde esta UCSR0A que es donde empieza el UART0
	
	uint8_t UART_Regs_t *myUART =usart_offset[com]

	
	myUART->UCSRA = 1<<U2X0; 
	
	myUART->UCSRB= 3 << TXEN0; 
	
	myUART->UCSRC= (3<< UCSZ00) |(3<<UPM00) ; // stop bit, pridad impar no se toca. 6 bits, paridad impar, asioncrono, 1 stop bit
	
	
	myUART->UBRR=131; //PUEDE SER DE parte alta o parte baja */
	
	
	
	

}

//calcular el baud colose match en la tabla 22-1 


void UART3_WriteByte(uint8_t data){
	
	//verificamos si el bit del transmition complete esta set quiere decir que ya hubo una transferencia
	while(!(UCSR2A & (1<< UDRE3)))
		;
		
	UDR2 = data;
		
	
	
	
	
}

uint8_t UART3_ReadByte(void){
	
	while(!(UCSR2A & (1<< RXC0)))
		;
	return UDR2
	
	
}

//getchar read 

//putchar write 

int main(void){
	
	
	//UART3 esta en el pin PJ1 -PJ2 
	
	uart_init();
	
	UART3_WriteByte('H');
	UART3_WriteByte('O');
	UART3_WriteByte('L');
	UART3_WriteByte('A');
	while(1)
		;
	
	
}


/*
tenemos 4 uarts dentro del micro. 
*/