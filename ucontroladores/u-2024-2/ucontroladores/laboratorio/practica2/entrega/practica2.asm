.INCLUDE "m2560def.inc"

;erik garcia chavez erik 
;01275863
;laboratorio uControladores

;practica de laboratorio No.2



main:
;variable de control que eligira los registos 
	ldi r24,0x01
	ldi r16,0x01 ;usare r16 para hacer la comparacion con r24

;se comprara r24 con r16 lo cual es una resta si son iguales, r16 siempre valdra 1, r24 es el que va a 
;estar cambiando por lo que se hace la resta, si ambos valores son iguales quiere decir que es uno, 
;pero si no quiere decri que es 0. por lo que son diferentes y va a uno u a al otro
	cp r24,r16
	breq es_uno

	cp r24,r16
	brne es_cero

	jmp main

termina_programa:
;para otro valor, no hace nada
nop
	

	
;si r24 es igual a 1 ingresa a esta subrutina, la cual es intercambia entre r31,r30,r28,r28
es_uno:
	
	ldi r31,0x1F
	ldi r30,0x1E
	ldi r29,0x1D
	ldi r28,0xF8
	
	;variable que controla el ciclo
	ldi r20,$08

	;****************************************************
	;para poder hacer el rotamiento de estos registros, usamoremos registros de respaldo
	;los cuales nos vana  ayudar a una vez que el registro este volteado poder moverlo al registro destino
	;en la preactica nos pedia que en este caso como R24 es uno 
	;que los registros R31-R30-R29-R28 se volteasen, lo que yo vi es que R31 y R28 se cambiaran entre si 
	;y R30 y R29 lo haran entre si, por lo que podemos emepzar haciendo etiquetas donde se modifican entre esos 2, yo 
	;al principio estaba intentando hacerlo entre si mismos, sin usar registros extras, para usar lo minimo la memoria,pero
	;no puede hacerlo al momento de que este el primer 1 en carry cunado este ingresa a un registro cambia por total, por 
	;ejemplo r31 terminaba en 0 y r28 si terminadab en F8, por loq ue tive que usar registros adicionesles, estos 
	;caputraran los bits al reves de los registros de R31-R28, una vez que se hayan eso eso, los movemos a sus registros 
	;respectivos en este caso  R17 contiene al reves R31 por lo que va a R28 
	;R18 contiene al reves R28 por lo que va a R31, lo mismo se haria con los demas solo que puede que use otros registros pero
	;la logica es la misma. 
	;****************************************************

	;r12 y r13 que tienen una copia del valor origial este no lo vamos a modificar, modificaremos los originales.
	
	mov r12,r31
	mov r13,r28

	mov r11,r30
	mov r10,r29

	;registros de apoyo.
	ldi r17,0x00
	ldi r18,0x00
	;para los regitstros r30 y r29
	ldi r19,0x00
	ldi r22,0x00

	clc ;tan solo limpiamos carry

;ciclo que voleta a r12 y lo guarda en r17
parte_externa_31:
	;r12=r31

	rol r12
	ror r17

	dec r20
	cpi r20,0x00

	brne parte_externa_31
	
	;volvemos a cargar el iterador
	ldi r20,0x08

;cilo que voltea a r28 y lo guarda en r18
	clc


parte_externa_28:

	rol r13
	ror r18

	dec r20
	cpi r20,0x00

	brne parte_externa_28
	

	ldi r20,0x08

	clc ;tan solo limpiamos carry

parte_centro_30:

	rol r11 ;r30
	ror r19 ;r30 al reves 

	dec r20
	cpi r20,0x00

	brne parte_centro_30
	
	ldi r20,0x00

	clc

	ldi r20,0x08

parte_centro_29:


	rol r10
	ror r22	

	dec r20
	brne parte_centro_29

comprobacion_extremos:

	;esta etiqueta de lo que se va a encargar va a ser de comparar si la version de los registros volteados es igual a su
	;version original, me explico 

	;tenemos por ejemplo en R31= 1100 0110 y en R28=0110 0011, si volteamos a R31 tenemos R31-> 0110 0011 que es lo mismo 
	;qie hay en r28 por lo que no tiene caso hacer ese intercambio y se cumplira de la misma manera para r28 a r31, porque 
	;si se voltea lo que hay en r28 tenemos r28-> 1100 0110 que es lo mismo que hay en R31, por lo que esto lo unico que hara
	;es gastar la memoria, por lo que no lo haremos ya que queda igual

	;r17 tiene lo que tiene R31 pero al reves, lo que se va a hacer es una resta por ejemplo si R28 tiene el 0110 0011
	;Y R17 al final tambien iene 0110 0011

	; 0110 0011
	; 0110 0011

	;haciendo la resta obtenermos 0, cp hare una resta pero sin modificar los registros, 
	;
	;r28 -> F8 r28=r17 ->F8
	cp r28, r17
	
	;si es diferente a 0, osea que no son iguales entonces se hace el intercambio
	brne intercambio_extremos


	;si el resultado es 0, quere decri que son iguales por lo que no hay intercambio
	breq comparacion_medio

	
comparacion_medio:

	;la logica es la misma. 

	cp r29, r19

	brne intercambio_medio


	ret

intercambio_extremos:

	;aqui se hace el intercambio
	mov r28,r17
	mov r31,r18
	
	jmp comparacion_medio

intercambio_medio:
	mov r30,r22
	mov r29,r19

	
	ret




;*****************************************************************************
es_cero:

	;el procedimiento para esta aprte es practicamente el mismo.
	
	ldi r29,0x12
	ldi r28,0xE
	ldi r27,0x21
	ldi r26,0x11 

	;variable que controla el ciclo
	ldi r20,0x08

	;registros con los que vamso a trabajr, los que vamos a rotar, en caso de que sean iguales al rotar no se hace elcambio
	;la logica es igual que el cunado r24 es igual a 1.
	mov r11,r29
	mov r12,r26

	mov r13,r28
	mov r14,r27

	;registros que van a tener los otros pero al reves
	ldi r17,0x00
	ldi r18,0x00

	ldi r19,0x00
	ldi r21,0x00





parte_extremo_29: 

	rol r11 ;r29
	ror r17

	dec r20
	cpi r20,0x00

	brne parte_extremo_29


	ldi r20,0x08


parte_extremo_26:

	rol r12 ;r26
	ror r18

	dec r20
	cpi r20,0x00
		
	brne parte_extremo_26

	
	ldi r20,0x08

parte_medio_28:

	rol r13 ;r28
	ror r19

	dec r20
	cpi r20,0x00

	brne parte_medio_28
	
	
	ldi r20,0x08
	
	
parte_medio_27:


	rol r14 ;r27 
	ror r21

	dec r20

	cpi r20,0x00

	brne parte_medio_27




comparacion_extremos:

	cp r26,r17
	
	;si son diferentes entonces si se intercambian en caso que sea 0 el resultado no se cambian
	brne intercambio_extremos_0

	breq comparacion_medios


comparacion_medios:

	cp r27,r19

	brne intercambio_medios

	jmp main



intercambio_extremos_0:

	mov r29,r18
	mov r26,r17

	jmp comparacion_medios


intercambio_medios:


	mov r28,r21
	mov r27,r19


	ret

