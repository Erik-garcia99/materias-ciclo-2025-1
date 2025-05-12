#include <avr/io.h>
#include "UART.h"

/*
funcioneds que esta bien

el inicalizador para UART0 esta ok mas supogo que de igual forma para UART2 y 3

putchar y puts OK

avaible, getcgar y gets OK




secuencias de escape

-clscr OK



*/










// Prototypes
// Initialization

#define FOSC 16000000 //velocidad del clock <- creo que esto va a valer verga, vamos a probar
//como se comporta si no debemos volver a reajustar para los 12M


// es una macro

/*****************************************************************************************/
//iniciamos nuestro UART



uint8_t *UART_offset[]=
{
	(uint8_t*)&UCSR0A,
	(uint8_t*)&UCSR1A,
	(uint8_t*)&UCSR2A,
	(uint8_t*)&UCSR3A

};



UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){

    UART_reg_t *myUART = UART_offset[com]; // eligo a mi UART

    myUART->UCSRB = (1 << TXEN0) | (1 << RXEN0); // Habilita TX y RX para UART0


    //establecer las paridades
	//uint8_t parity_mode= (parity == 1) ? 2 : 3;  // 2: Paridad par, 3: Paridad impar

    uint8_t parity_mode=0;

    switch(parity){

        //en caso que sea cero

        //1 paridad impar y 2 pardidad par
    case 1:
        parity_mode = 3; //padidad impar
        break;

    case 2:

        parity_mode = 2; //paridad par
        break;

    default:
        parity_mode = 0; //en caso que no sea ninguno permanece en 0 pero por si
        //alguna razon se mueve este nuermo lo ponemos de nuevo

        break;

    }

    // 0 = 1 stop bit
    // 1 = 2 stop bits
    //me manda sde de 1 o 2 por lo que ocn este ternearo decivo si es 1 sera 1 bit de parada 0
    //pero si es otro valor seran 2 bits de parada

    uint8_t stop_mode= (stop == 1)? 0:1;

    myUART->UCSRC = (parity_mode << UPM00) | (stop_mode <<USBS0);


    //tamanio del frame

    /*if(size!=9 && size <= 8){

		//estos no son necesario
		switch(size){

			case 6:
				myUART->UCSRC = 1 << UCSZ00;
				break;

			case 7:
				myUART->UCSRC = 2 << UCSZ00;
				break;

			case 8:
				myUART->UCSRC = 3<< UCSZ00;
				break;
		}

	}
	else{
		//en este caso del tamanio sera de 9 y terndremso que activar el bit del otro registro

		myUART->UCSRC = 3 << UCSZ00;
		myUART->UCSRB = 1<< UCSZ02; //habilitamos los 9 bits
	}*/

	if (size == 9) {
        myUART->UCSRC |= (3 << UCSZ00); // UCSZ01:UCSZ00 = 0b11
        myUART->UCSRB |= (1 << UCSZ02); // Habilitar bit 9
    } else
     {
        myUART->UCSRC |= ((size - 5) << UCSZ00); // Ej: 8 bits ? 3 << UCSZ00
    }

    uint16_t v_UBRR = (FOSC / (16 * baudrate)) - 1 ;
    myUART->UBRR = v_UBRR;
}


// Send

/*
puts funcionan para imprimir  TX

gets funcionan para recibir  RX

*/


//este haria lo mismo de putchar la unica deiferente es que este estara
//recorriendo la cadena que se quiere imprimir
void UART_puts(uint8_t com, char *str){

	//TXn trasmitir el contenido
	while(*str!= '\0'){
		//mientras haya contenido en el apuntador, que sea diferente a NULL
		UART_putchar(com,*str++);
	}
}



void UART_putchar(uint8_t com, char data){

	//este lo que hace es trasmitiri un caracter

	//perimro debemos de mostrar a cual UART es

	//que nos dice la teoria, la teroia nos dice que su UDR esta en 1 quiere decir
	//que el registro esta vacio,

	//entonces el ciclo va a espear para cunado este se vacie por completo
	//lo que hace es esperar mientras haya datos dentro del buffer

	UART_reg_t *myUART = UART_offset[com];


	// Calcular el bit UDRE según el UART (ej: UDRE0 para com=0, UDRE1 para com=1)
    uint8_t udre_bit = (com == 0) ? UDRE0 :
                      (com == 1) ? UDRE1 :
                      (com == 2) ? UDRE2 : UDRE3;

    //va a esperar hasta que se vacie por completo
	while (!(myUART->UCSRA & (1 << udre_bit)));
		; //espera a que el periferico este vacio

	myUART->UDR= data;
}



// Received



