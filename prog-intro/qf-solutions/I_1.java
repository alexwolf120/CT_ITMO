import java.util.*;

import static java.lang.Math.*;

public class  secondprog {
    public static void main(String[] args) {
        Scanner sec = new Scanner(System.in);
        int n = sec.nextInt();
        int [] mas = new int [3*n];
        for(int i = 0; i < mas.length; i++) {
            mas[i] = sec.nextInt();
        }
        int x1 = mas[0] - mas[2];
        int x2 = mas[0] + mas[2];
        int y1 = mas[1] - mas[2];
        int y2 = mas[1] + mas[2];
        for (int j = 3; j < mas.length - 2; j++) {
            x1 = min(x1,mas[j] - mas[j+2]);
            x2 = max(x2,mas[j] + mas[j+2]);
            y1 = min(y1,mas[j+1] - mas[j+2]);
            y2 = max(y2,mas[j+1] + mas[j+2]);
        }
        int x = (x1 + x2)/2;
        int y = (y1 + y2)/2;
        int h = (int)ceil((double)(max(x2 - x1, y2 - y1))/2);
        System.out.println(x + " " + y + " " + h);
    }
}