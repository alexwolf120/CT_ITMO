package expression;

import expression.exceptions.ParsExp;
import expression.exceptions.ExpressionParser;

public class Main {
    public static void main(String[] args) throws ParsExp {
        int c = new Subtract(new Multiply(new Variable("x"),new Variable("x")),new Subtract(new Multiply(new Const(2),new Variable("x")), new Const(1))).evaluate(5);
        String k = new ExpressionParser().parse("(1 + 2) * 3").toString();
        System.out.println(k);

        int m = new ExpressionParser().parse("x*y+(z -1   )/10").evaluate(2,2,11);
        System.out.println(m);
        //System.out.println(k);
    }
}