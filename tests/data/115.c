#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_CUSTOMERS 500
#define MAX_ACCOUNTS 1000
#define MAX_TRANSACTIONS 5000
#define MAX_NAME_LENGTH 50
#define MAX_ADDRESS_LENGTH 100
#define FILENAME_CUSTOMERS "customers.dat"
#define FILENAME_ACCOUNTS "accounts.dat"
#define FILENAME_TRANSACTIONS "transactions.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char phone[15];
    char email[50];
    char address[MAX_ADDRESS_LENGTH];
    Date birth_date;
    Date registration_date;
    int ssn; // Social Security Number or equivalent
    char occupation[50];
    int annual_income;
    int credit_score;
    int active;
} Customer;

typedef enum {
    SAVINGS,
    CHECKING,
    CREDIT_CARD,
    LOAN,
    INVESTMENT
} AccountType;

typedef struct {
    int id;
    int customer_id;
    AccountType type;
    char account_number[20];
    float balance;
    float interest_rate;
    Date opening_date;
    Date last_transaction_date;
    int active;
    float credit_limit; // For credit cards
    float loan_amount; // For loans
    float monthly_payment; // For loans
    int loan_term_months; // For loans
} Account;

typedef enum {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    PAYMENT,
    INTEREST,
    FEE
} TransactionType;

typedef struct {
    int id;
    int from_account_id;
    int to_account_id;
    TransactionType type;
    float amount;
    Date date;
    time_t timestamp;
    char description[100];
    int processed;
} Transaction;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Account accounts[MAX_ACCOUNTS];
    int account_count;
    Transaction transactions[MAX_TRANSACTIONS];
    int transaction_count;
} BankDatabase;

// Function prototypes
void initialize_database(BankDatabase *db);
int add_customer(BankDatabase *db, const char *name, const char *phone, const char *email,
                 const char *address, Date birth_date, int ssn, const char *occupation,
                 int annual_income);
int add_account(BankDatabase *db, int customer_id, AccountType type, const char *account_number,
                float initial_balance, float interest_rate, float credit_limit);
int process_transaction(BankDatabase *db, int from_account_id, int to_account_id,
                        TransactionType type, float amount, const char *description);
void apply_interest(BankDatabase *db);
void display_customer(const Customer *customer, const BankDatabase *db);
void display_account(const Account *account, const Customer *customer);
void display_transaction(const Transaction *transaction, const BankDatabase *db);
void display_all_customers(const BankDatabase *db);
void display_all_accounts(const BankDatabase *db);
void display_account_transactions(const BankDatabase *db, int account_id);
int find_customer_by_id(const BankDatabase *db, int customer_id);
int find_account_by_id(const BankDatabase *db, int account_id);
int find_account_by_number(const BankDatabase *db, const char *account_number);
void search_customers_by_name(const BankDatabase *db, const char *name);
void generate_monthly_statement(const BankDatabase *db, int account_id, int month, int year);
void transfer_between_accounts(BankDatabase *db, int from_account_id, int to_account_id, float amount);
int save_database(const BankDatabase *db);
int load_database(BankDatabase *db);
void generate_sample_data(BankDatabase *db);
void calculate_loan_payments(BankDatabase *db);
void detect_fraudulent_transactions(BankDatabase *db);
void display_top_customers(const BankDatabase *db);
void display_branch_statistics(const BankDatabase *db);
int is_valid_date(int day, int month, int year);
const char* get_account_type_name(AccountType type);
const char* get_transaction_type_name(TransactionType type);

