package expression;
public abstract class Operat implements Supint{
    Supint a;
    Supint b;
    public Operat(Supint a, Supint b){
        this.a = a;
        this.b = b;
    }
    public String toString(){
        return "(" + a.toString() + " " + sign() + " " + b.toString() + ")";
    }
    public int evaluate(int x){
        return res(a.evaluate(x), b.evaluate(x));
    }
    public int evaluate(int x, int y, int z){
        return res(a.evaluate(x, y, z), b.evaluate(x, y, z));
    }
    abstract int res(int a, int b);
    abstract String sign();
    @Override
    public int hashCode(){
        return a.hashCode() * 13 + b.hashCode() * 17 + sign().hashCode() * 19;
    }
    @Override
    public boolean equals(Object obg) {
        if (obg instanceof Operat) {
        return a.equals(((Operat) obg).a)
                && b.equals(((Operat) obg).b)
                && sign().equals(((Operat)obg).sign());
        }
        return false;
    }
}