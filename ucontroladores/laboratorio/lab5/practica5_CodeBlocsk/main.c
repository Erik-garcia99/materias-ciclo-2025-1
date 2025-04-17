#include <avr/io.h>

// Macros
#define SetBitPort(port, bit) __asm__ __volatile__("sbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
#define ClrBitPort(port, bit) __asm__ __volatile__("cbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")

#define TIME_WINDOW 300
#define SEED xx
/*******************/


enum ButtonStates
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
}

enum GameStates
{
    eGameRestart = 0,
    eWaitForStart,
    eStartCount,
    eEndCount,
    eGameOver,
    eYouWin,
    eYouLoose
}

/********/
//mejor si seria un arreglo este


//hagamos un enum para representar que led se sta haciendo referencia


uint8_t _CONF_PORT[]={

    //que se necesita en la salida de los puertos para que se prenda un led, vamos a lo simple, uno a uno
    0x02, //LD1
    0x01,
    0x04,
    0x02,
    0x01,
    0x04,
    0x08,
    0x04







};

uint8_t _CONF_DDR[]={

    //al parecer para que este pueda funcionar necesitamos, poner com entrada los demas leds que no se ocupen para prender
    //un led, es como ponerlos en corto
    0x03,
    0x03,
    0x06,
    0x06,
    0x05,
    0x05,
    0x0C,
    0x0C





};


enum _leds_{

    LD1,
    LD2,
    LD3,
    LD4,
    LD5,
    LD6,
    LD7,
    LD8

};



/*********/


// Prototypes
extern uint8_t myRand(uint8_t seed);
extern void delay(uint16_t mseg); //OK

void InitPorts(void); //al parecer este va asre igual porque en cada led se va a estar manipulando de disitntas maneras

uint8_t checkBtn(void); //OK

void updateLeds(uint8_t gameState);

// Global variable
uint32_t millis;

int main(void)
{
    uint8_t currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;

    //InitPorts();

    //configurar los pines 0 - 3  como salida
    DDRF = 0xF;

    while (1)
    {
        switch (check_Btn())
        {
        case eBtnShortPressed:
            currentGameState++;
            break;
        case eBtnLongPressed:
            currentGameState = eGameRestart;
            break;
        }

        switch (currentGameState)
        {
        case eGameRestart:
        {
            countdown = (myRand(SEED) + 1) * 20;
            countup = 0;
            currentGameState++;
            break;
        }
        case eWaitForStart:
            break;
        case eStartCount:
        {
            countdown--;
            if (countdown == 0)
                currentGameState++;
            break;
        }
        case eEndCount:
        {
            if (countdown != 0)
                currentGameState++;
            else
                countup++;
            break;
        }
        case eGameOver:
        {
            if ((countdown + countup) > TIME_WINDOW)
                currentGameState = eYouLoose;
            else
                currentGameState = eYouWin;
            break;
        }
        }

        updateLeds(currentGameState);
        delay(1);
        millis++;
    }
}


/*
void InitPorts(void){

    DDRF = 0x0F;


}*/
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
