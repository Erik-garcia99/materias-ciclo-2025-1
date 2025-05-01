#include <avr/io.h>

// Macros
#define SetBitPort(port, bit) __asm__ __volatile__("sbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
#define ClrBitPort(port, bit) __asm__ __volatile__("cbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")

#define TIME_WINDOW 300
#define SEED 5
#define BTN_PIN PF7


enum ButtonStates
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
};

enum GameStates
{
    eGameRestart = 0,
    eWaitForStart,
    eStartCount,
    eEndCount,
    eGameOver,
    eYouWin,
    eYouLoose
};

/*arreglo para asignar a los puertos*/

uint8_t _CONF_PORT[] = {

    // que se necesita en la salida de los puertos para que se prenda un led, vamos a lo simple, uno a uno
    0x42, // LD1 con el 4 se esta mandando 1 en todo momneto por el pin 6 para su uso
    0x41, // LD2
    0x44, // LD3
    0x42, // LD4
    0x41, // LD5
    0x44, // LD6
    0x48, // LD7
    0x44  // LD18

};



uint8_t _CONF_DDR[] = {

    // al parecer para que este pueda funcionar necesitamos, poner com entrada los demas leds que no se ocupen para prender
    // un led, es como ponerlos en corto
    0x43, // LD1 activamos el pin 6 como salida y el pin 7 como entrada y el pin 7 es que estamos cehcando en caso
    0x43, // LD2 de ser presionado
    0x46, // LD3
    0x46, // LD4
    0x45, // LD5
    0x45, // LD6
    0x4C, // LD7
    0x4C  // LD8

};





// Prototypes
extern uint8_t myRand(uint8_t seed);
extern void delay(uint16_t mseg);
void InitPorts(void);
uint8_t check_Btn(void);
void updateLeds(uint8_t gameState);

// Global variable
uint32_t millis;


/*

mis prototipos

*/


void waitState(void);

void startGame(void);

void endCount(void);



int main(void)
{
    uint8_t currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;
    InitPorts();

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
            //countdown = ((myRand(SEED) + 1) * 2) % 10;
            //countdown = (myRand(SEED) % 10) + 5 ;
            countdown = 5;
            countup = 0;
            currentGameState=eWaitForStart;
            break;
        }
        case eWaitForStart:
            break;
        case eStartCount:
        {
            countdown--;
            if (countdown == 0)
                currentGameState=eEndCount;
            break;
        }
        case eEndCount:
        {
            if (countdown != 0)
                currentGameState=eGameOver;
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




void InitPorts(void)
{

    /*
    // 0 configura de entrada en los puertos
    DDRF &= ~(1 << PF7); // entrada
    DDRF |= (1 << PF6);  // salida
    // confguro PF6 como salida y pf7 como entrada, esto para con el boton si se presiona deja fluir lo que sale de PF6 que sera 1 logico
    // el cual recibira PF7 y es el que tengo que estar leyendo si se ha producido la lectura del boton.


        0000 0000     0000 0000
        ~1100 0000 -> 0011 1111
                      0000 0000


    PORTF |= (1 << PF6) | (1 << PF7); // Enable pull-up for PF7 and set PF6 high  quiero que constantemente se este sacando un 1 por el pin 6 del puerto, por esa razon lo asigno
    // con un OR*/

    DDRF &= ~(1 << PF7);     // PF7 como entrada
    DDRF |= (1 << PF6);      // PF6 como salida
    PORTF |= (1 << PF7);     // Pull-up en PF7
    PORTF |= ~(1 << PF6);    // PF6 en LOW (clave)
}



uint8_t check_Btn(void)
{

    // el circuoto del boton esta en pull - up, por lo que para verificar si el boton
    // esta presionado o no, no presiondado = 1 - HIGH
    //  presionado = 0 - LOW
    // PINx lee el estado fisico del pin

    if(PINF & (1<<BTN_PIN)) return eBtnUndefined;

    delay(20); // esperar 20 ms antes de actuar

    if (PINF & (1 << BTN_PIN)) return eBtnUndefined;

    uint16_t tiempo_presionado = 0;

    while (1)
    {

        delay(1);
        tiempo_presionado++;

        if (PINF & (1 << BTN_PIN))
        {
            break;
        }

        // si se superoa el segundo se detecta como largo

        if (tiempo_presionado >= 1000)
        {

            //
            while (!(PINF & (1 << BTN_PIN)))
            {
                delay(1);
            }
            delay(20);
            return  eBtnLongPressed;
        }
    }

    delay(20);
    if (PINF & (1 << BTN_PIN))
    {

        return eBtnShortPressed;
    }

    return eBtnUndefined;
}








void updateLeds(uint8_t gameState){


    switch(gameState){


    case eWaitForStart:
        waitState();
        break;

    case eStartCount:
        startGame();break;

    case eEndCount:
        endCount();break;
    }


}



void waitState(void){


    static volatile uint8_t indice=0;
    DDRF = _CONF_DDR[indice];
    PORTF =_CONF_PORT[indice];
    delay(100);
    indice = (indice+1) %8;


}

void startGame(void){

    //generar una secuencia random

    static uint8_t seed = SEED;

    seed +=millis %255;

    uint8_t random = myRand(seed);
    uint8_t indice=  random %8;

    DDRF=_CONF_DDR[indice];
    PORTF = _CONF_PORT[indice];
    delay(100);


}


void endCount(void){


    DDRF &=0xF0;
    PORTF &=0xF0;

}
