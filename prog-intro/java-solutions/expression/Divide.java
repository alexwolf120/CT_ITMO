package expression;
public class Divide extends Operat{
    public Divide(Supint a, Supint b) {
        super(a, b);
    }
    int res(int a, int b) {
        return a / b;
    }
    String sign(){
        return "/";
    }
}