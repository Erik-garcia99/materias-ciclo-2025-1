
#include <avr/io.h>


.global delay_103us





delay_103us:


	nop
	nop


	ldi r22,5
	ldi r21, 7

nxt0:
	nop
	nop
	nop
	ldi r20, 8


nxt1:
	dec r21
	nop
	nop
	brne nxt1

nxt2:
	ldi r21,7
	nop
	nop
	dec r20
	brne nxt1

	dec r22
	nop
	brne nxt0

	ret