//aguanta mientras no haya dato en para trasmitir
/*uint8_t UART_available(uint8_t com){

	//si hay 1 queire decir que el buffer esta vacia

	//nosotros queremos esperar siempre y cunado este vacio, si hay un dato en el perferico
	//estara en 0 y nos sacara porque ya hay algo en el periferico

	 UART_reg_t *myUART = UART_offset[com]; //con esta varuable me desplajo al UART a usar
	 //si no existe dado en el periferico aqui se queda cunado exista arroja 1

	while(myUART->UCSRA & (1<<UDRE0))
		;

		return 1;  //regresa 1 cunado esta disponible
}
*/

uint8_t UART_available(uint8_t com){

	//RXC0 sta en 1 cuando hay un dato sin leer en RXC
	//y esta en 0 cunado este no tiene nada

	UART_reg_t *myUART = UART_offset[com];

	return (myUART->UCSRA & (1 << RXC0)) ; // Hay dato disponible
	//creo que va a asi pero si hay errores podemos invertirlo
}


/*
char UART_getchar(uint8_t com){


	char c;

	UART_reg_t *myUART = UART_offset[com];
	//okay, que es lo que esta pasando aqui, lo que esta pasando es que
	//si hay datos sin leer se establece en 1, al invertirlo nos da el 0
	//lo que quiere decri que esta disponible para la transmicion

	//si esta vacio esta en 0 al invertirlo nos da 1 lo que quiere decrir que esta
	//listo para recibir
	while(!(UART_available(com)))
		;

	return myUART->UDR;
}*/


char UART_getchar(uint8_t com) {
    UART_reg_t *myUART = UART_offset[com];
    while (!(myUART->UCSRA & (1 << RXC0))); // Espera dato
    return myUART->UDR;
}


void UART_gets(uint8_t com, char *str){


	char c; //este va a capturar el valor del char que se introdujo
	uint8_t i=0;

	while(1){
        c=UART_getchar(com);
		/*

		-debe de capturar hasta que se le de enter lo que quiere decir que se termino
			de escribir el texto

		-los espacios deben ser registrados un espacio cuenta como un caracter mas

		-cunado se hace un retoseso debe de regustrarse

		-cuando se limpia la pantalla

		*/
		//verificamos si se quiere borrar que en efecto haya algo que borrar
		if(c=='\b'){
			if(i>0){

				str[--i] = '\0'; //sustitumos el utlimo caracter con el nulo
				UART_putchar(com,'\b');
				UART_putchar(com, ' ');
				UART_putchar(com,'\b');
			}

			continue; // si no hay nada que borrar o si hay algoq ue borrar sigue con el ciclo
		}

		UART_putchar(com,c);

		if(c == '\r' || c== '\n'){
		//retorno de carro o salto de linea lo que quiere decir que se terminao de escribir el
		//texto actual.

			str[i]='\0'; //caracter nulo denotando que la
			UART_putchar(com,'\r'); //vuelve al inicio de la linea
			UART_putchar(com,'\n'); //salto de linea
			break; //rompesmos el ciclo y a esperar que se vuelva a escribir algo
		}

		if(i<127){

			str[i++]= c;
			//UART_putchar(com, c);
		}

	}

}


// Escape sequences
UART_clrscr( uint8_t com ){

    UART_reg_t *myUART = UART_offset[com];


    //que es lo que hace un clsrc es que limpia tpda la pantalla

    /*
        los numeros salen del sigueinte fromato
        \x1B representa el nuemero 27 que es ESC en ascii
        para representar ese valor en HEX se usa \xHH,
        el [2J es un estandar ANSI/VT100 que se establece lo que nos dice
        que con ESC[2J se borra toda la pantalla

        lo mismo para con posicionar al inicio de la termianl
        sin parametros H se estbalce en la fila1 - columna1
    */
    UART_puts(com,"\x1B[2J"); //borra toda la pantalla
    UART_puts(com,"\x1B[H"); // poen el curso al incio fila 1, columna 1


}


void UART_putnum(uint8_t com, uint8_t num){

    if(num >= 100){
        UART_putchar(com, '0'+(num/100));
        num%=100;
    }
    if(num>=10){
        UART_putchar(com, '0'+(num/10));
        num%=10;
    }

    UART_putchar(com,'0'+num);

}


UART_gotoxy(uint8_t com, uint8_t x, uint8_t y){

    UART_puts(com,"\x1B["); //inicio de la secuencia de esapce

    UART_putnum(com,y+1); //convertir a caracter
    UART_putchar(com,';');
    UART_putnum(com,x+1);
    UART_putchar(com,'H'); //final de la secuencia

}









UART_setColor(uint8_t com, uint8_t color){


    UART_puts(com,"\x1B["); //incio del comando espace
    UART_putchar(com, '0'+(color/10));
    UART_putchar(com,'0'+(color%10));
    UART_putchar(com,'m'); //final del comando
 }





