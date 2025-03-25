#define _SFR_ASM_COMPAT 1 
#define __SFR_OFFSET 0 
#include <avr/io.h> 

;garcia chavez erik 01275863
;practica 4 
;ucontroladores 

;r24 contiene el primero arguemto en la funcion en C. 

.global Delay 

Delay: 
		

		;antes que nada debo comprobar si el argumento es igual a cero, si es asi, debe volver
		;por lo que este es el unico que no va a dar exactamnte 0ms, tal solo vuelve
		cpi r24,0x00
		breq final		


		cpi r24,0x01
		breq un_mSeg


ciclo: 

	;rcall delay_general ;4 ciclos
	rcall delay_general
	
	dec r24

	cpi r24,0x01
	brne ciclo ; pro esto hacer que el delay de 1ms cuente con los 2 ciclos que este se va tardar en ir
	;cuando este no se cumpla estara a 15999 
	
	cpi r24,0x01
	breq un_mSeg

final:
		ret

		

un_mSeg:

	ldi r23,27 ;1 -> X
	ldi r22,117  ;1  -> Y
    
nxt0:
	nop ; 1XY
	nop ;1XY
    dec r22 ;1XY
    brne nxt0 ;2X(Y-1)
	dec r23  ;1X        
	nop ;1x
	nop ;1x
	brne nxt1 ;1;2(x-1) 
	rjmp final ;4   

nxt1:
    ldi r22, 117 ;1X    
    rjmp nxt0  ;4X

 


delay_general:

    ldi r18, 26    ;1 ->X
    ldi r19, 151    ;1 -> Y
Rnxt0:
	nop ;1XY
    dec r19 ;1XY 
    brne Rnxt0 ;2X(Y-1)  
    dec r18  ;1X   
    nop;1X
	nop;1X
	nop;1X
	nop;1X
	nop;1X
	brne recarga ;2(X-1) 
	ret              ;5

recarga:
	
    ldi r19, 151   ;1X
	nop;1X
    rjmp Rnxt0 ;4X 


