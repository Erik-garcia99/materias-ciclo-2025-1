
#include<avr/io.h>


extern void delay(uint16_t delay);

int main(void){

	delay(1000);

	__asm__("nop");



}
