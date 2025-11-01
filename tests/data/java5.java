import java.util.concurrent.*;

public class ProducerConsumerExample {
    private final BlockingQueue<Integer> queue;
    
    public ProducerConsumerExample(int capacity) {
        <｜fim▁hole｜>this.queue = new ArrayBlockingQueue<>(capacity);
    }
    
    public void start() {
        Thread producer = new Thread(() -> {
            try {
                for (int i = 1; i <= 5; i++) {
                    queue.put(i);
                    System.out.println("Produced: " + i);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        Thread consumer = new Thread(() -> {
            try {
                for (int i = 1; i <= 5; i++) {
                    int value = queue.take();
                    System.out.println("Consumed: " + value);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        
        producer.start();
        consumer.start();
        
        try {
            producer.join();
            consumer.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
    
    public static void main(String[] args) {
        new ProducerConsumerExample(5).start();
    }
}