//examen 2 


/*
entrada de entrada de salida FF, un puerto de entrada y salida 

*/


/*
UART_INIT 

*/



//131072 bps, 7 bits, 1 bit de pato, paridad impar, 8 MHz


/*
calculamos el UBRR =  (Fosc/ 16Boud)-1

80000/(8 * 131072) = 16,384 -1 = 7 U2X = 1 -> UBRR = 7, % (125000 / 131072 -1) * 100 = -4

U2X = 0 -> 3, % ((3+1 * 16))

COMO SABEMOS cual es el que nos conviene 

sacamos el error 

frecuencia que ahora si estoy logrando 








*/

void UART_ini(void){
	
	UBRR3 = 7; 
	UCSR3A = 1<< U2X;
	UCSR3B = 3 << TXEN3; // HABILITAMOS RX Y TX, CRACTER SIZE
	UCSR3C = 2<< UCSZ30 | 3 << UMP30; 
}


/*rango de voltaje de entrada del micro de 3.3 */


/*funcion que devuelve si el pin es de entrada, 1 si es de entrada y 0 si no es de entrada*/



uint8_t GPIO_isInput(uint8_t num_gpio){
	
	
	return !(DDRA & (1<<num_gpio)); //si es de entrada me regresa 1 porque 0 es entrada con DDR
	
}


/*
simular 1 pulso 

0 -> 0 (0.5us) | 1 (1us) 
1 -> 0 (1s) | 1 (0.5us)

*/

//ya hay el delay 


//se manda el bit mas significativo primero 



extern void delay_ns(uint16_t ns);
void sendByt(uint8_t byte){
	
	//se manda el LSB
	
	for(uint8_t idx = 7 ; idx > 0 ;idx-- ){
		
		uint8t bit = byte & (1<<idx); // este puede fallar, lo ideal es enmascarar por algo fijo 
		byte <<= 1;
		
		
		PORTD |= 1<<PD6;
		delay_ns(bit ? 1000: 500);
		PORTD & = ~(1<<PD6);
		delay_ns(delay_ns(bit ? 500: 1000););
		
	}
	PORTD |= 1<<PD6; // dejar en alto 
}



/*
softuart 
7 bits de paridad, 1 stop bit 

RX de PA5 @ 9600






*/


#define RX_PIN ((PINA << 2) & 0x80 )
//CON ESTO PUEDO TRUNCAR 
        000 -> 0 0000
        001 -> 0 0000

uint8_t softUartReadByte(void){
	
		uint8_t rx =0; 
		while(PINA & (1<<PA5))
			; //wait mientras PA5 es bajo 
		
		delay_us(103 + 52); //para leer a la mitad de la muestra para poder tener un colchon para poder atinar 
		
		
		for(uint8_t idx = 0 ; idx< 8; idx++){
			
			rx |= RX_PIN;
			rx >> = 1;
			delay(103);
			
		}
		 
	return rx;
	
}





