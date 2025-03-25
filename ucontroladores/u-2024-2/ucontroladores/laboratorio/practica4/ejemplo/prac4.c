
#include <avr/io.h> 

extern void Delay( void ); /* funcion prototype de Delay */ 

int main( void ){ 

/* configurar de salida el bits 3, 4 y 5d el PORTC*/      
	DDRC = 0x38; /* DDRC= 00111000 */
          
/* presentar valor inicial en puerto LEDs off*/       
	PORTC = 0x38;   
	while(1){ /* DDRC= 00111000 */          

	PORTC &= ~( 1 << 4 ); /* encender LED PC4 = 0   */ 
	Delay(); 
	PORTC |= ( 1 << 4 );  /* apagar LED PC4 = 1 */   
	Delay(); 
}  
} 
   
