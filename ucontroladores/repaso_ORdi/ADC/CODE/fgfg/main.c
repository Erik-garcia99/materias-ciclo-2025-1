/*
 */

#include <avr/io.h>

int main(void)
{

    DDRB |=0xFF;
    PORTB |=0xFF;
    while(1)
    ;

    return 0;
}
