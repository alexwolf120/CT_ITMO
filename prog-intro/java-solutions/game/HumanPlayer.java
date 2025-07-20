package game;

import java.io.PrintStream;
import java.util.Scanner;

public class HumanPlayer implements Player {
    private final PrintStream out;
    private final Scanner in;
    public HumanPlayer(final PrintStream out, final Scanner in) {
        this.out = out;
        this.in = in;
    }
    public HumanPlayer() {
        this(System.out, new Scanner(System.in));
    }
    int l1,l2;
    public Move makeMove(final Position position) {
        System.out.println();
        System.out.println("Current position");
        System.out.println(position);
        System.out.println("Enter you move for " + position.getTurn());
        boolean bool = true;
        while (bool){
            try{
                l1 = Integer.parseInt(in.next());
                l2 = Integer.parseInt(in.next());
                if(l1 <= Main.n || l2 <= Main.m) bool = false;
                else System.out.println("Введите корректные числа");
            }
            catch(Exception e){
                System.out.println("Надо вводить числа, а не буквы");
            }
        }
        return new Move( l1 - 1, l2 - 1, position.getTurn());
    }
}