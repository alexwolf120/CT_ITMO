public class SumFloat {
    public static void main(String[] args) {
        float sum = 0;
        for (int i = 0; i < args.length; i++) {
            int a = 0;
            int b = 0;
            for (int j = 0; j < args[i].length(); j++) {
                if(!Character.isWhitespace(args[i].charAt(j))) {
                    a = j + 1;
                    if (j == args[i].length() - 1) {
                        sum += Float.valueOf(args[i].substring(b, a));
                    }
                } else {
                    if (a > b) {
                        sum += Float.valueOf(args[i].substring(b, a));
                    }
                    b = j + 1;
                }
            }

        }
        System.out.println(sum);


    }
}
