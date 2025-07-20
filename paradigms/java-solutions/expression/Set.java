package expression;

public class Set extends Operat{
    public Set(Supint a, Supint b) {
        super(a, b);
    }

    @Override
    int res(int a, int b) {
        return a | (1 << b);
    }

    @Override
    String sign() {
        return "set";
    }
}
