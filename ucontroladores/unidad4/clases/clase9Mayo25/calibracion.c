//calibracion

/*

ADC = (Vin *1023)/Vref -offset

*/

//ADC


/*
convertidor de analogico a digital

medir contra Vcc

ADCSRA

ADEN la priera conversion srea barura - adc enable 

que valor se le debe de ingresar al prescalador? 

ADPS[2:0]configurar el prescalador que funcioncione entre 50KHz y 200KHz

aDSC


adate - auto trigger enable no existe. 

simple & code es lo que hace los primero 2 ciclos de reloj, al ADC le toma 10 ciclso comprar en ADC 

lo que hace es como "tomar una foto", para poder mantener esa senial sin que se pierda porque la senial a rapido 

podemos revisar cunado se haya terminao la coversion con ADIF que se hace 1 cunado se haya completado la conversion 

o con el mimso ADEN se hace cero cuando este termine con eso verificamos que se ha completado 

--REGISTROS 

ADMUX 

AREF - este no se usa al menos con que se le ponga algo fuera de el 

desabilitar la parte digitial del I/O es lo correcto. 

como se interperta la infromacioin, se retorna infromacion discreta,


con la formual ADC = Vin *1023 / Vref

 Vin =  ADC *Vref/1024

*/




//channel 9 - ADC 9 - usando 10 bits, REF si no se tiene nada especificado con serpecto a qeu se esta comparando se hace con AVCC el voltaje mas alto 
void ADC _Ini(void){
	
	//que canal quieroe leer (se vee en la tabal 1000001), ADLAR 
	ADMUX = (1<<MUX0) | (1<<ADLAR) | (1<<REFS0); // LEFT ad list, Vref= 5V -  AVCC , recorrido a la derecha 
	ADCSRB = 1<<MUX5;	
	ADCSRA = (1<<ADEN) | (7<<ADPS0); //ADC enable, PS (50KHz A 200KHz)   16MHz /PS frecuencia = 125KHz
	
	//que prescalador se tiene que dar 
	
	//cada 13 ciclos termina una conversion 
	
	//T= 1/Fadc  -  tiempo =  13/T
	
	//deshabilitar la aprte digitial
	DIDR2 = (1 <<ADC9D); //deshabilitar ADC9 la entrada digital 
	
	
}

uint16_t ADC_readA9(void){
	
	uint16_t reult= 0;
	//star convertion 
	ADCSRA |= 1<<ADSC;
	
	/*
	while(!(ADCSRA & (1<<ADIF)))
		; //estepera mientras ADIF ese en 0 
	
	//limpiar la bandera 
	
	ADCSRA |= (1<<ADIF); //se limpia con 1 la bandera */
	
	while(ADCSRA & (1<<ADSC))
		; //cunado incia la convesion se pone en 1 y esperamos hsta que el se haga cero 
	
	//el reulado esta en ADCL y ADCH
	// HHHH HHHH
	// LL00 0000 >> 6 -> 0000 00LL
	//QURERMOS HH HHHH HHLL
	
	//A AL DERECHA LO IDEAL si me intersa los 10 bits 
	//a la izquierda si solo me interan los perirmeors 8 
	
	//primero leemos parte baja se activa el candaod y despues la parta alta se quita el candado 
	//porque el ADC no se ve refeljado el resultado si no se lee antes la parte alta se pone un candado a eso 
	result = ADCL;
	result |= (ADCH << 8); 
	//ADC0 esta esta la posicion 0n 
	return result; //tiene el valor, ajustado a la derecha 
	
	
}


//ADC


/*
convertidor de analogico a digital

medir contra Vcc

ADCSRA

ADEN la priera conversion srea barura - adc enable 

que valor se le debe de ingresar al prescalador? 

ADPS[2:0]configurar el prescalador que funcioncione entre 50KHz y 200KHz

aDSC


adate - auto trigger enable no existe. 

simple & code es lo que hace los primero 2 ciclos de reloj, al ADC le toma 10 ciclso comprar en ADC 

lo que hace es como "tomar una foto", para poder mantener esa senial sin que se pierda porque la senial a rapido 

podemos revisar cunado se haya terminao la coversion con ADIF que se hace 1 cunado se haya completado la conversion 

o con el mimso ADEN se hace cero cuando este termine con eso verificamos que se ha completado 

--REGISTROS 

ADMUX 

AREF - este no se usa al menos con que se le ponga algo fuera de el 

desabilitar la parte digitial del I/O es lo correcto. 

como se interperta la infromacioin, se retorna infromacion discreta,


con la formual ADC = Vin *1023 / Vref

 Vin =  ADC *Vref/1024

*/


//--------------------
uint8_5 offset=0; 
//channel 9 - ADC 9 - usando 10 bits, REF si no se tiene nada especificado con serpecto a qeu se esta comparando se hace con AVCC el voltaje mas alto 
void ADC _Ini(void){
	
	ADCSRA = (1<<ADEN) | (7<<ADPS0); //ADC enable, PS (50KHz A 200KHz)   16MHz /PS frecuencia = 125KHz
	//calibracion de ADC
	
	//que canal quieroe leer (se vee en la tabal 1000001), ADLAR 
	ADMUX = (0x1F<<MUX0) | (3<<REFS0); // conexion a GDN 

	ADCSRB &= ~(1<<MUX5);
	
	//inica la conversion 
	ADCSRA |= 1<<ADSC;
	while(ADCSRA & (1<<ADSC))
		; 
	offset=ADC; 
	
	
	

	//configuracion del ADC 
	ADMUX = (1<<MUX0) | (1<<REFS0); // LEFT ad list, Vref= 5V -  AVCC , recorrido a la derecha 
	ADCSRB = 1<<MUX5;	
	ADCSRA = (1<<ADEN) | (7<<ADPS0); //ADC enable, PS (50KHz A 200KHz)   16MHz /PS frecuencia = 125KHz
	
	//que prescalador se tiene que dar 
	
	//cada 13 ciclos termina una conversion 
	
	//T= 1/Fadc  -  tiempo =  13/T
	
	//deshabilitar la aprte digitial
	DIDR2 = (1 <<ADC9D); //deshabilitar ADC9 la entrada digital 
	
	
	

	
	
}

uint16_t ADC_readA9(void){
	
	uint16_t reult= 0;
	//star convertion 
	ADCSRA |= 1<<ADSC;
	
	
	while(ADCSRA & (1<<ADSC))
		; 
	return (ADC-offset); //a cada ADC debemos de restarle el offset ya que es nuetro sesgo  
	
	
}