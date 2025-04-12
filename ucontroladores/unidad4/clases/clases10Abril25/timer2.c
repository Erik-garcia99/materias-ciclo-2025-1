
#cristal del tjuino 32KHz



#ticks = T/ Tt - 1S - Fxtal 32768 Hz -  1S * (32768/128) - 256 

##ticks = T/ Tt - 1S - Fxtal 32768 Hz -  5S * (32768/1024) = 160 



void timer2Ini(){
	//en el modo de operacion asincrono estan los pasos para poder que el periferico de timer2 se active 
	
	//1- desabilitar las interrupciones 
	
	TIMSK2 = 0; 
	
	//2- SELECT clock source by setting AS2 as appro
	
	//ASSR registro de asincrono es el modo que hace osilar a TIMER2 a 32KHz
	
	ASSR = 1<< AS2;
	
	//3 escribe los nuevos valores de TCNT, OCR2x, y TCCR2x
	
	TCNT2= 256-160; //limpiando el conteo 
	TCCR2A = 0; //CTC mode
	OCR2A = 160-1;
	TCCR2B = 7 << CS20; // PS - 1024 modo noremla
	
	
	//4. para hacer el cambio al modo de operacion asincrono, debo esperar los registros que se toco.
	
	while(ASSR  & ((1<<TCN2UB) | (1<<TCR2AUB) | (1<< TCR2BUB))
		; //WATI FOR ALLUPDATE FLAGS
	
	
	//5 clear the timer 2 interrup flags 
	
	TIFR2 = 7;
	
	//6 habilitar las interrupciones como se anecesario 
	
	sei(); 
	
	TIMSK2 = 1 << OCF2A; 
	
	
	
	
	
}


static uint8_t secFlag=0;
ISR(TIMER2_OVF_vect){
	
	secFlag =1; 
	PINB |= 1<<PB7;
	
}



/************************************************************/
//interrupcion 


// ticks = T/ Tt - 1S - Fxtal 32768 Hz -  2S * (32768/1024) = 64

//2s, 5s, 7s, 10s




void timer2Ini(){
	//en el modo de operacion asincrono estan los pasos para poder que el periferico de timer2 se active 
	
	//1- desabilitar las interrupciones 
	
	TIMSK2 = 0; 
	
	//2- SELECT clock source by setting AS2 as appro
	
	//ASSR registro de asincrono es el modo que hace osilar a TIMER2 a 32KHz
	
	ASSR = 1<< AS2;
	
	//3 escribe los nuevos valores de TCNT, OCR2x, y TCCR2x
	
	TCNT2= 0; //limpiando el conteo 
	TCCR2A = 0; //CTC mode
	OCR2A = 160-1;
	OCR2B= 64-1; //evento intermedio entre el conteo 
	TCCR2B = 7 << CS20; // PS - 1024 modo noremla
	
	
	//4. para hacer el cambio al modo de operacion asincrono, debo esperar los registros que se toco.
	
	while(ASSR  & ((1<<TCN2UB) | (1<<TCR2AUB) | (1<< TCR2BUB))
		; //WATI FOR ALLUPDATE FLAGS
	
	
	//5 clear the timer 2 interrup flags 
	
	TIFR2 = 7;
	
	//6 habilitar las interrupciones como se anecesario 
	
	sei(); 
	
	TIMSK2 = (1 << OCF2A) | (1<<OCF2B); //avisa cunado paso por A y por B 
	
	
	
	
	
}


static uint8_t secFlag=0;
ISR(TIMER2_COMPA_vect){
	
	secFlag +=5; 
	PINB |= 1<<PB7;
	
}

ISR(TIMER2_COMPB_vect){
	
	secFlag =1; 
	PINB &= ~(1<<PB7);
	
}