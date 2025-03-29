#include<avr/io.h>

//600 bps, 8 bits, even - paridad par, 2 stop bit 

//UBBR= 16MHz / (16*600) -1 =  1666 u2x=0 599.88 baudaje real
//UBBR= 16MHz / (8*600) -1 =  3332 /u2x =1 600.06 baujae real con doblre activado 
 void uart_init(){

	UCSR0A= 1 << U2X3; // el valor que va el UBRR es el que se calculo cuando se activa U2X 
	
	UCSR0B= 3<< TXEN3; //habilitacion de las interrupciones, por ahora solo nos interesa transmicion y recepcion 
	// UCSZn2 este funciona para seleccionar de que tamanio va a ser la cadena que se va a mandar, el dato, edto es con una tabla los otros 2
	//bits estan en C
	
	UCSR0C= (3<< UCSZ30) |(1<<UPM31) | (1<<USBS3); // stop bit, pridad par, define el tamanio del dato a enviar
	
	
	UBRR0=3332; //PUEDE SER DE parte alta o parte baja 

}

//calcular el baud colose match en la tabla 22-1 


void UART3_WriteByte(uint8_t data){
	
	//verificamos si el bit del transmition complete esta set quiere decir que ya hubo una transferencia
	while(!(UCSR0A & (1<< UDRE3)))
		;
		
	UDR0 = data;
		
	
	
	
	
}

uint8_t UART3_ReadByte(void){
	
	
	
	
}



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
