/**
 * Bank Management System
 * 
 * This class implements a comprehensive bank management system
 * that can handle accounts, transactions, customers, and loans.
 * 
 * Features:
 * - Account management (checking, savings, credit)
 * - Transaction processing
 * - Customer management
 * - Loan management
 * - Financial reporting
 * - Interest calculation
 * - Balance management
 */

import java.util.*;
import java.text.*;
import java.io.*;

/**
 * Account class represents a bank account
 */
class Account {
    private String accountNumber;
    private String accountType;
    private double balance;
    private double interestRate;
    private Date openDate;
    private Date lastTransactionDate;
    private List<Transaction> transactions;
    
    /**
     * Constructor for Account
     * @param accountNumber Account number
     * @param accountType Type of account
     * @param initialBalance Initial balance
     * @param interestRate Interest rate
     */
    public Account(String accountNumber, String accountType, double initialBalance, double interestRate) {
        this.accountNumber = accountNumber;
        this.accountType = accountType;
        this.balance = initialBalance;
        this.interestRate = interestRate;
        this.openDate = new Date();
        this.lastTransactionDate = new Date();
        this.transactions = new ArrayList<>();
    }
    
    /**
     * Get account number
     * @return Account number
     */
    public String getAccountNumber() {
        return accountNumber;
    }
    
    /**
     * Get account type
     * @return Account type
     */
    public String getAccountType() {
        return accountType;
    }
    
    /**
     * Get account balance
     * @return Account balance
     */
    public double getBalance() {
        return balance;
    }
    
    /**
     * Get interest rate
     * @return Interest rate
     */
    public double getInterestRate() {
        return interestRate;
    }
    
    /**
     * Get open date
     * @return Account open date
     */
    public Date getOpenDate() {
        return openDate;
    }
    
    /**
     * Get last transaction date
     * @return Last transaction date
     */
    public Date getLastTransactionDate() {
        return lastTransactionDate;
    }
    
    /**
     * Get transactions
     * @return List of transactions
     */
    public List<Transaction> getTransactions() {
        return transactions;
    }
    
    /**
     * Deposit money to account
     * @param amount Amount to deposit
     * @param description Transaction description
     * @return True if successful, false otherwise
     */
    public boolean deposit(double amount, String description) {
        if (amount <= 0) {
            return false;
        }
        
        balance += amount;
        lastTransactionDate = new Date();
        
        Transaction transaction = new Transaction(
            accountNumber, "DEPOSIT", amount, balance, description, lastTransactionDate
        );
        transactions.add(transaction);
        
        return true;
    }
    
    /**
     * Withdraw money from account
     * @param amount Amount to withdraw
     * @param description Transaction description
     * @return True if successful, false otherwise
     */
    public boolean withdraw(double amount, String description) {
        if (amount <= 0 || amount > balance) {
            return false;
        }
        
        balance -= amount;
        lastTransactionDate = new Date();
        
        Transaction transaction = new Transaction(
            accountNumber, "WITHDRAWAL", amount, balance, description, lastTransactionDate
        );
        transactions.add(transaction);
        
        return true;
    }
    
    /**
     * Apply interest to account
     * @return Applied interest amount
     */
    public double applyInterest() {
        double interest = balance * interestRate / 100;
        balance += interest;
        lastTransactionDate = new Date();
        
        Transaction transaction = new Transaction(
            accountNumber, "INTEREST", interest, balance, "Interest applied", lastTransactionDate
        );
        transactions.add(transaction);
        
        return interest;
    }
    
    /**
     * Get account summary
     * @return String summary
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Account Number: ").append(accountNumber).append("\n");
        sb.append("Type: ").append(accountType).append("\n");
        sb.append("Balance: $").append(String.format("%.2f", balance)).append("\n");
        sb.append("Interest Rate: ").append(String.format("%.2f%%", interestRate)).append("\n");
        sb.append("Open Date: ").append(openDate).append("\n");
        sb.append("Last Transaction: ").append(lastTransactionDate).append("\n");
        sb.append("Number of Transactions: ").append(transactions.size()).append("\n");
        
        return sb.toString();
    }
}

/**
 * Transaction class represents a bank transaction
 */
class Transaction {
    private String accountNumber;
    private String transactionType;
    private double amount;
    private double balance;
    private String description;
    private Date transactionDate;
    
