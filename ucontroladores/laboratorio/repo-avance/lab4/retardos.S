
#include <avr/io.h> 
.section .text

.global delay_103us
.global delay
.global delay1S





;primero debo revisar 2 condiciones, con delay, si el delay que se 
;quiere hacer es 0, en ese caso se manda directo




	






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


delay1S:

ldi r20, 241;	
	nop				;1 	
		nxt_S: ldi r21, 144;  		/x
		nop;						/x
		nop;						/x
		nop;						/x
			nxt2_S:ldi r22, 152 ; 	/xy
			nop;					/xy
			nop;					/xy
				nxt3_S:dec r22; 	/xyz
				brne nxt3_S;	 	/xy(2z-1)
			dec r21; 				/xy
			brne nxt2_S;	 		/x(2y-1)
		dec r20;	 				/x
		brne nxt_S;		 			/2x-1


	ret




delay:

	; en este tendre que hacer muchos descuentos por esto de
	;cunado sea 0 y  lo demas 

	cpi r24, 0x00 ; 1 
	breq final ;2 - 1

	cpi r24, 0x01
	breq un_mSeg; si r24 es 1 ms entonces ira a una subrutina la cual durara 
	   ;1ms constnado la cunado vino de C y el regreso 
	   ; al igual que sera la etiqueta final cunado regrese 
	   ; para un ms >  1


	;en caso que sea  un delay >1 deberia de ir a una fuion que calcule
	;exactamenteo 1ms sin importar nada fuera de el, esa funcion cada ciclo
	;dira 1ms segun lo que se mando por parametro 


	ciclo:

		call delay_general

		sbiw r24, 1

		cpi r24, 0x01
		brne ciclo

		cpi r24,0x01 
		breq un_mSeg ; se cumple siempre



final:
	; agrego un nop porque al final
	nop
	ret








un_mSeg:

; este durara un segundo contando todos los ciclos hasta antes de el 
; y contando en regreso de este y para C
	

	

delay_general:


	;ciclos anteriores 5 - 5 
	;usaremos r16-r17-r18
	
	ldi r16, X

	next_0-DG:
		ldi r17, Y

		nxt_1-DG:

			ldi r18, Z
			nxt_2-DG:
				dec r18
				brne nxt_2-DG

			dec r17
			brne nxt_1-DG

		dec r16
		brne nxt_0-DG
	ret




		







