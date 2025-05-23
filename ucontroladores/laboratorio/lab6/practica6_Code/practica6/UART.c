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
	uint8_t dot_flag = 0; // Bandera para detectar punto

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

		if (c == '.') {
            dot_flag = 1;
            continue; // No muestra el punto
        }

        if(dot_flag){

             /*if (c == '\r' || c == '\n') {
                str[i] = '\0';
                UART_putchar(com, '\r');
                UART_putchar(com, '\n');
                break;
            }
            continue;*/

            /*
            lo que voy a hacer es que despues del punto sera coindierado 0,

            va amostrar por el UART pero despues lo cambiaremos a un caracter nulo todos los demas
            */

            str[i++] = '\0'; //desoues de este punto ya no lo tomara en cuenta
            dot_flag = 0; //para que ya no enre aqui
		}


		//para 20 caracteres, si no lo regresamos a 127

		if(i<19){

			str[i++]= c;
			//UART_putchar(com, c);


		}
		else {

			/*
				el proceso para que ya no captrue mas caracteres sera algo parecido a que si se borrara
				esta en el indece maximo +1 entonces lo que hace es como el borraro de ese sigueitne
				caractere por lo que parece que no se esta capturando pero lo que hace es que se esta borrando
				constantement lo que se captrua despues
			*/

			str[i] = '\0'; //sustitumos el utlimo caracter con el nulo
			UART_putchar(com,'\b');
			UART_putchar(com, ' ');
			UART_putchar(com,'\b');
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






// Utils
void itoa(uint16_t number, char* str, uint8_t base){


    char *aux= str;

    if(base == 16){

        //asignamos un arreglo con las representaciones de los numero HEX en ASCI
        char hex[] = "0123456789ABCDEF";
        uint8_t index = 0;
        uint16_t temp = number; //hacemos un backup de number para trbajar con el y no pereder el valor original

        // Manejar el caso cuando el número es 0
        if (temp == 0) {
            aux[index++] = '0';
            aux[index] = '\0';
            return;
        }


        //este buffer guardara los numeros HEX en asci del resultado de la divicion de modulo,
        //osea el resto de la diviciion con el cual se saca la representacion HEX de un numero deicmla

        //asi como un idx el cual controla cunatos digitiso tiene el numero
        char buffer[16];
        uint8_t buf_idx = 0;

        while (temp > 0) {
            buffer[buf_idx++] = hex[temp % base];
            temp /= base;
        }


        //como de lo que se saca de la divicion la priemra divicion es lo ultimo que es de a reosetnacion por ejemplo 523

        /*
            523 se puede diviir 3 veces por 16 el cual da en la primea 11 - 0 - 2

            pero 523 en HEX es -  20B, por lo uqe haceos como si fuera una pila si lo podemos ver asi
        */
        // Invertir la cadena
        for (int i = buf_idx - 1; i >= 0; i--) {
            aux[index++] = buffer[i];
        }
        aux[index] = '\0'; // Terminar con nulo


    }


    else if(base == 2){


        uint8_t index = 0;
        uint16_t temp = number;

        char buffer[17]; //tiene tamaño 16 porque el number es un numero de 16 bits, el caracter nulo
        //se agrega despues en el apuntador

        uint8_t buf_idx=0;

        //con el binario es un poco mas sencillo pero el proceso es el mismo

        //verificamos si el numero es 0 entonces volvemos a mandar le 0 no tiene caso que
        //se pierda tiempo en un proceso que incluso nos puede dar error
        if(number ==0){

            aux[index++] = '0';
            aux[index] = '\0';
            return;
        }



        while(temp > 0){

            buffer[buf_idx++]= (temp%2)?'1':'0';
            temp/=2;
        }


        //rellenar con ceros a la izqueirda

        while(buf_idx < 16){
            buffer[buf_idx++]= '0';
        }

        index=0;

        for(int8_t i = buf_idx-1 ; i >= 0 ; i--){
            aux[index++] = buffer[i];
        }

        aux[index]='\0';


    }

}



uint16_t atoi(char *str) {
    uint16_t result = 0;
    while (*str != '\0') {
        if (*str >= '0' && *str <= '9') {
            result = result * 10 + (*str - '0');
        }
        str++;
    }
    return result;
}

