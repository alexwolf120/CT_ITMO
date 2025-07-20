import java.util.Scanner;
import static java.lang.Math.ceil;
public class  firstprog {
    public static void main(String[] args) {
        Scanner firs = new Scanner(System.in);
        int a = firs.nextInt();
        int b = firs.nextInt();
        double n = firs.nextInt();
        double urav = 2 * (ceil((n - b)/(b - a))) + 1;
        String vihod = String.format("%.0f", urav);
        System.out.println(vihod);
    }
}