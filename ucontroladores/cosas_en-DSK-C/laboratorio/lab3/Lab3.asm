;------------- definiciones e includes ------------------------------


.INCLUDE "m2560def.inc" ; Incluir definiciones de Registros para 2560

.equ INIT_VALUE = 0 ; Valor inicial R24

;------------- inicializar ------------------------------------------
ldi R24,INIT_VALUE
;------------- implementar ------------------------------------------
;call delay1uS
;call delay1mS
;call delay1S
;call myRand ; Retorna valor en R25
;------------- ciclo principal --------------------------------------
;garcia chavez erik 01275863
;practica 3 microcontroladores 
;delay por software

in r25, TCNT0
call numero_random
nop

rcall retardo_1S
nop

rcall retardo_103uS
nop


rcall retardo_1mS
nop










arriba: inc R24
        cpi R24,10
        breq abajo
        out PORTA,R24
        rjmp arriba


abajo: dec R24
        cpi R24,0
        breq arriba
        out PORTA,R24
        rjmp abajo






retardo_103uS: 

	;4 ciclos rcall
	clr r20 ;1
	clr r21 ; 1
	clr r22	;1 


	ldi r22,5 ;-> 1 -> x
	ldi r21, 7
	;--------
	;5 ciclos total
nxt0:
	nop ; ->1x
	nop; 1x
	nop ;1x
	ldi r20, 8 ;1x
	;-------
	;4x
	
nxt1:
	dec r21 ;1zyx
	nop ;1zyx
	nop ;1xyz
	brne nxt1 ; xy(2z-1)
	;----------
	;3xyz + xy(2z-1)
	
nxt2:
	ldi r21,7 ;-> 1xy
	nop ; 1xy
	nop ;1xy
	dec r20	;-> 1xy
	brne nxt1 ; x(2y-1)
	;---------------------
	;4xy + x(2y-1)

	dec r22 ;x -> 1x
	nop ; 1x
	brne nxt0 ;-> (2x-1)

	ret ;5 ciclos

	;-------------
	;2x+(2x-1)+5

	;--------------------------
	;7x + 5xy + 5xyz + 13 



retardo_1mS:

	; rcall -> 4 ciclos

	; para un retardo de 1mS es necesario 16,000 ticks 
	; esto porque (1x10^-3)*(16x10^6)  -> 16,000 




	ldi r20, 15 ; 1
	nop
	
	

	nxt_mS: 

	; este es el ciclo superior a todos 
	ldi r21, 59 ; 1x 
	nop ;1x

		nxt1_mS:

		ldi r22, 5 ; 1xy

			nxt2_mS: 
	
			; este en el ciclo mas pequenio. en el ciclo 

			dec r22 ; 1xyz
			brne nxt2_mS ; xy(2z-1)

		dec r21 ;1xy
		brne nxt1_mS ; x(2y-1)

	dec r20 ; 1x
	brne nxt_mS ; 2x-1


	ret ; final del delay







retardo_1S:

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


numero_random:

	ldi r17, 0xA5 ; cargamos el multiplicador en r17

	mul r25, r17 ; multiplicamos el valor de r25 * r17, cuyos valores se van 
	; a R0 y R1
	ldi r17, 1 ; cargamose l incrementos 
	add r0, r17 ; sumaos este incremento al byte bajo 

	mov r25, r0 ; el byte bajo lo movemos a r25 para obtener ese valor aleatorio

ret




