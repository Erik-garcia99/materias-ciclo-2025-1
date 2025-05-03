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
    0x82, // LD1 con el 4 se esta mandando 1 en todo momneto por el pin 6 para su uso
    0x81, // LD2
    0x84, // LD3
    0x82, // LD4
    0x81, // LD5
    0x84, // LD6
    0x88, // LD7
    0x84  // LD18

};



// Prototypes
extern uint8_t myRand(uint8_t seed); //esperemos que funcione, ya vreremos
extern void delay(uint16_t mseg);
extern void delay_103us();
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

        check_Btn();

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



    aqui debemos de cambiar algunas cosas, priemro los puertos como salida o entrada siguen siendo los mismos

    pero

    PF7 se activa el pull up interno, por lo que ese puerto esta en 1 siempre

    PF6 se activa como salida y que saque 0 no 1, esto para cunado se haga click haga corto con PF7 y se baje a 0

    /*
     0xxx xxxx

     1000 0000
     ~
     0111 1111 -> dejo pasar a los 1 y los ceros de vuelven ceros, se mantienen
    */

    //PF7 como entrada - entrada es 0
    DDRF &= ~(1<<PF7); //configruacmos pf7 como entrada
    PORTF |= (1<<PF7); // activamos PULL-UP

    DDRF |=(1<<PF6); //activamos a PF6 como salida
    PORTF &=~(1<<PF6); //sacamos el 0





}


//el check_btn  sera un poquito diferente
uint8_t check_Btn(){


    if(PINF & (1<<BTN_PIN)){
    //que me esta diciendo essto, que, como esta siempre leyendo 1 por lo decimos
    //si hay 1 en ese pin quiere decri que no esta precionado por lo que no sabemos que hay

        return eBtnUndefined;
    }

    delay(30); // un delay para un antirebote por software

    if(PINF & (1<<BTN_PIN)) return eBtnUndefined; // hacemos lo mismos

    //delay(1);
    uint16_t tiempoPrecionado = 0;




    if(!(PINF & (1<<BTN_PIN))){
    //este me quiere decir lo contrario, quiere decir que si no esta precionado me manda un 1 por loq ue aqui lo inverito
    //lo que seria un 0, por lo que no entraira

    //pero si es 0 que se esta precionando es 1 lo que quiere decir qeu se esta precionando

        while(!(PINF & (1<<BTN_PIN))){

            delay_103us();
            tiempoPrecionado++;

            if(tiempoPrecionado >= 9708){

                if(PINF & (1<<BTN_PIN)){
                    return eBtnLongPressed;
                }
            }

        }
        return eBtnShortPressed;

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
