// Función para manejar el envío del formulario
document.getElementById('formularioProductos').addEventListener('submit', function(event) {
    event.preventDefault(); // Prevenir el envío del formulario

    // Obtener valores del formulario
    var producto = document.getElementById('Producto').value;
    var cantidad = document.getElementById('cantidad').value;

    // Crear nuevo elemento de producto para agregar a la lista
    var nuevoProducto = document.createElement('div');
    nuevoProducto.classList.add('producto');
    nuevoProducto.innerHTML = '<strong>' + producto + '</strong> - Cantidad: ' + cantidad;

    // Agregar nuevo producto al contenedor de productos agregados
    document.getElementById('productosAgregados').appendChild(nuevoProducto);

    // Limpiar campos del formulario después de agregar el producto
    document.getElementById('Producto').value = '';
    document.getElementById('cantidad').value = '';
});