#ifdef

#define FOSC 16000000UL


//necesitamos la macro que inica el UART que recibe sus parametros para implementar el UART

/*
calcualar el error del UART

error_absotulo = |BoudRate_deseado - boudRate_real|

Error_poncentual = (error_absoluto / boudRare_deseado) * 100%

-> Error_poncencila = (BoudRate_real - boudRate_deseado / boudRate_deseado) * 100%

el boudrete_real se calcula redondeadeando el resultado si es mayor a 0.5 se redondea hacia arriba



*/

struct typedef{

    volatile uint8_t UCSRA,
    volatile uint8_t UCSRB,
    volatile uint8_t UCSRC,
    uint8_t resb,
    uint16_t UBRR,
    uint8_t UDR;


}UART_reg_t;



void UART_init(uint8_t com,uint32_t BOUD, uint8_t parity, uint8_t stop, uint8_t ch_size);


//recordando que puts es un funcion que imprimr resultado

void putchar(uint8_t com, char data);


void puts(uint8_t com, char *str);


//get- getchar es un funcion que toma valores por lo que getchar devulve el valor optenido de RX

uint8_t avaible(uint8_t com); //me dice si RX esta listo para recibir

char getchar(uint8_t com);


void gets(uint8_t com, chat *str);






#endif
