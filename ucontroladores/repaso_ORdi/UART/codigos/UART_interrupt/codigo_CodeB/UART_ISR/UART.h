

#ifndef UART_H
#define UART_H


#define FOSC 16000000UL
#define cantidad 64 // cantidad de elementos del arreglo

//para poder crear un programa el cual incialice cualquier uart necesitamos un apuntador a cada uart en su espacio en memoria

// tamanio del buffer
#define BUFFER_SIZE cantidad


//estructru para la cola circular
typedef struct{

    char buffer[BUFFER_SIZE];
    volatile unsigned char in_idx;
    volatile unsigned char out_idx;
}ring_buffer_t;





typedef struct{

    //volatile porque este valor va a cambiar por lo que quieroe que C no lo optimice
    volatile uint8_t UCSRA;
    volatile uint8_t UCSRC;
    volatile uint8_t UCSRB;
    volatile uint8_t resb;
    uint16_t UBRR;
    uint8_t UDR;
}UART_reg_t;




UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop);

uint16_t set_UBRR(uint32_t baudrate);

// Send
void UART_puts(uint8_t com, char *str);
void UART_putchar(uint8_t com, char data);

// Received*/
uint8_t UART_available(uint8_t com);
char UART_getchar(uint8_t com );
void UART_gets(uint8_t com, char *str);

/*
// Escape sequences
UART_clrscr( uint8_t com );
UART_setColor(uint8_t com, uint8_t color);
UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);

#define YELLOW  33 // Fixme
#define GREEN   32 // Fixme
#define BLUE    34 // Fixme


// Utils
void itoa(uint16_t number, char* str, uint8_t base);
uint16_t atoi(char *str);
*/
#endif

