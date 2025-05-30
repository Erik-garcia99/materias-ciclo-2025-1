/* erik garcia chavez 01275863
practica validacion con JS 

apicaciones web 2025-1
*/

// Esperar a que el DOM esté completamente cargado
document.addEventListener('DOMContentLoaded', function() {
    // Referencias a elementos del DOM
    const formulario = document.getElementById('formularioRegistro');
    const botonEnviar = document.getElementById('botonEnviar');
    const resumenDatos = document.getElementById('resumenDatos');
    
    // Elementos de los campos
    const campoNombre = document.getElementById('nombre');
    const campoEmail = document.getElementById('email');
    const campoContrasena = document.getElementById('password');
    const campoConfirmarContrasena = document.getElementById('confirmarContrasena');
    const campoTerminos = document.getElementById('terminos');
    
    // Elementos para mensajes de error
    const errorNombre = document.getElementById('errorNombre');
    const errorEmail = document.getElementById('errorEmail');
    const errorContrasena = document.getElementById('errorContrasena');
    const errorConfirmarContrasena = document.getElementById('errorConfirmarContrasena');
    const errorTerminos = document.getElementById('errorTerminos');
    
    // Estado de validación
    const estadoValidacion = {
        nombreValido: false,
        emailValido: false,
        contrasenaValido: false,
        confirmarContrasenaValido: false,
        terminosValido: false
    };
    
    // Expresión regular para validar email
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    
    // Función para validar el nombre
    function validarNombre() {
        const valor = campoNombre.value.trim();
        
        if (valor === '') {
            mostrarError(campoNombre, errorNombre, 'El nombre es obligatorio');
            return false;
        }
        
        if (valor.length < 3) {
            mostrarError(campoNombre, errorNombre, 'Mínimo 3 caracteres');
            return false;
        }
        
        limpiarError(campoNombre, errorNombre);
        return true;
    }
    
    // Función para validar el email
    function validarEmail() {
        const valor = campoEmail.value.trim();
        
        if (valor === '') {
            mostrarError(campoEmail, errorEmail, 'El email es obligatorio');
            return false;
        }
        
        if (!emailRegex.test(valor)) {
            mostrarError(campoEmail, errorEmail, 'Email no válido');
            return false;
        }
        
        limpiarError(campoEmail, errorEmail);
        return true;
    }
    
    // Función para validar la contraseña
    function validarContrasena() {
        const valor = campoContrasena.value;
        
        if (valor === '') {
            mostrarError(campoContrasena, errorContrasena, 'La contraseña es obligatoria');
            return false;
        }
        
        if (valor.length < 8) {
            mostrarError(campoContrasena, errorContrasena, 'Mínimo 8 caracteres');
            return false;
        }
        
        limpiarError(campoContrasena, errorContrasena);
        return true;
    }
    
    // Función para validar la confirmación de contraseña
    function validarConfirmarContrasena() {
        const valor = campoConfirmarContrasena.value;
        const contrasena = campoContrasena.value;
        
        if (valor === '') {
            mostrarError(campoConfirmarContrasena, errorConfirmarContrasena, 'Confirma tu contraseña');
            return false;
        }
        
        if (valor !== contrasena) {
            mostrarError(campoConfirmarContrasena, errorConfirmarContrasena, 'Las contraseñas no coinciden');
            return false;
        }
        
        limpiarError(campoConfirmarContrasena, errorConfirmarContrasena);
        return true;
    }
    
    // Función para validar los términos
    function validarTerminos() {
        if (!campoTerminos.checked) {
            errorTerminos.textContent = 'Debes aceptar los términos';
            return false;
        }
        
        errorTerminos.textContent = '';
        return true;
    }
    
    // Función para mostrar errores
    function mostrarError(campo, elementoError, mensaje) {
        campo.classList.add('invalido');
        campo.classList.remove('valido');
        elementoError.textContent = mensaje;
    }
    
    // Función para limpiar errores
    function limpiarError(campo, elementoError) {
        campo.classList.remove('invalido');
        campo.classList.add('valido');
        elementoError.textContent = '';
    }
    
    // Función para actualizar el estado de validación
    function actualizarEstadoValidacion() {
        estadoValidacion.nombreValido = validarNombre();
        estadoValidacion.emailValido = validarEmail();
        estadoValidacion.contrasenaValido = validarContrasena();
        estadoValidacion.confirmarContrasenaValido = validarConfirmarContrasena();
        estadoValidacion.terminosValido = validarTerminos();
        
        // Habilitar o deshabilitar el botón
        const todosValidos = Object.values(estadoValidacion).every(val => val === true);
        botonEnviar.disabled = !todosValidos;
    }
    
    // Eventos para validación en tiempo real
    campoNombre.addEventListener('input', validarNombre);
    campoEmail.addEventListener('input', validarEmail);
    campoContrasena.addEventListener('input', validarContrasena);
    campoConfirmarContrasena.addEventListener('input', validarConfirmarContrasena);
    campoTerminos.addEventListener('change', validarTerminos);
    
    // Evento para actualizar estado de validación cuando cambia cualquier campo
    formulario.addEventListener('input', actualizarEstadoValidacion);
    
    // Manejar el envío del formulario
    formulario.addEventListener('submit', function(event) {
        event.preventDefault();
        
        // Validar todos los campos
        actualizarEstadoValidacion();
        
        // Si todo es válido, mostrar el resumen
        if (Object.values(estadoValidacion).every(val => val === true)) {
            // Mostrar datos en el resumen
            document.getElementById('resNombre').textContent = campoNombre.value.trim();
            document.getElementById('resEmail').textContent = campoEmail.value.trim();
            
            // Mostrar el resumen
            resumenDatos.classList.add('visible');
            
            // Simular envío exitoso
            console.log('Formulario enviado con éxito');
            console.log('Datos registrados:');
            console.log(`Nombre: ${campoNombre.value.trim()}`);
            console.log(`Email: ${campoEmail.value.trim()}`);
            
            // Limpiar formulario (simulando envío exitoso)
            formulario.reset();
            
            // Deshabilitar botón nuevamente
            botonEnviar.disabled = true;
            
            // Restablecer clases de validación
            const campos = [campoNombre, campoEmail, campoContrasena, campoConfirmarContrasena];
            campos.forEach(campo => {
                campo.classList.remove('valido', 'invalido');
            });
            
            // Restablecer estado de validación
            Object.keys(estadoValidacion).forEach(key => {
                estadoValidacion[key] = false;
            });
            
            // Ocultar resumen después de 5 segundos
            setTimeout(() => {
                resumenDatos.classList.remove('visible');
            }, 5000);
        }
    });
});