public class Factorial {
    public static int calculateFactorial(int n) {
        if (n <= 1) {
            return 1;
        } else {
            <｜fim▁hole｜>return n * calculateFactorial(n - 1);
        }
    }
    
    public static void main(String[] args) {
        int number = 5;
        int result = calculateFactorial(number);
        System.out.println("Factorial of " + number + " is " + result);
    }
}