/*
// Utils
void itoa(uint16_t number, char* str, uint8_t base){


	//primero debo saber sobre que base se va a operar
	//convierte Función que convierte una numero de 16 bits a su
	//representación ASCII en la base
    //especificada

	//pimeor debemos saber que base es

	//recibe un caracter con terminacion '\0'

    //arreglo con los caractrers de HEX

	//char codigo;

    //uint8_t caracter=48; //le ponemos 0 solo para que entre al bucle despues de esto debe de cambiar
    //entonces sonostro vamos a tener un valro decimal, no un caracter ascii




    //lo cinvertimos a decimla y es lo que recibimos y tenemos que converitr en su valor HEX, octal o binaria pero en ASCII

    //a la idea que tengo necesito una pila, porque saco residui

    //nota importar reducir dividir entre 16 porque nomas se saca el modulo y sigue el mismo valor

    //********************************************************************
    // variables
    char HEX[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

    uint8_t temp=number;
    uint8_t item_HEX=0;

    char *aux=str;



    //necsito una pila porque el primer dato que sale sera le ultimo





    //********************************************************************



    //en caso que el numero sea 0
    uint8_t index= 0;
    if(number == 0){
        aux[index++] ='0';
        aux[index] = '\0';
    }



	if(base == 2){
        //binario
        HEX[1];

	}


	/*else if(number == 8){
        //octal


	}

	else if(base == 16){


        while(temp > 0){

            temp/16;
            item_HEX++;

        }

        temp=number; //volvemos a cagar a temp del valor que nos manda itoa

        char _HEX_ASCII[item_HEX];


        uint8_t _stack_point_=item_HEX-1;

        uint8_t residuo= temp;

        //empieza el algoritmo que dica cuales seran ls numerso HEX en ascii del numeor que se manda

        while(temp>0){

            residuo = temp%16;

            _HEX_ASCII[_stack_point_]= residuo;
            _stack_point_--;
            temp/=16;
        }

        //ahora sacamos los valores de la tabal y los ponemos
        //en su posicion en el puntador

        uint8_t i=0;
        uint8_t j=0;

        while(j<item_HEX){
            aux[i] = HEX[_HEX_ASCII[j]];
            i++;
            j++;
        }\
        aux[i]='\0'; //el ultimo lo remplazamos con el cracter nulo
	}

}
*/

//vamos a probar solo con HEX

void itoa(uint16_t number, char* str, uint8_t base){

    //vamos a probar de la otra forma

    //char HEX[]={"0123456789ABCDEF"};

    char HEX[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
\0\
    char *aux = str; // trabajamos con un apuntador aux

    char _HEX_STACK_[5]; //segun este es ara un valor de 16 bits, el ultimo deberia de ser un caracter nulo ,
    //empieza a almacenar desde 0 y sigue al final cunado se agregan todo le agegamos el '\0'






}





uint16_t atoi(char *str){

    char *aux= str;

    //primero debemos hacer este que devolvera un numero decimal de 16 bits

    //recibe una cadena y lo convierte en su parte decimal


    /*
        543

        '5'

    */

    //hacemos un arreglo para poder almacenar los elementos que se del tamanio del string enviado



    uint8_t cantidad=0;

    while(*aux!='\0'){

        cantidad++;
        *aux++;
    }

    uint8_t items[cantidad]; //arreglo reservado

    //recorrera cad caraacter a mi parecer desde el inicio hasta el final por lo que debemos de hacer
    //como una pila ingresar desde la ultima ubicacion,

    uint8_t num; // guardada el numero actual y este al ser multiplicado por el exponente sera agregado al la pia

    uint8_t idx= cantidad-1; //me indica en que pisicion del arreglo va

    //aseguramos doble
    while(*aux !='\0' && idx >=0){

        num= *aux - 48;

        items[idx]= num; // en la posicion idx ya items
        //exp*=10;
        //ya se agrego recoremos nustro arreglo
        idx--;
        *aux++;
    }

    aux=str;

    //[3,2,5] -> ejemplo


    uint16_t exp=1; //el esponente es para poder sumar al final y este represente decimales, decenas, centenas, millaes, etc.
    uint8_t acum = 0;
    uint8_t actual;
    for(uint8_t i=0; i< cantidad; i++){
        //vamos a recorrer la pila y calcular ese exponente

        actual = items[i];

        actual*=exp;

        exp*=10; //lo elevamos a la 10 en cada iteracion, pero este tendra un limite claro, cunado ya no cabe en 16 bits
        //con al esperanza de que no sean numereos mas grandes de 4 digitos

        acum += actual;

    }

    return acum; //retornarme el acumulador
}
