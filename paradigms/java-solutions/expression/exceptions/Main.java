package expression.exceptions;

public class Main {
    public static void main(String[] args){
        try {
            System.out.println(new ExpressionParser().parse("1000000*x*x*x*x*x/(x-1)").evaluate(0,0,0));
        }catch (OverFlowExc e){
            System.out.println(e.getMessage());
        }
    }
}