    /**
     * Constructor for Transaction
     * @param accountNumber Account number
     * @param transactionType Type of transaction
     * @param amount Transaction amount
     * @param balance Balance after transaction
     * @param description Transaction description
     * @param transactionDate Transaction date
     */
    public Transaction(String accountNumber, String transactionType, double amount, 
                   double balance, String description, Date transactionDate) {
        this.accountNumber = accountNumber;
        this.transactionType = transactionType;
        this.amount = amount;
        this.balance = balance;
        this.description = description;
        this.transactionDate = transactionDate;
    }
    
    /**
     * Get account number
     * @return Account number
     */
    public String getAccountNumber() {
        return accountNumber;
    }
    
    /**
     * Get transaction type
     * @return Transaction type
     */
    public String getTransactionType() {
        return transactionType;
    }
    
    /**
     * Get transaction amount
     * @return Transaction amount
     */
    public double getAmount() {
        return amount;
    }
    
    /**
     * Get balance after transaction
     * @return Balance after transaction
     */
    public double getBalance() {
        return balance;
    }
    
    /**
     * Get transaction description
     * @return Transaction description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get transaction date
     * @return Transaction date
     */
    public Date getTransactionDate() {
        return transactionDate;
    }
    
    /**
     * Get transaction details
     * @return String representation of transaction
     */
    public String getDetails() {
        StringBuilder sb = new StringBuilder();
        sb.append("Account: ").append(accountNumber).append("\n");
        sb.append("Type: ").append(transactionType).append("\n");
        sb.append("Amount: $").append(String.format("%.2f", amount)).append("\n");
        sb.append("Balance: $").append(String.format("%.2f", balance)).append("\n");
        sb.append("Description: ").append(description).append("\n");
        sb.append("Date: ").append(transactionDate).append("\n");
        
        return sb.toString();
    }
}

/**
 * Customer class represents a bank customer
 */
class Customer {
    private String customerId;
    private String firstName;
    private String lastName;
    private String address;
    private String phone;
    private String email;
    private Date birthDate;
    private List<Account> accounts;
    
