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

    UART_Reg_t *myUART = UART_offset[com];


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
