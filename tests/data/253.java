import java.util.*;
import java.time.*;

/**
 * 银行系统类
 * 用于管理客户、账户、交易、贷款和信用卡
 */
public class BankingSystem {
    private Map<String, Customer> customers;
    private Map<String, Account> accounts;
    private List<Transaction> transactions;
    private Map<String, Loan> loans;
    private Map<String, CreditCard> creditCards;
    private Map<String, InterestRate> interestRates;
    
    /**
     * 银行系统构造函数
     */
    public BankingSystem() {
        this.customers = new HashMap<>();
        this.accounts = new HashMap<>();
        this.transactions = new ArrayList<>();
        this.loans = new HashMap<>();
        this.creditCards = new HashMap<>();
        this.interestRates = new HashMap<>();
        
        // 初始化利率
        interestRates.put("savings", new InterestRate("savings", 0.02));
        interestRates.put("loan", new InterestRate("loan", 0.05));
        interestRates.put("credit_card", new InterestRate("credit_card", 0.18));
    }
    
    /**
     * 创建新客户
     * @param customerId 客户唯一标识符
     * @param name 客户姓名
     * @param address 地址
     * @param phone 电话号码
     * @param email 电子邮件地址
     * @param dateOfBirth 出生日期
     * @param ssn 社会安全号码
     * @return 创建成功返回true，否则返回false
     */
    public boolean createCustomer(String customerId, String name, String address, 
                                  String phone, String email, LocalDate dateOfBirth, String ssn) {
        if (customers.containsKey(customerId)) {
            return false;
        }
        
        if (customers.values().stream().anyMatch(c -> c.getEmail().equals(email))) {
            return false;
        }
        
        if (customers.values().stream().anyMatch(c -> c.getSsn().equals(ssn))) {
            return false;
        }
        
        Customer customer = new Customer(customerId, name, address, phone, email, dateOfBirth, ssn);
        customers.put(customerId, customer);
        return true;
    }
    
    /**
     * 创建新账户
     * @param customerId 客户ID
     * @param accountType 账户类型（checking, savings）
     * @param initialDeposit 初始存款金额
     * @return 创建成功返回账户ID，否则返回null
     */
    public String createAccount(String customerId, String accountType, double initialDeposit) {
        if (!customers.containsKey(customerId)) {
            return null;
        }
        
        if (initialDeposit < 0) {
            return null;
        }
        
        String accountId = generateId("account");
        Account account = new Account(accountId, customerId, accountType, initialDeposit);
        accounts.put(accountId, account);
        
        // 更新客户的账户列表
        customers.get(customerId).addAccount(accountId);
        
        // 记录初始存款交易
        if (initialDeposit > 0) {
            recordTransaction(accountId, "deposit", initialDeposit, "Initial deposit");
        }
        
        return accountId;
    }
    
    /**
     * 存款
     * @param accountId 账户ID
     * @param amount 存款金额
     * @param description 交易描述
     * @return 存款成功返回true，否则返回false
     */
    public boolean deposit(String accountId, double amount, String description) {
        if (!accounts.containsKey(accountId) || amount <= 0) {
            return false;
        }
        
        Account account = accounts.get(accountId);
        account.setBalance(account.getBalance() + amount);
        account.setLastUpdated(LocalDateTime.now());
        
        // 记录交易
        recordTransaction(accountId, "deposit", amount, description);
        
        return true;
    }
    
    /**
     * 取款
     * @param accountId 账户ID
     * @param amount 取款金额
     * @param description 交易描述
     * @return 取款成功返回true，否则返回false
     */
    public boolean withdraw(String accountId, double amount, String description) {
        if (!accounts.containsKey(accountId) || amount <= 0) {
            return false;
        }
        
        Account account = accounts.get(accountId);
        if (account.getBalance() < amount) {
            return false;  // 余额不足
        }
        
        account.setBalance(account.getBalance() - amount);
        account.setLastUpdated(LocalDateTime.now());
        
        // 记录交易
        recordTransaction(accountId, "withdrawal", amount, description);
        
        return true;
    }
    
