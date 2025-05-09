//clase 27 de marzo del 2025

//interrupciones - periferico 

//interrupcion, interrumpir el CPU, lo interrumpe para hacer otra cosa
/*

		cunado ocurre la interrupcion se va a unlugar que se llama vector de interrupciones que este tendra la direccion a donde debe de brincar 
		
		-guarda PC y push flags para seguir con su flujo , guarda banderas igual
		
		cunado al interrupcion se termina vuelve a PC +1 (esta regresa con RETI) hace pop de PC, de Flags 
		
		se define de ISR 
		
		interrupciones en el cap 14
		
		la que tenga la direccion mas chica es de mayor prioridad que la que tiene mayor direccion, por ejemplo reset tiene $0000 es el de prioridad
		porque es para el resest de la placa
		
		USART0 
		
		3 requisitos 
		-el CPU esta escuchando interrupciones (en banderas el bit I que debe de estar habilitado por parte del CPU)
		-ocupa que algo arroja un interrupcion, cada periferico puede mandar al menos 1 interrupcion 
		-debe haber una rutina de servicio de interrupciones, (definicion de ISR)
		
		en el registro B de UART estan los bits que habilitan las interrupciones del UART
		cunado en ambos registros estan habilitados las interrupciones, podemos escribir la macro 


*/

//clase 21 de marzo del 2024

#include<avr/io.h>
#include<avr/interrupt.h>
//como podemos crear una incializacion de manera generica

//calculamos cual es el que tiene menos error, en la velocidad asincrona

//velcoidad simple - 
uint8_t globalData;

uint8_t bufferData[4]={0,0,0,0};



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
	
	//clase 27 marzo interrupciones 
	sei(); //habilita las interupciones, enable global interrumpt
	
	//qaue el periferico tiene activado las banderas de las interrupciones 
	
	myUART->UCSRB |= (1<<RXCIE0) ; //habilita las interrupciones del periferico 
	
	
	
	

}
//macro ISR
//clase 27 de maroz, numeor de vector, este sera el numero -1, mapeo podemos encontrar en dependencias externas, ioxm... 
//el nombre del vector este hace refencia a la direccion del vector de interrupcion
ISR(USART0_UDR0_vect){
	
	//ISR code 
	//togglear un led PJ4
	//PINJ = 1<<PJ4; //si se escribe un 1 se togglear
	static uint8_t idx=0;
	
	UDR0 = bufferData[idx++];
	
	
}
void delay(){
	
	volatile uint32_t cnt = 0x3FFF;
	while(--cnt)
		asm("nop");
	
}


//calcular el baud colose match en la tabla 22-1 


void UART3_WriteByte(uint8_t data){
	
	//verificamos si el bit del transmition complete esta set quiere decir que ya hubo una transferencia
	static uint8_t idx=0;
	bufferData[idx++]= data;
	globalData=data	;
	//UDR2 = data;
	UCSR0B |= (1<<UDRE0);
	delay();
	
	//while(!(UCSR0A & (1<< TXC0)))
	//	;	
	
	
	
	
}

uint8_t UART3_ReadByte(void){
	
	while(!(UCSR0A & (1<< RXC0)))
		;
	return UDR0;
	
}




//getchar read 

//putchar write 

int main(void){
	
	
	//UART3 esta en el pin PJ1 -PJ2 
	DDRJ=1<<PJ4;
	
	uart_init(0);
	
	UART3_WriteByte('H');
	while(!(UCSR0A & (1<< UDRE0)))
		;
		asm("nop");
	UART3_WriteByte('O');
	UART3_WriteByte('L');
	UART3_WriteByte('A');
	UART3_WriteByte('\n');
	UART3_WriteByte('\r');
	
	while(1){
		
		UART3_WriteByte(UART3_ReadByte());
		
	}
	
	
}


/*
tenemos 4 uarts dentro del micro. 
*/


