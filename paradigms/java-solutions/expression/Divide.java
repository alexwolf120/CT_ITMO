package expression;
public class Divide extends Operat{
    public Divide(Supint a, Supint b) {
        super(a, b);
    }
    public int res(int a, int b) {
        return a / b;
    }
    public String sign(){
        return "/";
    }
}