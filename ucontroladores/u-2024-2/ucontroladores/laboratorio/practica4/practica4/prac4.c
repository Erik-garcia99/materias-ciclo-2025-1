/*garcic chavez erik 01275863\
practica 4 ucontroladores 
*/
#include<avr/io.h>
extern void Delay(uint8_t argumento); /* funcion prototype de Delay */ 
int main(void){



 	Delay(5);
	
	uint8_t encendido, apagado;
	uint8_t variable=100;

	/* configurar de salida el bits 2, 3 y 4 d el PORTB*/      
	DDRB = 0x1C; /* DDRC= 0001 1100 */
          
/* presentar valor inicial en puerto LEDs off*/       
	PORTB = 0x1C;   



  while(1){  //DDRC= 00111000           
    
    /* Encender LED PB2 */
        PORTB &= ~(1 << 2);
        for (encendido = 0; encendido < 10; encendido++) {
            Delay(variable);
        }

        /* Apagar LED PB2 */
        PORTB |= (1 << 2);
        for (apagado = 0; apagado < 10; apagado++) {
            Delay(variable);
        }
		
	} 



	/*
	while(1){  DDRC= 00111000           

		PORTB &= ~( 1 << 2 );  encender LED PB2 = 0    
		Delay(variable); 
		PORTB |= ( 1 << 2 );   apagar LED PB2 = 1    
		Delay(variable); 
	} */ 
	



}





	

   
