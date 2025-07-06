package queue;

public abstract class AbstractQueue implements Queue {
    protected int size;
    //post: Res = size

    public int size(){
        return size;
    }

    //post: Res = (size == 0)
    public boolean isEmpty(){
        return (size == 0);
    }

    protected abstract Object elementAbs();

    @Override
    public Object element() {
        return elementAbs();
    }

    protected abstract void enqueueAbs(Object element);


    public void enqueue(Object element) {
        enqueueAbs(element);
        size++;
    }

    protected abstract Object dequeueAbs();
    public Object dequeue() {
        Object temp = elementAbs();
        dequeueAbs();
        size--;
        return temp;
    }

    protected abstract void clearAbs();
    public void clear() {
        clearAbs();
        size = 0;
    }
    public int indexOf(final Object element) {
        return firstOrlast(element, "first");
    }
    public int lastIndexOf(final Object element) {
        return firstOrlast(element, "last");
    }
    private int firstOrlast(final Object element, String str){
        int res = -1;
        for(int i = 0; i < size; i++){
            if(str.equals("first")){
                if (element().equals(element) && res == -1 ) {
                    res = i;
                }
            }else{
                if (element().equals(element)){
                    res = i;
                }
            }
            enqueue(dequeue());
        }
        return res;
    }

}
