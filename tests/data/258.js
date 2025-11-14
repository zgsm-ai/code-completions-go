/**
 * 银行系统类
 * 用于管理客户、账户、交易、贷款和信用卡
 */
class BankingSystem {
    /**
     * 构造函数
     */
    constructor() {
        this.customers = new Map();   // 客户ID到客户信息的映射
        this.accounts = new Map();     // 账户ID到账户信息的映射
        this.transactions = [];       // 交易记录
        this.loans = new Map();        // 贷款ID到贷款信息的映射
        this.creditCards = new Map();   // 信用卡ID到信用卡信息的映射
        this.interestRates = {
            savings: 0.02,  // 储蓄利率
            loan: 0.05,     // 贷款利率
            creditCard: 0.18 // 信用卡利率
        };
    }
    
    /**
     * 创建新客户
     * @param {string} customerId - 客户唯一标识符
     * @param {string} name - 客户姓名
     * @param {string} address - 地址
     * @param {string} phone - 电话号码
     * @param {string} email - 电子邮件地址
     * @param {Date} dateOfBirth - 出生日期
     * @param {string} ssn - 社会安全号码
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createCustomer(customerId, name, address, phone, email, dateOfBirth, ssn) {
        if (this.customers.has(customerId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const customer of this.customers.values()) {
            if (customer.email === email) {
                return false;
            }
        }
        
        // 检查社会安全号码是否已被使用
        for (const customer of this.customers.values()) {
            if (customer.ssn === ssn) {
                return false;
            }
        }
        
        const customer = {
            customerId,
            name,
            address,
            phone,
            email,
            dateOfBirth,
            ssn,
            accountIds: [],
            loanIds: [],
            creditCardIds: [],
            createdAt: new Date()
        };
        
        this.customers.set(customerId, customer);
        return true;
    }
    
    /**
     * 创建新账户
     * @param {string} customerId - 客户ID
     * @param {string} accountType - 账户类型（checking, savings）
     * @param {number} initialDeposit - 初始存款金额
     * @returns {string|null} 创建成功返回账户ID，否则返回null
     */
    createAccount(customerId, accountType, initialDeposit) {
        if (!this.customers.has(customerId)) {
            return null;
        }
        
        if (initialDeposit < 0) {
            return null;
        }
        
        const accountId = this.generateId('account');
        const account = {
            accountId,
            customerId,
            accountType,
            balance: initialDeposit,
            createdAt: new Date(),
            updatedAt: new Date()
        };
        
        this.accounts.set(accountId, account);
        
        // 更新客户的账户列表
        this.customers.get(customerId).accountIds.push(accountId);
        
        // 记录初始存款交易
        if (initialDeposit > 0) {
            this.recordTransaction(accountId, 'deposit', initialDeposit, 'Initial deposit');
        }
        
        return accountId;
    }
    
    /**
     * 存款
     * @param {string} accountId - 账户ID
     * @param {number} amount - 存款金额
     * @param {string} description - 交易描述
     * @returns {boolean} 存款成功返回true，否则返回false
     */
    deposit(accountId, amount, description) {
        if (!this.accounts.has(accountId) || amount <= 0) {
            return false;
        }
        
        const account = this.accounts.get(accountId);
        account.balance += amount;
        account.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(accountId, 'deposit', amount, description);
        
        return true;
    }
    
    /**
     * 取款
     * @param {string} accountId - 账户ID
     * @param {number} amount - 取款金额
     * @param {string} description - 交易描述
     * @returns {boolean} 取款成功返回true，否则返回false
     */
    withdraw(accountId, amount, description) {
        if (!this.accounts.has(accountId) || amount <= 0) {
            return false;
        }
        
        const account = this.accounts.get(accountId);
        if (account.balance < amount) {
            return false;  // 余额不足
        }
        
        account.balance -= amount;
        account.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(accountId, 'withdrawal', amount, description);
        
        return true;
    }
    
    /**
     * 转账
     * @param {string} fromAccountId - 转出账户ID
     * @param {string} toAccountId - 转入账户ID
     * @param {number} amount - 转账金额
     * @param {string} description - 交易描述
     * @returns {boolean} 转账成功返回true，否则返回false
     */
    transfer(fromAccountId, toAccountId, amount, description) {
        if (!this.accounts.has(fromAccountId) || 
            !this.accounts.has(toAccountId) || 
            amount <= 0) {
            return false;
        }
        
        const fromAccount = this.accounts.get(fromAccountId);
        if (fromAccount.balance < amount) {
            return false;  // 余额不足
        }
        
        // 执行转账
        fromAccount.balance -= amount;
        const toAccount = this.accounts.get(toAccountId);
        toAccount.balance += amount;
        
        fromAccount.updatedAt = new Date();
        toAccount.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(fromAccountId, 'transfer_out', amount, description + ' to ' + toAccountId);
        this.recordTransaction(toAccountId, 'transfer_in', amount, description + ' from ' + fromAccountId);
        
        return true;
    }
    
