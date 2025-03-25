class Environment:
    def __init__(self):
        self.rooms = {'A': 'sucio', 'B': 'sucio'}
        self.posicion_actual = 'A'
    
    def obtener_percepcion(self):
        return (self.posicion_actual, self.rooms[self.posicion_actual])
    
    def ejecutar_accion(self, accion):
        if accion == "limpiar":
            self.rooms[self.posicion_actual] = 'limpio'
        elif accion == "mover_derecha":
            self.posicion_actual = 'B' if self.posicion_actual == 'A' else 'A'
        elif accion == "mover_izquierda":
            self.posicion_actual = 'A' if self.posicion_actual == 'B' else 'B'

def agente_reactivo(percepcion):
    posicion, estado = percepcion
    if estado == 'sucio':
        return "limpiar"
    else:
        if posicion == 'A':
            return "mover_derecha"
        else:
            return "mover_izquierda"

# Simulación del entorno
entorno = Environment()
print("Estado inicial:", entorno.rooms)

for paso in range(5):
    print("\n--- Paso", paso + 1, "---")
    percepcion_actual = entorno.obtener_percepcion()
    print(f"Percepción: [Habitación {percepcion_actual[0]} - {percepcion_actual[1]}]")
    
    accion = agente_reactivo(percepcion_actual)
    print("Acción seleccionada:", accion)
    
    entorno.ejecutar_accion(accion)
    print("Nuevo estado:", entorno.rooms)