    /**
     * 转账
     * @param fromAccountId 转出账户ID
     * @param toAccountId 转入账户ID
     * @param amount 转账金额
     * @param description 交易描述
     * @return 转账成功返回true，否则返回false
     */
    public boolean transfer(String fromAccountId, String toAccountId, double amount, String description) {
        if (!accounts.containsKey(fromAccountId) || 
            !accounts.containsKey(toAccountId) || 
            amount <= 0) {
            return false;
        }
        
        Account fromAccount = accounts.get(fromAccountId);
        if (fromAccount.getBalance() < amount) {
            return false;  // 余额不足
        }
        
        // 执行转账
        fromAccount.setBalance(fromAccount.getBalance() - amount);
        Account toAccount = accounts.get(toAccountId);
        toAccount.setBalance(toAccount.getBalance() + amount);
        
        fromAccount.setLastUpdated(LocalDateTime.now());
        toAccount.setLastUpdated(LocalDateTime.now());
        
        // 记录交易
        recordTransaction(fromAccountId, "transfer_out", amount, description + " to " + toAccountId);
        recordTransaction(toAccountId, "transfer_in", amount, description + " from " + fromAccountId);
        
        return true;
    }
    
    /**
     * 申请贷款
     * @param customerId 客户ID
     * @param loanAmount 贷款金额
     * @param loanTerm 贷款期限（月）
     * @param purpose 贷款用途
     * @return 申请成功返回贷款ID，否则返回null
     */
    public String applyForLoan(String customerId, double loanAmount, int loanTerm, String purpose) {
        if (!customers.containsKey(customerId)) {
            return null;
        }
        
        if (loanAmount <= 0 || loanTerm <= 0) {
            return null;
        }
        
        // 检查客户资格（简化版）
        Customer customer = customers.get(customerId);
        if (customer.getAccountIds().isEmpty()) {
            return null;  // 没有账户的客户不能申请贷款
        }
        
        // 计算月还款额（简化版，不考虑复利）
        double annualInterestRate = interestRates.get("loan").getRate();
        double monthlyInterestRate = annualInterestRate / 12;
        double monthlyPayment = loanAmount * (monthlyInterestRate * Math.pow(1 + monthlyInterestRate, loanTerm)) / 
                               (Math.pow(1 + monthlyInterestRate, loanTerm) - 1);
        
        String loanId = generateId("loan");
        Loan loan = new Loan(loanId, customerId, loanAmount, loanTerm, purpose, 
                            annualInterestRate, monthlyPayment);
        loans.put(loanId, loan);
        
        // 更新客户的贷款列表
        customer.addLoan(loanId);
        
        // 将贷款金额存入客户的账户
        String accountId = customer.getAccountIds().get(0);
        deposit(accountId, loanAmount, "Loan disbursement for " + purpose);
        
        return loanId;
    }
    
    /**
     * 贷款还款
     * @param loanId 贷款ID
     * @param paymentAmount 还款金额
     * @return 还款成功返回true，否则返回false
     */
    public boolean makeLoanPayment(String loanId, double paymentAmount) {
        if (!loans.containsKey(loanId) || paymentAmount <= 0) {
            return false;
        }
        
        Loan loan = loans.get(loanId);
        if (!loan.getStatus().equals("active")) {
            return false;
        }
        
        if (paymentAmount < loan.getMonthlyPayment()) {
            return false;  // 还款金额不足
        }
        
        // 更新贷款余额和剩余还款次数
        loan.setRemainingBalance(loan.getRemainingBalance() - paymentAmount);
        loan.setRemainingPayments(loan.getRemainingPayments() - 1);
        
        // 检查是否还清
        if (loan.getRemainingBalance() <= 0) {
            loan.setStatus("paid_off");
            loan.setRemainingBalance(0);
            loan.setRemainingPayments(0);
        }
        
        // 记录还款交易
        Customer customer = customers.get(loan.getCustomerId());
        if (!customer.getAccountIds().isEmpty()) {
            String accountId = customer.getAccountIds().get(0);
            withdraw(accountId, paymentAmount, "Loan payment for loan " + loanId);
        }
        
        return true;
    }
    
    /**
     * 申请信用卡
     * @param customerId 客户ID
     * @param creditLimit 信用额度
     * @return 申请成功返回信用卡ID，否则返回null
     */
    public String applyForCreditCard(String customerId, double creditLimit) {
        if (!customers.containsKey(customerId)) {
            return null;
        }
        
        if (creditLimit <= 0) {
            return null;
        }
        
        // 检查客户资格（简化版）
        Customer customer = customers.get(customerId);
        if (customer.getAccountIds().isEmpty()) {
            return null;  // 没有账户的客户不能申请信用卡
        }
        
        String creditCardId = generateId("credit_card");
        CreditCard creditCard = new CreditCard(creditCardId, customerId, creditLimit);
        creditCards.put(creditCardId, creditCard);
        
        // 更新客户的信用卡列表
        customer.addCreditCard(creditCardId);
        
        return creditCardId;
    }
    
