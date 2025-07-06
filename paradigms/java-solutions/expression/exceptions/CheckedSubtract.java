package expression.exceptions;
import expression.Subtract;
import expression.Supint;

public class CheckedSubtract extends Subtract {
    public CheckedSubtract(Supint a, Supint b) {
        super(a, b);
    }

    @Override
    public int res(int x, int y) {
        if (y > 0 && x < Integer.MIN_VALUE + y) {
            throw new OverFlowExc("overflow");
        } else if (y < 0 && x > Integer.MAX_VALUE + y){
            throw new OverFlowExc("overflow");
        }
        return x - y;

    }

}

