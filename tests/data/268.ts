/**
 * 银行系统类
 * 用于管理客户、账户、贷款和信用卡
 */
class BankingSystem {
    private customers: Map<string, Customer>;       // 客户ID到客户信息的映射
    private accounts: Map<string, Account>;        // 账户ID到账户信息的映射
    private loans: Map<string, Loan>;              // 贷款ID到贷款信息的映射
    private creditCards: Map<string, CreditCard>;  // 信用卡ID到信用卡信息的映射
    private transactions: Map<string, Transaction>; // 交易ID到交易信息的映射
    private branches: Map<string, Branch>;         // 分行ID到分行信息的映射
    
    /**
     * 构造函数
     */
    constructor() {
        this.customers = new Map();
        this.accounts = new Map();
        this.loans = new Map();
        this.creditCards = new Map();
        this.transactions = new Map();
        this.branches = new Map();
    }
    
    /**
     * 添加新客户
     * @param customerId - 客户唯一标识符
     * @param firstName - 客户名字
     * @param lastName - 客户姓氏
     * @param email - 电子邮件
     * @param phone - 电话号码
     * @param address - 地址
     * @param dateOfBirth - 出生日期
     * @param employmentStatus - 就业状况
     * @param annualIncome - 年收入
     * @param ssn - 社会保障号码
     * @returns 添加成功返回true，否则返回false
     */
    addCustomer(customerId: string, firstName: string, lastName: string, email: string, 
                 phone: string, address: string, dateOfBirth: Date, employmentStatus: string, 
                 annualIncome: number, ssn: string): boolean {
        if (this.customers.has(customerId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const customer of this.customers.values()) {
            if (customer.email === email) {
                return false;
            }
        }
        
        // 检查社会保障号码是否已被使用
        for (const customer of this.customers.values()) {
            if (customer.ssn === ssn) {
                return false;
            }
        }
        
        const customer = new Customer(
            customerId,
            firstName,
            lastName,
            email,
            phone,
            address,
            dateOfBirth,
            employmentStatus,
            annualIncome,
            ssn
        );
        
        this.customers.set(customerId, customer);
        
        return true;
    }
    
    /**
     * 更新客户信息
     * @param customerId - 客户ID
     * @param firstName - 新的名字（可选）
     * @param lastName - 新的姓氏（可选）
     * @param email - 新的电子邮件（可选）
     * @param phone - 新的电话号码（可选）
     * @param address - 新的地址（可选）
     * @param employmentStatus - 新的就业状况（可选）
     * @param annualIncome - 新的年收入（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateCustomer(customerId: string, firstName?: string, lastName?: string, 
                    email?: string, phone?: string, address?: string, 
                    employmentStatus?: string, annualIncome?: number): boolean {
        if (!this.customers.has(customerId)) {
            return false;
        }
        
        const customer = this.customers.get(customerId)!;
        
        // 如果提供了电子邮件，检查是否已被其他客户使用
        if (email && email !== customer.email) {
            for (const [id, c] of this.customers) {
                if (id !== customerId && c.email === email) {
                    return false;
                }
            }
            customer.email = email;
        }
        
        if (firstName !== undefined) {
            customer.firstName = firstName;
        }
        
        if (lastName !== undefined) {
            customer.lastName = lastName;
        }
        
        if (phone !== undefined) {
            customer.phone = phone;
        }
        
        if (address !== undefined) {
            customer.address = address;
        }
        
        if (employmentStatus !== undefined) {
            customer.employmentStatus = employmentStatus;
        }
        
        if (annualIncome !== undefined) {
            customer.annualIncome = annualIncome;
        }
        
        customer.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取客户信息
     * @param customerId - 客户ID
     * @returns 客户对象，若不存在则返回null
     */
    getCustomer(customerId: string): Customer | null {
        if (!this.customers.has(customerId)) {
            return null;
        }
        
        return this.customers.get(customerId)!;
    }
    
    /**
     * 删除客户
     * @param customerId - 客户ID
     * @returns 删除成功返回true，否则返回false
     */
    removeCustomer(customerId: string): boolean {
        if (!this.customers.has(customerId)) {
            return false;
        }
        
        // 检查客户是否有未结清的账户
        for (const account of this.accounts.values()) {
            if (account.customerId === customerId && account.status !== AccountStatus.Closed) {
                return false;
            }
        }
        
        // 检查客户是否有未还清的贷款
        for (const loan of this.loans.values()) {
            if (loan.customerId === customerId && loan.status !== LoanStatus.Paid) {
                return false;
            }
        }
        
        // 检查客户是否有未结清的信用卡
        for (const creditCard of this.creditCards.values()) {
            if (creditCard.customerId === customerId && creditCard.status !== CreditCardStatus.Closed) {
                return false;
            }
        }
        
        // 删除客户
        this.customers.delete(customerId);
        
        return true;
    }
    
    /**
     * 添加新账户
     * @param accountId - 账户唯一标识符
     * @param customerId - 客户ID
     * @param accountType - 账户类型
     * @param branchId - 分行ID
     * @param initialDeposit - 初始存款（可选）
     * @param overdraftLimit - 透支限额（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addAccount(accountId: string, customerId: string, accountType: AccountType, 
                 branchId: string, initialDeposit?: number, overdraftLimit?: number): boolean {
        if (this.accounts.has(accountId)) {
            return false;
        }
        
        if (!this.customers.has(customerId) || !this.branches.has(branchId)) {
            return false;
        }
        
        const account = new Account(
            accountId,
            customerId,
            accountType,
            branchId,
            initialDeposit || 0,
            overdraftLimit
        );
        
        this.accounts.set(accountId, account);
        
        // 如果有初始存款，记录交易
        if (initialDeposit && initialDeposit > 0) {
            this.recordTransaction(accountId, TransactionType.Deposit, initialDeposit, "Initial deposit");
        }
        
        return true;
    }
    
    /**
     * 更新账户信息
     * @param accountId - 账户ID
     * @param status - 新的状态（可选）
     * @param overdraftLimit - 新的透支限额（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateAccount(accountId: string, status?: AccountStatus, overdraftLimit?: number): boolean {
        if (!this.accounts.has(accountId)) {
            return false;
        }
        
        const account = this.accounts.get(accountId)!;
        
        if (status !== undefined) {
            account.status = status;
        }
        
        if (overdraftLimit !== undefined) {
            account.overdraftLimit = overdraftLimit;
        }
        
        account.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取账户信息
     * @param accountId - 账户ID
     * @returns 账户对象，若不存在则返回null
     */
    getAccount(accountId: string): Account | null {
        if (!this.accounts.has(accountId)) {
            return null;
        }
        
        return this.accounts.get(accountId)!;
    }
    
    /**
     * 关闭账户
     * @param accountId - 账户ID
     * @returns 关闭成功返回true，否则返回false
     */
    closeAccount(accountId: string): boolean {
        if (!this.accounts.has(accountId)) {
            return false;
        }
        
        const account = this.accounts.get(accountId)!;
        
        // 检查账户是否已经关闭
        if (account.status === AccountStatus.Closed) {
            return false;
        }
        
        // 检查账户余额是否为0
        if (account.balance !== 0) {
            return false;
        }
        
        // 更新账户状态
        account.status = AccountStatus.Closed;
        account.closedAt = new Date();
        account.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 存款
     * @param accountId - 账户ID
     * @param amount - 存款金额
     * @param description - 交易描述（可选）
     * @returns 存款成功返回true，否则返回false
     */
    deposit(accountId: string, amount: number, description?: string): boolean {
        if (!this.accounts.has(accountId)) {
            return false;
        }
        
        if (amount <= 0) {
            return false;
        }
        
        const account = this.accounts.get(accountId)!;
        
        // 检查账户是否已关闭
        if (account.status === AccountStatus.Closed) {
            return false;
        }
        
        // 更新账户余额
        account.balance += amount;
        account.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(accountId, TransactionType.Deposit, amount, description);
        
        return true;
    }
    
    /**
     * 取款
     * @param accountId - 账户ID
     * @param amount - 取款金额
     * @param description - 交易描述（可选）
     * @returns 取款成功返回true，否则返回false
     */
    withdraw(accountId: string, amount: number, description?: string): boolean {
        if (!this.accounts.has(accountId)) {
            return false;
        }
        
        if (amount <= 0) {
            return false;
        }
        
        const account = this.accounts.get(accountId)!;
        
        // 检查账户是否已关闭
        if (account.status === AccountStatus.Closed) {
            return false;
        }
        
        // 检查账户余额是否足够（考虑透支限额）
        if (account.balance - amount < -account.overdraftLimit) {
            return false;
        }
        
        // 更新账户余额
        account.balance -= amount;
        account.updatedAt = new Date();
        
        // 记录交易
        this.recordTransaction(accountId, TransactionType.Withdrawal, amount, description);
        
        return true;
    }
    
    /**
     * 转账
     * @param fromAccountId - 转出账户ID
     * @param toAccountId - 转入账户ID
     * @param amount - 转账金额
     * @param description - 交易描述（可选）
     * @returns 转账成功返回true，否则返回false
     */
    transfer(fromAccountId: string, toAccountId: string, amount: number, description?: string): boolean {
        if (!this.accounts.has(fromAccountId) || !this.accounts.has(toAccountId)) {
            return false;
        }
        
        if (amount <= 0) {
            return false;
        }
        
        const fromAccount = this.accounts.get(fromAccountId)!;
        const toAccount = this.accounts.get(toAccountId)!;
        
        // 检查账户是否已关闭
        if (fromAccount.status === AccountStatus.Closed || toAccount.status === AccountStatus.Closed) {
            return false;
        }
        
        // 检查转出账户余额是否足够（考虑透支限额）
        if (fromAccount.balance - amount < -fromAccount.overdraftLimit) {
            return false;
        }
        
        // 更新账户余额
        fromAccount.balance -= amount;
        toAccount.balance += amount;
        
        fromAccount.updatedAt = new Date();
        toAccount.updatedAt = new Date();
        
        // 记录交易
        const transactionId1 = this.recordTransaction(fromAccountId, TransactionType.TransferOut, amount, description, toAccountId);
        this.recordTransaction(toAccountId, TransactionType.TransferIn, amount, description, fromAccountId, transactionId1);
        
        return true;
    }
    
    /**
     * 记录交易
     * @param accountId - 账户ID
     * @param type - 交易类型
     * @param amount - 交易金额
     * @param description - 交易描述（可选）
     * @param relatedAccountId - 相关账户ID（可选）
     * @param relatedTransactionId - 相关交易ID（可选）
     * @returns 交易ID
     */
    private recordTransaction(accountId: string, type: TransactionType, amount: number, 
                               description?: string, relatedAccountId?: string, 
                               relatedTransactionId?: string): string {
        const transactionId = this.generateId("txn");
        
        const transaction = new Transaction(
            accountId,
            type,
            amount,
            description,
            relatedAccountId,
            relatedTransactionId
        );
        
        this.transactions.set(transactionId, transaction);
        
        return transactionId;
    }
    
    /**
     * 获取账户交易历史
     * @param accountId - 账户ID
     * @param type - 交易类型筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @param limit - 返回记录数限制（可选）
     * @returns 交易列表
     */
    getAccountTransactions(accountId: string, type?: TransactionType, 
                            startDate?: Date, endDate?: Date, limit?: number): Transaction[] {
        if (!this.accounts.has(accountId)) {
            return [];
        }
        
        let transactions: Transaction[] = [];
        
        for (const [transactionId, transaction] of this.transactions) {
            if ((transaction as any).accountId === accountId) {
                if (type && transaction.type !== type) {
                    continue;
                }
                
                if (startDate && transaction.timestamp < startDate) {
                    continue;
                }
                
                if (endDate && transaction.timestamp > endDate) {
                    continue;
                }
                
                transactions.push({...transaction, transactionId});
            }
        }
        
        // 按时间降序排序
        transactions.sort((a, b) => b.timestamp.getTime() - a.timestamp.getTime());
        
        if (limit && transactions.length > limit) {
            transactions = transactions.slice(0, limit);
        }
        
        return transactions;
    }
    
    /**
     * 添加新贷款
     * @param loanId - 贷款唯一标识符
     * @param customerId - 客户ID
     * @param loanType - 贷款类型
     * @param loanAmount - 贷款金额
     * @param interestRate - 利率
     * @param termMonths - 贷款期限（月）
     * @param collateral - 抵押品（可选）
     * @param guarantor - 担保人（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addLoan(loanId: string, customerId: string, loanType: LoanType, 
             loanAmount: number, interestRate: number, termMonths: number, 
             collateral?: string, guarantor?: string): boolean {
        if (this.loans.has(loanId)) {
            return false;
        }
        
        if (!this.customers.has(customerId)) {
            return false;
        }
        
        if (loanAmount <= 0 || interestRate <= 0 || termMonths <= 0) {
            return false;
        }
        
        const loan = new Loan(
            loanId,
            customerId,
            loanType,
            loanAmount,
            interestRate,
            termMonths,
            collateral,
            guarantor
        );
        
        this.loans.set(loanId, loan);
        
        return true;
    }
    
    /**
     * 更新贷款信息
     * @param loanId - 贷款ID
     * @param status - 新的状态（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateLoan(loanId: string, status?: LoanStatus): boolean {
        if (!this.loans.has(loanId)) {
            return false;
        }
        
        const loan = this.loans.get(loanId)!;
        
        if (status !== undefined) {
            // 验证状态转换是否有效
            if (!this.isValidLoanStatusTransition(loan.status, status)) {
                return false;
            }
            
            loan.status = status;
            
            if (status === LoanStatus.Paid) {
                loan.paidAt = new Date();
            }
        }
        
        loan.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 验证贷款状态转换是否有效
     * @param currentStatus - 当前状态
     * @param newStatus - 新状态
     * @returns 转换有效返回true，否则返回false
     */
    private isValidLoanStatusTransition(currentStatus: LoanStatus, newStatus: LoanStatus): boolean {
        switch (currentStatus) {
            case LoanStatus.Pending:
                return newStatus === LoanStatus.Approved || newStatus === LoanStatus.Rejected;
            case LoanStatus.Approved:
                return newStatus === LoanStatus.Active || newStatus === LoanStatus.Defaulted;
            case LoanStatus.Active:
                return newStatus === LoanStatus.Paid || newStatus === LoanStatus.Defaulted;
            case LoanStatus.Paid:
            case LoanStatus.Rejected:
            case LoanStatus.Defaulted:
                return false;  // 这些状态不能更改
            default:
                return false;
        }
    }
    
    /**
     * 获取贷款信息
     * @param loanId - 贷款ID
     * @returns 贷款对象，若不存在则返回null
     */
    getLoan(loanId: string): Loan | null {
        if (!this.loans.has(loanId)) {
            return null;
        }
        
        return this.loans.get(loanId)!;
    }
    
    /**
     * 计算贷款月还款额
     * @param loanId - 贷款ID
     * @returns 月还款额，若贷款不存在返回null
     */
    calculateLoanMonthlyPayment(loanId: string): number | null {
        if (!this.loans.has(loanId)) {
            return null;
        }
        
        const loan = this.loans.get(loanId)!;
        
        // 使用等额本息还款公式计算月还款额
        const monthlyInterestRate = loan.interestRate / 100 / 12;
        const loanAmount = loan.loanAmount;
        const termMonths = loan.termMonths;
        
        if (monthlyInterestRate === 0) {
            return loanAmount / termMonths;
        }
        
        const monthlyPayment = loanAmount * 
            (monthlyInterestRate * Math.pow(1 + monthlyInterestRate, termMonths)) / 
            (Math.pow(1 + monthlyInterestRate, termMonths) - 1);
        
        return monthlyPayment;
    }
    
    /**
     * 计算贷款剩余还款额
     * @param loanId - 贷款ID
     * @returns 剩余还款额，若贷款不存在返回null
     */
    calculateLoanRemainingBalance(loanId: string): number | null {
        if (!this.loans.has(loanId)) {
            return null;
        }
        
        const loan = this.loans.get(loanId)!;
        
        // 获取所有还款记录
        const payments = this.getLoanPayments(loanId);
        
        // 计算已还本金总额
        let totalPrincipalPaid = 0;
        for (const payment of payments) {
            totalPrincipalPaid += payment.principalAmount;
        }
        
        // 剩余还款额 = 贷款总额 - 已还本金总额
        const remainingBalance = loan.loanAmount - totalPrincipalPaid;
        
        return Math.max(0, remainingBalance);
    }
    
    /**
     * 添加贷款还款
     * @param loanId - 贷款ID
     * @param paymentAmount - 还款金额
     * @param paymentDate - 还款日期（可选）
     * @param description - 还款描述（可选）
     * @returns 还款ID，若还款失败返回null
     */
    addLoanPayment(loanId: string, paymentAmount: number, paymentDate?: Date, 
                    description?: string): string | null {
        if (!this.loans.has(loanId)) {
            return null;
        }
        
        if (paymentAmount <= 0) {
            return null;
        }
        
        const loan = this.loans.get(loanId)!;
        
        // 检查贷款状态
        if (loan.status !== LoanStatus.Active) {
            return null;
        }
        
        // 计算剩余还款额
        const remainingBalance = this.calculateLoanRemainingBalance(loanId)!;
        
        if (remainingBalance <= 0) {
            return null;  // 贷款已还清
        }
        
        // 计算月还款额
        const monthlyPayment = this.calculateLoanMonthlyPayment(loanId)!;
        
        // 计算应还利息
        const monthlyInterestRate = loan.interestRate / 100 / 12;
        const interestAmount = remainingBalance * monthlyInterestRate;
        
        // 计算应还本金
        const principalAmount = Math.min(paymentAmount - interestAmount, remainingBalance);
        
        // 如果还款金额不足以支付利息，则全部用于支付利息
        const actualPrincipalAmount = paymentAmount > interestAmount ? principalAmount : 0;
        const actualInterestAmount = paymentAmount - actualPrincipalAmount;
        
        // 记录还款
        const paymentId = this.generateId("payment");
        const payment = new LoanPayment(
            paymentAmount,
            actualPrincipalAmount,
            actualInterestAmount,
            paymentDate || new Date(),
            description
        );
        
        loan.payments.set(paymentId, payment);
        
        // 检查贷款是否已还清
        const newRemainingBalance = remainingBalance - actualPrincipalAmount;
        if (newRemainingBalance <= 0) {
            loan.status = LoanStatus.Paid;
            loan.paidAt = new Date();
        }
        
        loan.updatedAt = new Date();
        
        return paymentId;
    }
    
    /**
     * 获取贷款还款记录
     * @param loanId - 贷款ID
     * @returns 还款记录列表
     */
    getLoanPayments(loanId: string): LoanPayment[] {
        if (!this.loans.has(loanId)) {
            return [];
        }
        
        const loan = this.loans.get(loanId)!;
        const payments: LoanPayment[] = [];
        
        for (const [paymentId, payment] of loan.payments) {
            payments.push({...payment, paymentId});
        }
        
        // 按日期升序排序
        payments.sort((a, b) => a.paymentDate.getTime() - b.paymentDate.getTime());
        
        return payments;
    }
    
    /**
     * 添加新信用卡
     * @param creditCardId - 信用卡唯一标识符
     * @param customerId - 客户ID
     * @param accountId - 关联账户ID
     * @param creditLimit - 信用额度
     * @param annualFee - 年费（可选）
     * @param interestRate - 利率（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addCreditCard(creditCardId: string, customerId: string, accountId: string, 
                   creditLimit: number, annualFee?: number, interestRate?: number): boolean {
        if (this.creditCards.has(creditCardId)) {
            return false;
        }
        
        if (!this.customers.has(customerId) || !this.accounts.has(accountId)) {
            return false;
        }
        
        // 检查客户是否已有太多信用卡
        let cardCount = 0;
        for (const card of this.creditCards.values()) {
            if (card.customerId === customerId && card.status !== CreditCardStatus.Closed) {
                cardCount++;
            }
        }
        
        if (cardCount >= 5) {
            return false;  // 每个客户最多5张信用卡
        }
        
        const creditCard = new CreditCard(
            creditCardId,
            customerId,
            accountId,
            creditLimit,
            annualFee,
            interestRate
        );
        
        this.creditCards.set(creditCardId, creditCard);
        
        return true;
    }
    
    /**
     * 更新信用卡信息
     * @param creditCardId - 信用卡ID
     * @param status - 新的状态（可选）
     * @param creditLimit - 新的信用额度（可选）
     * @param interestRate - 新的利率（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateCreditCard(creditCardId: string, status?: CreditCardStatus, 
                      creditLimit?: number, interestRate?: number): boolean {
        if (!this.creditCards.has(creditCardId)) {
            return false;
        }
        
        const creditCard = this.creditCards.get(creditCardId)!;
        
        if (status !== undefined) {
            creditCard.status = status;
            
            if (status === CreditCardStatus.Closed) {
                creditCard.closedAt = new Date();
            }
        }
        
        if (creditLimit !== undefined) {
            creditCard.creditLimit = creditLimit;
        }
        
        if (interestRate !== undefined) {
            creditCard.interestRate = interestRate;
        }
        
        creditCard.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取信用卡信息
     * @param creditCardId - 信用卡ID
     * @returns 信用卡对象，若不存在则返回null
     */
    getCreditCard(creditCardId: string): CreditCard | null {
        if (!this.creditCards.has(creditCardId)) {
            return null;
        }
        
        return this.creditCards.get(creditCardId)!;
    }
    
    /**
     * 信用卡消费
     * @param creditCardId - 信用卡ID
     * @param amount - 消费金额
     * @param merchant - 商家
     * @param description - 交易描述（可选）
     * @returns 交易ID，若消费失败返回null
     */
    creditCardCharge(creditCardId: string, amount: number, merchant: string, 
                      description?: string): string | null {
        if (!this.creditCards.has(creditCardId)) {
            return null;
        }
        
        if (amount <= 0) {
            return null;
        }
        
        const creditCard = this.creditCards.get(creditCardId)!;
        
        // 检查信用卡状态
        if (creditCard.status !== CreditCardStatus.Active) {
            return null;
        }
        
        // 检查是否超过信用额度
        if (creditCard.currentBalance + amount > creditCard.creditLimit) {
            return null;
        }
        
        // 更新信用卡余额
        creditCard.currentBalance += amount;
        creditCard.updatedAt = new Date();
        
        // 记录交易
        const transactionId = this.recordCreditCardTransaction(creditCardId, 
            CreditCardTransactionType.Purchase, amount, merchant, description);
        
        return transactionId;
    }
    
    /**
     * 信用卡还款
     * @param creditCardId - 信用卡ID
     * @param amount - 还款金额
     * @param description - 交易描述（可选）
     * @returns 交易ID，若还款失败返回null
     */
    creditCardPayment(creditCardId: string, amount: number, description?: string): string | null {
        if (!this.creditCards.has(creditCardId)) {
            return null;
        }
        
        if (amount <= 0) {
            return null;
        }
        
        const creditCard = this.creditCards.get(creditCardId)!;
        
        // 检查信用卡状态
        if (creditCard.status !== CreditCardStatus.Active) {
            return null;
        }
        
        // 更新信用卡余额
        creditCard.currentBalance = Math.max(0, creditCard.currentBalance - amount);
        creditCard.updatedAt = new Date();
        
        // 记录交易
        const transactionId = this.recordCreditCardTransaction(creditCardId, 
            CreditCardTransactionType.Payment, amount, "Bank", description);
        
        return transactionId;
    }
    
    /**
     * 记录信用卡交易
     * @param creditCardId - 信用卡ID
     * @param type - 交易类型
     * @param amount - 交易金额
     * @param merchant - 商家
     * @param description - 交易描述（可选）
     * @returns 交易ID
     */
    private recordCreditCardTransaction(creditCardId: string, type: CreditCardTransactionType, 
                                       amount: number, merchant: string, 
                                       description?: string): string {
        const transactionId = this.generateId("cc_txn");
        
        const transaction = new CreditCardTransaction(
            type,
            amount,
            merchant,
            description
        );
        
        const creditCard = this.creditCards.get(creditCardId)!;
        creditCard.transactions.set(transactionId, transaction);
        
        return transactionId;
    }
    
    /**
     * 获取信用卡交易历史
     * @param creditCardId - 信用卡ID
     * @param type - 交易类型筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @param limit - 返回记录数限制（可选）
     * @returns 交易列表
     */
    getCreditCardTransactions(creditCardId: string, type?: CreditCardTransactionType, 
                              startDate?: Date, endDate?: Date, limit?: number): CreditCardTransaction[] {
        if (!this.creditCards.has(creditCardId)) {
            return [];
        }
        
        const creditCard = this.creditCards.get(creditCardId)!;
        let transactions: CreditCardTransaction[] = [];
        
        for (const [transactionId, transaction] of creditCard.transactions) {
            if (type && transaction.type !== type) {
                continue;
            }
            
            if (startDate && transaction.timestamp < startDate) {
                continue;
            }
            
            if (endDate && transaction.timestamp > endDate) {
                continue;
            }
            
            transactions.push({...transaction, transactionId});
        }
        
        // 按时间降序排序
        transactions.sort((a, b) => b.timestamp.getTime() - a.timestamp.getTime());
        
        if (limit && transactions.length > limit) {
            transactions = transactions.slice(0, limit);
        }
        
        return transactions;
    }
    
    /**
     * 添加新分行
     * @param branchId - 分行唯一标识符
     * @param name - 分行名称
     * @param address - 分行地址
     * @param phone - 分行电话
     * @param manager - 分行经理
     * @param openingHours - 营业时间
     * @returns 添加成功返回true，否则返回false
     */
    addBranch(branchId: string, name: string, address: string, phone: string, 
               manager: string, openingHours: string): boolean {
        if (this.branches.has(branchId)) {
            return false;
        }
        
        const branch = new Branch(
            branchId,
            name,
            address,
            phone,
            manager,
            openingHours
        );
        
        this.branches.set(branchId, branch);
        
        return true;
    }
    
    /**
     * 更新分行信息
     * @param branchId - 分行ID
     * @param name - 新的名称（可选）
     * @param address - 新的地址（可选）
     * @param phone - 新的电话（可选）
     * @param manager - 新的经理（可选）
     * @param openingHours - 新的营业时间（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateBranch(branchId: string, name?: string, address?: string, 
                  phone?: string, manager?: string, openingHours?: string): boolean {
        if (!this.branches.has(branchId)) {
            return false;
        }
        
        const branch = this.branches.get(branchId)!;
        
        if (name !== undefined) {
            branch.name = name;
        }
        
        if (address !== undefined) {
            branch.address = address;
        }
        
        if (phone !== undefined) {
            branch.phone = phone;
        }
        
        if (manager !== undefined) {
            branch.manager = manager;
        }
        
        if (openingHours !== undefined) {
            branch.openingHours = openingHours;
        }
        
        branch.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取分行信息
     * @param branchId - 分行ID
     * @returns 分行对象，若不存在则返回null
     */
    getBranch(branchId: string): Branch | null {
        if (!this.branches.has(branchId)) {
            return null;
        }
        
        return this.branches.get(branchId)!;
    }
    
    /**
     * 生成唯一ID
     * @param prefix - ID前缀
     * @returns 唯一ID字符串
     */
    private generateId(prefix: string): string {
        return `${prefix}_${Date.now()}_${Math.random().toString(36).substring(2, 10)}`;
    }
}

/**
 * 客户类
 */
class Customer {
    customerId: string;
    firstName: string;
    lastName: string;
    email: string;
    phone: string;
    address: string;
    dateOfBirth: Date;
    employmentStatus: string;
    annualIncome: number;
    ssn: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(customerId: string, firstName: string, lastName: string, email: string, 
                phone: string, address: string, dateOfBirth: Date, employmentStatus: string, 
                annualIncome: number, ssn: string) {
        this.customerId = customerId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.phone = phone;
        this.address = address;
        this.dateOfBirth = dateOfBirth;
        this.employmentStatus = employmentStatus;
        this.annualIncome = annualIncome;
        this.ssn = ssn;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
    
    /**
     * 获取全名
     */
    get fullName(): string {
        return `${this.firstName} ${this.lastName}`;
    }
    
    /**
     * 计算年龄
     */
    get age(): number {
        const today = new Date();
        let age = today.getFullYear() - this.dateOfBirth.getFullYear();
        const monthDiff = today.getMonth() - this.dateOfBirth.getMonth();
        
        if (monthDiff < 0 || (monthDiff === 0 && today.getDate() < this.dateOfBirth.getDate())) {
            age--;
        }
        
        return age;
    }
}

/**
 * 账户类
 */
class Account {
    accountId: string;
    customerId: string;
    accountType: AccountType;
    branchId: string;
    balance: number;
    overdraftLimit: number;
    status: AccountStatus;
    createdAt: Date;
    updatedAt: Date;
    closedAt?: Date;
    
    constructor(accountId: string, customerId: string, accountType: AccountType, 
                branchId: string, initialDeposit: number, overdraftLimit?: number) {
        this.accountId = accountId;
        this.customerId = customerId;
        this.accountType = accountType;
        this.branchId = branchId;
        this.balance = initialDeposit;
        this.overdraftLimit = overdraftLimit || 0;
        this.status = AccountStatus.Active;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 账户类型枚举
 */
enum AccountType {
    Checking = "checking",
    Savings = "savings",
    MoneyMarket = "money_market",
    CertificateOfDeposit = "certificate_of_deposit"
}

/**
 * 账户状态枚举
 */
enum AccountStatus {
    Active = "active",
    Frozen = "frozen",
    Closed = "closed"
}

/**
 * 交易类
 */
class BankTransaction {
    accountId: string;
    type: TransactionType;
    amount: number;
    description?: string;
    relatedAccountId?: string;
    relatedTransactionId?: string;
    timestamp: Date;
    
    constructor(accountId: string, type: TransactionType, amount: number,
                description?: string, relatedAccountId?: string,
                relatedTransactionId?: string) {
        this.accountId = accountId;
        this.type = type;
        this.amount = amount;
        this.description = description;
        this.relatedAccountId = relatedAccountId;
        this.relatedTransactionId = relatedTransactionId;
        this.timestamp = new Date();
    }
}

/**
 * 交易类型枚举
 */
enum TransactionType {
    Deposit = "deposit",
    Withdrawal = "withdrawal",
    TransferIn = "transfer_in",
    TransferOut = "transfer_out",
    Interest = "interest",
    Fee = "fee"
}

/**
 * 贷款类
 */
class Loan {
    loanId: string;
    customerId: string;
    loanType: LoanType;
    loanAmount: number;
    interestRate: number;
    termMonths: number;
    collateral?: string;
    guarantor?: string;
    status: LoanStatus;
    payments: Map<string, LoanPayment>;
    createdAt: Date;
    updatedAt: Date;
    approvedAt?: Date;
    paidAt?: Date;
    
    constructor(loanId: string, customerId: string, loanType: LoanType, 
                loanAmount: number, interestRate: number, termMonths: number, 
                collateral?: string, guarantor?: string) {
        this.loanId = loanId;
        this.customerId = customerId;
        this.loanType = loanType;
        this.loanAmount = loanAmount;
        this.interestRate = interestRate;
        this.termMonths = termMonths;
        this.collateral = collateral;
        this.guarantor = guarantor;
        this.status = LoanStatus.Pending;
        this.payments = new Map();
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 贷款类型枚举
 */
enum LoanType {
    Personal = "personal",
    Auto = "auto",
    Mortgage = "mortgage",
    Business = "business",
    Student = "student"
}

/**
 * 贷款状态枚举
 */
enum LoanStatus {
    Pending = "pending",
    Approved = "approved",
    Active = "active",
    Paid = "paid",
    Rejected = "rejected",
    Defaulted = "defaulted"
}

/**
 * 贷款还款类
 */
class LoanPayment {
    paymentAmount: number;
    principalAmount: number;
    interestAmount: number;
    paymentDate: Date;
    description?: string;
    
    constructor(paymentAmount: number, principalAmount: number, interestAmount: number, 
                paymentDate: Date, description?: string) {
        this.paymentAmount = paymentAmount;
        this.principalAmount = principalAmount;
        this.interestAmount = interestAmount;
        this.paymentDate = paymentDate;
        this.description = description;
    }
}

/**
 * 信用卡类
 */
class CreditCard {
    creditCardId: string;
    customerId: string;
    accountId: string;
    creditLimit: number;
    currentBalance: number;
    annualFee?: number;
    interestRate?: number;
    status: CreditCardStatus;
    transactions: Map<string, CreditCardTransaction>;
    createdAt: Date;
    updatedAt: Date;
    closedAt?: Date;
    lastPaymentDate?: Date;
    
    constructor(creditCardId: string, customerId: string, accountId: string, 
                creditLimit: number, annualFee?: number, interestRate?: number) {
        this.creditCardId = creditCardId;
        this.customerId = customerId;
        this.accountId = accountId;
        this.creditLimit = creditLimit;
        this.currentBalance = 0;
        this.annualFee = annualFee;
        this.interestRate = interestRate;
        this.status = CreditCardStatus.Active;
        this.transactions = new Map();
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 信用卡状态枚举
 */
enum CreditCardStatus {
    Active = "active",
    Frozen = "frozen",
    Closed = "closed",
    Lost = "lost",
    Stolen = "stolen"
}

/**
 * 信用卡交易类
 */
class CreditCardTransaction {
    type: CreditCardTransactionType;
    amount: number;
    merchant: string;
    description?: string;
    timestamp: Date;
    
    constructor(type: CreditCardTransactionType, amount: number, merchant: string, 
                description?: string) {
        this.type = type;
        this.amount = amount;
        this.merchant = merchant;
        this.description = description;
        this.timestamp = new Date();
    }
}

/**
 * 信用卡交易类型枚举
 */
enum CreditCardTransactionType {
    Purchase = "purchase",
    Payment = "payment",
    Fee = "fee",
    Interest = "interest",
    CashAdvance = "cash_advance"
}

/**
 * 分行类
 */
class Branch {
    branchId: string;
    name: string;
    address: string;
    phone: string;
    manager: string;
    openingHours: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(branchId: string, name: string, address: string, phone: string, 
                manager: string, openingHours: string) {
        this.branchId = branchId;
        this.name = name;
        this.address = address;
        this.phone = phone;
        this.manager = manager;
        this.openingHours = openingHours;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}