    /**
     * 申请贷款
     * @param {string} customerId - 客户ID
     * @param {number} loanAmount - 贷款金额
     * @param {number} loanTerm - 贷款期限（月）
     * @param {string} purpose - 贷款用途
     * @returns {string|null} 申请成功返回贷款ID，否则返回null
     */
    applyForLoan(customerId, loanAmount, loanTerm, purpose) {
        if (!this.customers.has(customerId)) {
            return null;
        }
        
        if (loanAmount <= 0 || loanTerm <= 0) {
            return null;
        }
        
        // 检查客户资格（简化版）
        const customer = this.customers.get(customerId);
        if (customer.accountIds.length === 0) {
            return null;  // 没有账户的客户不能申请贷款
        }
        
        // 计算月还款额（简化版，不考虑复利）
        const annualInterestRate = this.interestRates.loan;
        const monthlyInterestRate = annualInterestRate / 12;
        const monthlyPayment = loanAmount * (monthlyInterestRate * Math.pow(1 + monthlyInterestRate, loanTerm)) / 
                               (Math.pow(1 + monthlyInterestRate, loanTerm) - 1);
        
        const loanId = this.generateId('loan');
        const loan = {
            loanId,
            customerId,
            loanAmount,
            loanTerm,
            purpose,
            annualInterestRate,
            monthlyPayment,
            remainingBalance: loanAmount,
            remainingPayments: loanTerm,
            status: 'active',  // active, paid_off, defaulted
            createdAt: new Date(),
            updatedAt: new Date()
        };
        
        this.loans.set(loanId, loan);
        
        // 更新客户的贷款列表
        customer.loanIds.push(loanId);
        
        // 将贷款金额存入客户的账户
        const accountId = customer.accountIds[0];
        this.deposit(accountId, loanAmount, `Loan disbursement for ${purpose}`);
        
        return loanId;
    }
    
    /**
     * 贷款还款
     * @param {string} loanId - 贷款ID
     * @param {number} paymentAmount - 还款金额
     * @returns {boolean} 还款成功返回true，否则返回false
     */
    makeLoanPayment(loanId, paymentAmount) {
        if (!this.loans.has(loanId) || paymentAmount <= 0) {
            return false;
        }
        
        const loan = this.loans.get(loanId);
        if (loan.status !== 'active') {
            return false;
        }
        
        if (paymentAmount < loan.monthlyPayment) {
            return false;  // 还款金额不足
        }
        
        // 更新贷款余额和剩余还款次数
        loan.remainingBalance -= paymentAmount;
        loan.remainingPayments--;
        loan.updatedAt = new Date();
        
        // 检查是否还清
        if (loan.remainingBalance <= 0) {
            loan.status = 'paid_off';
            loan.remainingBalance = 0;
            loan.remainingPayments = 0;
        }
        
        // 记录还款交易
        const customer = this.customers.get(loan.customerId);
        if (customer.accountIds.length > 0) {
            const accountId = customer.accountIds[0];
            this.withdraw(accountId, paymentAmount, `Loan payment for loan ${loanId}`);
        }
        
        return true;
    }
    
    /**
     * 申请信用卡
     * @param {string} customerId - 客户ID
     * @param {number} creditLimit - 信用额度
     * @returns {string|null} 申请成功返回信用卡ID，否则返回null
     */
    applyForCreditCard(customerId, creditLimit) {
        if (!this.customers.has(customerId)) {
            return null;
        }
        
        if (creditLimit <= 0) {
            return null;
        }
        
        // 检查客户资格（简化版）
        const customer = this.customers.get(customerId);
        if (customer.accountIds.length === 0) {
            return null;  // 没有账户的客户不能申请信用卡
        }
        
        const creditCardId = this.generateId('credit_card');
        const creditCard = {
            creditCardId,
            customerId,
            creditLimit,
            usedCredit: 0,
            availableCredit: creditLimit,
            status: 'active',  // active, suspended, cancelled
            createdAt: new Date(),
            updatedAt: new Date()
        };
        
        this.creditCards.set(creditCardId, creditCard);
        
        // 更新客户的信用卡列表
        customer.creditCardIds.push(creditCardId);
        
        return creditCardId;
    }
    
