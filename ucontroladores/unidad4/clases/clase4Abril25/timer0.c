//clase 4 de abril 2024 
//sigueindo TIMER0
							//-es la frecuencia del oscilador del micro
//ticks = 1ms/Tt - 100ms *(16MHz/PS) = 250


void_delay_750ms(){
	//los 750 no cabe en el timer de 8 bits, por lo que se dividen
	
	TCNT0 = 0;
	TCCR0A = 2; //CTC
	TCCR0B =3; //PS - 64
	OCRA= 250-1; //ha pasado 1mS
	
	
	for(uint16_t counter = 0; counter< 750 ; counter++){
		while((TIFR0 & (1 << OCF0A) ) //SOLO me interesa ese bit que representea que OCRA llego a su valor establecido
			;
			
		//debemos limpiar la bandera 
		
		TIFR0 |= (1 << OCF0A);
		
	}
	
	
}


//timer como cronometro
/*

leyecdo PE1 tiempo en bajo 

*/

uint16_t low_time_pe1(){
//a 12MHZ 

//#ticks  = 2/3 ms *(12MHz /PS) = 12,000 /PS -	no da y por lo que va a hacer de 0.5mS A 12Mhz podremos ir a 2/3
// 6 ticks = 2mS
	
	//PE1 ya esta inicalizados 
	uint16_t counter=0;
	TCNT0=0; 
	TCCR0A = 2; //CTC
	TCCR0B= 3; //PS 64
	OCRA= 25-1 ; //0.1 de resolicion
	
	while(PINE & (1 << PE1)) //espera a que se presione el boton, porque por defecto esta en alto. 
		;
	
	while(!(PINE & (1 << PE1)) ){ //estamos leyendo del pin cunado este en bajo 
		
		while(TIFR0 & (1<<OCF0A)) //estamos esperando que pase 1mS
 			;
		TIFR0 |= (1<<OCF0A); //limpiamos la bandera para que todo este bien 
		counter++; //aumentamos el counter hasta el segundo 
		
	}
	
	
	
	
	
	return counter
	
}


static uint16_t milis=0;

static uint16_t timer0_12M(){
//a 12MHZ 

//#ticks  = 2/3 ms *(12MHz /PS) = 12,000 /PS -	no da y por lo que va a hacer de 0.5mS A 12Mhz podremos ir a 2/3
// 6 ticks = 2mS
	
	//PE1 ya esta inicalizados 
	uint16_t counter=0;
	TCNT0=0; 
	TCCR0A = 2; //CTC
	TCCR0B= 3; //PS 256
	OCRA= 125-1 ; //0.1 de resolicion
	sei();
}


ISR(TIMER0_COMPA_vect){
	
	
	static uint8_t dosTerciosn_cnt=0;
	dosTerciosn_cnt++;
	
	if(dosTerciosn_cnt == 6){
		dosTerciosn_cnt=0;
		milis+=2;
	}
}

/*practica 8 TIMERS*/




