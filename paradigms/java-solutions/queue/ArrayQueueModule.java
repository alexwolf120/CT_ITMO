package queue;

public class ArrayQueueModule {
    private static int size;
    private static int head;
    private static Object[] elements = new Object[10];

    static void ensureCapacity(int capacity){
        if (capacity >= elements.length){
            Object[] newElements = new Object[elements.length * 2];
            for (int i = 0; i < size; i++) {
                newElements[i] = elements[head + i- (head + i < elements.length ? 0 : elements.length)];
            }
            head = 0;
            elements = newElements;
        }
    }

    //pre: element != null
    //post: elements[tail] = element
    public static void enqueue(Object element){
        assert element != null;
        ensureCapacity(++size);
        elements[getTail()] = element;
    }

    //pre: size > 0
    //post: Res = elements[head]
    public static Object element(){
        assert size > 0;
        return elements[head];
    }

    //pre: size > 0
    //post: head = (head < elements.length ? 0 : elements.length),
    //      size = size - 1,
    //      Res = elements[head]
    public static Object dequeue(){
        Object ret = element();
        elements[head++] = null;
        head -= (head < elements.length ? 0 : elements.length);
        size--;
        return ret;
    }

    //post: Res = size
    public static int size(){
        return size;
    }

    //post: Res = (size == 0)
    public static boolean isEmpty(){
        return (size == 0);
    }

    //post: size = 0,
    //      head = 0
    public static void clear(){
        head = 0;
        size = 0;
        elements = new Object[10];
    }
    public static Object[] toArray() {
            Object[] newelem = new Object[size];
            for (int i = 0; i < size; i++) {
                newelem[i] = elements[head + i- (head + i < elements.length ? 0 : elements.length)];
            }
        return newelem;
    }
    private static int getTail(){
        int tail = size + head - 1;
        if(tail >= elements.length){
            return tail - elements.length;
        }
        return tail;
    }
}
