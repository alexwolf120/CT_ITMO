package queue;
// Model: elements[1]..elements[n]
// Inv: n >= 0 && forall i=1..n: elements[i] != null
// Let: immutable(k): forall i=1..k: elements'[i] = elements[i]
public interface Queue {
    // Pred: size > 0 && queue != null
    // Post: elements[head] && Immutable
    Object element();

    // Pred: element != null
    // Post: size' = size + 1 &&
    //       elements[size'] = element &&
    //       Immutable(size)
    void enqueue(Object element);

    // Pred: size > 0 && queue != null
    // Post: elements[head] &&
    // elements[head]' = null &&
    // forall i = 0..(size - 1)
    // elements[i] = elements[i + 1] && size--;
    Object dequeue();

    // Pred: queue != null && true
    // Post: R = size && size' = size && Immutable(n)
    int size();

    // Pred: queue != null && true
    // Post: R = (size() == 0) && size' = size && Immutable
    boolean isEmpty();

    // Pred: queue != null
    // Post: forall i = 0...size: elements[i] = null && size' == 0
    void clear();
    int lastIndexOf(Object element);
    int indexOf(Object element);
}
