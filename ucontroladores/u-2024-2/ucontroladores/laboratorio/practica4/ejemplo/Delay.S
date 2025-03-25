#define _SFR_ASM_COMPAT 1 
#define __SFR_OFFSET 0 
#include <avr/io.h> 

.global Delay 

Delay: push r24
		clr r24

	Rnxt0: push r24
			clr r24

	Rnxt1: nop 
			nop 
			nop 
			nop 
			nop 
			dec r24   
			brne Rnxt1  
			pop r24    
			dec r24
			brne Rnxt0
			pop r24
			ret
			
