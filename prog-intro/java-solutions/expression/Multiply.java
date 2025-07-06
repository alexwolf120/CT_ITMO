package expression;
public class Multiply extends Operat{
    public Multiply(Supint a, Supint b) {
        super(a, b);
    }
    @Override
    int res(int a, int b) {
        return a * b;
    }
    @Override
    String sign(){
        return "*";
    }
}