package expression.exceptions;

import java.text.ParseException;

public class ParsExp extends ParseException {
    public ParsExp(String s) {
        super(s,0);
    }
}
