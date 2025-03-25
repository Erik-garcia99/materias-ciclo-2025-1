import jade.core.Agent;
import jade.core.behaviours.CyclicBehaviour;
import jade.lang.acl.MessageTemplate;
import jade.lang.acl_ACLMessage;

public class TrafficLightAgent extends Agent {
    // Modelo interno: estado actual del semáforo y tiempo transcurrido
    private String currentLight = "RED"; // RED, GREEN, YELLOW
    private int timeElapsed = 0;
    private final int GREEN_DURATION = 30; // Tiempo máximo en verde

    @Override
    protected void setup() {
        System.out.println("Agente de tráfico " + getLocalName() + " iniciado.");
        // Registrar comportamiento para procesar mensajes
        addBehaviour(new CyclicBehaviour(this) {
            @Override
            public void action() {
                // Recibir percepciones (ej: mensajes de sensores)
                MessageTemplate mt = MessageTemplate.MatchPerformative(_ACLMessage.INFORM);
                _ACLMessage msg = receive(mt);
                if (msg != null) {
                    // Procesar mensaje (ej: "CARS_DETECTED")
                    String content = msg.getContent();
                    updateModel(content);
                    reactToModel();
                } else {
                    block(); // Esperar si no hay mensajes
                }
            }
        });
    }

    // Actualizar modelo interno basado en percepciones
    private void updateModel(String perception) {
        if ("CARS_DETECTED".equals(perception)) {
            if (currentLight.equals("GREEN")) {
                timeElapsed++;
            } else {
                // Si no hay coches, resetear tiempo
                timeElapsed = 0;
            }
        } else if ("NO_CARS".equals(perception)) {
            timeElapsed = 0; // Reiniciar tiempo si no hay coches
        }
    }

    // Tomar decisiones basadas en el modelo
    private void reactToModel() {
        if (currentLight.equals("GREEN") && timeElapsed >= GREEN_DURATION) {
            changeLightTo("YELLOW");
        } else if (currentLight.equals("YELLOW")) {
            changeLightTo("RED");
        } else if (currentLight.equals("RED") && timeElapsed > 5) {
            changeLightTo("GREEN"); // Cambiar a verde si hay coches esperando
        }
    }

    // Cambiar estado del semáforo (ej: actualizar un actuador)
    private void changeLightTo(String newLight) {
        currentLight = newLight;
        timeElapsed = 0; // Reiniciar contador
        System.out.println("Semáforo cambiado a " + currentLight);
        // Aquí se integraría con un sistema físico (ej: API de control de luces)
    }
}