    /**
     * Constructor for Customer
     * @param customerId Customer ID
     * @param firstName First name
     * @param lastName Last name
     * @param address Address
     * @param phone Phone number
     * @param email Email address
     * @param birthDate Birth date
     */
    public Customer(String customerId, String firstName, String lastName, 
                String address, String phone, String email, Date birthDate) {
        this.customerId = customerId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.birthDate = birthDate;
        this.accounts = new ArrayList<>();
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get first name
     * @return First name
     */
    public String getFirstName() {
        return firstName;
    }
    
    /**
     * Get last name
     * @return Last name
     */
    public String getLastName() {
        return lastName;
    }
    
    /**
     * Get full name
     * @return Full name
     */
    public String getFullName() {
        return firstName + " " + lastName;
    }
    
    /**
     * Get address
     * @return Address
     */
    public String getAddress() {
        return address;
    }
    
    /**
     * Get phone number
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get email address
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get birth date
     * @return Birth date
     */
    public Date getBirthDate() {
        return birthDate;
    }
    
    /**
     * Get accounts
     * @return List of accounts
     */
    public List<Account> getAccounts() {
        return accounts;
    }
    
    /**
     * Add account to customer
     * @param account Account to add
     */
    public void addAccount(Account account) {
        accounts.add(account);
    }
    
    /**
     * Get total balance across all accounts
     * @return Total balance
     */
    public double getTotalBalance() {
        double total = 0.0;
        for (Account account : accounts) {
            total += account.getBalance();
        }
        return total;
    }
    
    /**
     * Get customer summary
     * @return String summary
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Address: ").append(address).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Birth Date: ").append(birthDate).append("\n");
        sb.append("Number of Accounts: ").append(accounts.size()).append("\n");
        sb.append("Total Balance: $").append(String.format("%.2f", getTotalBalance())).append("\n");
        
        return sb.toString();
    }
}

/**
 * Loan class represents a bank loan
 */
class Loan {
    private String loanId;
    private String customerId;
    private double loanAmount;
    private double interestRate;
    private int termMonths;
    private Date startDate;
    private Date endDate;
    private double monthlyPayment;
    private double remainingBalance;
    private boolean isActive;
    
    /**
     * Constructor for Loan
     * @param loanId Loan ID
     * @param customerId Customer ID
     * @param loanAmount Loan amount
     * @param interestRate Interest rate
     * @param termMonths Loan term in months
     */
    public Loan(String loanId, String customerId, double loanAmount, 
               double interestRate, int termMonths) {
        this.loanId = loanId;
        this.customerId = customerId;
        this.loanAmount = loanAmount;
        this.interestRate = interestRate;
        this.termMonths = termMonths;
        this.startDate = new Date();
        
        // Calculate end date
        Calendar cal = Calendar.getInstance();
        cal.setTime(startDate);
        cal.add(Calendar.MONTH, termMonths);
        this.endDate = cal.getTime();
        
        // Calculate monthly payment
        this.monthlyPayment = calculateMonthlyPayment(loanAmount, interestRate, termMonths);
        this.remainingBalance = loanAmount;
        this.isActive = true;
    }
    
    /**
     * Calculate monthly payment
     * @param principal Loan principal
     * @param annualRate Annual interest rate
     * @param months Loan term in months
     * @return Monthly payment
     */
    private double calculateMonthlyPayment(double principal, double annualRate, int months) {
        double monthlyRate = annualRate / 12 / 100;
        
        if (monthlyRate == 0) {
            return principal / months;
        }
        
        double factor = Math.pow(1 + monthlyRate, months);
        return principal * monthlyRate * factor / (factor - 1);
    }
    
    /**
     * Get loan ID
     * @return Loan ID
     */
    public String getLoanId() {
        return loanId;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get loan amount
     * @return Loan amount
     */
    public double getLoanAmount() {
        return loanAmount;
    }
    
    /**
     * Get interest rate
     * @return Interest rate
     */
    public double getInterestRate() {
        return interestRate;
    }
    
    /**
     * Get term in months
     * @return Term in months
     */
    public int getTermMonths() {
        return termMonths;
    }
    
    /**
     * Get start date
     * @return Start date
     */
    public Date getStartDate() {
        return startDate;
    }
    
    /**
     * Get end date
     * @return End date
     */
    public Date getEndDate() {
        return endDate;
    }
    
    /**
     * Get monthly payment
     * @return Monthly payment
     */
    public double getMonthlyPayment() {
        return monthlyPayment;
    }
    
    /**
     * Get remaining balance
     * @return Remaining balance
     */
    public double getRemainingBalance() {
        return remainingBalance;
    }
    
    /**
     * Check if loan is active
     * @return True if active, false otherwise
     */
    public boolean isActive() {
        return isActive;
    }
    
    /**
     * Make a payment on the loan
     * @param amount Payment amount
     * @return True if successful, false otherwise
     */
    public boolean makePayment(double amount) {
        if (!isActive || amount <= 0) {
            return false;
        }
        
        double interestPayment = remainingBalance * interestRate / 12 / 100;
        double principalPayment = amount - interestPayment;
        
        if (principalPayment < 0) {
            // Payment only covers interest
            remainingBalance -= amount;
        } else {
            remainingBalance -= principalPayment;
        }
        
        // Check if loan is paid off
        if (remainingBalance <= 0) {
            remainingBalance = 0;
            isActive = false;
        }
        
        return true;
    }
    
    /**
     * Get total interest paid so far
     * @return Total interest paid
     */
    public double getTotalInterestPaid() {
        return (loanAmount - remainingBalance);
    }
    
    /**
     * Get loan summary
     * @return String summary
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        sb.append("Loan ID: ").append(loanId).append("\n");
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Loan Amount: $").append(String.format("%.2f", loanAmount)).append("\n");
        sb.append("Interest Rate: ").append(String.format("%.2f%%", interestRate)).append("\n");
        sb.append("Term: ").append(termMonths).append(" months\n");
        sb.append("Start Date: ").append(startDate).append("\n");
        sb.append("End Date: ").append(endDate).append("\n");
        sb.append("Monthly Payment: $").append(String.format("%.2f", monthlyPayment)).append("\n");
        sb.append("Remaining Balance: $").append(String.format("%.2f", remainingBalance)).append("\n");
        sb.append("Total Interest Paid: $").append(String.format("%.2f", getTotalInterestPaid())).append("\n");
        sb.append("Status: ").append(isActive ? "Active" : "Paid Off").append("\n");
        
        return sb.toString();
    }
}

/**
 * Bank class represents the entire banking system
 */
public class Bank {
    private String bankName;
    private String bankCode;
    private List<Customer> customers;
    private List<Account> accounts;
    private List<Loan> loans;
    private Map<String, Customer> customerMap;
    private Map<String, Account> accountMap;
    private Map<String, Loan> loanMap;
    
    /**
     * Constructor for Bank
     * @param bankName Bank name
     * @param bankCode Bank code
     */
    public Bank(String bankName, String bankCode) {
        this.bankName = bankName;
        this.bankCode = bankCode;
        this.customers = new ArrayList<>();
        this.accounts = new ArrayList<>();
        this.loans = new ArrayList<>();
        this.customerMap = new HashMap<>();
        this.accountMap = new HashMap<>();
        this.loanMap = new HashMap<>();
    }
    
    /**
     * Get bank name
     * @return Bank name
     */
    public String getBankName() {
        return bankName;
    }
    
    /**
     * Get bank code
     * @return Bank code
     */
    public String getBankCode() {
        return bankCode;
    }
    
    /**
     * Get customers
     * @return List of customers
     */
    public List<Customer> getCustomers() {
        return customers;
    }
    
    /**
     * Get accounts
     * @return List of accounts
     */
    public List<Account> getAccounts() {
        return accounts;
    }
    
    /**
     * Get loans
     * @return List of loans
     */
    public List<Loan> getLoans() {
        return loans;
    }
    
    /**
     * Add a new customer
     * @param customer Customer to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCustomer(Customer customer) {
        if (customerMap.containsKey(customer.getCustomerId())) {
            return false;
        }
        
        customers.add(customer);
        customerMap.put(customer.getCustomerId(), customer);
        return true;
    }
    
    /**
     * Add a new account
     * @param account Account to add
     * @return True if added successfully, false otherwise
     */
    public boolean addAccount(Account account) {
        if (accountMap.containsKey(account.getAccountNumber())) {
            return false;
        }
        
        accounts.add(account);
        accountMap.put(account.getAccountNumber(), account);
        return true;
    }
    
    /**
     * Add a new loan
     * @param loan Loan to add
     * @return True if added successfully, false otherwise
     */
    public boolean addLoan(Loan loan) {
        if (loanMap.containsKey(loan.getLoanId())) {
            return false;
        }
        
        loans.add(loan);
        loanMap.put(loan.getLoanId(), loan);
        return true;
    }
    
    /**
     * Get customer by ID
     * @param customerId Customer ID
     * @return Customer if found, null otherwise
     */
    public Customer getCustomer(String customerId) {
        return customerMap.get(customerId);
    }
    
    /**
     * Get account by number
     * @param accountNumber Account number
     * @return Account if found, null otherwise
     */
    public Account getAccount(String accountNumber) {
        return accountMap.get(accountNumber);
    }
    
    /**
     * Get loan by ID
     * @param loanId Loan ID
     * @return Loan if found, null otherwise
     */
    public Loan getLoan(String loanId) {
        return loanMap.get(loanId);
    }
    
    /**
     * Open a new account for a customer
     * @param customerId Customer ID
     * @param accountType Account type
     * @param initialBalance Initial balance
     * @param interestRate Interest rate
     * @return Account if created successfully, null otherwise
     */
    public Account openAccount(String customerId, String accountType, 
                          double initialBalance, double interestRate) {
        Customer customer = getCustomer(customerId);
        if (customer == null) {
            return null;
        }
        
        // Generate unique account number
        String accountNumber = generateAccountNumber();
        
        Account account = new Account(accountNumber, accountType, initialBalance, interestRate);
        customer.addAccount(account);
        addAccount(account);
        
        return account;
    }
    
    /**
     * Generate a unique account number
     * @return Generated account number
     */
    private String generateAccountNumber() {
        Random random = new Random();
        int number = 10000000 + random.nextInt(90000000);
        return String.valueOf(number);
    }
    
    /**
     * Approve a loan for a customer
     * @param customerId Customer ID
     * @param loanAmount Loan amount
     * @param interestRate Interest rate
     * @param termMonths Loan term in months
     * @return Loan if approved, null otherwise
     */
    public Loan approveLoan(String customerId, double loanAmount, 
                      double interestRate, int termMonths) {
        Customer customer = getCustomer(customerId);
        if (customer == null) {
            return null;
        }
        
        // Simple loan approval logic
        // In a real system, you would check credit score, income, etc.
        if (loanAmount > 100000 || loanAmount < 1000) {
            return null;
        }
        
        // Generate unique loan ID
        String loanId = generateLoanId();
        
        Loan loan = new Loan(loanId, customerId, loanAmount, interestRate, termMonths);
        addLoan(loan);
        
        return loan;
    }
    
    /**
     * Generate a unique loan ID
     * @return Generated loan ID
     */
    private String generateLoanId() {
        Random random = new Random();
        int number = 1000 + random.nextInt(9000);
        return "L" + String.valueOf(number);
    }
    
    /**
     * Process a transaction on an account
     * @param accountNumber Account number
     * @param transactionType Transaction type
     * @param amount Transaction amount
     * @param description Transaction description
     * @return True if successful, false otherwise
     */
    public boolean processTransaction(String accountNumber, String transactionType, 
                               double amount, String description) {
        Account account = getAccount(accountNumber);
        if (account == null) {
            return false;
        }
        
        if (transactionType.equals("DEPOSIT")) {
            return account.deposit(amount, description);
        } else if (transactionType.equals("WITHDRAWAL")) {
            return account.withdraw(amount, description);
        } else {
            return false;
        }
    }
    
    /**
     * Apply interest to all accounts
     */
    public void applyInterestToAllAccounts() {
        for (Account account : accounts) {
            account.applyInterest();
        }
    }
    
    /**
     * Get total deposits in a date range
     * @param startDate Start date
     * @param endDate End date
     * @return Total deposits
     */
    public double getTotalDeposits(Date startDate, Date endDate) {
        double total = 0.0;
        
        for (Account account : accounts) {
            for (Transaction transaction : account.getTransactions()) {
                Date transactionDate = transaction.getTransactionDate();
                if (!transactionDate.before(startDate) && !transactionDate.after(endDate)) {
                    if (transaction.getTransactionType().equals("DEPOSIT")) {
                        total += transaction.getAmount();
                    }
                }
            }
        }
        
        return total;
    }
    
    /**
     * Get total withdrawals in a date range
     * @param startDate Start date
     * @param endDate End date
     * @return Total withdrawals
     */
    public double getTotalWithdrawals(Date startDate, Date endDate) {
        double total = 0.0;
        
        for (Account account : accounts) {
            for (Transaction transaction : account.getTransactions()) {
                Date transactionDate = transaction.getTransactionDate();
                if (!transactionDate.before(startDate) && !transactionDate.after(endDate)) {
                    if (transaction.getTransactionType().equals("WITHDRAWAL")) {
                        total += transaction.getAmount();
                    }
                }
            }
        }
        
        return total;
    }
    
    /**
     * Get total balance across all accounts
     * @return Total balance
     */
    public double getTotalBalance() {
        double total = 0.0;
        
        for (Account account : accounts) {
            total += account.getBalance();
        }
        
        return total;
    }
    
    /**
     * Get total loan amount across all loans
     * @return Total loan amount
     */
    public double getTotalLoanAmount() {
        double total = 0.0;
        
        for (Loan loan : loans) {
            if (loan.isActive()) {
                total += loan.getRemainingBalance();
            }
        }
        
        return total;
    }
    
    /**
     * Generate bank report
     * @return Bank report
     */
    public String generateReport() {
        StringBuilder sb = new StringBuilder();
        sb.append("========================================\n");
        sb.append("           BANK REPORT\n");
        sb.append("========================================\n");
        sb.append("Bank Name: ").append(bankName).append("\n");
        sb.append("Bank Code: ").append(bankCode).append("\n");
        sb.append("Report Date: ").append(new Date()).append("\n\n");
        
        sb.append("SUMMARY STATISTICS\n");
        sb.append("-------------------\n");
        sb.append("Number of Customers: ").append(customers.size()).append("\n");
        sb.append("Number of Accounts: ").append(accounts.size()).append("\n");
        sb.append("Number of Loans: ").append(loans.size()).append("\n");
        sb.append("Total Balance: $").append(String.format("%.2f", getTotalBalance())).append("\n");
        sb.append("Total Active Loans: $").append(String.format("%.2f", getTotalLoanAmount())).append("\n\n");
        
        sb.append("ACCOUNT TYPES\n");
        sb.append("-------------\n");
        Map<String, Integer> accountTypes = new HashMap<>();
        for (Account account : accounts) {
            String type = account.getAccountType();
            accountTypes.put(type, accountTypes.getOrDefault(type, 0) + 1);
        }
        
        for (Map.Entry<String, Integer> entry : accountTypes.entrySet()) {
            sb.append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nLOAN STATISTICS\n");
        sb.append("----------------\n");
        int activeLoans = 0;
        int paidOffLoans = 0;
        double totalLoanValue = 0.0;
        double totalInterestCollected = 0.0;
        
        for (Loan loan : loans) {
            if (loan.isActive()) {
                activeLoans++;
                totalLoanValue += loan.getRemainingBalance();
            } else {
                paidOffLoans++;
                totalInterestCollected += (loan.getLoanAmount() - loan.getRemainingBalance());
            }
        }
        
        sb.append("Active Loans: ").append(activeLoans).append("\n");
        sb.append("Paid Off Loans: ").append(paidOffLoans).append("\n");
        sb.append("Total Active Loan Value: $").append(String.format("%.2f", totalLoanValue)).append("\n");
        sb.append("Total Interest Collected: $").append(String.format("%.2f", totalInterestCollected)).append("\n");
        
        return sb.toString();
    }
    
    /**
     * Save bank data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("BANK DATA EXPORT");
            writer.println("================");
            writer.println("Bank Name: " + bankName);
            writer.println("Bank Code: " + bankCode);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export customers
            writer.println("CUSTOMERS");
            writer.println("---------");
            for (Customer customer : customers) {
                writer.println("ID: " + customer.getCustomerId());
                writer.println("Name: " + customer.getFullName());
                writer.println("Address: " + customer.getAddress());
                writer.println("Phone: " + customer.getPhone());
                writer.println("Email: " + customer.getEmail());
                writer.println("Birth Date: " + customer.getBirthDate());
                writer.println("Number of Accounts: " + customer.getAccounts().size());
                writer.println("Total Balance: $" + String.format("%.2f", customer.getTotalBalance()));
                writer.println();
            }
            
            // Export accounts
            writer.println("ACCOUNTS");
            writer.println("--------");
            for (Account account : accounts) {
                writer.println("Number: " + account.getAccountNumber());
                writer.println("Type: " + account.getAccountType());
                writer.println("Balance: $" + String.format("%.2f", account.getBalance()));
                writer.println("Interest Rate: " + String.format("%.2f%%", account.getInterestRate()));
                writer.println("Open Date: " + account.getOpenDate());
                writer.println("Last Transaction: " + account.getLastTransactionDate());
                writer.println("Number of Transactions: " + account.getTransactions().size());
                writer.println();
            }
            
            // Export loans
            writer.println("LOANS");
            writer.println("-----");
            for (Loan loan : loans) {
                writer.println("ID: " + loan.getLoanId());
                writer.println("Customer ID: " + loan.getCustomerId());
                writer.println("Loan Amount: $" + String.format("%.2f", loan.getLoanAmount()));
                writer.println("Interest Rate: " + String.format("%.2f%%", loan.getInterestRate()));
                writer.println("Term: " + loan.getTermMonths() + " months");
                writer.println("Start Date: " + loan.getStartDate());
                writer.println("End Date: " + loan.getEndDate());
                writer.println("Monthly Payment: $" + String.format("%.2f", loan.getMonthlyPayment()));
                writer.println("Remaining Balance: $" + String.format("%.2f", loan.getRemainingBalance()));
                writer.println("Total Interest Paid: $" + String.format("%.2f", loan.getTotalInterestPaid()));
                writer.println("Status: " + (loan.isActive() ? "Active" : "Paid Off"));
                writer.println();
            }
            
            writer.close();
            return true;
        } catch (IOException e) {
            System.err.println("Error saving to file: " + e.getMessage());
            return false;
        }
    }
    
    /**
     * Main method for demonstration
     */
    public static void main(String[] args) {
        // Create a new bank
        Bank bank = new Bank("National Bank", "NB-001");
        
        // Create some customers
        Customer customer1 = new Customer("C001", "John", "Smith", 
                                    "123 Main St, New York, NY 10001", 
                                    "(555) 123-4567", "john.smith@email.com",
                                    new Date(1980, 5, 15));
        
        Customer customer2 = new Customer("C002", "Jane", "Doe", 
                                    "456 Oak Ave, Los Angeles, CA 90001", 
                                    "(555) 987-6543", "jane.doe@email.com",
                                    new Date(1985, 8, 22));
        
        Customer customer3 = new Customer("C003", "Bob", "Johnson", 
                                    "789 Pine Rd, Chicago, IL 60007", 
                                    "(555) 246-8135", "bob.johnson@email.com",
                                    new Date(1978, 3, 10));
        
        // Add customers to the bank
        bank.addCustomer(customer1);
        bank.addCustomer(customer2);
        bank.addCustomer(customer3);
        
        // Open accounts for customers
        Account account1 = bank.openAccount("C001", "CHECKING", 5000.00, 0.01);
        Account account2 = bank.openAccount("C001", "SAVINGS", 10000.00, 0.02);
        Account account3 = bank.openAccount("C002", "CHECKING", 3000.00, 0.01);
        Account account4 = bank.openAccount("C002", "SAVINGS", 15000.00, 0.02);
        Account account5 = bank.openAccount("C003", "CHECKING", 2500.00, 0.01);
        
        // Process some transactions
        bank.processTransaction("100000001", "DEPOSIT", 1000.00, "Salary deposit");
        bank.processTransaction("100000002", "WITHDRAWAL", 500.00, "Rent payment");
        bank.processTransaction("100000003", "DEPOSIT", 2000.00, "Investment return");
        
        // Apply interest to all accounts
        bank.applyInterestToAllAccounts();
        
        // Approve some loans
        Loan loan1 = bank.approveLoan("C001", 5000.00, 3.5, 60);
        Loan loan2 = bank.approveLoan("C002", 10000.00, 4.0, 120);
        Loan loan3 = bank.approveLoan("C003", 2000.00, 3.0, 36);
        
        // Make some loan payments
        loan1.makePayment(150.00);
        loan1.makePayment(150.00);
        loan2.makePayment(200.00);
        
        // Generate and print bank report
        String report = bank.generateReport();
        System.out.println(report);
        
        // Print customer summaries
        System.out.println("\n\nCUSTOMER DETAILS");
        System.out.println("================");
        System.out.println(customer1.getSummary());
        System.out.println("\n" + customer2.getSummary());
        System.out.println("\n" + customer3.getSummary());
        
        // Print account summaries
        System.out.println("\n\nACCOUNT DETAILS");
        System.out.println("=================");
        System.out.println(account1.getSummary());
        System.out.println("\n" + account2.getSummary());
        System.out.println("\n" + account3.getSummary());
        System.out.println("\n" + account4.getSummary());
        System.out.println("\n" + account5.getSummary());
        
        // Print loan summaries
        System.out.println("\n\nLOAN DETAILS");
        System.out.println("===============");
        System.out.println(loan1.getSummary());
        System.out.println("\n" + loan2.getSummary());
        System.out.println("\n" + loan3.getSummary());
        
        // Save bank data to file
        bank.saveToFile("bank_data_export.txt");
        
        System.out.println("\n\nBank data exported to bank_data_export.txt");
        System.out.println("Bank Management System demonstration completed successfully!");
    }
}
<｜fim▁hole｜>