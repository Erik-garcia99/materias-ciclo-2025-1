ldi r24,0x05
call myRand
nop



myRand:

	ldi R16, 0xA5      ; Multiplicador (a = 165)
    mul R24, R16       ; Multiplica semilla por 'a' (R1:R0 = R24 * R16)
    ldi R17, 0x4D      ; Constante aditiva (c = 77)
    add R0, R17        ; Suma constante al byte bajo del resultado
    mov R24, R0        ; Mueve resultado a registro de retorno
    clr R1             ; Limpia registro alto (requerido por convención AVR-GCC)
    ret
	

