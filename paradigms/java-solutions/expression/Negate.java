package expression;

public class Negate extends Ang{
    public Negate(Supint a) {
        super(a);
    }

    @Override
    public String toString() {
        return "-(" + a.toString() + ")";
    }
    @Override
    public int res(int a) {
        return -a;
    }

}
