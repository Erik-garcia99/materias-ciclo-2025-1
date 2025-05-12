#include<stdio.h>



void itoa(int num,char *cad,int base);




int main(void){



	char *str;
	char apoyo;
	int i=0;
	int numero;

	printf("ingrese el numero");
	scanf("%d",&numero);

	itoa(numero,str,16);

	printf("HEX:");
	while(*str!='\0'){
		//para imprimir la cadena 

		printf("%c",*str++);
		__asm__("nop");

	}



}



void itoa(int num,char *cad, int base){



	char HEX[] ={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	
	//vermos como podemos hacerlo, porque esto es mas C que coas de micros 

	int temp = num; //esta variabel toma el valor numerico que se nos envio

	//como sabemos la imprecion debe hacerse al revez de como lo hacemos 
	//necisto la cantidad de elementos de ese arreglo, 

	int items_HEX = 0;

	int num_arreglo = num;

	while(num_arreglo>0){
		//aqui hay algo, no se si seran solos mayores a cero o si despues tengo que agregar otro para el caracter nulo, por lo que eremos en el debbug 

		num_arreglo/=16;
		items_HEX++;

	}
	//items_HEX++;

	char _HEX_ASCII_[items_HEX]; //ya vreemos si se quda asi o si aumentamos en 1 

	//necesito algo que me de el final de la pila 

	int _stack_point= items_HEX-1; //le pasamos el final del arreglo 

	int residuo=temp;
	// int numero = num
	while(temp > 0){
		//aqui ya el el precos 
		residuo= temp%16;

		_HEX_ASCII_[_stack_point]=residuo; //gurda el valor ascii, mas bien la posicion en donde se enceuntra su valor dentro del arreglo 
		_stack_point--;
		temp/=16;
	}

	//_HEX_ASCII_[_stack_point]='\0'; // si se le agrega uno y agregamos el caracter nulo 

	int i=0;
	int j=0;

	while(j<items_HEX)
	{
		
		cad[i]=HEX[_HEX_ASCII_[j]];
		i++;
		j++;
	}
	

	cad[i]= '\0'; //al final del la cadena agregamos el caracter nulo 



}