int main() {
    BankDatabase db;
    initialize_database(&db);
    
    printf("Banking Management System\n");
    printf("1. Add Customer\n");
    printf("2. Add Account\n");
    printf("3. Process Transaction\n");
    printf("4. Display All Customers\n");
    printf("5. Display All Accounts\n");
    printf("6. Display Account Transactions\n");
    printf("7. Search Customer by Name\n");
    printf("8. Generate Monthly Statement\n");
    printf("9. Transfer Between Accounts\n");
    printf("10. Apply Interest\n");
    printf("11. Save Database\n");
    printf("12. Load Database\n");
    printf("13. Generate Sample Data\n");
    printf("14. Calculate Loan Payments\n");
    printf("15. Detect Fraudulent Transactions\n");
    printf("16. Display Top Customers\n");
    printf("17. Display Branch Statistics\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], phone[15], email[50], address[MAX_ADDRESS_LENGTH];
                char occupation[50];
                int ssn, annual_income, day, month, year;
                
                printf("Enter customer name: ");
                scanf(" %[^\n]", name);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                printf("Enter SSN: ");
                scanf("%d", &ssn);
                printf("Enter occupation: ");
                scanf(" %[^\n]", occupation);
                printf("Enter annual income: ");
                scanf("%d", &annual_income);
                
                Date birth_date = {day, month, year};
                
                time_t now = time(NULL);
                struct tm *current_time = localtime(&now);
                Date reg_date = {
                    current_time->tm_mday, 
                    current_time->tm_mon + 1, 
                    current_time->tm_year + 1900
                };
                
                int customer_id = add_customer(&db, name, phone, email, address, 
                                               birth_date, ssn, occupation, annual_income);
                if (customer_id != -1) {
                    db.customers[customer_id - 1].registration_date = reg_date;
                    printf("Customer added with ID: %d\n", customer_id);
                } else {
                    printf("Failed to add customer. Database is full.\n");
                }
                break;
            }
            case 2: {
                int customer_id, type;
                char account_number[20];
                float initial_balance, interest_rate, credit_limit;
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter account type (0-Savings, 1-Checking, 2-Credit Card, 3-Loan, 4-Investment): ");
                scanf("%d", &type);
                printf("Enter account number: ");
                scanf("%s", account_number);
                printf("Enter initial balance: ");
                scanf("%f", &initial_balance);
                printf("Enter interest rate (as percentage): ");
                scanf("%f", &interest_rate);
                
                credit_limit = 0.0f;
                if (type == CREDIT_CARD) {
                    printf("Enter credit limit: ");
                    scanf("%f", &credit_limit);
                }
                
                int account_id = add_account(&db, customer_id, (AccountType)type, account_number,
                                            initial_balance, interest_rate / 100.0f, credit_limit);
                if (account_id != -1) {
                    printf("Account added with ID: %d\n", account_id);
                } else {
                    printf("Failed to add account. Customer not found or database is full.\n");
                }
                break;
            }
            case 3: {
                int from_account_id, to_account_id, type;
                float amount;
                char description[100];
                
                printf("Enter from account ID (0 for bank): ");
                scanf("%d", &from_account_id);
                printf("Enter to account ID (0 for bank): ");
                scanf("%d", &to_account_id);
                printf("Enter transaction type (0-Deposit, 1-Withdrawal, 2-Transfer, 3-Payment, 4-Interest, 5-Fee): ");
                scanf("%d", &type);
                printf("Enter amount: ");
                scanf("%f", &amount);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                
                if (process_transaction(&db, from_account_id, to_account_id, 
                                        (TransactionType)type, amount, description)) {
                    printf("Transaction processed successfully.\n");
                } else {
                    printf("Failed to process transaction. Check account IDs and balances.\n");
                }
                break;
            }
            case 4: {
                display_all_customers(&db);
                break;
            }
            case 5: {
                display_all_accounts(&db);
                break;
            }
            case 6: {
                int account_id;
                printf("Enter account ID: ");
                scanf("%d", &account_id);
                display_account_transactions(&db, account_id);
                break;
            }
            case 7: {
                char name[MAX_NAME_LENGTH];
                printf("Enter customer name to search: ");
                scanf(" %[^\n]", name);
                search_customers_by_name(&db, name);
                break;
            }
            case 8: {
                int account_id, month, year;
                printf("Enter account ID: ");
                scanf("%d", &account_id);
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_monthly_statement(&db, account_id, month, year);
                break;
            }
            case 9: {
                int from_account_id, to_account_id;
                float amount;
                printf("Enter from account ID: ");
                scanf("%d", &from_account_id);
                printf("Enter to account ID: ");
                scanf("%d", &to_account_id);
                printf("Enter amount: ");
                scanf("%f", &amount);
                transfer_between_accounts(&db, from_account_id, to_account_id, amount);
                break;
            }
            case 10: {
                apply_interest(&db);
                printf("Interest applied to all eligible accounts.\n");
                break;
            }
            case 11: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 12: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 13: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 14: {
                calculate_loan_payments(&db);
                break;
            }
            case 15: {
                detect_fraudulent_transactions(&db);
                break;
            }
            case 16: {
                display_top_customers(&db);
                break;
            }
            case 17: {
                display_branch_statistics(&db);
                break;
            }
            case 0: {
                printf("Exiting program.\n");
                break;
            }
            default: {
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }
    } while (choice != 0);
    
    return 0;
}

void initialize_database(BankDatabase *db) {
    <｜fim▁hole｜>db->customer_count = 0;
    db->account_count = 0;
    db->transaction_count = 0;
}

int add_customer(BankDatabase *db, const char *name, const char *phone, const char *email,
                 const char *address, Date birth_date, int ssn, const char *occupation,
                 int annual_income) {
    if (db->customer_count >= MAX_CUSTOMERS) {
        return -1;
    }
    
    Customer *customer = &db->customers[db->customer_count];
    customer->id = db->customer_count + 1;
    strncpy(customer->name, name, MAX_NAME_LENGTH - 1);
    customer->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->phone, phone, 14);
    customer->phone[14] = '\0';
    strncpy(customer->email, email, 49);
    customer->email[49] = '\0';
    strncpy(customer->address, address, MAX_ADDRESS_LENGTH - 1);
    customer->address[MAX_ADDRESS_LENGTH - 1] = '\0';
    customer->birth_date = birth_date;
    customer->ssn = ssn;
    strncpy(customer->occupation, occupation, 49);
    customer->occupation[49] = '\0';
    customer->annual_income = annual_income;
    customer->credit_score = 300 + (rand() % 550); // Random credit score between 300-850
    customer->active = 1;
    
    db->customer_count++;
    return customer->id;
}

