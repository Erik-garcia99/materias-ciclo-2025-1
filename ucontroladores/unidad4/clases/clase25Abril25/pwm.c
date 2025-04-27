//clase 25 de abril del 2025 

/*
TIEMRS PWM para el TIEMR 0 

PWM - modos 

FAST PWM: 
1 periodo es desde el flanco de subida hasta que baja 

ancho de punto es el tiempo que esta en alto o en bajo. 

el tiempo que se le pone  OCRA es el tiempo de dura en alto. 


---------------------------------

PHASE CORRECT PWM - PWM SIMETRICO: 

mejor para algo mecanico 


-------------------------------------------


vamos a usar los bists de COM0nx 

lo que hace el PWM con COM0A1 estan los 2 modos, el modo invertido y no invertido, en el no invertido cunado OC0A hace match pone 0 y cunado 
va a bottom pone 1, y en invertdio hace lo contrario 


registro TCCR0B - F0C - podemos forzar la compracion para que haga clear 


*/

void main(void){
	
	int8_t direccion =1;
	//el tiempo que esta en ocra, en el modo no invertido es el tiempo que durara ocra porque esta 1
	
	//50% de intensidad del led en PB7 en PWM 
	//DEBEMOS DE ahbilitar el pin que va ser el de salida del timer, debemos habilitarlo como salida 
	DDRB |= 1<<PB7; //0C0A COMO SALIDA 
	
	
	TCCR0A =  (2 << COM0A0) | (3<<WGM00); //OC0A fast PWM no invertido, va a contar hasta el tope 0xFF
	//prescalador 
	//Ft = Fcpu / PS*TOP ->(16,000,000 / 256)
	// Ttotal = 0.00016 *256 = 0.004046
	//Ftotal = 1/0.004096 = 244Hz del PWM 
	TCCR0B = (4<<CS00); //PS 256
	
	//donde pongo el valor del pwm 
	OCR0A = 128;//50%, para llegar nuestro 50% 
	
	while(1)
	{
		
		if(OCR0A == 254){
			direccion = -1;
		}
		if(OCR0A ==1){
			direccion = 1;
		}
		
		OCR0A+=direccion;
		delay(); //11ms
		
	}
	
	
	
	
	
	
	
	
	
}




