package search;
public class BinarySearch {
    //Contract
    //Pred args.length > 0 && args[i + 1] <= args[i] && args.length >= i >= 1
    //Post R
    public static void main(String[] args) {
        //args.length > 0 && args[i + 1] <= args[i]
        //Model:
        int[] a = new int[args.length - 1];
        //args.length > 0 && args[i + 1] <= args[i]
        int x = Integer.parseInt(args[0]);
        // args.length > 0 && args[i + 1] <= args[i] && x == args[0]
        //Model:
        int k = 0;
        //I: args.length > 0 && args[i + 1] <= args[i] && x == args[0]
        //k' == k
        for (int i = 0; i < a.length; i++) {
            //i' < a.length && I
            a[i] = Integer.parseInt(args[i+1]);
            k += Integer.parseInt(args[i+1]);
            //i' < a.length && I && k' == k' + args[i+1] && a[i'] == args[i'+1]
        }
        //I
        if(k % 2 != 0){
            //I && k % 2 != 0
            System.out.println(binarySearchInter(a, x));
        }else{
            //I && k % 2 == 0
            System.out.println(binarySearchRec(a, -1, a.length, x));
        }

    }
    //Contract
    //Pred args.length > 0 args[i + 1] <= args[i] args.length >= i >= 1
    //Post a[i] <= x && i == min
    static int binarySearchInter(int[] a, int x) {
        int l = 0;
        // l' == l && l == 0 -> l' == 0
        int r = a.length;
        // r' == r && r == a.length -> r' == a.length
        //i' == i
        for (int i = 0; i < a.length; i++) {
            //i' == i && i' < a.length
            int m = l + (r - l) / 2;
            //m' == l' + (r' - l') / 2
            if (a[m] > x) {
                //a[m'] > x && i < a.length
                l = m;
                //l' == m'
            } else {
                //a[m'] <= x && i < a.length
                r = m;
                //r' == m'
            }
        }
        return r;
        //R == r
    }
    //Contract
    //Pred args.length > 0 args[i + 1] <= args[i] args.length >= i >= 1
    //Post a[i] <= x && i == min
    static int binarySearchRec(int[] a, int l, int r, int x) {
        //r' == r && l' == l
        if (r - l> 1) {
            //r - l > 1 && args.length > 0 args[i + 1] <= args[i] args.length >= i >= 1
            int m = l + (r - l) / 2;
            //m' == l' + (r' - l') / 2 && r - l > 1
            if (x < a[m]) {
                // x < a[m']
                return binarySearchRec(a, m, r, x);
                // binarySearchRec(a, m', r', x)
            } else {
                // x >= a[m]
                return binarySearchRec(a, l, m, x);
                // binarySearchRec(a, m', r', x)
            }
        }
        return r;
        //R == r'
    }

}