int add_account(BankDatabase *db, int customer_id, AccountType type, const char *account_number,
                float initial_balance, float interest_rate, float credit_limit) {
    if (db->account_count >= MAX_ACCOUNTS) {
        return -1;
    }
    
    // Validate customer exists
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        return -1;
    }
    
    Account *account = &db->accounts[db->account_count];
    account->id = db->account_count + 1;
    account->customer_id = customer_id;
    account->type = type;
    strncpy(account->account_number, account_number, 19);
    account->account_number[19] = '\0';
    account->balance = initial_balance;
    account->interest_rate = interest_rate;
    account->active = 1;
    
    // Set opening date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    account->opening_date.day = current_time->tm_mday;
    account->opening_date.month = current_time->tm_mon + 1;
    account->opening_date.year = current_time->tm_year + 1900;
    account->last_transaction_date = account->opening_date;
    
    // Set additional fields based on account type
    if (type == CREDIT_CARD) {
        account->credit_limit = credit_limit;
        account->balance = -initial_balance; // Negative balance for credit card debt
    } else if (type == LOAN) {
        account->loan_amount = initial_balance;
        account->balance = initial_balance; // Positive balance for loan amount
        account->monthly_payment = 0.0f;
        account->loan_term_months = 0;
    }
    
    db->account_count++;
    
    // Create initial transaction if there's an initial balance
    if (initial_balance != 0.0f && type != CREDIT_CARD) {
        char description[100];
        sprintf(description, "Initial deposit for account %s", account_number);
        process_transaction(db, 0, account->id, DEPOSIT, initial_balance, description);
    } else if (type == CREDIT_CARD && initial_balance != 0.0f) {
        char description[100];
        sprintf(description, "Initial charge for credit card %s", account_number);
        process_transaction(db, account->id, 0, WITHDRAWAL, initial_balance, description);
    }
    
    return account->id;
}

int process_transaction(BankDatabase *db, int from_account_id, int to_account_id,
                        TransactionType type, float amount, const char *description) {
    if (amount <= 0) {
        printf("Transaction amount must be positive.\n");
        return 0;
    }
    
    // Validate accounts
    Account *from_account = NULL;
    Account *to_account = NULL;
    
    if (from_account_id != 0) {
        int from_index = find_account_by_id(db, from_account_id);
        if (from_index == -1 || !db->accounts[from_index].active) {
            printf("Invalid from account.\n");
            return 0;
        }
        from_account = &db->accounts[from_index];
    }
    
    if (to_account_id != 0) {
        int to_index = find_account_by_id(db, to_account_id);
        if (to_index == -1 || !db->accounts[to_index].active) {
            printf("Invalid to account.\n");
            return 0;
        }
        to_account = &db->accounts[to_index];
    }
    
    // Check balance constraints
    if (from_account && type == WITHDRAWAL) {
        if (from_account->type == CHECKING && from_account->balance < amount) {
            printf("Insufficient funds in checking account.\n");
            return 0;
        } else if (from_account->type == CREDIT_CARD) {
            float available_credit = from_account->credit_limit + from_account->balance;
            if (available_credit < amount) {
                printf("Insufficient credit limit.\n");
                return 0;
            }
        }
    }
    
    if (from_account && to_account && type == TRANSFER) {
        if (from_account->balance < amount) {
            printf("Insufficient funds for transfer.\n");
            return 0;
        }
    }
    
    // Add transaction
    if (db->transaction_count >= MAX_TRANSACTIONS) {
        printf("Transaction database is full.\n");
        return 0;
    }
    
    Transaction *transaction = &db->transactions[db->transaction_count];
    transaction->id = db->transaction_count + 1;
    transaction->from_account_id = from_account_id;
    transaction->to_account_id = to_account_id;
    transaction->type = type;
    transaction->amount = amount;
    strncpy(transaction->description, description, 99);
    transaction->description[99] = '\0';
    transaction->processed = 1;
    transaction->timestamp = time(NULL);
    
    // Set transaction date
    struct tm *transaction_time = localtime(&transaction->timestamp);
    transaction->date.day = transaction_time->tm_mday;
    transaction->date.month = transaction_time->tm_mon + 1;
    transaction->date.year = transaction_time->tm_year + 1900;
    
    // Update account balances
    if (from_account) {
        if (type == WITHDRAWAL || type == TRANSFER) {
            from_account->balance -= amount;
        } else if (type == PAYMENT) {
            from_account->balance += amount; // Payment reduces debt
        }
        from_account->last_transaction_date = transaction->date;
    }
    
    if (to_account) {
        if (type == DEPOSIT || type == TRANSFER) {
            to_account->balance += amount;
        } else if (type == WITHDRAWAL) {
            to_account->balance -= amount; // For credit cards, withdrawal increases debt
        }
        to_account->last_transaction_date = transaction->date;
    }
    
    db->transaction_count++;
    return 1;
}

void apply_interest(BankDatabase *db) {
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    Date current_date = {
        current_time->tm_mday, 
        current_time->tm_mon + 1, 
        current_time->tm_year + 1900
    };
    
    for (int i = 0; i < db->account_count; i++) {
        Account *account = &db->accounts[i];
        if (!account->active) continue;
        
        // Check if at least one month has passed since last transaction
        int months_passed = 0;
        months_passed = (current_date.year - account->last_transaction_date.year) * 12 + 
                        (current_date.month - account->last_transaction_date.month);
        
        if (months_passed < 1) continue;
        
        float interest = 0.0f;
        char description[100];
        
        switch (account->type) {
            case SAVINGS:
            case INVESTMENT:
                interest = account->balance * account->interest_rate * (months_passed / 12.0f);
                account->balance += interest;
                sprintf(description, "Interest applied for %d months", months_passed);
                process_transaction(db, 0, account->id, INTEREST, interest, description);
                break;
                
            case CREDIT_CARD:
                interest = fabsf(account->balance) * account->interest_rate * (months_passed / 12.0f);
                account->balance -= interest; // Increase debt
                sprintf(description, "Interest charged for %d months", months_passed);
                process_transaction(db, account->id, 0, FEE, interest, description);
                break;
                
            case LOAN:
                interest = account->balance * account->interest_rate * (months_passed / 12.0f);
                // Interest is handled separately for loans in calculate_loan_payments
                break;
                
            default:
                break;
        }
    }
}

