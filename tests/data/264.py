class BankingSystem:
    """
    银行系统类，用于管理账户、交易、贷款和信用卡
    """
    
    def __init__(self):
        self.customers = {}  # 客户ID到客户信息的映射
        self.accounts = {}  # 账户ID到账户信息的映射
        self.transactions = []  # 交易记录
        self.loans = {}  # 贷款ID到贷款信息的映射
        self.credit_cards = {}  # 信用卡ID到信用卡信息的映射
        self.interest_rates = {
            'savings': 0.02,  # 储蓄利率
            'loan': 0.05,     # 贷款利率
            'credit_card': 0.18  # 信用卡利率
        }
    
    def create_customer(self, customer_id, name, address, phone, email, date_of_birth):
        """
        创建新客户
        @param customer_id: 客户唯一标识符
        @param name: 客户姓名
        @param address: 地址
        @param phone: 电话号码
        @param email: 电子邮件地址
        @param date_of_birth: 出生日期
        @return: 创建成功返回True，否则返回False
        """
        if customer_id in self.customers:
            return False
        
        if any(customer['email'] == email for customer in self.customers.values()):
            return False
        
        self.customers[customer_id] = {
            'name': name,
            'address': address,
            'phone': phone,
            'email': email,
            'date_of_birth': date_of_birth,
            'accounts': [],
            'loans': [],
            'credit_cards': [],
            'created_at': self.get_current_timestamp()
        }
        return True
    
    def create_account(self, customer_id, account_type, initial_deposit=0):
        """
        创建新账户
        @param customer_id: 客户ID
        @param account_type: 账户类型（'checking', 'savings'）
        @param initial_deposit: 初始存款金额
        @return: 创建成功返回账户ID，否则返回None
        """
        if customer_id not in self.customers:
            return None
        
        account_id = self.generate_id('account')
        self.accounts[account_id] = {
            'customer_id': customer_id,
            'account_type': account_type,
            'balance': initial_deposit,
            'created_at': self.get_current_timestamp()
        }
        
        # 更新客户的账户列表
        self.customers[customer_id]['accounts'].append(account_id)
        
        # 记录初始存款交易
        if initial_deposit > 0:
            self.record_transaction(
                account_id=account_id,
                transaction_type='deposit',
                amount=initial_deposit,
                description='Initial deposit'
            )
        
        return account_id
    
    def deposit(self, account_id, amount, description=""):
        """
        存款
        @param account_id: 账户ID
        @param amount: 存款金额
        @param description: 交易描述
        @return: 存款成功返回True，否则返回False
        """
        if account_id not in self.accounts or amount <= 0:
            return False
        
        self.accounts[account_id]['balance'] += amount
        
        # 记录交易
        self.record_transaction(
            account_id=account_id,
            transaction_type='deposit',
            amount=amount,
            description=description or "Deposit"
        )
        
        return True
    
    def withdraw(self, account_id, amount, description=""):
        """
        取款
        @param account_id: 账户ID
        @param amount: 取款金额
        @param description: 交易描述
        @return: 取款成功返回True，否则返回False
        """
        if account_id not in self.accounts or amount <= 0:
            return False
        
        if self.accounts[account_id]['balance'] < amount:
            return False  # 余额不足
        
        self.accounts[account_id]['balance'] -= amount
        
        # 记录交易
        self.record_transaction(
            account_id=account_id,
            transaction_type='withdrawal',
            amount=amount,
            description=description or "Withdrawal"
        )
        
        return True
    
    def transfer(self, from_account_id, to_account_id, amount, description=""):
        """
        转账
        @param from_account_id: 转出账户ID
        @param to_account_id: 转入账户ID
        @param amount: 转账金额
        @param description: 交易描述
        @return: 转账成功返回True，否则返回False
        """
        if (from_account_id not in self.accounts or 
            to_account_id not in self.accounts or 
            amount <= 0):
            return False
        
        if self.accounts[from_account_id]['balance'] < amount:
            return False  # 余额不足
        
        # 执行转账
        self.accounts[from_account_id]['balance'] -= amount
        self.accounts[to_account_id]['balance'] += amount
        
        # 记录转出交易
        self.record_transaction(
            account_id=from_account_id,
            transaction_type='transfer_out',
            amount=amount,
            reference_id=to_account_id,
            description=description or f"Transfer to {to_account_id}"
        )
        
        # 记录转入交易
        self.record_transaction(
            account_id=to_account_id,
            transaction_type='transfer_in',
            amount=amount,
            reference_id=from_account_id,
            description=description or f"Transfer from {from_account_id}"
        )
        
        return True
    
    def apply_for_loan(self, customer_id, loan_amount, loan_term, purpose):
        """
        申请贷款
        @param customer_id: 客户ID
        @param loan_amount: 贷款金额
        @param loan_term: 贷款期限（月）
        @param purpose: 贷款用途
        @return: 申请成功返回贷款ID，否则返回None
        """
        if customer_id not in self.customers:
            return None
        
        if loan_amount <= 0 or loan_term <= 0:
            return None
        
        # 检查客户资格（简化版，实际应用中会有更复杂的信用评估）
        customer_accounts = [self.accounts[acc_id] for acc_id in self.customers[customer_id]['accounts']]
        if not customer_accounts:
            return None  # 没有账户的客户不能申请贷款
        
        # 计算月还款额（简化版，不考虑复利）
        monthly_interest_rate = self.interest_rates['loan'] / 12
        monthly_payment = loan_amount * (monthly_interest_rate * (1 + monthly_interest_rate) ** loan_term) / \
                         ((1 + monthly_interest_rate) ** loan_term - 1)
        
        loan_id = self.generate_id('loan')
        self.loans[loan_id] = {
            'customer_id': customer_id,
            'loan_amount': loan_amount,
            'loan_term': loan_term,
            'purpose': purpose,
            'interest_rate': self.interest_rates['loan'],
            'monthly_payment': monthly_payment,
            'remaining_balance': loan_amount,
            'remaining_payments': loan_term,
            'status': 'approved',  # pending, approved, rejected, paid_off
            'created_at': self.get_current_timestamp()
        }
        
        # 更新客户的贷款列表
        self.customers[customer_id]['loans'].append(loan_id)
        
        # 将贷款金额存入客户的账户
        if customer_accounts:
            account_id = self.customers[customer_id]['accounts'][0]
            self.deposit(account_id, loan_amount, f"Loan disbursement for {purpose}")
        
        return loan_id
    
    def make_loan_payment(self, loan_id, payment_amount):
        """
        贷款还款
        @param loan_id: 贷款ID
        @param payment_amount: 还款金额
        @return: 还款成功返回True，否则返回False
        """
        if loan_id not in self.loans or payment_amount <= 0:
            return False
        
        loan = self.loans[loan_id]
        if loan['status'] != 'approved':
            return False
        
        if payment_amount < loan['monthly_payment']:
            return False  # 还款金额不足
        
        # 更新贷款余额和剩余还款次数
        loan['remaining_balance'] -= payment_amount
        loan['remaining_payments'] -= 1
        
        # 检查是否还清
        if loan['remaining_balance'] <= 0:
            loan['status'] = 'paid_off'
            loan['remaining_balance'] = 0
            loan['remaining_payments'] = 0
        
        # 记录还款交易
        customer_id = loan['customer_id']
        if customer_id in self.customers and self.customers[customer_id]['accounts']:
            account_id = self.customers[customer_id]['accounts'][0]
            self.withdraw(account_id, payment_amount, f"Loan payment for loan {loan_id}")
        
        return True
    
    def apply_for_credit_card(self, customer_id, credit_limit):
        """
        申请信用卡
        @param customer_id: 客户ID
        @param credit_limit: 信用额度
        @return: 申请成功返回信用卡ID，否则返回None
        """
        if customer_id not in self.customers:
            return None
        
        if credit_limit <= 0:
            return None
        
        # 检查客户资格（简化版）
        customer_accounts = [self.accounts[acc_id] for acc_id in self.customers[customer_id]['accounts']]
        if not customer_accounts:
            return None  # 没有账户的客户不能申请信用卡
        
        credit_card_id = self.generate_id('credit_card')
        self.credit_cards[credit_card_id] = {
            'customer_id': customer_id,
            'credit_limit': credit_limit,
            'used_credit': 0,
            'available_credit': credit_limit,
            'interest_rate': self.interest_rates['credit_card'],
            'status': 'active',  # active, suspended, cancelled
            'created_at': self.get_current_timestamp()
        }
        
        # 更新客户的信用卡列表
        self.customers[customer_id]['credit_cards'].append(credit_card_id)
        
        return credit_card_id
    
    def use_credit_card(self, credit_card_id, amount, description=""):
        """
        使用信用卡消费
        @param credit_card_id: 信用卡ID
        @param amount: 消费金额
        @param description: 交易描述
        @return: 消费成功返回True，否则返回False
        """
        if credit_card_id not in self.credit_cards or amount <= 0:
            return False
        
        credit_card = self.credit_cards[credit_card_id]
        if credit_card['status'] != 'active':
            return False
        
        if amount > credit_card['available_credit']:
            return False  # 超出可用额度
        
        # 更新信用卡使用情况
        credit_card['used_credit'] += amount
        credit_card['available_credit'] -= amount
        
        # 记录交易
        self.record_transaction(
            account_id=None,
            transaction_type='credit_card_purchase',
            amount=amount,
            reference_id=credit_card_id,
            description=description or "Credit card purchase"
        )
        
        return True
    
    def pay_credit_card_bill(self, credit_card_id, payment_amount):
        """
        还信用卡账单
        @param credit_card_id: 信用卡ID
        @param payment_amount: 还款金额
        @return: 还款成功返回True，否则返回False
        """
        if credit_card_id not in self.credit_cards or payment_amount <= 0:
            return False
        
        credit_card = self.credit_cards[credit_card_id]
        if credit_card['status'] != 'active':
            return False
        
        if payment_amount > credit_card['used_credit']:
            payment_amount = credit_card['used_credit']  # 不能还超过欠款金额
        
        # 更新信用卡使用情况
        credit_card['used_credit'] -= payment_amount
        credit_card['available_credit'] += payment_amount
        
        # 记录还款交易
        customer_id = credit_card['customer_id']
        if customer_id in self.customers and self.customers[customer_id]['accounts']:
            account_id = self.customers[customer_id]['accounts'][0]
            self.withdraw(account_id, payment_amount, f"Credit card payment for card {credit_card_id}")
        
        return True
    
    def get_account_balance(self, account_id):
        """
        获取账户余额
        @param account_id: 账户ID
        @return: 账户余额，账户不存在返回None
        """
        if account_id not in self.accounts:
            return None
        
        return self.accounts[account_id]['balance']
    
    def get_customer_accounts(self, customer_id):
        """
        获取客户的所有账户
        @param customer_id: 客户ID
        @return: 账户信息列表
        """
        if customer_id not in self.customers:
            return []
        
        result = []
        for account_id in self.customers[customer_id]['accounts']:
            if account_id in self.accounts:
                account_info = self.accounts[account_id].copy()
                account_info['account_id'] = account_id
                result.append(account_info)
        
        return result
    
    def get_account_transactions(self, account_id, limit=10, transaction_type=None):
        """
        获取账户交易记录
        @param account_id: 账户ID
        @param limit: 返回记录数限制
        @param transaction_type: 交易类型筛选（可选）
        @return: 交易记录列表
        """
        if account_id not in self.accounts:
            return []
        
        result = []
        for transaction in self.transactions:
            if transaction['account_id'] == account_id:
                if transaction_type is None or transaction['transaction_type'] == transaction_type:
                    result.append(transaction.copy())
        
        # 按时间戳降序排序
        result.sort(key=lambda x: x['timestamp'], reverse=True)
        
        if len(result) > limit:
            result = result[:limit]
        
        return result
    
    def calculate_monthly_interest(self, account_id):
        """
        计算账户月利息（仅对储蓄账户）
        @param account_id: 账户ID
        @return: 利息金额，账户不存在或不是储蓄账户返回0
        """
        if account_id not in self.accounts:
            return 0
        
        account = self.accounts[account_id]
        if account['account_type'] != 'savings':
            return 0
        
        monthly_interest_rate = self.interest_rates['savings'] / 12
        interest = account['balance'] * monthly_interest_rate
        
        # 将利息存入账户
        self.deposit(account_id, interest, "Monthly interest")
        
        return interest
    
    def generate_account_statement(self, account_id, start_date=None, end_date=None):
        """
        生成账户对账单
        @param account_id: 账户ID
        @param start_date: 开始日期（可选）
        @param end_date: 结束日期（可选）
        @return: 对账单字典
        """
        if account_id not in self.accounts:
            return None
        
        account = self.accounts[account_id]
        customer_id = account['customer_id']
        customer = self.customers.get(customer_id, {})
        
        # 获取指定日期范围内的交易记录
        transactions = []
        for transaction in self.transactions:
            if transaction['account_id'] == account_id:
                transaction_date = transaction['timestamp']
                
                if start_date and transaction_date < start_date:
                    continue
                
                if end_date and transaction_date > end_date:
                    continue
                
                transactions.append(transaction.copy())
        
        # 计算期初和期末余额
        opening_balance = 0
        closing_balance = account['balance']
        
        # 按时间戳排序交易记录
        transactions.sort(key=lambda x: x['timestamp'])
        
        # 计算存款总额和取款总额
        total_deposits = sum(t['amount'] for t in transactions if t['transaction_type'] in ['deposit', 'transfer_in'])
        total_withdrawals = sum(t['amount'] for t in transactions if t['transaction_type'] in ['withdrawal', 'transfer_out'])
        
        statement = {
            'account_id': account_id,
            'customer_name': customer.get('name', ''),
            'account_type': account['account_type'],
            'opening_balance': opening_balance,
            'closing_balance': closing_balance,
            'total_deposits': total_deposits,
            'total_withdrawals': total_withdrawals,
            'transactions': transactions,
            'generated_at': self.get_current_timestamp()
        }
        
        return statement
    
    def record_transaction(self, account_id, transaction_type, amount, reference_id=None, description=""):
        """
        记录交易
        @param account_id: 账户ID
        @param transaction_type: 交易类型
        @param amount: 交易金额
        @param reference_id: 关联ID（可选）
        @param description: 交易描述
        """
        transaction = {
            'account_id': account_id,
            'transaction_type': transaction_type,
            'amount': amount,
            'reference_id': reference_id,
            'description': description,
            'timestamp': self.get_current_timestamp()
        }
        
        self.transactions.append(transaction)
    
    def generate_id(self, prefix):
        """
        生成唯一ID
        @param prefix: ID前缀
        @return: 唯一ID字符串
        """
        import uuid
        return f"{prefix}_{uuid.uuid4().hex[:8]}"
    
    def get_current_timestamp(self):
        """
        获取当前时间戳
        @return: 当前时间戳字符串
        """
        import datetime
        return datetime.datetime.now().isoformat()