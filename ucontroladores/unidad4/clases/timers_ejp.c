//timers 

//ticks = 1ms? 

/*
#ins = T / Tcpu    -T periodo
#ticks= T / Tt    -Tt - periodo del timer 

Tt = Tcpu / PS - 1/(Fcpu/PS) PS- prescalador que se escoja 8 - 64 - 256 - 1024s

#este resultado debe de caber en mi registro de 8 bits 

#ticks = 1mS/(1/(Fcpu/PS)) = 1ms * 16MHz = 16,000

#ticks = 1mS/(1/(Fcpu/64)) = 1ms * 16MHz = 250 check cunado TCNT0 llegue a 250 ha pasado 1 ms

	OCRA = 250 -1;  este tiene el valor que queremos lograr


*/
void Timer0_1ms(void){
	
	/* modo normal:
	
	como hacer que despues de 250 tick arroje overflow. agregando un offset para que llegue a ese 250
	TCNT0 = 6; //inicia en 6 para tener ese offset 
	
	TCCR0A = 0; //normal mode
	TCCR0B = 3 << CS00 ; //precalador = 64
	
	TIMSK0 = 1 << TOIE0; //enable TOV int
	sei();
	*/
	TCNT0 0; //ahora tope esta deifnido por OCRA, ahora elc ontador va a contar desde 0 - OCRA (250) para 1 ms
	OCRA=250-1; //tanto para el modo normal y OCRA le restados 1 a contador valor porque el estado se refleja 1 despues que este se cumplio
	TCCR0A = 2 << WGM00; //CTC mode
	TCCR0B = 3 << CS00 ; //precalador = 64
	TIMSK0 = 1 << OCIE0A; //habilita la interrupcion para OCRA
	sei();
	
}

static uint8_t secFlag= 0;
//buscar en el vector de interrupciopnes el nombre que recibe 
ISR(TIMER0_COMPA_vect){
	//si queremos usar eso como un contador de mS, 
	/* normal mode
	static uint16_t milis=0; 
	TCNT0 += 6;
	milis++;
	//cada 1000 milis ha pasado 1 S y se setea la bandera que indica ese estado
	if(milis => 1000){
		
		secFlag;
		milis=0;
		
	}
	PINB |=1<<PB7;
	*/
	
	static uint16_t milis=0; 
	milis++;
	//cada 1000 milis ha pasado 1 S y se setea la bandera que indica ese estado
	if(milis >= 1000){
		
		secFlag;
		milis=0;
		
	}
	PINB |=1<<PB7;
	
}




int main(void){
	
	
	DDRB |= 1<< PB7;
	Timer0_1ms(0);
}










 
*/