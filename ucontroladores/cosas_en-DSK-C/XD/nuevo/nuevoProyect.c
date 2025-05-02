#include<avr/io.h>

/* %0 -> port
  %1 -> bit*/
#define SetBitPort(port, bit) __asm__ __volatile__("sbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
#define ClrBitPort(port, bit) __asm__ __volatile__("cbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
/*memory indica al compilador que puede modificar la memoria
I  indica que los arguementos deben de ser tratados como constantes inmediatas
*/

// Press States
#define NOT_PRESSED 0
#define SHORT_PRESSED 1
#define LONG_PRESSED 2

enum ButtonStates
{
	eBtnUndefined = 0,
	eBtnShortPressed,
	eBtnLongPressed
};

#define BTN_PIN PK0
#define SEND_PIN PE1

/*
apoyo
*/
//*****************

extern void delay1S(void);

//*************

extern void delay_103us(void);
extern void delay(uint16_t mseg);


void InitPorts(void);
uint8_t check_Btn(void);
void sendData(uint8_t *data);
void sendByte(uint8_t data);

uint8_t data[2][14] =
	{
		{0x53, 0x68, 0x6f, 0x72, 0x74, 0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x0D, 0x0A, 0x00},
		{0x4c, 0x6f, 0x6e, 0x67, 0x20, 0x70, 0x72, 0x65, 0x73, 0x73, 0x21, 0x0D, 0x0A, 0x00}};

int main(void){
	delay(1000);
	
	uint8_t variable=0;


}

