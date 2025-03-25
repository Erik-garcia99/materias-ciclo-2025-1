public class SimpleReactiveAgent {
    private String currentPercept;

    public void perceive(String percept) {
        this.currentPercept = percept;
    }

    public String react() {
        if ("night".equalsIgnoreCase(currentPercept)) {
            return "Encender la LUZ  -> ¡¡Luz encendida!!";
        } else {
            return "Apagar la LUZ  -> ¡¡Luz apagada!!";
        }
    }

    public static void main(String[] args) {
        SimpleReactiveAgent agent = new SimpleReactiveAgent();

        // Simulación de percepciones durante el día y la noche
        agent.perceive("day");
        System.out.println(agent.react());

        agent.perceive("night");
        System.out.println(agent.react());
    }
}