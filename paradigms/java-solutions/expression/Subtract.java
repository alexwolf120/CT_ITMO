package expression;
public class Subtract extends Operat{
    public Subtract(Supint a, Supint b) {
        super(a, b);
    }
    public int res(int a, int b) {
        return a - b;
    }
    public String sign(){
        return "-";
    }
}