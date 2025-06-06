#include <avr/interrupt.h>
#include <inttypes.h>

static volatile uint8_t SecFlag;

void Timer0_Ini ( void )
{
    TCNT0=0x06; /* Inicializar valor para el timer0 */
    TCCR0A=0x00; /* inicializa timer0 en modo 0 (normal) */
    /* Inicializar con fuente de osc. Int. */
    TCCR0B=0x03; /* con Prescalador 64 */
    TIMSK0=0x01; /* habilita interrupcion del Timer0 */
    sei(); /* habilita interrupciones (global) */
}

uint8_t Timer0_SecFlag ( void )
{
    if( SecFlag )
    {
        SecFlag=0;
        return 1;
    }
    else
    {
        return 0;
    }
}

ISR (TIMER0_OVF_vect)
{ /* TIMER0_OVF_vect */
    static uint16_t mSecCnt;
    TCNT0+=0x06; /* reinicializar Timer0 sin perder conteo */
    mSecCnt++; /* Incrementa contador de milisegundos */
    if( mSecCnt==1000 )
    {
        mSecCnt=0;
        SecFlag=1; /* Bandera de Segundos */
    }
}