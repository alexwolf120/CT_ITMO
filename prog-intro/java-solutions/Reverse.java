import java.util.*;

public class Reverse {
    public static void main(String[] args) {
        int two[][] = new int[1][1];
        Scanner sc = new Scanner(System.in);
        int stolb = 0;
        while (sc.hasNextLine()) {
            Scanner k = new Scanner(sc.nextLine());
            int strok = 0;
            two[stolb] = new int[1];
            while (k.hasNextInt()){
                if (strok >= two[stolb].length){
                    two[stolb] = Arrays.copyOf(two[stolb],two[stolb].length + 1);
                }
                two[stolb][strok] = k.nextInt();
                strok++;
            }
            two[stolb] = Arrays.copyOf(two[stolb], strok);
            if (strok >= two[stolb].length) {
                two = Arrays.copyOf(two, two.length + 1);
            }
            stolb++;
        }
        two = Arrays.copyOf(two, stolb);
        for (int i = stolb-1; i >= 0; i--) {
            for (int j = two[i].length-1; j >= 0; j--) {
                System.out.print(two[i][j]);
                System.out.print(" ");
            }
            System.out.println();
        }
    }
}