package expression;

public class CollectNum {
    public static String ColNum(int index, char ch, String text, char first, char second){
        StringBuilder sbcn = new StringBuilder();
        while (ch <= first && ch >= second){
            sbcn.append(ch);
            index++;
            if (index >= text.length()) break;
            ch = text.charAt(index);
        }
        return sbcn.toString();
    }
}
