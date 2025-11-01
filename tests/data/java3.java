import java.util.*;

public class BankAccount {
    private String accountNumber;
    private String accountHolder;
    private double balance;
    private List<String> transactionHistory;
    
    public BankAccount(String accountNumber, String accountHolder, double initialBalance) {
        this.accountNumber = accountNumber;
        this.accountHolder = accountHolder;
        this.balance = initialBalance;
        <｜fim▁hole｜>this.transactionHistory = new ArrayList<>();
        addTransaction("Account created with initial balance: $" + initialBalance);
    }
    
    public void deposit(double amount) {
        if (amount <= 0) {
            System.out.println("Deposit amount must be positive");
            return;
        }
        
        balance += amount;
        addTransaction("Deposited: $" + amount);
        System.out.println("Deposit successful. New balance: $" + balance);
    }
    
    public void withdraw(double amount) {
        if (amount <= 0) {
            System.out.println("Withdrawal amount must be positive");
            return;
        }
        
        if (amount > balance) {
            System.out.println("Insufficient funds. Current balance: $" + balance);
            return;
        }
        
        balance -= amount;
        addTransaction("Withdrew: $" + amount);
        System.out.println("Withdrawal successful. New balance: $" + balance);
    }
    
    private void addTransaction(String description) {
        transactionHistory.add(new Date() + " - " + description);
    }
    
    public static void main(String[] args) {
        BankAccount account = new BankAccount("123456789", "John Doe", 1000.0);
        account.displayBalance();
        account.deposit(500.0);
        account.withdraw(200.0);
    }
    
    public void displayBalance() {
        System.out.println("Account Number: " + accountNumber);
        System.out.println("Account Holder: " + accountHolder);
        System.out.println("Current Balance: $" + balance);
    }
}