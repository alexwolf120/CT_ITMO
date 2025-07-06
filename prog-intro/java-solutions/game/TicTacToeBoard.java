package game;

import java.util.Arrays;
import java.util.Map;

public class TicTacToeBoard implements Board, Position {
    private static final Map<Cell, String> CELL_TO_STRING = Map.of(
            Cell.E, ".",
            Cell.X, "X",
            Cell.O, "0"
    );

    private final Cell[][] field;
    private Cell turn;
    int m,n,k;
    public TicTacToeBoard(int m, int n, int k) {
        this.m = m;
        this.n = n;
        this.k = k;
        field = new Cell[m][n];
        for (Cell[] row : field) {
            Arrays.fill(row, Cell.E);
        }
        turn = Cell.X;
    }

    @Override
    public Cell getTurn() {
        return turn;
    }

    @Override
    public Position getPosition() {
        return this;
    }

    @Override
    public GameResult makeMove(Move move) {
        if (!isValid(move)) {
            return GameResult.LOSE;
        }

        field[move.getRow()][move.getCol()] = move.getValue();
        if (checkWin()) {
            return GameResult.WIN;
        }

        if (checkDraw()) {
            return GameResult.DRAW;
        }

        turn = turn == Cell.X ? Cell.O : Cell.X;
        return GameResult.UNKNOWN;
    }

    private boolean checkDraw() {
        int count = 0;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c <n; c++) {
                if (field[r][c] == Cell.E) {
                    count++;
                }
            }
        }
        if (count == 0) {
            return true;
        }
        return false;
    }

    private boolean checkWin() {
        for (int r = 0; r < m; r++) {
            int count = 0;
            for (int c = 0; c < n; c++) {
                if (field[r][c] == turn) {
                    count++;
                    if (count == k) {
                        return true;
                    }
                } else {
                    count = 0;
                }
            }
        }
        for (int c = 0; c < n; c++) {
            int count = 0;
            for (int r = 0; r < m; r++) {
                if (field[r][c] == turn) {
                    count++;
                    if (count == k) {
                        return true;
                    }
                } else {
                    count = 0;
                }
            }
        }
        for (int i = 0; i < m - k + 1; i++) {
            for (int j = 0; j < n - k + 1; j++) {
                int count = 0;
                for (int t = 0; t < k; t++) {
                    if (field[i][j] == turn) {
                        if (field[i][j] == field[i + t][j + t]) {
                            count++;
                            if (count == k) {
                                return true;
                            }
                        } else {
                            count = 0;
                        }
                    }
                }
            }
        }
        if (m > 3 && n > 3){
            for (int i = m - 1; i > m - k; i--) {
                for (int j = 0; j < n - k + 1; j++) {
                    int count = 0;
                    for (int t = 0; t < k; t++) {
                        if (field[i][j] == turn) {
                            if (field[i][j] == field[i - t][j + t]) {
                                count++;
                                if (count == k) {
                                    return true;
                                }
                            } else {
                                count = 0;
                            }
                        }
                    }
                }
            }
        }
        if (m == 3 || n == 3){
            return field[0][0] == turn && field[1][1] == turn && field[2][2] == turn
                    || field[0][2] == turn && field[1][1] == turn && field[2][0] == turn;
        }
        return false;
    }

    public boolean isValid(final Move move) {
        return 0 <= move.getRow() && move.getRow() < m
                && 0 <= move.getCol() && move.getCol() < n
                && field[move.getRow()][move.getCol()] == Cell.E
                && turn == move.getValue();
    }

    //@Override
    //public Cell getCell(int row, int column) {
        //return field[row][column];
    //}

    @Override
    public String toString() {
        final StringBuilder sb = new StringBuilder("   ");
        for (int i = 0; i < n; i++){
            sb.append(i+1 + "  ");
        }
        sb.append("\n");
        for (int r = 0; r < m; r++) {
            sb.append(r + 1);
            for (Cell cell : field[r]) {
                sb.append("  " + CELL_TO_STRING.get(cell) );
            }
            sb.append("  " + System.lineSeparator());
        }
        sb.setLength(sb.length() - System.lineSeparator().length());
        return sb.toString();
    }
}
