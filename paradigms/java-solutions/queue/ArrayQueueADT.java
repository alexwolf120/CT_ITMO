package queue;

public class ArrayQueueADT {
    private int size;
    private int head;
    private Object[] elements = new Object[10];

    static void ensureCapacity(ArrayQueueADT queue, int capacity){
        assert queue != null;
        int len = queue.elements.length;
        if (capacity > len){
            Object[] newelem = new Object[queue.elements.length * 2];
            for (int i = 0; i < queue.size; i++) {
                newelem[i] = queue.elements[queue.head + i- (queue.head + i < len ? 0 : len)];
            }
            queue.head = 0;
            queue.elements = newelem;
        }
    }

    //pre: element != null,
    //     queue != null
    //post: elements[tail] = element
    public static void enqueue(ArrayQueueADT queue, Object element){
        assert queue != null;
        assert element != null;
        ensureCapacity(queue, ++queue.size);
        queue.elements[queue.getTail(queue)] = element;
    }

    //pre: queue != null,
    //     size > 0
    //post: Res = elements[head]
    public static Object element(ArrayQueueADT queue){
        assert queue != null;
        assert queue.size > 0;
        return queue.elements[queue.head];
    }

    //pre: queue != null,
    //     size > 0
    //post: Res = elements[head]
    public static Object dequeue(ArrayQueueADT queue){
        assert queue != null;
        Object ret = element(queue);
        queue.elements[queue.head++] = null;
        queue.head -= (queue.head < queue.elements.length ? 0 : queue.elements.length);
        queue.size--;
        return ret;
    }

    //pre: queue != null
    //post: Res = size
    public static int size(ArrayQueueADT queue){
        assert queue != null;
        return queue.size;
    }

    //pre: queue != null
    //post: Res = (size == 0)
    public static boolean isEmpty(ArrayQueueADT queue){
        assert queue != null;
        return (queue.size == 0);
    }

    //pre: queue != null
    //post: size = 0,
    //      head = 0
    public static void clear(ArrayQueueADT queue){
        assert queue != null;
        queue.head = 0;
        queue.size = 0;
        queue.elements = new Object[10];
    }
    public static Object[]toArray(ArrayQueueADT queue) {
        assert queue != null;
        int len = queue.elements.length;
        Object[] newElements = new Object[queue.size];
        for (int i = 0; i < queue.size; i++) {
            newElements[i] = queue.elements[queue.head + i- (queue.head + i < len ? 0 : len)];
        }
        return newElements;
    }
    private static int getTail(ArrayQueueADT queue){
        assert queue != null;
        int tail = queue.size + queue.head - 1;
        if(tail >= queue.elements.length){
            return tail - queue.elements.length;
        }
        return tail;
    }
}