void display_customer(const Customer *customer, const BankDatabase *db) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s\n", customer->name);
    printf("Phone: %s\n", customer->phone);
    printf("Email: %s\n", customer->email);
    printf("Address: %s\n", customer->address);
    printf("Birth Date: %02d/%02d/%d\n", 
           customer->birth_date.day, customer->birth_date.month, customer->birth_date.year);
    printf("Registration Date: %02d/%02d/%d\n", 
           customer->registration_date.day, customer->registration_date.month, customer->registration_date.year);
    printf("SSN: %d\n", customer->ssn);
    printf("Occupation: %s\n", customer->occupation);
    printf("Annual Income: $%d\n", customer->annual_income);
    printf("Credit Score: %d\n", customer->credit_score);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
    
    // Display customer's accounts
    printf("Accounts:\n");
    for (int i = 0; i < db->account_count; i++) {
        if (db->accounts[i].customer_id == customer->id && db->accounts[i].active) {
            printf("  - Account ID: %d, Type: %s, Balance: $%.2f\n", 
                   db->accounts[i].id, get_account_type_name(db->accounts[i].type), 
                   db->accounts[i].balance);
        }
    }
}

void display_account(const Account *account, const Customer *customer) {
    printf("\nAccount ID: %d\n", account->id);
    printf("Account Number: %s\n", account->account_number);
    printf("Customer: %s (ID: %d)\n", customer->name, account->customer_id);
    printf("Type: %s\n", get_account_type_name(account->type));
    printf("Balance: $%.2f\n", account->balance);
    printf("Interest Rate: %.2f%%\n", account->interest_rate * 100);
    printf("Opening Date: %02d/%02d/%d\n", 
           account->opening_date.day, account->opening_date.month, account->opening_date.year);
    printf("Last Transaction Date: %02d/%02d/%d\n", 
           account->last_transaction_date.day, account->last_transaction_date.month, 
           account->last_transaction_date.year);
    printf("Status: %s\n", account->active ? "Active" : "Inactive");
    
    if (account->type == CREDIT_CARD) {
        printf("Credit Limit: $%.2f\n", account->credit_limit);
        printf("Available Credit: $%.2f\n", account->credit_limit + account->balance);
    } else if (account->type == LOAN) {
        printf("Loan Amount: $%.2f\n", account->loan_amount);
        printf("Monthly Payment: $%.2f\n", account->monthly_payment);
        printf("Loan Term: %d months\n", account->loan_term_months);
    }
}

void display_transaction(const Transaction *transaction, const BankDatabase *db) {
    printf("\nTransaction ID: %d\n", transaction->id);
    printf("Type: %s\n", get_transaction_type_name(transaction->type));
    printf("Amount: $%.2f\n", transaction->amount);
    printf("Date: %02d/%02d/%d\n", 
           transaction->date.day, transaction->date.month, transaction->date.year);
    printf("Description: %s\n", transaction->description);
    
    if (transaction->from_account_id != 0) {
        int index = find_account_by_id(db, transaction->from_account_id);
        if (index != -1) {
            printf("From Account: %s\n", db->accounts[index].account_number);
        }
    } else {
        printf("From Account: Bank\n");
    }
    
    if (transaction->to_account_id != 0) {
        int index = find_account_by_id(db, transaction->to_account_id);
        if (index != -1) {
            printf("To Account: %s\n", db->accounts[index].account_number);
        }
    } else {
        printf("To Account: Bank\n");
    }
    
    printf("Status: %s\n", transaction->processed ? "Processed" : "Pending");
}

