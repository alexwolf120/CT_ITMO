package expression.exceptions;
import expression.Multiply;
import expression.Supint;

public class CheckedMultiply extends Multiply {
    public CheckedMultiply(Supint a, Supint b) {
        super(a, b);
    }
    @Override
    public int res(int x, int y) {
        if(x > 0 && y > 0 && x > Integer.MAX_VALUE / y){
                throw new OverFlowExc("overflow");
        }
        else if(x > 0 && y < 0 && y < Integer.MIN_VALUE / x){
            throw new OverFlowExc("overflow");
        }
        else if(x < 0 && y > 0 && x < Integer.MIN_VALUE / y){
                throw new OverFlowExc("overflow");
        }
        else if(x < 0 && y < 0 && y < Integer.MAX_VALUE / x){
                throw new OverFlowExc("overflow");
        }
        return x * y;
    }
}
