package info.kgeorgiy.ja.mitropolov;

import java.io.*;
import java.nio.charset.StandardCharsets;
import java.util.*;

// :NOTE: # нет i18n и i10n
public class Leitner {

    private static final String DICT_FILE = "dictionary.txt";
    private static final String BUCKET_FILE = "buckets.txt";
    private static final int NUM_BOXES = 10;
    private static final double BASE_WEIGHT = 1.5;

    private static final double[] WEIGHTS = new double[NUM_BOXES];

    static {
        for (int i = 0; i < NUM_BOXES; i++) {
            WEIGHTS[i] = Math.pow(BASE_WEIGHT, -(i + 1));
        }
    }

    private static class Dictionary {
        String word;
        String translation;

        Dictionary(String w, String t) {
            word = w;
            translation = t;
        }
    }

    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in, StandardCharsets.UTF_8)) {
            Map<Integer, List<Dictionary>> boxes = loadBucket();
            Random random = new Random();
            String lastWord = null;
            System.out.println("Варианты:");
            System.out.println(" Введите перевод ");
            System.out.println(" 1 — посмотреть распределение");
            System.out.println(" Пустая строка — сохранить и выйти\n");

            while (true) {
                Dictionary e;
                int box;
                do {
                    box = chooseBox(boxes, random);
                    List<Dictionary> list = boxes.get(box);
                    e = list.get(random.nextInt(list.size()));
                    if (distinctWordCount(boxes) < 2) break;
                } while (e.word.equals(lastWord));

                lastWord = e.word;

                System.out.printf("Ящик %d - Переведите «%s» ==", box, e.word);
                String ans = scanner.nextLine().trim();

                if (ans.isEmpty()) {
                    System.out.println("Сохранение и выход");
                    saveBucket(boxes);
                    break;
                }
                if (ans.equalsIgnoreCase("1")) {
                    showBoxes(boxes);
                    continue;
                }

                int newBox;
                if (ans.equalsIgnoreCase(e.translation)) {
                    newBox = Math.min(box + 1, NUM_BOXES);
                    System.out.println("Правильный перевод");
                } else {
                    newBox = 1;
                    System.out.printf("Неправильно, правильно: «%s»\n", e.translation);
                }
                boxes.get(box).remove(e);
                boxes.get(newBox).add(e);

                saveBucket(boxes);
            }
        } catch (Exception e) {
            System.err.println("Произошла ошибка " + e.getMessage());
        }

    }

    private static Map<Integer, List<Dictionary>> loadBucket() {
        File f = new File(BUCKET_FILE);
        Map<Integer, List<Dictionary>> boxes = new HashMap<>();
        for (int i = 1; i <= NUM_BOXES; i++) {
            boxes.put(i, new ArrayList<>());
        }

        if (f.exists()) {
            try (BufferedReader br = new BufferedReader(
                    new InputStreamReader(new FileInputStream(f), StandardCharsets.UTF_8))) {
                String line;
                while ((line = br.readLine()) != null) {
                    line = line.trim();
                    if (line.isEmpty() || line.startsWith("#")) continue;
                    String[] parts = line.split("\\|", 3);
                    // :NOTE: * сообщать об ошибках?
                    if (parts.length == 3) {
                        int box = Integer.parseInt(parts[0].trim());
                        String w = parts[1].trim();
                        String t = parts[2].trim();
                        if (box >= 1 && box <= NUM_BOXES) {
                            boxes.get(box).add(new Dictionary(w, t));
                        }
                    }
                }
            } catch (IOException ex) {
                System.err.println("Не удалось загрузить состояние");
                return initFromDict();
            }
            if (distinctWordCount(boxes) == 0) {
                System.err.println("buckets.txt пуст — инициализирую из dictionary.txt");
                return initFromDict();
            }

            Set<String> neWords = new HashSet<>();
            for (List<Dictionary> list : boxes.values()) {
                for (Dictionary d : list) {
                    neWords.add(d.word);
                }
            }

            List<Dictionary> allFromDict;
            try {
                allFromDict = loadDictionary();
            } catch (IOException e) {
                System.err.println("Ошибка при загрузке dictionary.txt: " + e.getMessage());
                return boxes;
            }

            for (Dictionary d : allFromDict) {
                if (!neWords.contains(d.word)) {
                    boxes.get(1).add(d);
                }
            }

            return boxes;
        } else {
            return initFromDict();
        }
    }

    private static int distinctWordCount(Map<Integer, List<Dictionary>> boxes) {
        Set<String> uniq = new HashSet<>();
        // :NOTE: - код стайл
        for (List<Dictionary> lst : boxes.values()) {
            for (Dictionary e : lst) uniq.add(e.word);
        }
        return uniq.size();
    }

    private static Map<Integer, List<Dictionary>> initFromDict() {
        List<Dictionary> all;
        try {
            all = loadDictionary();
        } catch (IOException e) {
            System.err.println("Ошибка при инициализации из словаря: " + e.getMessage());
            return new HashMap<>();
        }
        Map<Integer, List<Dictionary>> boxes = new HashMap<>();
        for (int i = 1; i <= NUM_BOXES; i++) {
            boxes.put(i, new ArrayList<>());
        }
        boxes.get(1).addAll(all);
        return boxes;
    }

    private static List<Dictionary> loadDictionary() throws IOException {
        List<Dictionary> list = new ArrayList<>();
        File dictFile = new File(DICT_FILE);
        if (!dictFile.exists()) {
            throw new FileNotFoundException("Файл словаря не найден: " + DICT_FILE);
        }
        try (BufferedReader br = new BufferedReader(
                new InputStreamReader(new FileInputStream(dictFile), StandardCharsets.UTF_8))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = line.trim();
                if (line.isEmpty() || line.startsWith("#")) continue;
                String[] parts = line.split("\\|", 2);
                if (parts.length == 2) {
                    list.add(new Dictionary(parts[0].trim(), parts[1].trim()));
                }
            }
        }
        return list;
    }

    private static void saveBucket(Map<Integer, List<Dictionary>> boxes) {
        try (BufferedWriter bw = new BufferedWriter(
                new OutputStreamWriter(new FileOutputStream(BUCKET_FILE), StandardCharsets.UTF_8))) {
            for (int i = 1; i <= NUM_BOXES; i++) {
                for (Dictionary e : boxes.get(i)) {
                    bw.write(i + "|" + e.word + "|" + e.translation);
                    bw.newLine();
                }
            }
        } catch (IOException ex) {
            System.err.println("Ошибка при сохранении: " + ex.getMessage());
        }
    }

    private static int chooseBox(Map<Integer, List<Dictionary>> boxes, Random random) {
        double total = 0;
        List<Integer> arr = new ArrayList<>();
        List<Double> weightList = new ArrayList<>();
        for (int i = 1; i <= NUM_BOXES; i++) {
            if (!boxes.get(i).isEmpty()) {
                arr.add(i);
                weightList.add(WEIGHTS[i - 1]);
                total += WEIGHTS[i - 1];
            }
        }
        double r = random.nextDouble() * total, acc = 0;
        for (int i = 0; i < arr.size(); i++) {
            acc += weightList.get(i);
            if (r <= acc) return arr.get(i);
        }
        return arr.get(arr.size() - 1);
    }

    private static void showBoxes(Map<Integer, List<Dictionary>> boxes) {
        System.out.println("\nТекущее распределение:");
        for (int i = 1; i <= NUM_BOXES; i++) {
            System.out.printf("  Ящик %2d: %3d слов\n", i, boxes.get(i).size());
        }
        System.out.println();
    }
}
