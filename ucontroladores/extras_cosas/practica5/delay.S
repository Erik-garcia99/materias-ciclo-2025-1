

#include <avr/io.h>


.section .text

.global delay_103us
.global delay
.global delay1S

;erik garcia chavez 01275863
;practica 4 arhivo de retardos
; ucontroladores
;2025-1



;primero debo revisar 2 condiciones, con delay, si el delay que se
;quiere hacer es 0, en ese caso se manda directo




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

		sbiw r24, 1 ; 2

		cpi r24, 0x01 ;1
		brne ciclo ; 2 - 1

		cpi r24,0x01
		breq un_mSeg ; se cumple siempre



final:
	; agrego un nop porque al final

	ret



un_mSeg:

; este durara un segundo contando todos los ciclos hasta antes de el
; y contando en regreso de este y para C


	; que es lo que traigo yo atras

	;7 ciclos de C
	; jmp -> 3 ciclos

	;usaremos de igual r16-r17-r18


	ldi r16,2 ; 1
	ciclo0_1mS:
		ldi r17,11 ; 1x
		nop ; 1x

		ciclo1_1mS:
			ldi r18, 241 ; 1xy

			ciclo2_1mS:
				dec r18 ; 1xyz
				brne ciclo2_1mS ; xy(2z-1)

			dec r17 ; 1xy
			brne ciclo1_1mS ;x(2y-1)

		dec r16 ; 1x
		brne ciclo0_1mS ; (2x-1)

	jmp final





delay_general:


	;ciclos anteriores 5 - 5
	;usaremos r16-r17-r18

	; este no va a durar exactamento delo 16,000 porque va a contar
	;en su ciclo las comparaciones fuera y al parecer tan solo tendra
	;en el ultimo que le faltaria 1 de la comparacion que no es correcta

	ldi r16, 115 ; 1

	ciclo_0:
		ldi r17, 1 ; 1x
		nop

		ciclo_1:

			ldi r18, 44 ; 1xy
			ciclo_2:
				dec r18 ; 1zxy
				brne ciclo_2 ; xy(2z-1)

			dec r17 ; 1xy
			brne ciclo_1 ;x(2y-1)

		dec r16 ; 1x
		brne ciclo_0 ;(2x-1)
	ret












