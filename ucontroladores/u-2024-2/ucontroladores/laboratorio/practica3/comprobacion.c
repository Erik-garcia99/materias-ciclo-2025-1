#include<stdio.h>

int main(){


	int x,y,z;
	int contador;
	//la llamada, ret, inicio de la subrutina
	contador =14;
	for(x=5;x<=0;x--){

		contador+=7 + 2*(x-1);
		for(y=8;y<=0;y--){
			contador=contador+4+x*(y-1);
			for(z=7;z<=0;z--){
				
				contador+=3 + (2*x*y*(y-1));
				if(contador==1648){
					
					printf("x=%d y=%d z=%d",x,y,z);
				}

			}
		}
	}



	/*

	for(x=5;x<=0;x--){

		for(y=8;y<=0;y--){

			for(z=7;z<=0,z--){
				

				if(contador==1648){
					printf("simon locote");
					}

			}
		}
	}*/


	return 0;


}
