package game;

import java.util.Scanner;

public class Main {
    public static int m,n,k;
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        boolean bool = true;
        while (bool){
            try{
                System.out.println("Введите размеры доски и длину выйгрыша");
                m = Integer.parseInt(sc.next());
                n = Integer.parseInt(sc.next());
                k = Integer.parseInt(sc.next());
                if(k <= n || k <= m) {
                    bool = false;
                } else {
                    System.out.println("Введите корректные числа");
                }
            }
            catch(Exception e){
                System.out.println("Надо вводить числа а не буквы");
            }
        }
        int pl;
        int wins;
        int win1 = 0;
        int win2 = 0;
        System.out.println("Введите количество побед");
        wins =sc.nextInt();
        System.out.println("Для игры с компьютером введите 0, Для игры человека с человеком введите 1");
        pl = sc.nextInt();
        int w = 0;
        while(win1 != wins && win2 != wins){
            final int result;
            if (w % 2 == 0){
                if (pl == 0){
                    result = new TwoPlayerGame(new TicTacToeBoard(m,n,k),
                            new HumanPlayer(),
                            new RandomPlayer(m,n,k)
                    ).play(true);
                } else{
                    result = new TwoPlayerGame(new TicTacToeBoard(m,n,k),
                        new HumanPlayer(),
                        new HumanPlayer()
                ).play(true);
                }
                switch (result) {
                    case 1:
                        win1++;
                        System.out.println("First player won");
                        break;
                    case 2:
                        win2++;
                        System.out.println("Second player won");
                        break;
                    case 0:
                        System.out.println("Draw");
                        break;
                    default:
                        throw new AssertionError("Unknown 1 result " + result);
                }
                w++;
                System.out.println("Player1 " + win1 + " - Player2 " + win2);
            }else{
                if (pl == 0){
                    result = new TwoPlayerGame(new TicTacToeBoard(m,n,k),
                            new RandomPlayer(m,n,k),
                            new HumanPlayer()
                    ).play(true);
                } else{
                    result = new TwoPlayerGame(new TicTacToeBoard(m,n,k),
                            new HumanPlayer(),
                            new HumanPlayer()
                    ).play(true);
                }
                switch (result) {
                    case 1:
                        win2++;
                        System.out.println("Second player won");
                        break;
                    case 2:
                        win1++;
                        System.out.println("First player won");
                        break;
                    case 0:
                        System.out.println("Draw");
                        break;
                    default:
                        throw new AssertionError("Unknown 1 result " + result);
                }
                w++;
                System.out.println("Player1 " + win1 + " - Player2 " + win2);
            }

        }if (win1 > win2){
            System.out.println("Player 1 WIN");

        }else{
            System.out.println("Player 1 WIN");
        }
    }
}
