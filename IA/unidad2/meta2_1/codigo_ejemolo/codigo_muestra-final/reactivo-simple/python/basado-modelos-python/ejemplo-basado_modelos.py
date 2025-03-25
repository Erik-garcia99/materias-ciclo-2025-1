from mesa import Agent, Model
from mesa.time import RandomActivation

class IrrigationAgent(Agent):
    """Agente que controla el riego basado en modelos de humedad deseada."""
    def __init__(self, unique_id, model):
        super().__init__(unique_id, model)
        self.soil_moisture = 100  # 0-200: seco-húmedo (estado inicial)
        self.desired_moisture = 150
        self.pump_status = "OFF"  # Bomba de agua

    def step(self):
        self.soil_moisture += self.model.moisture_change
        if self.soil_moisture < self.desired_moisture - 10:
            self.pump_status = "ON"
            print(f"Agente {self.unique_id}: ¡Activar bomba de agua! (Humedad: {self.soil_moisture})")
        elif self.soil_moisture > self.desired_moisture + 10:
            self.pump_status = "OFF"
            print(f"Agente {self.unique_id}: Detener bomba de agua (Humedad: {self.soil_moisture})")

class IrrigationModel(Model):
    """Modelo ambiental que modifica la humedad del suelo."""
    def __init__(self):
        self.schedule = RandomActivation(self)
        self.moisture_change = -2  # Cambio por cada paso (evaporación)
        a = IrrigationAgent(1, self)
        self.schedule.add(a)

    def step(self):
        self.schedule.step()
        self.moisture_change += 5  # Simula lluvia intermitente

# Ejecutar la simulación
model = IrrigationModel()
for _ in range(10):
    model.step()