    /**
     * 使用信用卡消费
     * @param {string} creditCardId - 信用卡ID
     * @param {number} amount - 消费金额
     * @param {string} description - 交易描述
     * @returns {boolean} 消费成功返回true，否则返回false
     */
    useCreditCard(creditCardId, amount, description) {
        if (!this.creditCards.has(creditCardId) || amount <= 0) {
            return false;
        }
        
        const creditCard = this.creditCards.get(creditCardId);
        if (creditCard.status !== 'active') {
            return false;
        }
        
        if (amount > creditCard.availableCredit) {
            return false;  // 超出可用额度
        }
        
        // 更新信用卡使用情况
        creditCard.usedCredit += amount;
        creditCard.availableCredit = creditCard.creditLimit - creditCard.usedCredit;
        creditCard.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(null, 'credit_card_purchase', amount, description, creditCardId);
        
        return true;
    }
    
    /**
     * 还信用卡账单
     * @param {string} creditCardId - 信用卡ID
     * @param {number} paymentAmount - 还款金额
     * @returns {boolean} 还款成功返回true，否则返回false
     */
    payCreditCardBill(creditCardId, paymentAmount) {
        if (!this.creditCards.has(creditCardId) || paymentAmount <= 0) {
            return false;
        }
        
        const creditCard = this.creditCards.get(creditCardId);
        if (creditCard.status !== 'active') {
            return false;
        }
        
        if (paymentAmount > creditCard.usedCredit) {
            paymentAmount = creditCard.usedCredit;  // 不能还超过欠款金额
        }
        
        // 更新信用卡使用情况
        creditCard.usedCredit -= paymentAmount;
        creditCard.availableCredit = creditCard.creditLimit - creditCard.usedCredit;
        creditCard.updatedAt = new Date();
        
        // 记录还款交易
        const customer = this.customers.get(creditCard.customerId);
        if (customer.accountIds.length > 0) {
            const accountId = customer.accountIds[0];
            this.withdraw(accountId, paymentAmount, `Credit card payment for card ${creditCardId}`);
        }
        
        return true;
    }
    
    /**
     * 获取账户余额
     * @param {string} accountId - 账户ID
     * @returns {number|null} 账户余额，账户不存在返回null
     */
    getAccountBalance(accountId) {
        if (!this.accounts.has(accountId)) {
            return null;
        }
        
        return this.accounts.get(accountId).balance;
    }
    
    /**
     * 获取客户的所有账户
     * @param {string} customerId - 客户ID
     * @returns {Array} 账户信息列表
     */
    getCustomerAccounts(customerId) {
        const result = [];
        
        if (!this.customers.has(customerId)) {
            return result;
        }
        
        const customer = this.customers.get(customerId);
        for (const accountId of customer.accountIds) {
            if (this.accounts.has(accountId)) {
                result.push(this.accounts.get(accountId));
            }
        }
        
        return result;
    }
    
