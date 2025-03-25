import time
import random

# Definición de productos y sus precios iniciales
productos = [
    {"nombre": "Auriculares", "precio": 100},
    {"nombre": "Smartphone", "precio": 700},
    {"nombre": "Altavoz Portátil", "precio": 80}
]

# Presupuesto del usuario
presupuesto = 100

# Función para verificar precios y proporcionar recomendaciones
def verificar_precios():
    productos_recomendados = []
    for producto in productos:
        # Simulación de fluctuaciones de precios
        cambio_precio = random.randint(-5, 5)  # Simula cambios aleatorios de precio
        producto["precio"] += cambio_precio
        # Evita que el precio sea negativo
        producto["precio"] = max(0, producto["precio"])
        
        # Revisa si el producto está dentro del presupuesto
        if producto["precio"] <= presupuesto:
            productos_recomendados.append(producto)
    return productos_recomendados

# Función principal del agente
def main():
    print("Agente de Monitorización de Precios iniciado...")
    print(f"Presupuesto actual: ${presupuesto}\n")
    
    while True:
        print("Verificando precios de productos...")
        recomendados = verificar_precios()
        
        # Imprime recomendaciones
        if recomendados:
            print("\nProductos disponibles en tu presupuesto:")
            for producto in recomendados:
                print(f"- {producto['nombre']}: ${producto['precio']}")
        else:
            print("\nNingún producto se ajusta a tu presupuesto en este momento.")
        
        # Espera antes de la próxima verificación
        time.sleep(2)  # Retardo de 2 segundos

        # Para evitar un bucle infinito, esta es una salida manual (en producción usar CTRL+C)
        pregunta = input("\n¿Realizar otra verificación? (S/N): ").lower()
        if pregunta != 's':
            break

    print("\nAgente de monitorización detenido.")

if __name__ == "__main__":
    main()