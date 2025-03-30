
#include<avr/io.h>
#include "UART.h"


UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop){

    uint8_t UART_reg_t *myUART = UART_offset[com];

    myUART->UCSRC =()


}
