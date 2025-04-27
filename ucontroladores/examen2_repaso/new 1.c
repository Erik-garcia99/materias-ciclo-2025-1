/*configuracion del uart 

uart1

1,000,000 bps
9 bits de datos
2 bits de paro
paridad par 
u2x = 1 8MHZ


 - perimto debemos de ver cual ecuacion es al vamos a impelemtar para calcular el ubrr 
 
 UBRRn = (Fosc / (8 *BOUD)) -1 


UBBR1 = (8MHz / (8 * 1,000,000)) - 1
 va a hacer doble velocidad por que la velocidad normal nos da numero negativo por lo que wtf??? igual la velocidad normal wtf??
 
 
*/


void Init_uart1(void){

	
	UCSR1A = 1 << U2X1; //aplicamos la doble velocidad al uart1
	UCSR1B = 1<< USCZ12; //habilitamos los 9 bits del frame 
	UCSR1B |= 3<< TXEN1	; //aplicamos una or para oa afectar los demas y ahbilitaamos la recepcion y transmicion del uart
	
	UCSR1C = (2 << UPM10) | (1<< USBS1) | (3 << UCSZ10); // PARIDAD PAR, 2 STOP BITS Y HAILITAMOS LOS DEMAS BITS PARA LOS 9 BITS DEL FRAME 
	
	UBRR1 = 0; //?? 
	
	
}




//EJERCICIO DE LOS 2 LEDS 

#include<avr/io.h>

#define ON 1
#define OFF 0


void init(void){
	
	
	DDRA |= (3 << PA1); // configuramos PA0 y PA2 como entrada, 1 como salida
	
	//0000 1110
	//queda PA0 como entrada  y escribirmos en el PA3 un 1 que siemppre esgtara mandando 
	PORTA |= (0x01 << PA3); // 1000
	
}


void __LED_GREEN__(uint8_t STATE){
	
	if(!STATE){
				
		//el verde es el que esta prendido siempre y cuando no se presione el boton 
		
		PORTA |=(1<<PA1);
		
	}
	else{
		//en caos que se presione el boton de apaga en verde
		PORTA &=~(0x01<<PA1); 
	}
	
}



void __LED_RED__(uint8_t STATE){
	
	uint8_t estado;
	if(STATE){
		
		//se activa el rojo pero se apaga el verde
		// estado |=(1<<PA1);
		PORTA |=(3<<PA1);
		//quiero que PA2 este en 1 y PA1 no que se pague pero si que este en 0, porque ocupo su 1 
		//PORT ACT - 0000 1010
		// quiero que mande 0000 1110 
	}
	
	else{
		
		PORTA &=~(0x01<<PA1); 
		
	}
	
}


uint8_t SW_STATE(void){
	
	
	//debo de ller todo mi pin, leo si no esta presionado el pin 0, negado porque esta en pull up 
	
	if(!(PINA & (1<<PA0))){
		
		return 1;
		
	}
	else{
		return 0;
		
	}
}


void __UPDATE_STATUS__(void){
	
	if(SW_STATE()){
		
		__LED_RED__(ON);
		__LED_GREEN__(OFF);
		
		
	}
	else{
		__LED_GREEN__(ON);
		__LED_RED__(OFF);
		
		
	}
	
}


int main(void){
	
	
	init();

	while(1){
		PORTA =(1<<PA2);	
		//__UPDATE_STATUS__();

	}
	
}