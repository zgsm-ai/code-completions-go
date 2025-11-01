import java.io.*;
import java.util.HashMap;
import java.util.Map;

public class WordCounter {
    private Map<String, Integer> wordCounts;
    
    public WordCounter() {
        <｜fim▁hole｜>wordCounts = new HashMap<>();
    }
    
    public void processFile(String filePath) throws IOException {
        try (BufferedReader reader = new BufferedReader(new FileReader(filePath))) {
            String line;
            while ((line = reader.readLine()) != null) {
                String[] words = line.split("\\s+");
                for (String word : words) {
                    if (!word.isEmpty()) {
                        word = word.toLowerCase().replaceAll("[^a-zA-Z]", "");
                        if (!word.isEmpty()) {
                            wordCounts.put(word, wordCounts.getOrDefault(word, 0) + 1);
                        }
                    }
                }
            }
        }
    }
    
    public void displayWordCounts() {
        System.out.println("Word Count:");
        for (Map.Entry<String, Integer> entry : wordCounts.entrySet()) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }
    }
    
    public static void main(String[] args) {
        if (args.length < 1) {
            System.out.println("Usage: java WordCounter <file_path>");
            return;
        }
        
        WordCounter counter = new WordCounter();
        try {
            counter.processFile(args[0]);
            counter.displayWordCounts();
        } catch (IOException e) {
            System.err.println("Error processing file: " + e.getMessage());
        }
    }
}