#include<avr/io.h>
#include "UART.h"



//okay ya sabemos mas osbre UART, rearemos la practica 6 para preacticas UART.

//sabemos que debemos crear un rograma para un UART x,


uint8_t *UART_offset[]={

    //este arreglo reresenta la direccion del registro princiapl de cada UART
    //para poder acceder a ese esapcio de memoria de cada UART

    (uint8_t*)&UCSR0A,
    (uint8_t*)&UCSR1A,
    (uint8_t*)&UCSR2A,
    (uint8_t*)&UCSR3A,
};


//incilaisacion del UART


UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){

    //algo esta oasando en el inicio, por lo que mejor pongamos todos los valores dentro de vairbales

    uint16_t UBRR_value = 0;
    uint8_t cha_size= 0;
    uint8_t parity_mode = 0 ;
    uint8_t stop_bit = 0;


    //que dice la lieteratura, debemos establecer la velcdad del baudaje

    UART_reg_t *myUART = UART_offset[com];


    //establecemos la veolicad del baudaje

    UBRR_value = (FOSC / (16 * baudrate) ) - 1;

    //calculamos cual seria el bit de paridad
    //con default esta en 0, desactivado

    switch(parity){

        //caso 1 impar
        case 1 : parity_mode = 3; break;

        case 2 : parity_mode = 2; break;

    }


    //stop bit stop = 1 -> 0, stop = 2 -> 1

    stop_bit = (stop == 1) ? 0: 1;


    //caracter size

    //por default esta para 5, porque la variabel tiene 0
    switch(size){

        case 6: cha_size =1; break;

        case 7: cha_size= 2; break;

        case 8: cha_size=3;break;
    }

    //registro UCSRB - habilitar RX y TX

    myUART->UCSRB = (1 << RXEN0) | (1<<TXEN0);

    myUART->UCSRC = (parity_mode << UPM00) | (stop_bit << USBS0) | (cha_size << UCSZ00);
    myUART->UBRR = UBRR_value;



    /*
    //configureacion del regstro UCSRB

    //habilitamos el bit para eltransmisor y el receptor


    myUART->UCSRB = (1 << TXEN0) | (1 << RXEN0);

    //registro UCSRC

    //estabelcer los bits de apridad

    uint8_t parity_mode = 0; //desactivado

    switch(parity){

        //1 en parity es impar
        case 1 : parity_mode = 3; break;

        //2 es par
        case 2 : parity_mode = 2 ; break;

        //en caso que sea 0 esta deshabilitado y salimos de la condicion
        default : break;
    }

    myUART->UCSRC |= (parity_mode << UPM00);


    //establecer el stop bit


    //si se manda
    volatile uint8_t stop_bit = (stop == 1)? 0:1;

    myUART->UCSRC |= (stop_bit << USBS0);


    //caracter el frame  de 5 - 8
    uint8_t ch_size = 0;
    switch(size){

        case 5 : myUART->UCSRC |= (ch_size << UCSZ00);break;

        case 6 : ch_size = 1;
                myUART->UCSRC |= (ch_size << UCSZ00);break;


        case 7: ch_size = 2;
                myUART->UCSRC |= (ch_size << UCSZ00);break;
        case 8: ch_size=3;

                myUART->UCSRC |= (ch_size << UCSZ00);break;
    }


      //la funcion recibe el baudaje que se quiere llegar, pero debemos aplicar el prescalador (UBBR)
    //haremos con velcidad doble
    volatile uint16_t UBRR_value = (FOSC / (8 * baudrate ))- 1;

    myUART->UBRR |= UBRR_value;

    //habiitadmos le velcidad doble

    myUART->UCSRA = (1 << U2X0);
    */


}



//funcion putchar

/*
esta funcion lo que hace es mandar lo que hay en registro del tramsisor (UDR)
hacia el pin, por medio de la comunicacion serie


*/

