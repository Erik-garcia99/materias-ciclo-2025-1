call delay1S
nop

delay1S:
ldi R24,241;				//1
nxt:ldi R25,187;			//1x
	nop						//1x
	nop						//1x
	nxt2:ldi R26,70;		//1x*1y
		nop					//1x*1y
		nop					//1x*1y
		nxt3:dec R26;	//1x*1y*1z
			nop			//1x*1y*1z
			nop			//1x*1y*1z
			brne nxt3;	//(2z-1)xy
		dec R25;		//1x*1y
		brne nxt2;		//(2y-1)x
	dec R24;			//1x
	brne nxt;			//2x-1
	ret	
					//+10
