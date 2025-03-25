;------------- definiciones e includes ------------------------------
;INCLUDE "m1280def.inc" ; Incluir definiciones de Registros para 1280

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

rcall retardo_1S
nop


rcall retardo1mS

nop

rcall miRetardo



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




;creo que es retardo de 103uS

miRetardo: 
	clr r20 ; R25 -> R20 
	clr r21 ; R26 -> R21
	clr r22	;1 ciclo -> R24 -> R22


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
	brne nxt1 ; 2xy(z-1)
	;----------
	;3xyz + 2xy(z-1)
	
nxt2:
	ldi r21,7 ;-> 1xy
	nop ; 1xy
	nop ;1xy
	dec r20	;-> 1xy
	brne nxt1 ; 2x(y-1)
	;---------------------
	;4xy + 2x(y-1)

	dec r22 ;x -> 1x
	nop ; 1x
	brne nxt0 ;-> 2(x-1)

	ret ;5 ciclos

	;-------------
	;2x+2(x-1)+5

	;--------------------------
	;7x + 4xy + 5xyz + 12 



retardo1mS:

	; rcall -> 4 ciclos

	; para un retardo de 1mS es necesario 16,000 ticks 
	; esto porque (1x10^-3)*(16x10^6)  -> 16,000 


; -> retardo con 3 ciclos, creo que con este sera mejor 

	;calculos: 

	; 4 + 5 + 4 + 1x+ 1xy+ 1xyz+ xy(2z-1) + 1xy + x(2y-1) + 1x+ (2x-1)




	ldi r20, 15 ; 1
	nop
	

	nxt_mS: 

	; este es el ciclo superior a todos 
	ldi r21, 59 ; 1x 
	nop

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


;utilizaremos igual 3 ciclos para facilitarnos el manejo de un numero tan grande de ciclos 

; 1S = 16,000,000 de ciclos 

	; de primera instancia tenemos 4 y 5 ciclos de entrada y salida, tendremos 1 para el ciclo superior


	; 4 + 5 + 1




	ldi r20, 199 ; 1 

	nxt_S:

	ldi r21, 200 ; 1x

	
	nxt2_S:

	ldi r22, 133 ; 1xy


	nxt3_S: 
	dec r22 ;1xyz

	brne nxt3_S ;xy(2z-1)

	dec r21 ; 1xy
	brne nxt2_S ; x(2y-1)

	dec r20 ; 1x
	brne nxt_S ; 2x-1



	ret