void UART_putchar(uint8_t com, char data){

    UART_reg_t *myUART = UART_offset[com];

    /*
    UDREn indica si el budder de transmison esta vacio y listo para recibir nuevos datos

    este bit esta en 1 quiere decir que est vacio por lo tanto esta listo para recibir nuevos datos
    UDREn = 0, esta lleno por lo que no puedo recibir nuevos datos

    porque en la funcion se niega?
    se esta negando porque si UDREn =0 quiere decri que hay datos dentro del buffer por lo que negamos (!) es para esperar
    hasta que el buffer este vacio por lo que esta vacio pasa la condicion y manda lo que hay en UDR al pin TX
    */
    while (!(myUART->UCSRA & (1 << UDRE0)));

    myUART->UDR = data;
}


void UART_puts(uint8_t com, char *str){

    //el puts imprimira una cadena asi que una cadena, como no sabemos exactamente
    //el tamanio de esta, como e sun apuntador lo que hago es deliminar el final de esta
    /*
    con el caracter nulo '/0',

    por lo que mandmos llamar a putchar tantas veces hata que se llegue al caracter nulo
    */
    while(*str != '\0'){

        UART_putchar(com,*str);
        str++;
    }
}



/*
funciones para el receptor

gets - getchar - available


funcion principal

getchar - captura un caracer (RX)

que es lo que hara (getchar) el getchar lo quqe hace es lo que entre por medio de ese pin
lo va a capturar RX.

-funcion avaible: retorna 1 si hay datos en el perferico


*/

uint8_t UART_available(uint8_t com)
{
    UART_reg_t *myUART = UART_offset[com];
    //e periferico es UDR si hay datos dentro de el

    /*
        RXC0 es la bandera que indica si la recepcion se ha completado
        RXC0 esta en 1 (RCX0=1) si hya datos sin leer dentro del buffer
        (RCX0 = 0) si el buffer esta vacio
    */

    return (myUART->UCSRA & (1<<RXC0));
}



char UART_getchar(uint8_t com ){


    /*
    lo que llegue, lo pasamos a UDR
    */
    UART_reg_t *myUART = UART_offset[com];
    //este va a eserar hasa que se llene el buffer y entonces mandara la infromacion a UDR
    while(!(UART_available(com)));
    return myUART->UDR;
}

void UART_gets(uint8_t com, char *str){


    char c; //caracter que entro por UART
    uint8_t i = 0; //indice del arreglo donde esta la cadena



    while(1){

		c = UART_getchar(com);

        /*
        cuales son los tispo de condiciones que nos podemos entronctrar
        cunado el suaurio ingresa datos.

        -que entrara cunado el usuario ya no quiere seguri captiradno
        si el suario da (enter) quiere decir que ya no quiere capturar mas.
        */

        if(c == '\n' || c == '\r'){

            if(i >0){
                //para finalizar debe haber almenos un dato razonable, en este caso
                //algun digito decimal que guardar
                str[i] =  '\0';
                UART_putchar(com, '\n');
                UART_putchar(com,  '\r');
                break;
            }
            //si no va a volver hasta que haya algo dentro de UDR lo cual sea logico para el
            //el entido del programa
            continue;
        }

        //borrar los datos
        if(c == '\b'){

            //para borrar algo antes debe haber algo que borrar
            if(i > 0){
                str[--i]= '\0';
                UART_putchar(com,'\b'); //regresa
                UART_putchar(com,' ');
                UART_putchar(com,'\b');
            }

            continue;

        }

        if(c == '.'){

            str[i++] = "\0";
        }


        UART_putchar(com,c);

        //canidad defindio en el archivo .h
        if(i < cantidad){
            str[i++] = c;
        }
        else{
            //este hara el efecto de que ya no puede capturar mas

            str[i] = '\0';
            UART_putchar(com,'\b'); //regresa
            UART_putchar(com, ' ');
            UART_putchar(com,'\b');
        }

    }

}