    /**
     * 获取账户交易记录
     * @param {string} accountId - 账户ID
     * @param {number} limit - 返回记录数限制
     * @param {string} transactionType - 交易类型筛选（可选）
     * @returns {Array} 交易记录列表
     */
    getAccountTransactions(accountId, limit, transactionType) {
        const result = [];
        
        if (!this.accounts.has(accountId)) {
            return result;
        }
        
        for (const transaction of this.transactions) {
            if (transaction.accountId && transaction.accountId === accountId) {
                if (transactionType && transaction.transactionType !== transactionType) {
                    continue;
                }
                
                result.push(transaction);
            }
        }
        
        // 按时间戳降序排序
        result.sort((a, b) => b.timestamp - a.timestamp);
        
        if (result.length > limit) {
            return result.slice(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取信用卡交易记录
     * @param {string} creditCardId - 信用卡ID
     * @param {number} limit - 返回记录数限制
     * @param {string} transactionType - 交易类型筛选（可选）
     * @returns {Array} 交易记录列表
     */
    getCreditCardTransactions(creditCardId, limit, transactionType) {
        const result = [];
        
        if (!this.creditCards.has(creditCardId)) {
            return result;
        }
        
        for (const transaction of this.transactions) {
            if (transaction.creditCardId && transaction.creditCardId === creditCardId) {
                if (transactionType && transaction.transactionType !== transactionType) {
                    continue;
                }
                
                result.push(transaction);
            }
        }
        
        // 按时间戳降序排序
        result.sort((a, b) => b.timestamp - a.timestamp);
        
        if (result.length > limit) {
            return result.slice(0, limit);
        }
        
        return result;
    }
    
    /**
     * 计算月利息（仅对储蓄账户）
     * @param {string} accountId - 账户ID
     * @returns {number} 利息金额，账户不存在或不是储蓄账户返回0
     */
    calculateMonthlyInterest(accountId) {
        if (!this.accounts.has(accountId)) {
            return 0;
        }
        
        const account = this.accounts.get(accountId);
        if (account.accountType !== 'savings') {
            return 0;
        }
        
        const annualInterestRate = this.interestRates.savings;
        const monthlyInterestRate = annualInterestRate / 12;
        const interest = account.balance * monthlyInterestRate;
        
        // 将利息存入账户
        this.deposit(accountId, interest, 'Monthly interest');
        
        return interest;
    }
    
    /**
     * 生成账户对账单
     * @param {string} accountId - 账户ID
     * @param {Date} startDate - 开始日期（可选）
     * @param {Date} endDate - 结束日期（可选）
     * @returns {string} 对账单字符串
     */
    generateAccountStatement(accountId, startDate, endDate) {
        if (!this.accounts.has(accountId)) {
            return 'Account not found';
        }
        
        const account = this.accounts.get(accountId);
        const customer = this.customers.get(account.customerId);
        
        let statement = 'Account Statement\n';
        statement += '=================\n';
        statement += `Account ID: ${accountId}\n`;
        statement += `Account Type: ${account.accountType}\n`;
        statement += `Customer: ${customer.name}\n`;
        statement += `Customer ID: ${customer.customerId}\n\n`;
        
        // 获取指定日期范围内的交易记录
        const accountTransactions = this.getAccountTransactions(accountId, 1000, null);
        const filteredTransactions = [];
        
        for (const transaction of accountTransactions) {
            const transactionDate = new Date(transaction.timestamp);
            transactionDate.setHours(0, 0, 0, 0);
            
            if (startDate) {
                const start = new Date(startDate);
                start.setHours(0, 0, 0, 0);
                if (transactionDate < start) {
                    continue;
                }
            }
            
            if (endDate) {
                const end = new Date(endDate);
                end.setHours(23, 59, 59, 999);
                if (transactionDate > end) {
                    continue;
                }
            }
            
            filteredTransactions.push(transaction);
        }
        
        // 按时间升序排序
        filteredTransactions.sort((a, b) => a.timestamp - b.timestamp);
        
        // 计算期初和期末余额
        let openingBalance = 0;
        let closingBalance = account.balance;
        
        // 计算存款总额和取款总额
        let totalDeposits = 0;
        let totalWithdrawals = 0;
        
        for (const transaction of filteredTransactions) {
            if (transaction.transactionType === 'deposit' || 
                transaction.transactionType === 'transfer_in') {
                totalDeposits += transaction.amount;
            } else if (transaction.transactionType === 'withdrawal' || 
                       transaction.transactionType === 'transfer_out') {
                totalWithdrawals += transaction.amount;
            }
        }
        
        statement += `Opening Balance: $${openingBalance.toFixed(2)}\n`;
        statement += `Total Deposits: $${totalDeposits.toFixed(2)}\n`;
        statement += `Total Withdrawals: $${totalWithdrawals.toFixed(2)}\n`;
        statement += `Closing Balance: $${closingBalance.toFixed(2)}\n\n`;
        
        // 交易明细
        statement += 'Transaction Details\n';
        statement += '-------------------\n';
        
        for (const transaction of filteredTransactions) {
            const date = new Date(transaction.timestamp);
            statement += `${date.toISOString()} | `;
            statement += `${transaction.transactionType} | `;
            statement += `$${transaction.amount.toFixed(2)} | `;
            statement += `${transaction.description}\n`;
        }
        
        return statement;
    }
    
    /**
     * 记录交易
     * @param {string} accountId - 账户ID
     * @param {string} transactionType - 交易类型
     * @param {number} amount - 交易金额
     * @param {string} description - 交易描述
     * @param {string} creditCardId - 信用卡ID（可选）
     */
    recordTransaction(accountId, transactionType, amount, description, creditCardId) {
        const transaction = {
            transactionId: this.generateId('transaction'),
            accountId,
            transactionType,
            amount,
            description,
            timestamp: new Date(),
            creditCardId
        };
        
        this.transactions.push(transaction);
    }
    
    /**
     * 生成唯一ID
     * @param {string} prefix - ID前缀
     * @returns {string} 唯一ID字符串
     */
    generateId(prefix) {
        return `${prefix}_${Math.random().toString(36).substring(2, 10)}`;
    }
}