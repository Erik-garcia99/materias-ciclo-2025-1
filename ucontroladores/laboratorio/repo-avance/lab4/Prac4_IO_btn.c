#include <avr/io.h>

#define SetBitPort(port, bit) __asm__ ( ?? )
#define ClrBitPort(port, bit) __asm__ ( ?? )

//Press States
#define NOT_PRESSED 0
#define SHORT_PRESSED 1
#define LONG_PRESSED 2

enum ButtonStates
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
}

#define BTN_PIN PK0
#define SEND_PIN PE1

extern void delay_103us(void);
extern void delay(uint16_t mseg);
void InitPorts(void);
uint8_t check_Btn(void);
void sendData(uint8_t *data);
void sendByte(uint8_t data);

uint8_t data[2][14]=
{
	{0x53,0x68,0x6f,0x72,0x74,0x20,0x70,0x72,0x65,0x73,0x73,0x0D,0x0A,0x00},
	{0x4c,0x6f,0x6e,0x67,0x20,0x70,0x72,0x65,0x73,0x73,0x21 ,0x0D,0x0A,0x00}
};

int main(void)
{
	UCSR0B &= ~(1<<TXEN0); // Disable UART-TX

	InitPorts();
	while(1){
		switch(check_Btn())
		{
			case eBtnShortPressed:
			{
			 	sendData(data[0]);
				break;
			}
			case eBtnLongPressed:
			{
				sendData(data[1]);
				break;
			}
		}
		
	}
}
