.INCLUDE "m2560def.inc"
.equ INIT_VALUE=0

	ldi r24, INIT_VALUE

arriba:
	inc r24
	cpi r24,10
	breq abajo 
	rjmp arriba

abajo:
	dec r24
	cpi r24,0
	breq arriba
	rjmp abajo
