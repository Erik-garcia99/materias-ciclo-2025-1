#include <avr/io.h>

// Macros
#define SetBitPort(port, bit) __asm__ ( ?? )
#define ClrBitPort(port, bit) __asm__ ( ?? )

#define TIME_WINDOW 300
#define SEED xx

enum ButtonStates
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
}

enum GameStates
{
    eGameRestart = 0,
    eWaitForStart
    eStartCount,
    eEndCount,
    eGameOver,
    eYouWin,
    eYouLoose
}

// Prototypes
extern uint8_t myRand(uint8_t seed);
extern void    delay(uint16_t mseg);
void    InitPorts(void);
uint8_t checkBtn(void);
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
                countdown = (myRand(SEED)+1)*20;
                countup = 0;
                currentGameState++;
                break;
            case eWaitForStart:
                break;
            case eStartCount:
                countdown--;
                if (countdown == 0)
                currentGameState++;
                break;
            case eEndCount:
                if (countdown != 0)
                currentGameState++;
                else
                countup++;
                break;
            case eGameOver:
                if ((countdown+countup)>TIME_WINDOW)
                currentGameState = eYouLoose;
                else 
                currentGameState = eYouWin;
                break;
        }
        
        updateLeds(currentGameState);
        delay(1);
        millis++;
    }
}