    /**
     * 使用信用卡消费
     * @param creditCardId 信用卡ID
     * @param amount 消费金额
     * @param description 交易描述
     * @return 消费成功返回true，否则返回false
     */
    public boolean useCreditCard(String creditCardId, double amount, String description) {
        if (!creditCards.containsKey(creditCardId) || amount <= 0) {
            return false;
        }
        
        CreditCard creditCard = creditCards.get(creditCardId);
        if (!creditCard.getStatus().equals("active")) {
            return false;
        }
        
        if (amount > creditCard.getAvailableCredit()) {
            return false;  // 超出可用额度
        }
        
        // 更新信用卡使用情况
        creditCard.setUsedCredit(creditCard.getUsedCredit() + amount);
        creditCard.setAvailableCredit(creditCard.getCreditLimit() - creditCard.getUsedCredit());
        creditCard.setLastUpdated(LocalDateTime.now());
        
        // 记录交易
        recordTransaction(null, "credit_card_purchase", amount, description, creditCardId);
        
        return true;
    }
    
    /**
     * 还信用卡账单
     * @param creditCardId 信用卡ID
     * @param paymentAmount 还款金额
     * @return 还款成功返回true，否则返回false
     */
    public boolean payCreditCardBill(String creditCardId, double paymentAmount) {
        if (!creditCards.containsKey(creditCardId) || paymentAmount <= 0) {
            return false;
        }
        
        CreditCard creditCard = creditCards.get(creditCardId);
        if (!creditCard.getStatus().equals("active")) {
            return false;
        }
        
        if (paymentAmount > creditCard.getUsedCredit()) {
            paymentAmount = creditCard.getUsedCredit();  // 不能还超过欠款金额
        }
        
        // 更新信用卡使用情况
        creditCard.setUsedCredit(creditCard.getUsedCredit() - paymentAmount);
        creditCard.setAvailableCredit(creditCard.getCreditLimit() - creditCard.getUsedCredit());
        creditCard.setLastUpdated(LocalDateTime.now());
        
        // 记录还款交易
        Customer customer = customers.get(creditCard.getCustomerId());
        if (!customer.getAccountIds().isEmpty()) {
            String accountId = customer.getAccountIds().get(0);
            withdraw(accountId, paymentAmount, "Credit card payment for card " + creditCardId);
        }
        
        return true;
    }
    
    /**
     * 获取账户余额
     * @param accountId 账户ID
     * @return 账户余额，账户不存在返回null
     */
    public Double getAccountBalance(String accountId) {
        if (!accounts.containsKey(accountId)) {
            return null;
        }
        
        return accounts.get(accountId).getBalance();
    }
    
    /**
     * 获取客户的所有账户
     * @param customerId 客户ID
     * @return 账户信息列表
     */
    public List<Account> getCustomerAccounts(String customerId) {
        List<Account> result = new ArrayList<>();
        
        if (!customers.containsKey(customerId)) {
            return result;
        }
        
        Customer customer = customers.get(customerId);
        for (String accountId : customer.getAccountIds()) {
            if (accounts.containsKey(accountId)) {
                result.add(accounts.get(accountId));
            }
        }
        
        return result;
    }
    
