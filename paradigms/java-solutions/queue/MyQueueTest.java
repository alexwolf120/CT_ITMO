package queue;
import java.util.Queue;

public class MyQueueTest {
    public static void fill(ArrayQueue queue, String prefix) {
        for (int i = 0; i < 10; i++) {
            queue.enqueue(prefix + i);
        }
    }

    public static void dump(ArrayQueue queue) {
        while (!queue.isEmpty()) {
            System.out.println(queue.size() + " " + queue.dequeue());
        }
    }

    public static void main(String[] args) {
        LinkedQueue stack1 = new LinkedQueue();
        stack1.enqueue(5);
        stack1.enqueue(4);
        stack1.enqueue(3);
        stack1.enqueue(2);
        stack1.enqueue(1);
        System.out.println(stack1.element());
        System.out.println(stack1.size());
        stack1.dequeue();
        stack1.dequeue();
        System.out.println(stack1.element());
        System.out.println(stack1.size());
        System.out.println(stack1.isEmpty());
        stack1.clear();
        System.out.println(stack1.isEmpty());
        System.out.println(stack1.size());
//        fill(stack1, "s1_");
//        fill(stack2, "s2_");
//        dump(stack1);
//        dump(stack2);
    }
}