
#include<avr/io.h>
#include<avr/interrupt.h>
#include "UART.h"

//macros que nos pueden ayudar a reducri el codigo

//como saber si el buffer esta vacio, es cunado ambos indicen tienene el mismo valro de inicio
#define IS_BUFFER_EMPY(buf) (buf.in_idx == buf.out_idx)

/*
UART con interrupciones

uso de 2 colas, una para recepcion y otra la transmision


*/
//incializa sus valores en 0
//solo funcionaran para el buffer del UART0
ring_buffer_t tx_buffer[4]= {{0}};
ring_buffer_t rx_buffer[4] = {{0}};






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

    //habilitar interrupciones para RX y TX

    sei();

    myUART->UCSRB |= (1<<RXCIE0) | (1<<UDREI0);




}



//funcion putchar

/*
esta funcion lo que hace es mandar lo que hay en registro del tramsisor (UDR)
hacia el pin, por medio de la comunicacion serie


*/




//modificar esta funcion para UART ISR TX
void UART_putchar(uint8_t com, char data){

    UART_reg_t *myUART = UART_offset[com];

    /*
    UDREn indica si el budder de transmison esta vacio y listo para recibir nuevos datos

    este bit esta en 1 quiere decir que est vacio por lo tanto esta listo para recibir nuevos datos
    UDREn = 0, esta lleno por lo que no puedo recibir nuevos datos

    porque en la funcion se niega?
    se esta negando porque si UDREn =0 quiere decri que hay datos dentro del buffer por lo que negamos (!) es para esperar
    hasta que el buffer este vacio por lo que esta vacio pasa la condicion y manda lo que hay en UDR al pin TX


    estas lineas ya no son necesarios ya que todo se hace pro medio de las interrupciones

    esta parte es la que realiza la ISR
    while (!(myUART->UCSRA & (1 << UDRE0)));

    myUART->UDR = data;*/

    //ptchar ahora lo que hara es tomar el valor y meterlo dentro del buffer de transmision

    /*
    aspectos que debemos tomar en cuenta, es en no sobreescribir datos que ya estan

    debemos limpiar la bandera de interrupcion del UART0 de transmision.
    */

    //en el cosigo solo hay ISR para UART0 pero y para los demas?, solo pidio UART0 por lo que se puede unca condicion


    //como saber si esta lleno

    //le paso los valores que hay en la direccion en donde esta guardada la estrucutra (com)
    ring_buffer_t *tx = &tx_buffer[com];


    while(((tx->in_idx+1) % BUFFER_SIZE) == tx->out_idx); //esperar a que se libere si el buffer esta lleno

    tx->buffer[tx->in_idx] = data;

    //ahora debo de actualizar el indice de entrada
    //aout no se actualiza aqui, se actualia en la ISR
    tx->in_idx = (tx->in_idx + 1) % BUFFER_SIZE;


    //habilitamos la interrupcion
    myUART->UCSRB |= (1<< UDRIE0);




}

//escribir la ISR par TX
//esta ISR lo se va a encargar es de tomar el dato que hay en el buffer y mandaro al pin asociado a TX
ISR(USART0_UDRE_vect){

    UART_reg_t *myUART = UART_offset[0]; //traigo la estructura del UART0

    //si la bander UDRE esta en 1 quiere decir que esta lista para recibir nueva informacion y lista
    //para ser lanzada,

    //va a tomar datos del buffer circular y lo mandara al pin a TX

    //necesito tomar el dato que esta en mi buffer de transmision
    ring_buffer_t *tx = &tx_buffer[0]; //UART0
    if(tx->in_idx != tx->out_idx){

        char data = tx->buffer[tx->out_idx];

        //no se si hya datos dentreo del periferico por lo que necesito saber el buffer esta lleno,
        //si es asi no esperar hasta que este vacio
        myUART->UDR = data;

        //actualizar el indice de salida circular

        tx->out_idx = (tx->out_idx + 1) % BUFFER_SIZE;


        //VERIFICAMOS SI AUN QUEDAN DATOS

        if(tx->in_idx == tx->out_idx){

            myUART->UCSRB &= ~(1<<UDRE0);
            tx->in_idx = 0;
            tx->out_idx = 0;
        }

    }
    else{
        //la interrupcion de desahabilita escribiendo 1 en UDREn
        //esta queda en duda si deshabilito en total o solo limpio la bandera que causa la interripcion

        myUART->UCSRB &= ~(1<<UDRE0);
        //si desactivamos la interrupcion quiere decir que no hya datos que mandar, por lo que
        /*
        el buffer esta vacio o ya se recorrio, todo por lo que mandamos a cero los indices que no
        */
        tx->in_idx = 0;
        tx->out_idx = 0;



    }
}



//aplicamos las mismas ISR para UART2 y UAR3

ISR(USART2_UDRE_vect){

    UART_reg_t *myUART = UART_offset[2]; //traigo la estructura del UART2

    ring_buffer_t *tx = &tx_buffer[2]; //UART2
    if(tx->in_idx != tx->out_idx){

        char data = tx->buffer[tx->out_idx];

        myUART->UDR = data;

        //actualizar el indice de salida circular

        tx->out_idx = (tx->out_idx + 1) % BUFFER_SIZE;


        //VERIFICAMOS SI AUN QUEDAN DATOS

        if(tx->in_idx == tx->out_idx){

            myUART->UCSRB &= ~(1<<UDRE0);
            tx->in_idx = 0;
            tx->out_idx = 0;
        }

    }
    else{
        myUART->UCSRB &= ~(1<<UDRE0);
        tx->in_idx = 0;
        tx->out_idx = 0;



    }
}



ISR(USART3_UDRE_vect){

    UART_reg_t *myUART = UART_offset[3]; //traigo la estructura del UART3

    ring_buffer_t *tx = &tx_buffer[3]; //UART3
    if(tx->in_idx != tx->out_idx){

        char data = tx->buffer[tx->out_idx];

        myUART->UDR = data;

        //actualizar el indice de salida circular

        tx->out_idx = (tx->out_idx + 1) % BUFFER_SIZE;


        //VERIFICAMOS SI AUN QUEDAN DATOS

        if(tx->in_idx == tx->out_idx){

            myUART->UCSRB &= ~(1<<UDRE0);
            tx->in_idx = 0;
            tx->out_idx = 0;
        }

    }
    else{
        myUART->UCSRB &= ~(1<<UDRE0);
        tx->in_idx = 0;
        tx->out_idx = 0;



    }
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

ISR(USART0_RX_vect){


    UART_reg_t *myUART = UART_offset[0]; //RX UART0

    ring_buffer_t *tx = tx_buffer[0];

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





