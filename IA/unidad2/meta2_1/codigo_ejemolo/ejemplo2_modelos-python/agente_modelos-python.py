from mesa import Agent, Model
from mesa.time import RandomActivation

class TrafficLightAgent(Agent):
    def __init__(self, unique_id, model):
        super().__init__(unique_id, model)
        self.state = "RED"  # Estado inicial
        self.desired_state = "GREEN"

    def step(self):
        # Modelo interno: Cambiar estado según reglas
        if self.state == "RED":
            self.state = "GREEN"
        elif self.state == "GREEN":
            self.state = "YELLOW"
        else:
            self.state = "RED"
        print(f"Agente {self.unique_id}: Cambio a {self.state}")

class TrafficModel(Model):
    def __init__(self):
        self.schedule = RandomActivation(self)
        # Crear 3 agentes de semáforo
        for i in range(3):
            a = TrafficLightAgent(i, self)
            self.schedule.add(a)

    def step(self):
        self.schedule.step()

# Ejecución
model = TrafficModel()
for i in range(5):  # Simular 5 pasos
    model.step()