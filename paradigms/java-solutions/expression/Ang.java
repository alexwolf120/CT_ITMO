package expression;

public abstract class Ang implements Supint{
    protected final Supint a;
    public Ang(Supint a){
        this.a = a;
    }
    public abstract String toString();
    @Override
    public int evaluate(int x){
        return res(a.evaluate(x));
    }
    @Override
    public int evaluate(int x, int y, int z){
        return res(a.evaluate(x, y, z));
    }
    abstract int res(int a);


}
