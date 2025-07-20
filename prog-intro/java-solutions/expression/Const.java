package expression;
public class Const implements Supint{
    private int con;
    public Const(int con){
        this.con = con;
    }

    @Override
    public int evaluate(int x) {
        return con;
    }
    public String toString(){
        return String.valueOf(con);
    }

    @Override
    public int evaluate(int x, int y, int z) {
        return con;
    }
    @Override
    public int hashCode(){
        return con;
    }
    @Override
    public boolean equals(Object obg) {
        if (obg instanceof Const) {
            if (con == ((Const) obg).con){
                return true;
            }
        }
        return false;
    }
}