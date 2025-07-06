package queue;

public class ArrayQueue extends AbstractQueue {
    private int head = 0;
    private Object[] elements = new Object[10];

    private void ensureCapacity(int capacity){
        int len = elements.length;
        if (capacity >= len){
            Object[] newelem = new Object[len * 2];
            for (int i = 0; i < size; i++) {
                newelem[i] = elements[head + i - (head + i < len ? 0 : len)];
            }
            head = 0;
            elements = newelem;
        }
    }
    public void enqueueAbs(Object element){
        assert element != null;
        ensureCapacity(size);
        elements[getTail()] = element;
    }
    public Object elementAbs(){
        assert size > 0;
        return elements[head];
    }
    public Object dequeueAbs(){
        assert size > 0;
        Object ret = elementAbs();
        elements[head++] = null;
        head -= (head < elements.length ? 0 : elements.length);

        return ret;
    }
    public void clearAbs(){
        head = 0;
        size = 0;
        elements = new Object[10];
    }
    private int getTail(){
        int tail = size + head;
        if(tail >= elements.length){
            return tail - elements.length;
        }
        return tail;
    }
    public Object[] toArray() {
        int len = elements.length;
        Object[] newelem = new Object[size];
        // ensureCapacity
        for (int i = 0; i < size; i++) {
            newelem[i] = elements[head + i - (head + i < len ? 0 : len)];
        }
        return newelem;
    }
}

