#include<avr/io.h>
#include "UART.h"




UART_reg_t *UART_offset[]={

    //lo que esta pasando es que estoy sacando la direccion de cada inicio de cada
    //UART de la memoria.
    (uint8_t*)&UCSR0A,
    (uint8_t*)&UCSR1A,
    (uint8_t*)&UCSR2A,
    (uint8_t*)&UCSR3A

};



void UART_init(uint8_t com,uint32_t BOUD, uint8_t parity, uint8_t stop, uint8_t ch_size){

    UART_reg_t *myUART = UART_offset[com];


    //habilitamspo RX y TX del UART que estemos usando con myUART


    myUART->UCSRB |= (1<<RXEN0) | (1<<TXEN0);


    //para el bit de paridas habran 3 casos, en donde no hay, par e impar

    // 0 -> sin bit de paridad
    // 1 -> impar ODD
    // 2 -> PAR even


    uint8_t parity_mode= 0;

    switch(parity){

        case 0: parity_mode = 0;break; //

        case 1 :
            parity_mode = 3; break; //impar
        case 2 : parity_mode = 2;break; //par
    }

    // si manda 1 o 2 son los bits que este quera como stop bit
    uint8_t stop_bit = (stop == 1) ? 0:1;

    //solo nos ocuparemos de hasta 8 bits
    uint8_t character =0;

    switch(ch_size){


        case 5:
            character = 0; break;

        case 6 : character = 1; break;

        case 7: character = 2; break;

        case 8: character = 3; break;

    }


    myUART->UCSRC |= (parity_mode << UPM00) | (stop_bit<<USBS0) | (character << UCSZ00);

    //ahora toca seleccionr a UBRR el cual es con la forumual

    uint16_t UBRR_value = (FOSC / (16 *(BOUD))) - 1;

    myUART->UBRR = UBRR_value;
}


//funciones para trasnmitir


//la funcion de puts solo envia a putchar lo que se quiere enviar

void UART_puts(uint8_t com, char *str){

    //indicamos el caracter cero como caracter que finaliza una cadena
    while(*str != '\0'){

        UART_putchar(com, *str);
        *str++;

    }

}


//todo esto sin interupcioens



void UART_putchar(uint8_t com, char data){

    UART_reg_t *myUART = UART_offset[com];

    //este bit de UDRE0 indca si el buffer esta preparado para recibir nueva infromacion
    //si UDREm esta en 1 el buffer esta vacio
    //entonces esta esperando a que se llene
    while(!(myUART->UCSRA & (1<<UDRE0)));


    myUART->UDR = data;


}


uint8_t UART_avaible(uint8_t com){

    //indica si RX esta disponoble

    UART_reg_t *myUART = UART_offset[com];

    //este me va a mandar 0 si el buffer esta limpio
    //vamnda 1 si hay datos sin leer dentro del buffer de RX
    return (myUART->UCSRA & (1<<RXC0));
}


char UART_getchar(uint8_t com){

    //hetchar va a obtener caracter por caracter que entra por UART
    //entonces traemos de nuestros puerto UART


    UART_reg_t *myUART = UART_offset[com];

    //espero hsta que se termine de llenar el buffer
    while(!(UART_avaible(com)));

    //ahora lo que se recbibio por RX eta en el buffer, UDR por lo que ahota debemos mandarlo
    //

    return myUART->UDR;



}


void UART_gets(uint8_t com, char *str){


    char c;
    uint8_t i=0;

    //en este es todo el tratamineto de la cadena que vamos a recbiri por RX
    //creo que RX es lo mas dificl y tardado
    while(1){

        c = UART_getchar(com);


        /*
        como sabemos debemos de tener varias consideraciones


        1-como saber que se quere terminar la cadena, con un enter

        */

        if(c == '\n' || c == '\r'){
            //la cadena termina
            if(i>0){
                //esta condifcion quiere decir que hay algo en al cadena, al menos 1 digito

                str[i]='\0'; //terminamso la cadena con un caracter nulo
                UART_putchar(com,'\r');
                UART_putchar(com,'\n');
            }
            break;
        }



        /*
        el usuario quiere borrar algo en la pantalla
        */
        if(c == '\b'){
            //hay algo que borrar
            if(i>0){
                //el quie estaba como penuntulo ahora es el ultimo
                str[--i] = '\0';
                UART_putchar(com,'\b');
                UART_putchar(com,"");
                UART_putchar(com,'\b');
            }

            continue; //si np, no haya nada que borrar por lo que continua

        }

        //ahora sera la parte de ingresar los datos al buffer del arreglo
        UART_putchar(com,c); //imprimmismo el valor
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





