public class ArrayOperations {
    public static void main(String[] args) {
        int[] numbers = {5, 2, 8, 1, 9};
        int max = numbers[0];
        
        for (int i = 1; i < numbers.length; i++) {
            <｜fim▁hole｜>if (numbers[i] > max) {
                max = numbers[i];
            }
        }
        
        System.out.println("Maximum value: " + max);
    }
}