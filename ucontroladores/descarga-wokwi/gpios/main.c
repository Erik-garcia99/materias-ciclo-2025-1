#include<avr/io.h>

#define inc_buttom PC0
#define dec_buttom PC1


uint8_t valueDisplay[] =
{
   0X03,0X9F,0X25,0X0D,0X99,
   0X49,0X41,0X1F,0X01,0X19
};

void display(uint8_t value){

	PORTA= valueDisplay[value];
}


uint8_t is_buttom_press(uint8_t pin){

	//PC0

	if(!(PINC & (1 << pin))){
		return 1;
	}
	return 0;
}

void delya(){

	uint16_t counter = 0xFFFF;

	while(counter > 0 ){
		counter--;
	}


}
int main(void){


	DDRA=0xFF; // todo el puerto A es de salida.  PA0 - PA7 
	
	PORTA=0XFF;

	DDRC &= ~( 3 << PC0);

	display(6);
	uint8_t count= 0;
	while(1){
		if(is_buttom_press(inc_buttom)){
			count++;
		}
		else if(is_buttom_press(dec_buttom)){
			count--;
		}
		delay();
		display(count%10);

	}


}


