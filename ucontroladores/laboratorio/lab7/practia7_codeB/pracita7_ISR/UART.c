#include <avr/io.h>
#include<avr/interrupt.h>


/*
funcion a agregar : ISR



funciones amodificar putchar, getchar


*****************************************************************************

primero, la cola circular como bamos a saber cunado esta llena?,


out_idx -> sera la cabeza del arreglo y lo ponemos en el indeice 0

-recordando que una cola ingresa por la parte posterior, por lo que debemos de al final cunado saquemos datos de la cola sera dese el
indice 0 porque para mi esa seria mi cola,

in_idx sera el la cola, es en donde actualmente tenemos datos, este no sera el eu sale






****************************************************************************************
funciones a mosificar

2 ISR (USART0_UDRE_vet) -> para TX - ISR(USRT0_RX_vect) -> para RX

putchar y getchar

*/


// Definición de colas
ring_buffer_t tx_buffers[4] = {{0}};
ring_buffer_t rx_buffers[4] = {{0}};



uint8_t *UART_offset[]=
{
	(uint8_t*)&UCSR0A,
	(uint8_t*)&UCSR1A,
	(uint8_t*)&UCSR2A,
	(uint8_t*)&UCSR3A

};




// Prototypes
// Initialization
UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){


    //la inicaliazacion e la misma

    UART_reg_t *myUART = UART_offset[com]; // eligo a mi UART

    //habilitar las interrupciones de RX
    myUART->UCSRB = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // Habilita TX y RX para UART0

    uint8_t parity_mode=0;

    switch(parity){
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


    uint8_t stop_mode= (stop == 1)? 0:1;
    myUART->UCSRC = (parity_mode << UPM00) | (stop_mode <<USBS0);
    //tamanio del frame
	if (size == 9) {
        myUART->UCSRC |= (3 << UCSZ00); // UCSZ01:UCSZ00 = 0b11
        myUART->UCSRB |= (1 << UCSZ02); // Habilitar bit 9
    } else
     {
        myUART->UCSRC |= ((size - 5) << UCSZ00); // Ej: 8 bits ? 3 << UCSZ00
    }

    uint16_t v_UBRR = (FOSC / (16 * baudrate)) - 1 ;
    myUART->UBRR = v_UBRR;


    //para empezar este UART debe de tener habilitado las interrupcioens

    sei(); //con esta funcionse habilitan las interruciones de manerja global

}



//la ISR debe de accionarse, dependiod a que UART le correxponde la interrupcion.
//ahora mismo el perifereico esta listo para mandar una interrupcion, por lo que ahora le toca a la ISR atender esa interrupcion
ISR(UART0_UDR0_VEC){
    //estara hecho para el UART0


}


//interrupcion para RX
ISR(USART0_RX_vect){



}



//send
/*
lo que hace la funcion putchar seria enviar el caracter a la cola TX y hbaulutar la interrupcion UDRE

*/
void UART_putchar(uint8_t com, char data){


    ring_buffer *tx = &tx_buffers[com]; //va a elegir sobre cual UART se mandara.

    //calculamos cobre que indice esta in_dx esta actualmente.
    uint8_t next_int = (tx->in_dx +1 ) % BUFFER_SIZE;

    /*
        lo que esta pasando es que esta esperando si el buffer esta llegno, como podemos saber si el buffer esta lleno,

    */
    while(next_int == tx->out_dx);


    //una vez que se hayan liberados espapcios en el buffer ingresamos el sigueite dato

    tx->buffer[tx->in_dx] = data;
    tx->in_dx = next_int;

    //habilitamos la interrupcion UDRE

    UART_reg_t *myUART = UART_offset[com];

    if(!(myUART->UCSRB & (1 << UDRIE0))){
        myUART->UCSRB |= (1 << UDRIE0);
    }else{
          UCSR0B &= ~(1 << UDRIE0); // Deshabilitar interrupción si no hay datos, para que no este pasando la interrupcion siempre
    }

}



// Received
uint8_t UART_available(uint8_t com);


char UART_getchar(uint8_t com );


// Send
void UART_puts(uint8_t com, char *str)
{

    // TXn trasmitir el contenido
    while (*str != '\0')
    {
        // mientras haya contenido en el apuntador, que sea diferente a NULL
        UART_putchar(com, *str++);
    }
}


void UART_gets(uint8_t com, char *str)
{

    char c; // este va a capturar el valor del char que se introdujo
    uint8_t i = 0;
    uint8_t dot_flag = 0; // Bandera para detectar punto

    while (1)
    {
        c = UART_getchar(com);
        if (c == '\b')
        {
            if (i > 0)
            {

                str[--i] = '\0'; // sustitumos el utlimo caracter con el nulo
                UART_putchar(com, '\b');
                UART_putchar(com, ' ');
                UART_putchar(com, '\b');
            }

            continue; // si no hay nada que borrar o si hay algoq ue borrar sigue con el ciclo
        }

        UART_putchar(com, c);

        if (c == '\r' || c == '\n')
        {
            // retorno de carro o salto de linea lo que quiere decir que se terminao de escribir el
            // texto actual.

            str[i] = '\0';           // caracter nulo denotando que la
            UART_putchar(com, '\r'); // vuelve al inicio de la linea
            UART_putchar(com, '\n'); // salto de linea
            break;                   // rompesmos el ciclo y a esperar que se vuelva a escribir algo
        }

        if (c == '.')
        {
            dot_flag = 1;
            continue; // No muestra el punto
        }

        if (dot_flag)
        {

            str[i++] = '\0'; // desoues de este punto ya no lo tomara en cuenta
            dot_flag = 0;    // para que ya no enre aqui
        }

        // para 20 caracteres, si no lo regresamos a 127

        if (i < 19)
        {

            str[i++] = c;
            // UART_putchar(com, c);
        }
        else
        {
            str[i] = '\0'; // sustitumos el utlimo caracter con el nulo
            UART_putchar(com, '\b');
            UART_putchar(com, ' ');
            UART_putchar(com, '\b');
        }
    }
}

//*****************************************************************************************************

// Escape sequences
UART_clrscr( uint8_t com ){

    UART_reg_t *myUART = UART_offset[com];

    UART_puts(com,"\x1B[2J"); //borra toda la pantalla
    UART_puts(com,"\x1B[H"); // poen el curso al incio fila 1, columna 1


}



UART_setColor(uint8_t com, uint8_t color){


    UART_puts(com,"\x1B["); //incio del comando espace
    UART_putchar(com, '0'+(color/10));
    UART_putchar(com,'0'+(color%10));
    UART_putchar(com,'m'); //final del comando
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

//**************************************************************************************************


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

        char buffer[16];
        uint8_t buf_idx = 0;

        while (temp > 0) {
            buffer[buf_idx++] = hex[temp % base];
            temp /= base;
        }

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
