package expression;

public class Variable implements Supint {
    private String str;

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
        switch (str) {
            case "x":
                return x;
            case "y":
                return y;
            default:
                return z;
        }
        /*if (str.equals("x")){
            return x;
        }else if (str.equals("y")){
            return y;
        }else {
            return z;
        }*/
    }

    @Override
    public int hashCode() {
        return str.hashCode();
    }

    @Override
    public boolean equals(Object obg) {
        if (obg instanceof Variable) {
            if (str.equals(((Variable) obg).str)) {
                return true;
            }
        }
        return false;
    }
}