//vamos  aver todo desde cero

#include<avr/io.h>

/*
erik garcia chavez 01275863 - practica5
GPIOS

*/


//Macros
#define SetBitPort(port, bit) __asm__ __volatile__("sbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")
#define ClrBitPort(port, bit) __asm__ __volatile__("cbi %0, %1" : : "I"(_SFR_IO_ADDR(port)), "I"(bit) : "memory")

/*
las entradas son las que tienen el pull-up que puede estar activado o desactivado.
*/

//PF6 como salida y manda siempre 1
//PF7 como entrada
//PF7 tiene un  pull-down para evitar interferencias o que el boton este flotado cunado este
//no este precionado, necesito aterrizarlo.


#define TIME_WINDOW 300
#define BTN_PIN PF7
#define SEED 10


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



//arreglos que configuran PORTF y DDRF

const uint8_t _CONF_DDR[] = {

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

const uint8_t _CONF_PORT[] = {

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



// Prototypes
extern uint8_t myRand(uint8_t seed); //esperemos que funcione, ya vreremos
extern void delay(uint16_t mseg);
void InitPorts(void);
uint8_t check_Btn(void);
void updateLeds(uint8_t gameState);



//---------------------------------------
//funciones propias

void _waitState_(void);
void _startGame_(void);
void _endCount_(void);
void _youLoose_(void);
void _youWin_(void);


//-------- apoyo

void __LOW_NIBBLE__(void);

void __HIGH_NIBBLE__(void);


//--------------------------------------

// Global variable
uint32_t millis;

int main(void)
{
    uint8_t currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;
    InitPorts();


    while(1){

        _youWin_();

    }


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


void InitPorts(void){

    /*
    el not o eso, afecta a loque poneomos ene ste caso

    ~(1<<PF6) -> 0100 0000 -> al aplicar el not 1011 1111, por lo que esto afecta al registro PINF, es como la mascara padre



    */
    DDRF |= (1<<PF6); //PF6 como salida, PULL-UP desable, no importa si desactivo lo dem
    //necesito mandar el 1 por mientras, mientras se configura lo demas
    PORTF |=(1<<PF6);

    //PF7 funciona como entrada, de entrada debo de desactiva rl PULLUP
    DDRF &=~(1<<PF7);

    /*
     0xxx xxxx

     1000 0000
     ~
     0111 1111 -> dejo pasar a los 1 y los ceros de vuelven ceros, se mantienen
    */

    PORTF &=~(1<<PF7);


}


//el check_btn  sera un poquito diferente
uint8_t check_Btn(){

    //creamos una varbale que recoja los que hay en el pin?

    uint8_t btn = PINF & (1<<BTN_PIN); //leo lo que hay en el pin

    DDRB = (1<<PF7);

    if(btn == 1){
        SetBitPort(PORTB,1);

        return 0;
    }

    if(btn !=1){
        ClrBitPort(PORTB,7);

        return 0;
    }




    /*
    //okay, sabemos que PF7 esta en 0 aterrizado a tierra, por lo que si no se presiona esta en 0

    if(!(PIN & (1){
        //como ahora lo tenemos en DOWN, lo invertimos, esto para que, porque actulamet estoy leyendo 0
        /*
            lo que quiere decir que no esta presionado el boton

        return eBtnUndefined;
    }

    delay(30); esperamos


    if((!(PIN & (1<<BTN_PIN))){
        return eBtnUndefined;
    }*/

}







//funciones de los estados


void _waitState_(void){

    static volatile uint8_t indice=0; // static para mantenre el valor entre entrada y salida de la funcion y no quiero
    //que se optimice ya que estara cambiando constantemente

    DDRF = _CONF_DDR[indice];
    PORTF= _CONF_PORT[indice];
    delay(1);

    indice= (indice+1) % 8;

}

void _startGame_(void){


    static uint8_t seed = SEED;

    uint8_t random=myRand(seed);
    seed = random;

    uint8_t indice = random % 8;

    DDRF = _CONF_DDR[indice];
    PORTF = _CONF_PORT[indice];
    delay(100);

}




void _endCount_(void){

    DDRF &=0xF0;
    PORTF &= 0xF0;

}


void _youLoose_(void){

    //7 6 5 4  3 2 1 0


    for(uint8_t idx =0 ; idx< 4 ; idx++){

        __LOW_NIBBLE__();
    }

    _endCount_();

    delay(500);



    for(uint8_t idx =0 ; idx< 4; idx++){

        __HIGH_NIBBLE__();

    }
    _endCount_();
    delay(500);


}


void __LOW_NIBBLE__(void){

    static volatile uint8_t idx =0;

    DDRF = _CONF_DDR[idx] | (DDRF & 0xF0);
    PORTF = _CONF_PORT[idx] | (PORTF & 0xF0);
    delay(1);

    idx = (idx + 1)%4;

}


void __HIGH_NIBBLE__(void){


    static volatile uint8_t idx = 4;

    DDRF = _CONF_DDR[idx] | (DDRF & 0xF0);
    PORTF = _CONF_PORT[idx] | (PORTF & 0xF0);
    delay(1);

    idx = 4 + ((idx + 1) % 4);
}


void _youWin_(void){


    for(uint8_t idx=0; idx< 8 ; idx++){

        _waitState_();
    }

    _endCount_();

    delay(250);






}
