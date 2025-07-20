import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileReader;
import java.util.*;

public class WordStatInput {
    public static void main(String[] args) {
        Map<String, Integer> map = new LinkedHashMap<>();
        String slova = "";
        try (BufferedReader reader = new BufferedReader(new FileReader(args[0]))) {
            String stroka;
            while ((stroka = reader.readLine()) != null) {
                stroka = stroka.toLowerCase();
                stroka += " ";
                int i = 0;
                while(i < stroka.length()) {
                    if ((Character.isLetter(stroka.charAt(i))) || (stroka.charAt(i) == '\'' || (Character.DASH_PUNCTUATION == Character.getType(stroka.charAt(i)))) && !(Character.isWhitespace(stroka.charAt(i)))) {
                        while ((Character.isLetter(stroka.charAt(i)) || stroka.charAt(i) == '\'') || (Character.DASH_PUNCTUATION == Character.getType(stroka.charAt(i)))&& !(Character.isWhitespace(stroka.charAt(i)))) {
                            slova += stroka.charAt(i);
                            i++;
                        }
                        if (!map.containsKey(slova)) {
                            map.put(slova, 1);
                        } else {
                            int k = map.get(slova);
                            k++;
                            map.replace(slova, k);
                        }
                        slova = "";
                    } else {
                        i++;
                    }
                }
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        try (BufferedWriter wrott = new BufferedWriter(new FileWriter(args[1]))) {
            for (String j : map.keySet()) {
                wrott.write(j + " " + map.get(j) + "\n");
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}
