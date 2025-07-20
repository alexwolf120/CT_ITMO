package expression.exceptions;

import expression.Add;
import expression.Supint;

public class CheckedAdd extends Add {

    public CheckedAdd(Supint a, Supint b) {
        super(a, b);
    }


    @Override
    public int res(int a, int b) {
        if (b > 0 && a > 0 && b > Integer.MAX_VALUE - a) {
            throw new OverFlowExc("overflow");
        } else if(b < 0 && a < 0 && b < Integer.MIN_VALUE - a) {
            throw new OverFlowExc("overflow");
        }
        return a + b;
    }
}
