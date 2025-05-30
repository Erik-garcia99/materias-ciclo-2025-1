//querySelector

// const heading = document.querySelector('.heading h2');
// heading.textContent = "Titulo Nuevo";
// heading.classList.add('nueva-clase');
// console.log(heading);

//agregar un nuevo enlace
const enlace = document.querySelectorAll('.navegacion a');
enlace[0].textContent = 'Otro Menu';
enlace[0].classList.add('navegacion__enlace');
enlace[0].href= "http://google.com/";
console.log(enlace);


//generar un nuevo enlace
const nuevoEnlace = document.createElement('A');
nuevoEnlace.href = 'nuevoEnlace.html';
nuevoEnlace.textContent= 'Nuevo Enlace';
nuevoEnlace.classList.add('navegacion__enlace');
//agregar el enlace al documento

const nav = document.querySelector('.navegacion');
nav.appendChild(nuevoEnlace);
console.log(nuevoEnlace);

//Eventos con JS
// console.log(1);
// window.onload = function() {
//     console.log(3);
// };
// window.addEventListener('load', function(){ 
//     //load esperar a que el JS y los archivos que dependen del HTML esten listos
//     console.log(2);
// });

// console.log(5);

//podemos crear funciones separadas
// window.addEventListener('load', imprimir) ;
// window.onload = function() {
//      console.log(3);
// };
// document.addEventListener('DOMContentLoaded', function() {
//     //solo espera por el HTML pero no por el CSS o imagenes, es mas rapido
//     console.log(4);
// });
// console.log(5);
// function imprimir(){
//     console.log(2);
// }

// window.onscroll= function() {
//     console.log('scrolling...')
// }


//Eventos donde el usuario introduzca datos en los elementos 
const datos = {
    nombre: '',
    email: '',
    mensaje: ''
}
const nombre = document.querySelector('#nombre');
const email = document.querySelector('#email');
const mensaje = document.querySelector('#mensaje');

nombre.addEventListener('input',leerInputs);
email.addEventListener('input',leerInputs);
mensaje.addEventListener('input',leerInputs);
//Evento con el submit, cuando el usuario de click en el boton enviar
const formulario = document.querySelector('.formulario');
formulario.addEventListener('submit', function(evento) {
    evento.preventDefault();
    //validar el formulario
    const { nombre, email, mensaje } = datos;
    console.log(nombre,email,mensaje);
    if (nombre == '' || email == '' || mensaje =='') {
        mostrarAlerta('Todos los campos son obligatorios',true);
        return;
    }
    //enviar el formulario

    mostrarAlerta('Enviando formulario',false);
});

function leerInputs(evento) {
    datos[evento.target.id]= evento.target.value;
    console.log(datos);
}

function mostrarAlerta(mensaje, error=null)
{
    const alerta = document.createElement('P');
    alerta.textContent = mensaje;
    if (error)
        alerta.classList.add('error');
    else 
        alerta.classList.add('exito');
    formulario.appendChild(alerta);
    setTimeout(()=> {
        alerta.remove();
    },3000);
    console.log(alerta);
}




