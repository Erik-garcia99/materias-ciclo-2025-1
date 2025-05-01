#include <avr/io.h>
#include <stdint.h>

// Macros
#define SetBitPort(port, bit)  __asm__ __volatile__("sbi %0, %1" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))
#define ClrBitPort(port, bit)  __asm__ __volatile__("cbi %0, %1" : : "I" (_SFR_IO_ADDR(port)), "I" (bit))

#define TIME_WINDOW 300
#define SEED 10
#define BTN_PIN       PF7     // Boton en pin K0
#define SEND_PIN      PE1     // LED o salida en pin E1
#define SALIDA        PF6
#define NOT_PRESSED   0
#define SHORT_PRESSED 1
#define LONG_PRESSED  2

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
const uint8_t leds[] = {

    0x42,
    0x41,    
    0x44,
    0x42,
    0x41,
    0x44,
    0x48,
    0x44

};
const uint16_t DDRs[] = {
  0x43,
  0x43,
  0x46,
  0x46,
  0x45,
  0x45,
  0x4C,
  0x4C
  
};

void ledsWalkingZero(void);
void ledsRandomPattern(void);
void ledsOff(void);
void ledsLooseAlternating(void);
void ledsWinBlink(void);

// Prototypes
extern uint8_t myRand(uint8_t seed);
extern void    delay(uint16_t mseg);
void    InitPorts(void);
uint8_t check_Btn(void);
void    updateLeds(uint8_t gameState);

// Global variable
uint32_t millis;

int main(void)
{
    uint8_t  currentGameState = eGameRestart;
    uint16_t countdown = 0;
    uint16_t countup = 0;
    InitPorts();

    while(1)
    {    
        switch(check_Btn())
        {
            case eBtnShortPressed: currentGameState++;
                break;
            case eBtnLongPressed:  currentGameState = eGameRestart;
                break;
        }
        
        switch(currentGameState)
        {
            case eGameRestart:
            {
                countdown = (( myRand(SEED) + 1) * 10)%50;
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
                if ( (countdown + countup) > TIME_WINDOW)
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

void InitPorts(void) //Inicializacion requerida de los puertos utilizados en esta practica. 
                    //El pin PE1 debe dejarse en un nivel alto.
{   //Configurar PK0 como entrada y activar 
    DDRF &= ~(1 << BTN_PIN);    // Boton en PK0 como entrada. 0 = ENTRADA
    DDRF |= (1 << PF6);
    PORTF |= (1 << PF6);

    DDRE |= (1 << SEND_PIN); //Activar en PE1 como salida. 1 = SALIDA
    SetBitPort(PORTE,1);   //Poner en alto nivel al pin PE1. Usando la macro SetBirPort
}


uint8_t check_Btn(void)
{
    if ((PINF & (1 << BTN_PIN)))     // Si tenemos un 1 en BTN_PIN
    {
        uint16_t cont = 0;          // Contador para validar si es Largo o Corto
        delay(30);                  // Esperamos al rebote
        if (!(PINF & (1 << BTN_PIN))) // Si despues del rebote ya no esta presionado
        {
            return eBtnUndefined;     // No fue presionado realmente
        }
        while ((PINF & (1 << BTN_PIN))) // Mientras siga presionado
        {
            delay103uS();            // Esperamos 103 microsegundos
            cont++;                  // Contamos los ciclos
            if (cont >= 9708)        // Si superamos el tiempo para 1 segundo
            {
                while ((PINF & (1 << BTN_PIN))); // Esperamos que se suelte
                return eBtnLongPressed;
            }
        }// Si salir del while antes de llegar a 9708 ciclos, fue corto
        return eBtnShortPressed;
    }
    return eBtnUndefined; // Si nunca hubo un 1 en BTN_PIN
}

void updateLeds(uint8_t gameState)
{
    switch(gameState)
    {
        case eWaitForStart:
            fWaitForStart();       // Secuencia MSB a LSB
            break;

        case eStartCount:
            fStartCount();     // Aleatorio cada 100ms
            break;

        case eEndCount:
            fEndCount();               // Apaga todos
            break;

        case eYouLoose:
            fYouLoose();  // Nibble ON/OFF alternado
            break;

        case eYouWin:
            fYouWin();          // Todos ON/OFF simultaneo
            break;

        default:
            fEndCount();               // Seguridad por si acaso
            break;
    }
}

void fWaitForStart(void){
  for (uint8_t i = 0; i < 8; i++) {
  DDRF = DDRs[i];    // Configura direccion (sin desplazamiento)
  PORTF = leds[i];  // Establece valores LED (sin desplazamiento)
  delay(2);       // Retardo aumentado para mejor visualizacion
  }
}

void fStartCount(void)
{
    static uint8_t seed = SEED;
    // Genera nuevo numero aleatorio
    seed = myRand( rand() );
    // Asegura un indice va?lido entre 0 y 7
    uint8_t index = seed % 8;

    // Configura la direccion del puerto y el valor del LED
    DDRF = DDRs[index];
    PORTF = leds[index];
    // Espera 100 ms antes de cambiar al siguiente patron
    delay(100);
}

void fEndCount(void){
  DDRF = 0x40;
  PORTF = 0x40;
}

void fYouLoose(void){
  uint16_t counter;
    // Mostrar primeros 4 LEDs por 500 ms
    for (counter = 0; counter < 13; counter++) {
        for (uint8_t i = 0; i < 4; i++) {
            DDRF = DDRs[i];
            PORTF = leds[i];
            delay(2);  // Peque?o retardo entre cada LED
        }
    }

    // Mostrar altimos 4 LEDs por 500 ms
    for (counter = 0; counter < 13; counter++) {
        for (uint8_t i = 4; i < 8; i++) {
            DDRF = DDRs[i];
            PORTF = leds[i];
            delay(2);  // Peque?o retardo entre cada LED
        }
    }
}

void fYouWin(void){
          for (uint16_t conter = 0; conter <7; conter++){
                  for (uint8_t i = 0; i < 8; i++) {
                  DDRF = DDRs[i];    // Configura direccion (sin desplazamiento)
                  PORTF = leds[i];  // Establece valores LED (sin desplazamiento)
                  delay(2);       // Retardo aumentado para mejor visualizacion
                  };       // Retardo aumentado para mejor visualizacion
            }
          fEndCount();
          delay(250);
}


