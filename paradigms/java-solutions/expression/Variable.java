package expression;

public class Variable implements Supint {
    private final String str;

    public Variable(String str) {
        this.str = str;
    }

    @Override
    public int evaluate(int x) {
        return x;
    }

    public String toString() {
        return str;
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return switch (str) {
            case "x" -> x;
            case "y" -> y;
            case "z" -> z;
            default -> 0;
        };
    }

    @Override
    public int hashCode() {
        return str.hashCode();
    }

    @Override
    public boolean equals(Object obg) {
        if (obg instanceof Variable) {
            return str.equals(((Variable) obg).str);
        }
        return false;
    }
}