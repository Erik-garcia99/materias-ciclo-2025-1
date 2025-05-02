#include<avr/io.h>
.section .text:

.global delay
.global main
;funcion que es para un delay general de 0 - 65,535 ms ~= 1 S

delay:

	cpi r24, 0x00 ; 1
	breq final ;2 - 1

	cpi r24, 0x01
	breq un_mSeg


	ciclo:

		call delay_general

		sbiw r24, 1 ; 2

		cpi r24, 0x01 ;1
		brne ciclo ; 2 - 1

		cpi r24,0x01
		breq un_mSeg



final:

	ret




un_mSeg:


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






