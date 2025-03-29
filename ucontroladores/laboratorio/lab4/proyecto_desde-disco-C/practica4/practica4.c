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

int main(void)
{
	UCSR0B &= ~(1 << TXEN0); // Disable UART-TX

	// uint8_t bit =3;

	InitPorts();

	/*while(1){
		SetBitPort(PORTE, 1);

	}*/

	InitPorts();
	while (1)
	{
		switch (check_Btn())
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

void InitPorts(void)
{

	// conn 1 es salida, con 0 es entrada

	DDRK &= ~(1 << BTN_PIN);
	//PORTK &= ~(1 << BTN_PIN); //habilita o deshabilitas los pull-up's internos

	DDRE |= (1 << SEND_PIN);
}

uint8_t check_Btn(void)
{

	// el circuoto del boton esta en pull - up, por lo que para verificar si el boton
	// esta presionado o no, no presiondado = 1 - HIGH
	//  presionado = 0 - LOW

	if (PINK & (1 << BTN_PIN))
	{
		return 0;
	}

	// PINx lee el estado fisico del pin

	delay(20); // esperar 20 ms antes de actuar

	if (PINK & (1 << BTN_PIN))
	{

		return NOT_PRESSED;
	}

	uint16_t tiempo_presionado = 0;

	while (1)
	{

		delay(1);
		tiempo_presionado++;

		if (PINK & (1 << BTN_PIN))
		{
			break;
		}

		// si se superoa el segundo se detecta como largo

		if (tiempo_presionado >= 1000)
		{

			//
			while (!(PINK & (1 << BTN_PIN)))
			{
				delay(1);
			}
			delay(20);
			return LONG_PRESSED;
		}
	}

	delay(20);
	if (PINK & (1 << BTN_PIN))
	{

		return SHORT_PRESSED;
	}

	return 0;
}


void sendData(uint8_t *data){


	//perimo es calcular cunatos elementos tiene este arreglo, ahora se que 
	//tiene esos pero pueden ser mas o menos 

	uint8_t i=0;

	while(data[i]!=0x00){
	
		sendByte(data[i]);
		i++;
	
	}

}


void sendByte(uint8_t data){


	//paso 1, poner PE1 en 0 logico 
	ClrBitPort(PORTE, 1);
	delay_103us();

	for(uint8_t idx=0; idx  < 8; idx++){
	
		if(data & 0x01){
			SetBitPort(PORTE,1);
		}
		else{
			ClrBitPort(PORTE, 1);
		}

		delay_103us();
		data >>=  1;
	}

	
	SetBitPort(PORTE,1);
	delay_103us();


}
