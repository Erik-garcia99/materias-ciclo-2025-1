import jade.core.Profile;
import jade.core.ProfileImpl;
import jade.core.Runtime;
import jade.wrapper.AgentContainer;
import jade.wrapper.AgentController;
import jade.wrapper.StaleProxyException;

public class ReactiveSimulation {
    public static void main(String[] args) {
        Runtime rt = Runtime.instance();
        Profile p = new ProfileImpl();
        AgentContainer container = rt.createMainContainer(p);

        try {
            AgentController agent = container.createNewAgent("Reactivo", ReactiveAgent.class.getName(), null);
            agent.start();
        } catch (StaleProxyException e) {
            e.printStackTrace();
        }
    }
}
