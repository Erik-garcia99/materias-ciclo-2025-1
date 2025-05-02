;------------- definiciones e includes ------------------------------ 
.INCLUDE "m2560def.inc" ; Incluir definiciones de Registros para 1280 
.equ INIT_VALUE  = 0 
; Valor inicial R24 
;------------- inicializar ------------------------------------------ 
ldi R24,INIT_VALUE 
;------------- ciclo principal -------------------------------------- 
