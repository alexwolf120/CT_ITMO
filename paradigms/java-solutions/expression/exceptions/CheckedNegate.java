package expression.exceptions;

import expression.Negate;
import expression.Supint;
public class CheckedNegate extends Negate {

    public CheckedNegate(Supint a) {
        super(a);
    }

    @Override
    public int res(int x) {
        if (x == Integer.MIN_VALUE) {
            throw new OverFlowExc("overflow");
        }
        return -x;
    }
}

