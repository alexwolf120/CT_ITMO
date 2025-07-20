package expression;

public class Clear extends Operat {
    public Clear(Supint a, Supint b) {
        super(a, b);
    }

    @Override
    public int res(int a, int b) {
        return ~(1 << b) & a;
    }

    @Override
    public String sign() {
        return "clear";
    }
}
