package expression;
public class Add extends Operat{
    public Add(Supint a, Supint b) {
        super(a, b);
    }
    @Override
    int res(int a, int b) {
        return a + b;
    }
    @Override
    String sign(){
        return "+";
    }
}