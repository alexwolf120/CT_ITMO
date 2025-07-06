package game;

import java.util.Random;

public class RandomPlayer implements Player {
    private final Random random = new Random();
    private int m,n,k;
    public RandomPlayer(int m, int n, int k){
        this.m = m;
        this.n = n;
        this.k = k;
    }

    @Override
    public Move makeMove(Position position) {
        while (true) {
            final Move move = new Move(
                    random.nextInt(m),
                    random.nextInt(n),
                    position.getTurn()
            );
            if (position.isValid(move)) {
                return move;
            }
        }
    }
}