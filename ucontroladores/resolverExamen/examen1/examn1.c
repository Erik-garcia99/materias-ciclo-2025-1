/*
que es un uContorlador 

un IC con con memoria, puertas y un procesador, 

5D la direccion de PC 


*/


//limpiar todos los registros 

//lo pdomeos hacer con el registro indirecto, 

/*
Z -> 29

st -Z, R
*/

/*
AMD 

es 30 porque es prececremento, entonces indirectamnete vamos a ir a las posiciones de memoria de cada reigsotr 
LDI r30,30
LDI R31,0

-> DESPUES VAMOS 
loop
	st -Z,R
	brne loop
 

*/