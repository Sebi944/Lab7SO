import java.util.concurrent.Semaphore;

class Resource {
    private final Semaphore semaphore = new Semaphore(1);

    public void useResource(String color) {
        try {
            semaphore.acquire();
            System.out.println("Thread " + Thread.currentThread().getId() + " (" + color + ") is accessing the resource.");
            Thread.sleep(1000); // Simulating resource usage
            System.out.println("Thread " + Thread.currentThread().getId() + " (" + color + ") has finished using the resource.");
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            System.out.println("Thread was interrupted.");
        } finally {
            semaphore.release();
        }
    }
}

class ProcessThread extends Thread {
    private final Resource resource;
    private final String color;

    public ProcessThread(Resource resource, String color) {
        this.resource = resource;
        this.color = color;
    }

    @Override
    public void run() {
        resource.useResource(color);
    }
}

public class Main {
    public static void main(String[] args) {
        Resource resource = new Resource();
        ProcessThread[] threads = new ProcessThread[10];
        String[] colors = {"white", "black"};

        for (int i = 0; i < threads.length; i++) {
            String color = colors[(int) (Math.random() * 2)];
            threads[i] = new ProcessThread(resource, color);
        }

        for (ProcessThread thread : threads) {
            thread.start();
        }

        for (ProcessThread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
                System.out.println("Thread join was interrupted.");
            }
        }
    }
}
