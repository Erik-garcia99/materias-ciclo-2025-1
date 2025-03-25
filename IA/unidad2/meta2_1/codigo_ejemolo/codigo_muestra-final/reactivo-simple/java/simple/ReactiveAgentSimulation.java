import java.util.Random;

class ReactiveAgent {
    private int x, y;
    private Environment env;
    private Random rand;

    public ReactiveAgent(int startX, int startY, Environment env) {
        this.x = startX;
        this.y = startY;
        this.env = env;
        this.rand = new Random();
    }

    public void step() {
        int newX = x + (rand.nextBoolean() ? 1 : -1);
        int newY = y + (rand.nextBoolean() ? 1 : -1);

        if (!env.isObstacle(newX, newY)) {
            x = newX;
            y = newY;
        } else {
            System.out.println("Obstáculo detectado en (" + newX + ", " + newY + "). Cambiando dirección.");
        }
    }

    public void printPosition() {
        System.out.println("Agente en posición: (" + x + ", " + y + ")");
    }
}

class Environment {
    private int width, height;
    private boolean[][] obstacles;

    public Environment(int width, int height) {
        this.width = width;
        this.height = height;
        this.obstacles = new boolean[width][height];
        placeRandomObstacles(5);  // Agrega 5 obstáculos aleatorios
    }

    private void placeRandomObstacles(int count) {
        Random rand = new Random();
        for (int i = 0; i < count; i++) {
            int x = rand.nextInt(width);
            int y = rand.nextInt(height);
            obstacles[x][y] = true;
        }
    }

    public boolean isObstacle(int x, int y) {
        return x < 0 || x >= width || y < 0 || y >= height || obstacles[x][y];
    }
}

public class ReactiveAgentSimulation {
    public static void main(String[] args) {
        Environment env = new Environment(10, 10);
        ReactiveAgent agent = new ReactiveAgent(5, 5, env);

        for (int i = 10; i > 0; i--) {
            agent.step();
            agent.printPosition();
        }
    }
}
