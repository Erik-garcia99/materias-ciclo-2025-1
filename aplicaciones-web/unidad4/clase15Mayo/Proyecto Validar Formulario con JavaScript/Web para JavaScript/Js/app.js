//alert("hola mundo desde Js") //muestra mensaje por el navegador 

//varibales 

//const heading = document.querySelector('h2') // regresa un elemento de HTML 

// const heading = document.querySelector('.header__texto h2') // para clases, con esto podemos cambiar elemento dentro del docuemnto 


//const heading = document.querySelector('#heading h2')

//console.log(heading)

//heading = document.querySelector('.header__texto h2') //podemos obterne clases o ID's

//validar el formulario 

/*
const heading = document.querySelector('#heading');

//funciones para 

heading.textContent = 'nuevoHeading'; //para cambiar el ID

heading.classList.add('nueva-clase') //cambiamos la clase 

console.log(heading)
*/


//cambiamos la navegacion 

/*
const enlaces =  document.querySelectorAll('.navegacion a'); //selecciona todos los elemtnos de navegacion
enlaces[0].textContent ='Nuevo enlace' //cambiamos el texto de la pagina
//console.log(enlaces)
enlaces[0].classList.add('nuevaClase') //agregamos una nueva clase 
enlaces[0].classList.remove('navegacion__enlace')*/

//enlaces[0].href = 'https://google.com' //podemos aplicarle una direccion 


/*const heading= document.getElementById('heading')
console.log(heading)*/


/*
const nuevoEnlace =  document.createElement('A')
nuevoEnlace.href = 'nuevo-enlace.html'
nuevoEnlace.textContent = 'otro Menu'
nuevoEnlace.classList.add('navegacin__enlace')


const navegacion = document.querySelector('.navegacion')
navegacion.appendChild(nuevoEnlace)

console.log(nuevoEnlace)


//eventos en JS

console.log(1)

window.addEventListener('load', function(){

    console.log(2);


});

window.onload= function(){
    console.log(3);
};

console.log(4)


document.addEventListener('DOMContentLoaded', function(){


    console.log(5)

});

console.log(6)

*/


/*
window.onscroll= function(){
    console.log("scrolling...") //detecta cunado se detecta el scroll
}*/
//validacion de formulario

//eventos de lso inputs 
// const nombre = document.querySelector('#nombre')
// const email = document.querySelector('#email')
// const mensaje = document.querySelector('#mensaje')






// nombre.addEventListener('input',function(e){

//     console.log(e.target.value); //giarda los datos que se estan escribiendo 


// });


// email.addEventListener('input',function(e){

//     console.log(e.target.value); //giarda los datos que se estan escribiendo 


// });


// mensaje.addEventListener('input',function(e){

//     console.log(e.target.value); //giarda los datos que se estan escribiendo 


// });


const datos = {
    nombre:'',
    email:'',
    mensaje: ''
}


const nombre = document.querySelector('#nombre')
const email = document.querySelector('#email')
const mensaje = document.querySelector('#mensaje')

nombre.addEventListener('input',leerTexto);
email.addEventListener('input',leerTexto);
mensaje.addEventListener('input',leerTexto);



//submit 

const form = document.querySelector('.formulario')

form.addEventListener('submit' ,function(evento){

    evento.preventDefault();
    const {nombre,email,mensaje} = datos; 

    console.log(nombre,email,mensaje); 

    if(nombre.trim=='' || email.trim=='' || mensaje.trim==''){
        // console.log('los campos son obligatorios')
        mostrarError('todos los campos son oligatorios')
    }

    
    console.log('enviando formulario')    

});

function leerTexto(e){

    datos[e.target.id] = e.target.value;

    console.log(datos)

}




function mostrarError(mensaje){
    const error = document.createElement('P');
    error.textContent =  mensaje
    error.classList.add('error')
    form.appendChild(error);
    //para que desaparezca en 5 S
    setTimeout(()=>{
        error.remove();}
        ,3000);
    console.log(error)
    
}

//funcion que muestre exito al enviar 






























