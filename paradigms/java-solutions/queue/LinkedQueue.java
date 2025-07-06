package queue;

public class LinkedQueue extends AbstractQueue{
    private Node head;
    private Node tail;
    protected void enqueueAbs(Object element){
        Node pom = tail;
        tail = new Node(element, null);
        if(size != 0){
            pom.next = tail;
        }else{
            head = tail;
        }

    }
    protected Object elementAbs(){
        assert size > 0;
        return head.value;
    }
    protected void clearAbs(){
        head = null;
        tail = null;
    }
    protected Object dequeueAbs(){
        assert size > 0;
        Object ret = head.value;
        head = head.next;
        return ret;
    }
    private static class Node{
        private final Object value;
        private Node next;
        public Node(Object value, Node next){
            this.value = value;
            this.next = next;
        }
    }

}
