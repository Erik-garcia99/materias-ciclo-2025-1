import random
import time

class TemperatureSensor:
    """Simula un sensor de temperatura."""
    def get_temperature(self):
        """Devuelve una temperatura aleatoria entre 15 y 35°C."""
        return random.randint(15, 35)

class Reactor:
    """Simula un reactor químico."""
    def __init__(self):
        self.temperature = 20  # Temperatura inicial

    def heat_up(self, value):
        """Aumenta la temperatura (simulado)."""
        self.temperature += value
        print(f"Aumentando temperatura a {self.temperature}°C")

    def cool_down(self, value):
        """Disminuye la temperatura (simulado)."""
        self.temperature -= value
        print(f"Disminuyendo temperatura a {self.temperature}°C")

class ReactiveTemperatureController:
    """Agente reactivo para controlar la temperatura del reactor."""
    def __init__(self, reactor, target_temp=25):
        self.reactor = reactor
        self.target_temp = target_temp
        self.sensor = TemperatureSensor()

    def run(self):
        """Bucle principal del agente reactivo."""
        while True:
            # Percepción: obtener la temperatura actual
            current_temp = self.sensor.get_temperature()
            print(f"\nPercepción: Temperatura actual = {current_temp}°C")
            
            # Reacción: ajustar el reactor según la temperatura actual
            if abs(current_temp - self.target_temp) > 2:
                if current_temp < self.target_temp:
                    # Calentar el reactor
                    self.reactor.heat_up(1)
                else:
                    # Enfriar el reactor
                    self.reactor.cool_down(1)
            time.sleep(1)  # Esperar antes de la próxima percepción

if __name__ == "__main__":
    reactor = Reactor()
    controller = ReactiveTemperatureController(reactor)
    print("Iniciando agente reactivo de control de temperatura...")
    controller.run()