#include <avr/io.h>

// PC0 out

// PA0 in

int main(void)
{

    DDRC |= (1 << PC0); // CONTROLA SI ES DE ENTRADA O DE SALIDA
                        //  PORTC |=(1<<PC0);

    DDRA &= ~(1 << PA0); // buttom como entrada
    uint8_t buttom_press = !(PINA & (1 << PA0))

                               while (1)
    {

        if (buttom_press)
        {
            // EL CIRCUITO ESTA EN SU FORMA PULL UP, POR LO QUE SI APARECE UN 0 SE PRENDE EL LED

            PORTC |= (1 << PC0);
        }

        else
        {
            // LED APAGADO
            PORTC &= ~(1 << PC0);
        }
    }
}