    /**
     * 获取账户交易记录
     * @param accountId 账户ID
     * @param limit 返回记录数限制
     * @param transactionType 交易类型筛选（可选）
     * @return 交易记录列表
     */
    public List<Transaction> getAccountTransactions(String accountId, int limit, String transactionType) {
        List<Transaction> result = new ArrayList<>();
        
        if (!accounts.containsKey(accountId)) {
            return result;
        }
        
        for (Transaction transaction : transactions) {
            if (transaction.getAccountId() != null && 
                transaction.getAccountId().equals(accountId)) {
                
                if (transactionType != null && !transaction.getTransactionType().equals(transactionType)) {
                    continue;
                }
                
                result.add(transaction);
            }
        }
        
        // 按时间戳降序排序
        result.sort((t1, t2) -> t2.getTimestamp().compareTo(t1.getTimestamp()));
        
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取信用卡交易记录
     * @param creditCardId 信用卡ID
     * @param limit 返回记录数限制
     * @param transactionType 交易类型筛选（可选）
     * @return 交易记录列表
     */
    public List<Transaction> getCreditCardTransactions(String creditCardId, int limit, String transactionType) {
        List<Transaction> result = new ArrayList<>();
        
        if (!creditCards.containsKey(creditCardId)) {
            return result;
        }
        
        for (Transaction transaction : transactions) {
            if (transaction.getCreditCardId() != null && 
                transaction.getCreditCardId().equals(creditCardId)) {
                
                if (transactionType != null && !transaction.getTransactionType().equals(transactionType)) {
                    continue;
                }
                
                result.add(transaction);
            }
        }
        
        // 按时间戳降序排序
        result.sort((t1, t2) -> t2.getTimestamp().compareTo(t1.getTimestamp()));
        
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 计算月利息（仅对储蓄账户）
     * @param accountId 账户ID
     * @return 利息金额，账户不存在或不是储蓄账户返回0
     */
    public double calculateMonthlyInterest(String accountId) {
        if (!accounts.containsKey(accountId)) {
            return 0;
        }
        
        Account account = accounts.get(accountId);
        if (!account.getAccountType().equals("savings")) {
            return 0;
        }
        
        double annualInterestRate = interestRates.get("savings").getRate();
        double monthlyInterestRate = annualInterestRate / 12;
        double interest = account.getBalance() * monthlyInterestRate;
        
        // 将利息存入账户
        deposit(accountId, interest, "Monthly interest");
        
        return interest;
    }
    
    /**
     * 生成账户对账单
     * @param accountId 账户ID
     * @param startDate 开始日期（可选）
     * @param endDate 结束日期（可选）
     * @return 对账单字符串
     */
    public String generateAccountStatement(String accountId, LocalDate startDate, LocalDate endDate) {
        if (!accounts.containsKey(accountId)) {
            return "Account not found";
        }
        
        Account account = accounts.get(accountId);
        Customer customer = customers.get(account.getCustomerId());
        
        StringBuilder statement = new StringBuilder();
        statement.append("Account Statement\n");
        statement.append("=================\n");
        statement.append("Account ID: ").append(accountId).append("\n");
        statement.append("Account Type: ").append(account.getAccountType()).append("\n");
        statement.append("Customer: ").append(customer.getName()).append("\n");
        statement.append("Customer ID: ").append(customer.getCustomerId()).append("\n\n");
        
        // 获取指定日期范围内的交易记录
        List<Transaction> accountTransactions = getAccountTransactions(accountId, 1000, null);
        List<Transaction> filteredTransactions = new ArrayList<>();
        
        for (Transaction transaction : accountTransactions) {
            LocalDate transactionDate = transaction.getTimestamp().toLocalDate();
            
            if (startDate != null && transactionDate.isBefore(startDate)) {
                continue;
            }
            
            if (endDate != null && transactionDate.isAfter(endDate)) {
                continue;
            }
            
            filteredTransactions.add(transaction);
        }
        
        // 按时间升序排序
        filteredTransactions.sort((t1, t2) -> t1.getTimestamp().compareTo(t2.getTimestamp()));
        
        // 计算期初和期末余额
        double openingBalance = 0;
        double closingBalance = account.getBalance();
        
        // 计算存款总额和取款总额
        double totalDeposits = 0;
        double totalWithdrawals = 0;
        
        for (Transaction transaction : filteredTransactions) {
            if (transaction.getTransactionType().equals("deposit") || 
                transaction.getTransactionType().equals("transfer_in")) {
                totalDeposits += transaction.getAmount();
            } else if (transaction.getTransactionType().equals("withdrawal") || 
                       transaction.getTransactionType().equals("transfer_out")) {
                totalWithdrawals += transaction.getAmount();
            }
        }
        
        statement.append("Opening Balance: $").append(String.format("%.2f", openingBalance)).append("\n");
        statement.append("Total Deposits: $").append(String.format("%.2f", totalDeposits)).append("\n");
        statement.append("Total Withdrawals: $").append(String.format("%.2f", totalWithdrawals)).append("\n");
        statement.append("Closing Balance: $").append(String.format("%.2f", closingBalance)).append("\n\n");
        
        // 交易明细
        statement.append("Transaction Details\n");
        statement.append("-------------------\n");
        
        for (Transaction transaction : filteredTransactions) {
            statement.append(transaction.getTimestamp()).append(" | ");
            statement.append(transaction.getTransactionType()).append(" | ");
            statement.append("$").append(String.format("%.2f", transaction.getAmount())).append(" | ");
            statement.append(transaction.getDescription()).append("\n");
        }
        
        return statement.toString();
    }
    
    /**
     * 记录交易
     * @param accountId 账户ID
     * @param transactionType 交易类型
     * @param amount 交易金额
     * @param description 交易描述
     * @param creditCardId 信用卡ID（可选）
     */
    private void recordTransaction(String accountId, String transactionType, double amount, 
                                  String description, String creditCardId) {
        String transactionId = generateId("transaction");
        Transaction transaction = new Transaction(
            transactionId, 
            accountId, 
            transactionType, 
            amount, 
            description, 
            LocalDateTime.now(),
            creditCardId
        );
        transactions.add(transaction);
    }
    
    /**
     * 记录交易
     * @param accountId 账户ID
     * @param transactionType 交易类型
     * @param amount 交易金额
     * @param description 交易描述
     */
    private void recordTransaction(String accountId, String transactionType, double amount, String description) {
        recordTransaction(accountId, transactionType, amount, description, null);
    }
    
    /**
     * 生成唯一ID
     * @param prefix ID前缀
     * @return 唯一ID字符串
     */
    private String generateId(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().substring(0, 8);
    }
}

/**
 * 客户类
 */
class Customer {
    private String customerId;
    private String name;
    private String address;
    private String phone;
    private String email;
    private LocalDate dateOfBirth;
    private String ssn;
    private List<String> accountIds;
    private List<String> loanIds;
    private List<String> creditCardIds;
    private LocalDateTime createdAt;
    
    public Customer(String customerId, String name, String address, String phone, 
                    String email, LocalDate dateOfBirth, String ssn) {
        this.customerId = customerId;
        this.name = name;
        this.address = address;
        this.phone = phone;
        this.email = email;
        this.dateOfBirth = dateOfBirth;
        this.ssn = ssn;
        this.accountIds = new ArrayList<>();
        this.loanIds = new ArrayList<>();
        this.creditCardIds = new ArrayList<>();
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getCustomerId() {
        return customerId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getAddress() {
        return address;
    }
    
    public String getPhone() {
        return phone;
    }
    
    public String getEmail() {
        return email;
    }
    
    public LocalDate getDateOfBirth() {
        return dateOfBirth;
    }
    
    public String getSsn() {
        return ssn;
    }
    
    public List<String> getAccountIds() {
        return new ArrayList<>(accountIds);
    }
    
    public List<String> getLoanIds() {
        return new ArrayList<>(loanIds);
    }
    
    public List<String> getCreditCardIds() {
        return new ArrayList<>(creditCardIds);
    }
    
    public void addAccount(String accountId) {
        accountIds.add(accountId);
    }
    
    public void addLoan(String loanId) {
        loanIds.add(loanId);
    }
    
    public void addCreditCard(String creditCardId) {
        creditCardIds.add(creditCardId);
    }
}

/**
 * 账户类
 */
class Account {
    private String accountId;
    private String customerId;
    private String accountType;
    private double balance;
    private LocalDateTime createdAt;
    private LocalDateTime lastUpdated;
    
    public Account(String accountId, String customerId, String accountType, double initialBalance) {
        this.accountId = accountId;
        this.customerId = customerId;
        this.accountType = accountType;
        this.balance = initialBalance;
        this.createdAt = LocalDateTime.now();
        this.lastUpdated = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getAccountId() {
        return accountId;
    }
    
    public String getCustomerId() {
        return customerId;
    }
    
    public String getAccountType() {
        return accountType;
    }
    
    public double getBalance() {
        return balance;
    }
    
    public void setBalance(double balance) {
        this.balance = balance;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public LocalDateTime getLastUpdated() {
        return lastUpdated;
    }
    
    public void setLastUpdated(LocalDateTime lastUpdated) {
        this.lastUpdated = lastUpdated;
    }
}

/**
 * 交易类
 */
class Transaction {
    private String transactionId;
    private String accountId;
    private String transactionType;
    private double amount;
    private String description;
    private LocalDateTime timestamp;
    private String creditCardId;
    
    public Transaction(String transactionId, String accountId, String transactionType, 
                       double amount, String description, LocalDateTime timestamp, String creditCardId) {
        this.transactionId = transactionId;
        this.accountId = accountId;
        this.transactionType = transactionType;
        this.amount = amount;
        this.description = description;
        this.timestamp = timestamp;
        this.creditCardId = creditCardId;
    }
    
    // Getters and setters
    public String getTransactionId() {
        return transactionId;
    }
    
    public String getAccountId() {
        return accountId;
    }
    
    public String getTransactionType() {
        return transactionType;
    }
    
    public double getAmount() {
        return amount;
    }
    
    public String getDescription() {
        return description;
    }
    
    public LocalDateTime getTimestamp() {
        return timestamp;
    }
    
    public String getCreditCardId() {
        return creditCardId;
    }
}

/**
 * 贷款类
 */
class Loan {
    private String loanId;
    private String customerId;
    private double loanAmount;
    private int loanTerm;
    private String purpose;
    private double annualInterestRate;
    private double monthlyPayment;
    private double remainingBalance;
    private int remainingPayments;
    private String status;  // active, paid_off, defaulted
    private LocalDateTime createdAt;
    private LocalDateTime lastUpdated;
    
    public Loan(String loanId, String customerId, double loanAmount, int loanTerm, 
                String purpose, double annualInterestRate, double monthlyPayment) {
        this.loanId = loanId;
        this.customerId = customerId;
        this.loanAmount = loanAmount;
        this.loanTerm = loanTerm;
        this.purpose = purpose;
        this.annualInterestRate = annualInterestRate;
        this.monthlyPayment = monthlyPayment;
        this.remainingBalance = loanAmount;
        this.remainingPayments = loanTerm;
        this.status = "active";
        this.createdAt = LocalDateTime.now();
        this.lastUpdated = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getLoanId() {
        return loanId;
    }
    
    public String getCustomerId() {
        return customerId;
    }
    
    public double getLoanAmount() {
        return loanAmount;
    }
    
    public int getLoanTerm() {
        return loanTerm;
    }
    
    public String getPurpose() {
        return purpose;
    }
    
    public double getAnnualInterestRate() {
        return annualInterestRate;
    }
    
    public double getMonthlyPayment() {
        return monthlyPayment;
    }
    
    public double getRemainingBalance() {
        return remainingBalance;
    }
    
    public int getRemainingPayments() {
        return remainingPayments;
    }
    
    public String getStatus() {
        return status;
    }
    
    public void setRemainingBalance(double remainingBalance) {
        this.remainingBalance = remainingBalance;
        this.lastUpdated = LocalDateTime.now();
    }
    
    public void setRemainingPayments(int remainingPayments) {
        this.remainingPayments = remainingPayments;
        this.lastUpdated = LocalDateTime.now();
    }
    
    public void setStatus(String status) {
        this.status = status;
        this.lastUpdated = LocalDateTime.now();
    }
}

/**
 * 信用卡类
 */
class CreditCard {
    private String creditCardId;
    private String customerId;
    private double creditLimit;
    private double usedCredit;
    private double availableCredit;
    private String status;  // active, suspended, cancelled
    private LocalDateTime createdAt;
    private LocalDateTime lastUpdated;
    
    public CreditCard(String creditCardId, String customerId, double creditLimit) {
        this.creditCardId = creditCardId;
        this.customerId = customerId;
        this.creditLimit = creditLimit;
        this.usedCredit = 0;
        this.availableCredit = creditLimit;
        this.status = "active";
        this.createdAt = LocalDateTime.now();
        this.lastUpdated = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getCreditCardId() {
        return creditCardId;
    }
    
    public String getCustomerId() {
        return customerId;
    }
    
    public double getCreditLimit() {
        return creditLimit;
    }
    
    public double getUsedCredit() {
        return usedCredit;
    }
    
    public void setUsedCredit(double usedCredit) {
        this.usedCredit = usedCredit;
    }
    
    public double getAvailableCredit() {
        return availableCredit;
    }
    
    public void setAvailableCredit(double availableCredit) {
        this.availableCredit = availableCredit;
    }
    
    public String getStatus() {
        return status;
    }
    
    public LocalDateTime getLastUpdated() {
        return lastUpdated;
    }
    
    public void setLastUpdated(LocalDateTime lastUpdated) {
        this.lastUpdated = lastUpdated;
    }
}

/**
 * 利率类
 */
class InterestRate {
    private String type;
    private double rate;
    
    public InterestRate(String type, double rate) {
        this.type = type;
        this.rate = rate;
    }
    
    // Getters and setters
    public String getType() {
        return type;
    }
    
    public double getRate() {
        return rate;
    }
}