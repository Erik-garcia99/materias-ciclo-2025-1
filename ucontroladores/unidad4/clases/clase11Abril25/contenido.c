/*
practica timers
autobout[8000bps:200,000bps] MAX VALUE 
		[125us : 2us]
		
F = 1/T  ->

cuantos ticks ocupo para contar 125us

--la frecuencia seria 16 o 12 dependiendo de lo que tenemos 

PS = prescalador 
ticks = 125us * (16Mhz/PS) -> 125us * (16Mhz/8) = 250 - tiempo mas grande que quiero contar. */

//void   autobaoudrate0(void)
uint16_t getLowTimer_uS(void){
	
	TCCR0B= 0 ; //deshabilita el timer
	//a quien reviso si esto se hizo cero, quiero saber cunado el start bit se hace cero
	//PE0
	//cunado llega a cero empiece el cronometro, pero cunado llegue a alto es cunado empezamos a contar 
	//estas lineas me retrasan mi conteo porque estran inciando el conteo del timer, por lo que lo ponemos antes 
	TCNT0=0; //inical el conteo 
	TCCR0A = 0; // NORMAL MODE 
	while(PINE &(1<<PE0))
		; //espera mientras PE1 es alto 
	TCCR0B = 2 << CS00;  //precalador = 8 //empieza a contar hasta ahora 
	
	//ahora el cambio de 0 a 1
	
	while(!(PINE &(1<<PE0)))
		; //espera mientras PE1  es bajo 
	
	TCCR0B =0 ; //volvemos a deshabilitar mi timer 
	
	//sacamos el tiempo con los ticks, TCNT0 conteine el numero de tocks 
	
	//T = #ticks /ftell
	//T = ticks / 16MHz - T = ticks / 12MHz - Ft = (16Mhz/8) - Ft = (12Mhz/8)  
	//T = 250 / 2,000,000 -> (16MHz/8 ) 
	//uint16_t time= TCNT0 / 2; //-> esto porque estamos escalando para que nos pueda dar un valor entero 
	//return time; 
	
	
	UBRR0 = TCNT0-1 // VELOCIDAD doble 
	

}

bouds = 1000000/ (TCNT0 / 2);
UBBRx = 16MHz / (16* (1 /getLowTim1er_uS())) -1;
	 =16MHz / (16/getLowTim1er_uS()) -1;
 



 