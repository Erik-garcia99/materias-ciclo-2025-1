//script de la practica 


const datos ={

    name:'',
    last_name:'',
    pass:'',
    confirm_pass:'',
    check:false
};


const nameInput = document.querySelector('#name');
const lastNameInput = document.querySelector('#last_name');
const passInput = document.querySelector('#pass');
const confirmPassInput = document.querySelector('#confirm_pass');
const checkInput = document.querySelector('#check');


// Eventos (corregir nombre función leerInput)
nameInput.addEventListener('input', leerInput);
lastNameInput.addEventListener('input', leerInput);
passInput.addEventListener('input', leerInput);
confirmPassInput.addEventListener('input', leerInput);
checkInput.addEventListener('change', leerInput); // Cambiar a 'change' para checkbox


const form = document.querySelector('#data_user')

form.addEventListener('submit', function(e) {


    e.preventDefault();
    
    if(!validarFormulario()){
        return;
    }

    console.log("Formulario válido:", datos)
});


function validarFormulario(){


    let valido = true;

    //resetea errores 
    document.querySelectorAll('.error').forEach(error => error.remove());
    const camposRequeridos = ['name', 'last_name', 'pass', 'confirm_pass'];

    camposRequeridos.forEach(id => {
        if(!datos[id]){
            mostrarError(`El campo ${id.replace('_', ' ')} es requerido`, id);
            valido= false;
        }
    });

    if(datos.pass !== datos.confrm_pass){
        mostrarError("Las contraseñas no coinciden", 'confirm_pass')
        valido = false;
    }

    if(!datos.check){
        mostrarError("Debes aceptar los términos", 'check')
        valido = false;
    }

    return valido

}


function leerInput(e) {
     if (e.target.type === 'checkbox') {
        datos[e.target.id] = e.target.checked;
    } else {
        datos[e.target.id] = e.target.value.trim();
    }
    console.log(datos);
}


function mostrarError(mensaje, campoId) {
    const campo = document.getElementById(campoId);
    const error = document.createElement('p');
    error.className = 'error';
    error.style.color = 'red';
    error.textContent = mensaje;
    campo.insertAdjacentElement('afterend', error);
}











