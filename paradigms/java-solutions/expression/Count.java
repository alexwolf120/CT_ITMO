package expression;

public class Count extends Ang {
    public Count(Supint a) {
        super(a);
    }
    @Override
    int res(int a) {
        return Integer.bitCount(a);
    }

    @Override
    public String toString() {
        return "count(" + a.toString() + ")";
    }


}
