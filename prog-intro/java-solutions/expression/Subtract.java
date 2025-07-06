package expression;
public class Subtract extends Operat{
    public Subtract(Supint a, Supint b) {
        super(a, b);
    }
    int res(int a, int b) {
        return a - b;
    }
    String sign(){
        return "-";
    }
}