void display_all_customers(const BankDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_accounts(const BankDatabase *db) {
    printf("\n=== All Accounts ===\n");
    printf("Total Accounts: %d\n\n", db->account_count);
    
    for (int i = 0; i < db->account_count; i++) {
        if (db->accounts[i].active) {
            int customer_index = find_customer_by_id(db, db->accounts[i].customer_id);
            display_account(&db->accounts[i], &db->customers[customer_index]);
            printf("------------------------\n");
        }
    }
}

void display_account_transactions(const BankDatabase *db, int account_id) {
    int account_index = find_account_by_id(db, account_id);
    if (account_index == -1 || !db->accounts[account_index].active) {
        printf("Account not found or inactive.\n");
        return;
    }
    
    printf("\n=== Transactions for Account %s ===\n", 
           db->accounts[account_index].account_number);
    
    int found = 0;
    for (int i = 0; i < db->transaction_count; i++) {
        if (db->transactions[i].processed && 
            (db->transactions[i].from_account_id == account_id || 
             db->transactions[i].to_account_id == account_id)) {
            display_transaction(&db->transactions[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No transactions found for this account.\n");
    }
}

int find_customer_by_id(const BankDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_account_by_id(const BankDatabase *db, int account_id) {
    for (int i = 0; i < db->account_count; i++) {
        if (db->accounts[i].id == account_id && db->accounts[i].active) {
            return i;
        }
    }
    return -1;
}

int find_account_by_number(const BankDatabase *db, const char *account_number) {
    for (int i = 0; i < db->account_count; i++) {
        if (strcmp(db->accounts[i].account_number, account_number) == 0 && 
            db->accounts[i].active) {
            return i;
        }
    }
    return -1;
}

void search_customers_by_name(const BankDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active && strstr(db->customers[i].name, name) != NULL) {
            display_customer(&db->customers[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No customers found with name containing '%s'.\n", name);
    }
}

void generate_monthly_statement(const BankDatabase *db, int account_id, int month, int year) {
    int account_index = find_account_by_id(db, account_id);
    if (account_index == -1 || !db->accounts[account_index].active) {
        printf("Account not found or inactive.\n");
        return;
    }
    
    Account *account = &db->accounts[account_index];
    int customer_index = find_customer_by_id(db, account->customer_id);
    Customer *customer = &db->customers[customer_index];
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Monthly Statement for %s %s ===\n", 
           month_names[month - 1], year);
    printf("Account: %s (%s)\n", account->account_number, get_account_type_name(account->type));
    printf("Account Holder: %s\n", customer->name);
    printf("Address: %s\n\n", customer->address);
    
    float deposits = 0.0f, withdrawals = 0.0f, interest = 0.0f, fees = 0.0f;
    int transaction_count = 0;
    
    printf("Date\t\tDescription\t\tAmount\tBalance\n");
    printf("--------------------------------------------------------\n");
    
    float running_balance = 0.0f;
    
    // Find opening balance
    for (int i = 0; i < db->transaction_count; i++) {
        if (db->transactions[i].processed && 
            (db->transactions[i].from_account_id == account_id || 
             db->transactions[i].to_account_id == account_id)) {
            
            Transaction *t = &db->transactions[i];
            
            // Check if transaction is before the statement period
            if (t->date.year < year || 
                (t->date.year == year && t->date.month < month)) {
                
                // Update running balance based on transaction type
                if (t->to_account_id == account_id) {
                    running_balance += t->amount;
                } else if (t->from_account_id == account_id) {
                    running_balance -= t->amount;
                }
            }
        }
    }
    
    printf("%02d/%02d/%d\tOpening Balance\t\t\t$%.2f\n", 
           1, month, year, running_balance);
    
    // Display transactions for the month
    for (int i = 0; i < db->transaction_count; i++) {
        if (db->transactions[i].processed && 
            (db->transactions[i].from_account_id == account_id || 
             db->transactions[i].to_account_id == account_id)) {
            
            Transaction *t = &db->transactions[i];
            
            // Check if transaction is in the statement month
            if (t->date.year == year && t->date.month == month) {
                printf("%02d/%02d/%d\t%-20s\t$%.2f\t", 
                       t->date.day, t->date.month, t->date.year, 
                       t->description, t->amount);
                
                // Update running balance and transaction totals
                if (t->to_account_id == account_id) {
                    running_balance += t->amount;
                    if (t->type == DEPOSIT) deposits += t->amount;
                    else if (t->type == INTEREST) interest += t->amount;
                } else if (t->from_account_id == account_id) {
                    running_balance -= t->amount;
                    if (t->type == WITHDRAWAL) withdrawals += t->amount;
                    else if (t->type == FEE) fees += t->amount;
                }
                
                printf("$%.2f\n", running_balance);
                transaction_count++;
            }
        }
    }
    
    printf("--------------------------------------------------------\n");
    printf("Summary:\n");
    printf("Total Deposits: $%.2f\n", deposits);
    printf("Total Withdrawals: $%.2f\n", withdrawals);
    printf("Interest Earned: $%.2f\n", interest);
    printf("Fees Charged: $%.2f\n", fees);
    printf("Closing Balance: $%.2f\n", running_balance);
    printf("Total Transactions: %d\n", transaction_count);
}

void transfer_between_accounts(BankDatabase *db, int from_account_id, int to_account_id, float amount) {
    int from_index = find_account_by_id(db, from_account_id);
    int to_index = find_account_by_id(db, to_account_id);
    
    if (from_index == -1 || !db->accounts[from_index].active) {
        printf("Source account not found or inactive.\n");
        return;
    }
    
    if (to_index == -1 || !db->accounts[to_index].active) {
        printf("Destination account not found or inactive.\n");
        return;
    }
    
    if (db->accounts[from_index].balance < amount) {
        printf("Insufficient funds in source account.\n");
        return;
    }
    
    char description[100];
    sprintf(description, "Transfer from %s to %s", 
            db->accounts[from_index].account_number, db->accounts[to_index].account_number);
    
    if (process_transaction(db, from_account_id, to_account_id, TRANSFER, amount, description)) {
        printf("Transfer of $%.2f from account %s to account %s completed successfully.\n", 
               amount, db->accounts[from_index].account_number, db->accounts[to_index].account_number);
    } else {
        printf("Transfer failed.\n");
    }
}

int save_database(const BankDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "wb");
    if (customers_file != NULL) {
        fwrite(&db->customer_count, sizeof(int), 1, customers_file);
        fwrite(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *accounts_file = fopen(FILENAME_ACCOUNTS, "wb");
    if (accounts_file != NULL) {
        fwrite(&db->account_count, sizeof(int), 1, accounts_file);
        fwrite(db->accounts, sizeof(Account), db->account_count, accounts_file);
        fclose(accounts_file);
    } else {
        return 0;
    }
    
    FILE *transactions_file = fopen(FILENAME_TRANSACTIONS, "wb");
    if (transactions_file != NULL) {
        fwrite(&db->transaction_count, sizeof(int), 1, transactions_file);
        fwrite(db->transactions, sizeof(Transaction), db->transaction_count, transactions_file);
        fclose(transactions_file);
        return 1;
    }
    
    return 0;
}

int load_database(BankDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "rb");
    if (customers_file != NULL) {
        fread(&db->customer_count, sizeof(int), 1, customers_file);
        fread(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *accounts_file = fopen(FILENAME_ACCOUNTS, "rb");
    if (accounts_file != NULL) {
        fread(&db->account_count, sizeof(int), 1, accounts_file);
        fread(db->accounts, sizeof(Account), db->account_count, accounts_file);
        fclose(accounts_file);
    } else {
        return 0;
    }
    
    FILE *transactions_file = fopen(FILENAME_TRANSACTIONS, "rb");
    if (transactions_file != NULL) {
        fread(&db->transaction_count, sizeof(int), 1, transactions_file);
        fread(db->transactions, sizeof(Transaction), db->transaction_count, transactions_file);
        fclose(transactions_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(BankDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* occupations[] = {"Engineer", "Teacher", "Doctor", "Lawyer", "Accountant", 
                          "Manager", "Salesperson", "Nurse", "Technician", "Analyst"};
    
    srand(time(NULL));
    
    // Add 50 customers
    for (int i = 0; i < 50 && db->customer_count < MAX_CUSTOMERS; i++) {
        char name[60], email[60], phone[15], address[100], occupation[50];
        sprintf(name, "%s %s", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(email, "%s.%s@email.com", first_names[rand() % 18], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        strcpy(occupation, occupations[rand() % 10]);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1950 + (rand() % 50);
        Date birth_date = {day, month, year};
        
        int annual_income = 30000 + (rand() % 150000);
        
        add_customer(db, name, phone, email, address, birth_date, 
                    1000000 + i, occupation, annual_income);
        
        // Update registration date to a recent date
        db->customers[i].registration_date.day = 1 + (rand() % 28);
        db->customers[i].registration_date.month = 1 + (rand() % 12);
        db->customers[i].registration_date.year = 2010 + (rand() % 10);
    }
    
    // Add accounts for customers
    for (int i = 0; i < db->customer_count; i++) {
        if (db->account_count >= MAX_ACCOUNTS) break;
        
        int customer_id = db->customers[i].id;
        
        // Add a checking account
        char account_num[20];
        sprintf(account_num, "CHK%06d", 100001 + db->account_count);
        add_account(db, customer_id, CHECKING, account_num, 
                  100.0f + (rand() % 5000), 0.01f, 0.0f);
        
        // Add a savings account
        sprintf(account_num, "SAV%06d", 100001 + db->account_count);
        add_account(db, customer_id, SAVINGS, account_num, 
                  500.0f + (rand() % 10000), 0.02f, 0.0f);
        
        // Add credit card for some customers
        if (rand() % 2) {
            sprintf(account_num, "CC%06d", 100001 + db->account_count);
            add_account(db, customer_id, CREDIT_CARD, account_num, 
                      0.0f, 0.18f, 5000.0f + (rand() % 15000));
        }
        
        // Add investment account for some customers
        if (rand() % 3) {
            sprintf(account_num, "INV%06d", 100001 + db->account_count);
            add_account(db, customer_id, INVESTMENT, account_num, 
                      1000.0f + (rand() % 20000), 0.05f, 0.0f);
        }
    }
    
    // Add some transactions
    for (int i = 0; i < 500 && db->transaction_count < MAX_TRANSACTIONS; i++) {
        int account_id = 1 + (rand() % db->account_count);
        Account *account = &db->accounts[account_id - 1];
        
        float amount = 10.0f + (rand() % 1000);
        TransactionType type;
        int from_id = 0, to_id = 0;
        char description[100];
        
        // Generate transaction based on account type
        switch (account->type) {
            case CHECKING:
                if (rand() % 2) {
                    type = DEPOSIT;
                    to_id = account_id;
                    sprintf(description, "Paycheck deposit");
                } else {
                    type = WITHDRAWAL;
                    from_id = account_id;
                    sprintf(description, "ATM withdrawal");
                }
                break;
                
            case SAVINGS:
                type = DEPOSIT;
                to_id = account_id;
                sprintf(description, "Savings deposit");
                break;
                
            case CREDIT_CARD:
                type = WITHDRAWAL;
                from_id = account_id;
                sprintf(description, "Purchase at store");
                break;
                
            case INVESTMENT:
                type = DEPOSIT;
                to_id = account_id;
                sprintf(description, "Investment contribution");
                break;
                
            default:
                continue;
        }
        
        process_transaction(db, from_id, to_id, type, amount, description);
        
        // Randomly generate some transfers
        if (rand() % 10 == 0 && db->account_count > 1) {
            int from_account = 1 + (rand() % db->account_count);
            int to_account;
            do {
                to_account = 1 + (rand() % db->account_count);
            } while (to_account == from_account);
            
            amount = 50.0f + (rand() % 500);
            sprintf(description, "Transfer to another account");
            process_transaction(db, from_account, to_account, TRANSFER, amount, description);
        }
    }
}

void calculate_loan_payments(BankDatabase *db) {
    printf("\n=== Loan Payments Calculation ===\n");
    
    for (int i = 0; i < db->account_count; i++) {
        Account *account = &db->accounts[i];
        if (account->active && account->type == LOAN) {
            if (account->monthly_payment > 0.0f) {
                float total_payment = account->monthly_payment * account->loan_term_months;
                float total_interest = total_payment - account->loan_amount;
                
                printf("Loan Account: %s\n", account->account_number);
                printf("  Principal: $%.2f\n", account->loan_amount);
                printf("  Monthly Payment: $%.2f\n", account->monthly_payment);
                printf("  Loan Term: %d months\n", account->loan_term_months);
                printf("  Total Payment: $%.2f\n", total_payment);
                printf("  Total Interest: $%.2f\n\n", total_interest);
            } else {
                printf("Loan Account: %s - No payment schedule set\n", account->account_number);
            }
        }
    }
}

void detect_fraudulent_transactions(BankDatabase *db) {
    printf("\n=== Fraud Detection ===\n");
    
    int suspicious_count = 0;
    
    // Look for unusual transaction patterns
    for (int i = 0; i < db->transaction_count; i++) {
        Transaction *transaction = &db->transactions[i];
        if (!transaction->processed) continue;
        
        int suspicious = 0;
        char reason[100] = "";
        
        // Large transactions
        if (transaction->amount > 5000.0f) {
            suspicious = 1;
            strcat(reason, "Large amount; ");
        }
        
        // Frequent transactions from same account in short time
        int same_account_count = 0;
        for (int j = 0; j < db->transaction_count; j++) {
            if (i != j && db->transactions[j].processed &&
                (db->transactions[j].from_account_id == transaction->from_account_id ||
                 db->transactions[j].to_account_id == transaction->to_account_id)) {
                
                double time_diff = difftime(transaction->timestamp, db->transactions[j].timestamp);
                if (time_diff < 3600 && time_diff > -3600) { // Within 1 hour
                    same_account_count++;
                }
            }
        }
        
        if (same_account_count > 5) {
            suspicious = 1;
            strcat(reason, "Frequent transactions; ");
        }
        
        // Unusual transaction times (middle of night)
        struct tm *transaction_time = localtime(&transaction->timestamp);
        if (transaction_time->tm_hour < 6 || transaction_time->tm_hour > 23) {
            suspicious = 1;
            strcat(reason, "Unusual time; ");
        }
        
        if (suspicious) {
            printf("Suspicious Transaction ID: %d\n", transaction->id);
            printf("  Reason: %s\n", reason);
            printf("  Amount: $%.2f\n", transaction->amount);
            printf("  Date: %02d/%02d/%d\n", 
                   transaction->date.day, transaction->date.month, transaction->date.year);
            
            if (transaction->from_account_id != 0) {
                int account_index = find_account_by_id(db, transaction->from_account_id);
                if (account_index != -1) {
                    printf("  From Account: %s\n", db->accounts[account_index].account_number);
                }
            }
            
            if (transaction->to_account_id != 0) {
                int account_index = find_account_by_id(db, transaction->to_account_id);
                if (account_index != -1) {
                    printf("  To Account: %s\n", db->accounts[account_index].account_number);
                }
            }
            
            printf("\n");
            suspicious_count++;
        }
    }
    
    if (suspicious_count == 0) {
        printf("No suspicious transactions detected.\n");
    } else {
        printf("Total suspicious transactions: %d\n", suspicious_count);
    }
}

void display_top_customers(const BankDatabase *db) {
    printf("\n=== Top Customers by Total Balance ===\n");
    
    // Calculate total balance for each customer
    float customer_balances[MAX_CUSTOMERS] = {0.0f};
    
    for (int i = 0; i < db->customer_count; i++) {
        if (!db->customers[i].active) continue;
        
        for (int j = 0; j < db->account_count; j++) {
            if (db->accounts[j].active && db->accounts[j].customer_id == db->customers[i].id) {
                if (db->accounts[j].type != LOAN && db->accounts[j].type != CREDIT_CARD) {
                    customer_balances[i] += db->accounts[j].balance;
                } else if (db->accounts[j].type == LOAN) {
                    customer_balances[i] -= db->accounts[j].balance; // Loans are liabilities
                } else if (db->accounts[j].type == CREDIT_CARD) {
                    customer_balances[i] += db->accounts[j].balance; // Negative balance for debt
                }
            }
        }
    }
    
    // Create a temporary array to sort customers by balance
    int sorted_indices[MAX_CUSTOMERS];
    for (int i = 0; i < db->customer_count; i++) {
        sorted_indices[i] = i;
    }
    
    // Sort customers by total balance (descending)
    for (int i = 0; i < db->customer_count - 1; i++) {
        for (int j = 0; j < db->customer_count - i - 1; j++) {
            if (customer_balances[sorted_indices[j]] < customer_balances[sorted_indices[j + 1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 customers
    int count = db->customer_count < 10 ? db->customer_count : 10;
    printf("%-5s %-20s %-15s %-15s %-15s\n", 
           "Rank", "Name", "Credit Score", "Annual Income", "Total Balance");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        int index = sorted_indices[i];
        if (db->customers[index].active) {
            printf("%-5d %-20s %-15d $%-14d $%-14.2f\n", 
                   i + 1, db->customers[index].name, 
                   db->customers[index].credit_score, 
                   db->customers[index].annual_income, 
                   customer_balances[index]);
        }
    }
}

void display_branch_statistics(const BankDatabase *db) {
    printf("\n=== Branch Statistics ===\n");
    
    int active_customers = 0;
    int active_accounts = 0;
    float total_deposits = 0.0f;
    float total_loans = 0.0f;
    int account_types[5] = {0}; // Savings, Checking, Credit Card, Loan, Investment
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            active_customers++;
        }
    }
    
    for (int i = 0; i < db->account_count; i++) {
        if (db->accounts[i].active) {
            active_accounts++;
            account_types[db->accounts[i].type]++;
            
            if (db->accounts[i].type == SAVINGS || db->accounts[i].type == CHECKING || 
                db->accounts[i].type == INVESTMENT) {
                total_deposits += db->accounts[i].balance;
            } else if (db->accounts[i].type == LOAN) {
                total_loans += db->accounts[i].balance;
            } else if (db->accounts[i].type == CREDIT_CARD) {
                total_deposits += db->accounts[i].balance; // Negative balance for debt
            }
        }
    }
    
    printf("Active Customers: %d\n", active_customers);
    printf("Active Accounts: %d\n", active_accounts);
    printf("Total Deposits: $%.2f\n", total_deposits);
    printf("Total Loans: $%.2f\n", total_loans);
    printf("Net Funds: $%.2f\n", total_deposits - total_loans);
    
    printf("\nAccount Types:\n");
    printf("  Savings Accounts: %d\n", account_types[SAVINGS]);
    printf("  Checking Accounts: %d\n", account_types[CHECKING]);
    printf("  Credit Cards: %d\n", account_types[CREDIT_CARD]);
    printf("  Loans: %d\n", account_types[LOAN]);
    printf("  Investment Accounts: %d\n", account_types[INVESTMENT]);
    
    // Calculate average income and credit score
    if (active_customers > 0) {
        int total_income = 0;
        int total_credit_score = 0;
        
        for (int i = 0; i < db->customer_count; i++) {
            if (db->customers[i].active) {
                total_income += db->customers[i].annual_income;
                total_credit_score += db->customers[i].credit_score;
            }
        }
        
        printf("\nAverage Annual Income: $%.2f\n", (float)total_income / active_customers);
        printf("Average Credit Score: %.1f\n", (float)total_credit_score / active_customers);
    }
    
    // Transaction statistics
    int transaction_types[6] = {0}; // Deposit, Withdrawal, Transfer, Payment, Interest, Fee
    float transaction_amounts[6] = {0.0f};
    
    for (int i = 0; i < db->transaction_count; i++) {
        if (db->transactions[i].processed) {
            transaction_types[db->transactions[i].type]++;
            transaction_amounts[db->transactions[i].type] += db->transactions[i].amount;
        }
    }
    
    printf("\nTransaction Statistics:\n");
    printf("  Total Transactions: %d\n", db->transaction_count);
    printf("  Deposits: %d (Total: $%.2f)\n", transaction_types[DEPOSIT], transaction_amounts[DEPOSIT]);
    printf("  Withdrawals: %d (Total: $%.2f)\n", transaction_types[WITHDRAWAL], transaction_amounts[WITHDRAWAL]);
    printf("  Transfers: %d (Total: $%.2f)\n", transaction_types[TRANSFER], transaction_amounts[TRANSFER]);
    printf("  Payments: %d (Total: $%.2f)\n", transaction_types[PAYMENT], transaction_amounts[PAYMENT]);
    printf("  Interest: %d (Total: $%.2f)\n", transaction_types[INTEREST], transaction_amounts[INTEREST]);
    printf("  Fees: %d (Total: $%.2f)\n", transaction_types[FEE], transaction_amounts[FEE]);
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Check for leap year
    if (month == 2 && ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0))) {
        days_in_month[1] = 29;
    }
    
    if (day < 1 || day > days_in_month[month - 1]) return 0;
    
    return 1;
}

const char* get_account_type_name(AccountType type) {
    switch (type) {
        case SAVINGS: return "Savings";
        case CHECKING: return "Checking";
        case CREDIT_CARD: return "Credit Card";
        case LOAN: return "Loan";
        case INVESTMENT: return "Investment";
        default: return "Unknown";
    }
}

const char* get_transaction_type_name(TransactionType type) {
    switch (type) {
        case DEPOSIT: return "Deposit";
        case WITHDRAWAL: return "Withdrawal";
        case TRANSFER: return "Transfer";
        case PAYMENT: return "Payment";
        case INTEREST: return "Interest";
        case FEE: return "Fee";
        default: return "Unknown";
    }
}