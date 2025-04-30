#include <avr/io.h>

// Macros
#define SetBitPort(port, bit) __asm__ __volatile__("sbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
#define ClrBitPort(port, bit) __asm__ __volatile__("cbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")

#define TIME_WINDOW 300
#define SEED 0x10
/*******************/
#define BTN_PIN PF7
//#define NOT_PRESSED 0
//#define SHORT_PRESSED 1
//#define LONG_PRESSED 2

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

/********/
// mejor si seria un arreglo este

// hagamos un enum para representar que led se sta haciendo referencia
/*
tuvimos un problema, si ponemos asi el valor tal cual para DDRx y PORTx estariamos desactivando los puertos de entrada
para el boton por lo que es necesario hacer un cambio, 1 de 2, o hacemos un procedimineto para que deje pasar esos bits
o ponemos por defecto los valores que hacen el pin 6 y 7 de entrada en el puerto F donde el pin 6 siempre esta el alto.

es lo que haremos por lo que necesitamos que

DDRx =  01xx xxxx
PORTx = x1xx xxxx

lo que nos da un 4 en la parte alta y ya dependido la parte baja de lo que se requiera de salida para que prendan los led.
*/

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

/*********/

// Prototypes
extern uint8_t myRand(uint8_t seed);

extern void delay(uint16_t mseg); // OK

// ta OK
void InitPorts(void); // al parecer este va asre igual porque en cada led se va a estar manipulando de disitntas maneras

uint8_t check_Btn(void); // OK

void updateLeds(uint8_t gameState);

/*
mis prototipos

*/

void waitState();
void startGame();

// Global variable
uint32_t millis;
uint32_t lastUpdate=0;
volatile uint8_t _start_game_=0;

int main(void)
{
    uint8_t currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;

    // varibale para el modod wait para que este contantemente ciclado siempre y cunado no se ha presionado el boton.
    uint8_t bucle = 1;
    // InitPorts();
    uint8_t lectura;
    uint8_t btn;
	
	btn = (myRand(SEED) + 1) * 20;
    // configurar los pines 0 - 3  como salida
	
	__asm__("nop");
    // prueba

		btn = (myRand(SEED) + 1) * 20;
    // configurar los pines 0 - 3  como salida
	
	__asm__("nop");
    // prueba

    // currentGameState++;

    while (1)
    {

        //btn=  check_Bt();

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
            //el juego se va a inicar cunado se haga un longPress, quiere decir que al principio estara apagado no es asi
            /*
                esto es lo que va a pasar cunando se inice el juego,
                para inicar el juego debemos de presionar el boton un largo rato, este nos dara nuestro numero random
                que sera el tiempo que este nos daera para ganarle al juego, entre que apaga los leds y tenemos
                un tiempo limite para presionar el boton

                entonces como podemos hacer para pasar de los leds de wait al otro estado?

            */

            // cuando se presione la priemra vez lo que hara es que ser mirara ese patron de espera que hemos diseniado
            // por lo que debemos se saber, el como implementar

            // ya se como va esta madre, este va a esperar mientras no se precione el boton este tendra que mostrar la secuencia
            // de espera, cuendo se presione este se va a detener y <currentGameState> va a incrementar en 1
            // que dara inicio al juego
            //lectura = check_Btn();

            //updateLeds(eWaitForStart);

            /*if(btn == eBtnShortPressed){
                PORTF &= 0xF0;
                _start_game_=1;
                currentGameState = eStartCount;
            }*/

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

void InitPorts(void)
{

    // 0 configura de entrada en los puertos
    DDRF &= ~(1 << PF7); // entrada
    DDRF |= (1 << PF6);  // salida
    // confguro PF6 como salida y pf7 como entrada, esto para con el boton si se presiona deja fluir lo que sale de PF6 que sera 1 logico
    // el cual recibira PF7 y es el que tengo que estar leyendo si se ha producido la lectura del boton.

    /*
        0000 0000     0000 0000
        ~1100 0000 -> 0011 1111
                      0000 0000

    */
    PORTF |= (1 << PF6); // quiero que constantemente se este sacando un 1 por el pin 6 del puerto, por esa razon lo asigno
    // con un OR
}

/*
ok el check btn no esta del todo bien, esto porque

*/
uint8_t check_Btn(void)
{

    // el circuoto del boton esta en pull - up, por lo que para verificar si el boton
    // esta presionado o no, no presiondado = 1 - HIGH
    //  presionado = 0 - LOW
    // PINx lee el estado fisico del pin

    delay(20); // esperar 20 ms antes de actuar

    if (PINF & (1 << BTN_PIN))
    {

        return eBtnUndefined;
    }

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
}

void updateLeds(uint8_t gameState)
{
    // esta funcion debe de devolver estos estados en los cuales se esta haceindo los cambios necesarios para los leds

    // por lo que creo que deberia de ser un switch o esta funcion invocar a otras que haga ese proceso.


    switch (gameState)
    {

        case eWaitForStart:
        {
            waitState();
            break;
        }

        case eStartCount:
//            startGame();
            break;



    }
}

/*
void waitState(uint8_t btn) {
    static uint32_t lastChange = 0;
    uint8_t indice=0;

    while(1){
        while(indice<8){
            DDRF = _CONF_DDR[indice];
            PORTF = _CONF_PORT[indice];
            delay(100);
            indice++;
        }
        indice =0;
        if(_start_game_==1){
            break;
        }
    }
}*/

void waitState() {

    volatile uint8_t indice=0;
    while(indice<8){
        DDRF = _CONF_DDR[indice];
        PORTF =_CONF_PORT[indice];
        indice++;
    }
}

/*
void startGame() {

    uint8_t indice=0;

    uint8_t random;

    for(uint8_t idx = 0; idx<random; idx++){


        if(indice>=8){
                //volvemos a regresar el indice al inicio
            indice =0;
        }

    }


}*/
