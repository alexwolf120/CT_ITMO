package search;

public class BinarySearchMax {
    //Contract
    //Pred args.length > 0 && a[i] < a[i + 1]
    //Post R
    public static void main(String[] args) {
        //args.length > 0 && args[i + 1] <= args[i]
        //Model:
        int[] a = new int[args.length];
        //I: args.length > 0 && args[i] < args[i + 1]
        for (int i = 0; i < a.length; i++) {
            //i' < a.length && I
            a[i] = Integer.parseInt(args[i]);
            //i' < a.length && I && a[i'] == args[i'+1]
        }
        System.out.println(binarySearchMax(a));
        //R

    }

    //Contract
    //Pred args.length > 0 && a[i] < a[i + 1]
    //Post max(a[i])
    static int binarySearchMax(int[] a) {
        int l = 0;
        // l' == l && l == 0 -> l' == 0
        int r = a.length;
        // r' == r && r == a.length -> r' == a.length
        while(l < r - 1){
            //l' < r' - 1
            int m = l + (r - l) / 2;
            // m' == l' + (r' - l') /2
            if (a[m] > a[l]) {
                //m' == l' + (r' - l') /2 && a[m] > a[l]
                l = m;
                //l' == l' + (r' - l') /2
            } else {
                //m' == l' + (r' - l') /2 && a[m] <= a[l]
                r = m;
                //r' == l' + (r' - l') /2
            }
        }
        return a[l];
        //R = a[l']
    }
}