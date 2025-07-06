package expression.exceptions;

import expression.Divide;;
import expression.Supint;


public class CheckedDivide extends Divide {
    public CheckedDivide(Supint first, Supint second) {
        super(first, second);
    }

    @Override
    public int res(int a, int b) {
        if (b == 0) {
            throw new OverFlowExc("division by zero");
        }
        else if (b == -1 && a == Integer.MIN_VALUE) {
            throw new OverFlowExc("overflow");
        }
        return a / b;
    }
}
