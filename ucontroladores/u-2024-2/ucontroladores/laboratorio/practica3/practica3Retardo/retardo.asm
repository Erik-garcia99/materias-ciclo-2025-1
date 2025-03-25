.INCLUDE "m2560def.inc"
;---------------------------------

;garcia chavez erik 
;01275863
;laboratorio microControladores
;ciclo 2024-2


;----------------------------------

sbi DDRE, PE1 ;PE1 como salida

sbi PORTE, PE1 ;escribe en 1 PE1

clr r16

sts UCSR0B, r16






;principal
next: 

	sbi PORTB, 7 ; escribe 1 en PB7
	rcall miRetardo

	cbi PORTB, 7 ;escribe 0 en PB7

	rcall miRetardo

	;prestar bits 

	ldi r18, 0x55
	rcall prestarBits
	ldi r18, 0x41
	rcall prestarBits
	ldi r18,0x42
	rcall prestarBits
	ldi r18, 0x43
	rcall prestarBits
	rjmp next



;rcall miRetardo ;4 ciclos





miRetardo: 
	clr r25
	clr r26
	clr r24	;1 ciclo
	ldi r24,5 ;-> 1 -> x
	ldi r26, 7
	;--------
	;5 ciclos total
nxt0:
	nop ; ->1x
	nop; 1x
	nop ;1x
	ldi r25, 8 ;1x
	;-------
	;4x
	
nxt1:
	dec r26 ;1zyx
	nop ;1zyx
	nop ;1xyz
	brne nxt1 ; 2xy(z-1)
	;----------
	;3xyz + 2xy(z-1)
	
nxt2:
	ldi r26,7 ;-> 1xy
	nop ; 1xy
	nop ;1xy
	dec r25	;-> 1xy
	brne nxt1 ; 2x(y-1)
	;---------------------
	;4xy + 2x(y-1)

	dec r24 ;x -> 1x
	nop ; 1x
	brne nxt0 ;-> 2(x-1)

	ret ;5 ciclos

	;-------------
	;2x+2(x-1)+5

	;--------------------------
	;7x + 4xy + 5xyz + 12 





prestarBits:

	;-------------------------------------------------------------------

	;el registro DDRx se utiliza para configurar un pin como entrada o salida. Seguido, 
	;esta el registro PORTxn que habilita con 5v o deshabilita con 0v. Luego, tenemos el registro PINxn 
	;que sirve para leer el estado un pin configurado previamente como entrada.


	;DDRX: Escribiendo en cada bit (“1” lógico = Salida | “0” lógico = Entrada).
	;PORTX: Escribiendo en cada bit la salida tendrá un nivel de Voltaje (1=5v|0=0v).
	;PINX: Este registro es de solo Lectura (5v =”1” lógico | 0v =“0” lógico).
	;-----------------------------------------------------------------


	ldi r19,0x08
	cbi PORTE,1
	;poner en 0 logico a PE1, el bit 1 del puerto E 
ciclo:

	nop
	rcall miRetardo ;espera 103 uSegundos
	
	;pasar el LSB de r24 a carry, por el MSB entra  un 0
	ror r18
	
	brcs carrySet ;<- hace un test con la bandera de acarreo si la bandera esta encensida ponemos PE1 en SET
	
	brcc carryClear


segundoPaso:

	rcall miRetardo

	dec r19

	cpi r19,0x00

	brne ciclo
	
	nop
	sbi PORTE,PE1

	rcall miRetardo


	ret 



;rutinas de apoyo
carrySet:


	sbi PORTE,PE1

	jmp segundoPaso


carryClear:

	cbi PORTE,PE1

	jmp segundoPaso

	
