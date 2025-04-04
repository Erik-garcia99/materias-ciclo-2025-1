


//estrucutra que define a los distintos uarts
typedef struct{

	uint8_t UCSRA;
	uint8_t UCSRB;
	uint8_t UCSRC;
	uint8_t resb;
	uint16_t UBRR;

}UART_reg_t;


/*
para poder acceder a ;ps registros

*/

uint8_t *UART_offset[]=
{
	(uint8_t*)&UCSR0A,
	(uint8_t*)&UCSR1A,
	(uint8_t*)&UCSR2A,
	(uint8_t*)&UCSR3A

};


// Prototypes
// Initialization


UART_Ini(uint8_t com, uint32_t baudrate, uint8_t size, uint8_t parity, uint8_t stop);
/*
// Send
void UART_puts(uint8_t com, char *str);
void UART_putchar(uint8_t com, char data);

// Received
uint8_t UART_available(uint8_t com);
char UART_getchar(uint8_t com );
void UART_gets(uint8_t com, char *str);

// Escape sequences
UART_clrscr( uint8_t com );
UART_setColor(uint8_t com, uint8_t color);
UART_gotoxy(uint8_t com, uint8_t x, uint8_t y);

#define YELLOW  0 // Fixme
#define GREEN   0 // Fixme
#define BLUE    0 // Fixme

// Utils
void itoa(uint16_t number, char* str, uint8_t base)
uint16_t atoi(char *str)*/
