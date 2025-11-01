import java.io.FileWriter;
import java.io.IOException;

public class FileOperations {
    public static void main(String[] args) {
        String fileName = "example.txt";
        try {
            FileWriter writer = new FileWriter(fileName);
            writer.write("Hello, Java!");
            writer.close();
            System.out.println("File created successfully.");
        } catch (IOException e) {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }<｜fim▁hole｜>
    }
}