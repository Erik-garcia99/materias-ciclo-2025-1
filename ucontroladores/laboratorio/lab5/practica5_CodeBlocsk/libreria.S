#include <avr/io.h> 
.section .text

.global delay
.global delay103uS
.global delay1mS
.global myRand       ; Hacemos visible esta funci�n al compilador de C

myRand:
    ; Entrada: seed en r24
    ; Salida: n�mero pseudoaleatorio en r24

    mov r16, r24      ; r16 = seed

    ; Multiplicar X por 5: (X << 2) + X
    mov r18, r16      ; r18 = copia de X
    lsl r16           ; X * 2
    lsl r16           ; X * 4
    add r16, r18      ; X * 5

    ; Sumar 1
    ldi r18, 1
    add r16, r18      ; 5*X + 1 (mod 256 autom�ticamente)

    ; Devolver resultado en r24
    mov r24, r16

    ret



delay:
	cpi r24,0   			//1
	breq final				//2-1

	cpi r24,1				//1
	breq delay1miliS		//2-1

	loop:
		call delay1mS
		sbiw r24,1
	
		cpi r24,1				//1
		brsh loop				//	Salta si r24 >= X (Mayor o igual) //1

	final:
ret




delay1mS:    
	ldi R25,130;					//1
	nxt3:ldi R26,10;			//1x
		nxt4:ldi R27,3;		//1x*1y
			nxt5:dec R27;		//1x*1y*1z
				brne nxt5;		//(2z-1)xy
			dec R26;			//1x*1y
			brne nxt4;			//(2y-1)x
		dec R25;				//1x
		brne nxt3;				//2x-1
	ret

delay1miliS:    
	ldi R25,130;				//1
	nxt6:ldi R26,10;			//1x
		nxt7:ldi R27,3;		//1x*1y
			nxt8:dec R27;	//1x*1y*1z
				brne nxt8;	//(2z-1)xy
			dec R26;		//1x*1y
			brne nxt7;		//(2y-1)x
		dec R25;			//1x
		brne nxt6;			//2x-1
	jmp final


delay103uS:
	ldi R25,7;				//1
	nxt:ldi R26,77;			//1n
		nxt2:dec R26;		//1m*1n
			brne nxt2;		//(2m-1)*n
		dec R25;			//1n
		brne nxt;			//2n-1
	ret

