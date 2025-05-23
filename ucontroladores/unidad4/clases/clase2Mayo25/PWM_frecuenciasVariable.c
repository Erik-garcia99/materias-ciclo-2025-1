./*
FRECUEICNAI VARIABLE SOLO PUDE SALIR POR B por lo que sale por B, sera el ancho de B por el tope de B 
PG5 PWN variable con frecuencia variable 

*/


uint8_t getchar(){
	//este ne una seccion dentro de UART 
	
	
}



void main(void){
	
	int8_t pecent = 50 ;
	//el tiempo que esta en ocra, en el modo no invertido es el tiempo que durara ocra porque esta 1
	
	//50% de intensidad del led en PB7 en PWM 
	//DEBEMOS DE ahbilitar el pin que va ser el de salida del timer, debemos habilitarlo como salida 
	DDRG |= 1<<PG7; //0C0A COMO SALIDA 
	
	
	TCCR0A =  (2 << COM0A0) | (3<<WGM00); //OC0B fast PWM no invertido, va a contar hasta el tope 0CRRA 
	//prescalador 
	//Ft = Fcpu / PS*TOP ->(16,000,000 / 256)
	// Ttotal = 0.00016 *256 = 0.004046
	//Ftotal = 1/0.004096 = 244Hz del PWM 
	TCCR0B = (4<<CS00) | (1<<WGM02); //PS 256
	
	// el que controla el ancho 0CR0A, OCR0A funciona como tope 
	
	//FPWM =  FCPU / (PS * OCRA + 1) -> UN TICK DESPUES DE 1  FORMULA ESCRTA EN LA SECCION DE TIMERS 
	
	
	OCR0A = 100
	OCR0B = (OC0A * percent) / 100;//OCR0A EL PERIODOS Y OCR0B el ancho, como calcula procentaje 
	
	while(1)
	{
		
		/*if(OCR0A == 254){
			direccion = -1;
		}
		if(OCR0A ==1){
			direccion = 1;
		}
		
		OCR0A+=direccion;*/
		switch(UART_getchar()){
			
			case 'F' : //inc Frec
				/*para aumentar  0CR0A - periodos
				para aumentar la ferecuencia aumentamos OCR0A*/
				
				if(OCR0A > 0 ){
					
					OCR0A--;
					//OCR0B = OC0A / 2 ;
				}
					
				
				break;
				
			case 'f' : //dec Frec
				//LO CONTRARIO QUE QUERMOS QUE SEA MAS LENTA 
				
				if(0CR0A < 256 ){
					
					0CR0A++;
					//OCR0B = OC0A / 2 ;
				}
				
				break; 
				
			case 'P' : //inc PWM (ancho del pulso)
				if(percent < 100)
					percent--;
				
				break;
				// orale maquina // orale maquina // orale maquina // orale maquina // orale maquina 
			case 'p' : //dec PWM	
				if(percent > 0)
					percent++;
				break;
			
			
			
		}
		OCR0B = (OC0A * percent) / 100;
		
		//delay(); //11ms
		
	}
	
	
	
	
	
	
	
	
	
}
