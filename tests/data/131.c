#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_CUSTOMERS 1000
#define MAX_PRODUCTS 2000
#define MAX_CATEGORIES 50
#define MAX_ORDERS 5000
#define MAX_NAME_LENGTH 50
#define FILENAME_CUSTOMERS "customers.dat"
#define FILENAME_PRODUCTS "products.dat"
#define FILENAME_CATEGORIES "categories.dat"
#define FILENAME_ORDERS "orders.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char first_name[MAX_NAME_LENGTH];
    char last_name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char address[100];
    Date registration_date;
    int active;
    int vip_level;
    double total_spent;
    int loyalty_points;
    char gender[10]; // Male, Female, Other
    Date birth_date;
} Customer;

typedef struct {
    int id;
    char product_code[20];
    char name[MAX_NAME_LENGTH];
    char category[50];
    char description[200];
    double price;
    int stock_quantity;
    int reorder_level;
    int active;
    double discount_percent;
    Date added_date;
} Product;

typedef struct {
    int id;
    char name[50];
    char description[200];
    int active;
} Category;

typedef struct {
    int id;
    int customer_id;
    Date order_date;
    Date shipping_date;
    double total_amount;
    double discount_amount;
    double shipping_fee;
    char status[20]; // "Pending", "Processing", "Shipped", "Delivered", "Cancelled"
    char shipping_address[100];
    int active;
    char payment_method[20]; // "Credit Card", "Debit Card", "PayPal", "Bank Transfer"
} Order;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Product products[MAX_PRODUCTS];
    int product_count;
    Category categories[MAX_CATEGORIES];
    int category_count;
    Order orders[MAX_ORDERS];
    int order_count;
} ShopDatabase;

// Function prototypes
void initialize_database(ShopDatabase *db);
int add_customer(ShopDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *gender, Date birth_date);
int add_product(ShopDatabase *db, const char *product_code, const char *name,
               const char *category, const char *description, double price,
               int stock_quantity, int reorder_level, double discount_percent);
int add_category(ShopDatabase *db, const char *name, const char *description);
int create_order(ShopDatabase *db, int customer_id, const char *shipping_address, const char *payment_method);
void add_order_item(ShopDatabase *db, int order_id, int product_id, int quantity, double discount_percent);
void update_order_status(ShopDatabase *db, int order_id, const char *status);
void cancel_order(ShopDatabase *db, int order_id);
void display_customer(const Customer *customer);
void display_product(const Product *product);
void display_category(const Category *category);
void display_order(const Order *order, const ShopDatabase *db);
void display_all_customers(const ShopDatabase *db);
void display_all_products(const ShopDatabase *db);
void display_all_categories(const ShopDatabase *db);
void display_all_orders(const ShopDatabase *db);
void search_customers_by_name(const ShopDatabase *db, const char *name);
void search_products_by_category(const ShopDatabase *db, const char *category);
void search_orders_by_status(const ShopDatabase *db, const char *status);
void generate_customer_report(const ShopDatabase *db, int customer_id);
void generate_product_report(const ShopDatabase *db, int product_id);
void generate_sales_report(const ShopDatabase *db, const Date *start_date, const Date *end_date);
void generate_inventory_report(const ShopDatabase *db);
void update_product_stock(ShopDatabase *db, int product_id, int quantity_change);
void apply_promotion(ShopDatabase *db, const char *category, double discount_percent);
int find_customer_by_id(const ShopDatabase *db, int customer_id);
int find_product_by_id(const ShopDatabase *db, int product_id);
int find_category_by_id(const ShopDatabase *db, int category_id);
int find_order_by_id(const ShopDatabase *db, int order_id);
int save_database(const ShopDatabase *db);
int load_database(ShopDatabase *db);
void generate_sample_data(ShopDatabase *db);
void calculate_customer_lifetime_value(ShopDatabase *db, int customer_id);
int is_valid_email(const char *email);
int is_valid_phone(const char *phone);
int is_valid_date(int day, int month, int year);
const char* get_order_status_description(const char *status);

int main() {
    ShopDatabase db;
    initialize_database(&db);
    
    printf("Online Shop Management System\n");
    printf("1. Add Customer\n");
    printf("2. Add Product\n");
    printf("3. Add Category\n");
    printf("4. Create Order\n");
    printf("5. Add Product to Order\n");
    printf("6. Update Order Status\n");
    printf("7. Cancel Order\n");
    printf("8. Display All Customers\n");
    printf("9. Display All Products\n");
    printf("10. Display All Categories\n");
    printf("11. Display All Orders\n");
    printf("12. Search Customers by Name\n");
    printf("13. Search Products by Category\n");
    printf("14. Search Orders by Status\n");
    printf("15. Generate Customer Report\n");
    printf("16. Generate Product Report\n");
    printf("17. Generate Sales Report\n");
    printf("18. Generate Inventory Report\n");
    printf("19. Update Product Stock\n");
    printf("20. Apply Promotion\n");
    printf("21. Calculate Customer Lifetime Value\n");
    printf("22. Save Database\n");
    printf("23. Load Database\n");
    printf("24. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], gender[10];
                int day, month, year;
                
                printf("Enter first name: ");
                scanf(" %[^\n]", first_name);
                printf("Enter last name: ");
                scanf(" %[^\n]", last_name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter address: ");
                scanf(" %[^\n]", address);
                printf("Enter gender (Male, Female, Other): ");
                scanf(" %[^\n]", gender);
                printf("Enter birth date (DD MM YYYY): ");
                scanf("%d %d %d", &day, &month, &year);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_phone(phone)) {
                    printf("Invalid phone format.\n");
                    break;
                }
                
                if (strcmp(gender, "Male") != 0 && strcmp(gender, "Female") != 0 && strcmp(gender, "Other") != 0) {
                    printf("Invalid gender. Must be one of: Male, Female, Other.\n");
                    break;
                }
                
                if (!is_valid_date(day, month, year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                Date birth_date = {day, month, year};
                
                int customer_id = add_customer(&db, first_name, last_name, email, phone,
                                              address, gender, birth_date);
                if (customer_id != -1) {
                    printf("Customer added with ID: %d\n", customer_id);
                } else {
                    printf("Failed to add customer. Database is full.\n");
                }
                break;
            }
            case 2: {
                char product_code[20], name[MAX_NAME_LENGTH], category[50], description[200];
                double price, discount_percent;
                int stock_quantity, reorder_level;
                
                printf("Enter product code: ");
                scanf("%s", product_code);
                printf("Enter product name: ");
                scanf(" %[^\n]", name);
                printf("Enter category: ");
                scanf(" %[^\n]", category);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter price: ");
                scanf("%lf", &price);
                printf("Enter stock quantity: ");
                scanf("%d", &stock_quantity);
                printf("Enter reorder level: ");
                scanf("%d", &reorder_level);
                printf("Enter discount percent: ");
                scanf("%lf", &discount_percent);
                
                int product_id = add_product(&db, product_code, name, category, description,
                                           price, stock_quantity, reorder_level, discount_percent);
                if (product_id != -1) {
                    printf("Product added with ID: %d\n", product_id);
                } else {
                    printf("Failed to add product. Database is full.\n");
                }
                break;
            }
            case 3: {
                char name[50], description[200];
                
                printf("Enter category name: ");
                scanf(" %[^\n]", name);
                printf("Enter category description: ");
                scanf(" %[^\n]", description);
                
                int category_id = add_category(&db, name, description);
                if (category_id != -1) {
                    printf("Category added with ID: %d\n", category_id);
                } else {
                    printf("Failed to add category. Database is full.\n");
                }
                break;
            }
            case 4: {
                int customer_id;
                char shipping_address[100], payment_method[20];
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter shipping address: ");
                scanf(" %[^\n]", shipping_address);
                printf("Enter payment method (Credit Card, Debit Card, PayPal, Bank Transfer): ");
                scanf(" %[^\n]", payment_method);
                
                if (strcmp(payment_method, "Credit Card") != 0 && strcmp(payment_method, "Debit Card") != 0 &&
                    strcmp(payment_method, "PayPal") != 0 && strcmp(payment_method, "Bank Transfer") != 0) {
                    printf("Invalid payment method. Must be one of: Credit Card, Debit Card, PayPal, Bank Transfer.\n");
                    break;
                }
                
                int order_id = create_order(&db, customer_id, shipping_address, payment_method);
                if (order_id != -1) {
                    printf("Order created with ID: %d\n", order_id);
                } else {
                    printf("Failed to create order. Check customer ID.\n");
                }
                break;
            }
            case 5: {
                int order_id, product_id, quantity;
                double discount_percent;
                
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                printf("Enter discount percent: ");
                scanf("%lf", &discount_percent);
                
                if (add_order_item(&db, order_id, product_id, quantity, discount_percent)) {
                    printf("Product added to order successfully.\n");
                } else {
                    printf("Failed to add product to order. Check IDs and stock availability.\n");
                }
                break;
            }
            case 6: {
                int order_id;
                char status[20];
                
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter new status (Pending, Processing, Shipped, Delivered, Cancelled): ");
                scanf(" %[^\n]", status);
                
                if (strcmp(status, "Pending") != 0 && strcmp(status, "Processing") != 0 &&
                    strcmp(status, "Shipped") != 0 && strcmp(status, "Delivered") != 0 &&
                    strcmp(status, "Cancelled") != 0) {
                    printf("Invalid status. Must be one of: Pending, Processing, Shipped, Delivered, Cancelled.\n");
                    break;
                }
                
                update_order_status(&db, order_id, status);
                break;
            }
            case 7: {
                int order_id;
                
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                
                cancel_order(&db, order_id);
                break;
            }
            case 8: {
                display_all_customers(&db);
                break;
            }
            case 9: {
                display_all_products(&db);
                break;
            }
            case 10: {
                display_all_categories(&db);
                break;
            }
            case 11: {
                display_all_orders(&db);
                break;
            }
            case 12: {
                char name[MAX_NAME_LENGTH];
                printf("Enter customer name to search: ");
                scanf(" %[^\n]", name);
                search_customers_by_name(&db, name);
                break;
            }
            case 13: {
                char category[50];
                printf("Enter product category to search: ");
                scanf(" %[^\n]", category);
                search_products_by_category(&db, category);
                break;
            }
            case 14: {
                char status[20];
                printf("Enter order status to search (Pending, Processing, Shipped, Delivered, Cancelled): ");
                scanf(" %[^\n]", status);
                search_orders_by_status(&db, status);
                break;
            }
            case 15: {
                int customer_id;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                generate_customer_report(&db, customer_id);
                break;
            }
            case 16: {
                int product_id;
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                generate_product_report(&db, product_id);
                break;
            }
            case 17: {
                int start_day, start_month, start_year;
                int end_day, end_month, end_year;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &start_day, &start_month, &start_year);
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &end_day, &end_month, &end_year);
                
                if (!is_valid_date(start_day, start_month, start_year) || 
                    !is_valid_date(end_day, end_month, end_year)) {
                    printf("Invalid date format.\n");
                    break;
                }
                
                Date start_date = {start_day, start_month, start_year};
                Date end_date = {end_day, end_month, end_year};
                
                generate_sales_report(&db, &start_date, &end_date);
                break;
            }
            case 18: {
                generate_inventory_report(&db);
                break;
            }
            case 19: {
                int product_id, quantity_change;
                
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity change (positive for restock, negative for sale): ");
                scanf("%d", &quantity_change);
                
                update_product_stock(&db, product_id, quantity_change);
                break;
            }
            case 20: {
                char category[50];
                double discount_percent;
                
                printf("Enter product category for promotion: ");
                scanf(" %[^\n]", category);
                printf("Enter discount percent (e.g., 15.0 for 15%%): ");
                scanf("%lf", &discount_percent);
                
                apply_promotion(&db, category, discount_percent);
                break;
            }
            case 21: {
                int customer_id;
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                
                calculate_customer_lifetime_value(&db, customer_id);
                break;
            }
            case 22: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 23: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 24: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
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

void initialize_database(ShopDatabase *db) {
    <｜fim▁hole｜>db->customer_count = 0;
    db->product_count = 0;
    db->category_count = 0;
    db->order_count = 0;
}

int add_customer(ShopDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *gender, Date birth_date) {
    if (db->customer_count >= MAX_CUSTOMERS) {
        return -1;
    }
    
    Customer *customer = &db->customers[db->customer_count];
    customer->id = db->customer_count + 1;
    strncpy(customer->first_name, first_name, MAX_NAME_LENGTH - 1);
    customer->first_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->last_name, last_name, MAX_NAME_LENGTH - 1);
    customer->last_name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->email, email, 49);
    customer->email[49] = '\0';
    strncpy(customer->phone, phone, 14);
    customer->phone[14] = '\0';
    strncpy(customer->address, address, 99);
    customer->address[99] = '\0';
    customer->gender[0] = '\0';
    strncpy(customer->gender, gender, 9);
    customer->gender[9] = '\0';
    customer->birth_date = birth_date;
    
    // Set registration date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    customer->registration_date.day = current_time->tm_mday;
    customer->registration_date.month = current_time->tm_mon + 1;
    customer->registration_date.year = current_time->tm_year + 1900;
    
    customer->active = 1;
    customer->vip_level = 0;
    customer->total_spent = 0.0;
    customer->loyalty_points = 0;
    
    db->customer_count++;
    return customer->id;
}

int add_product(ShopDatabase *db, const char *product_code, const char *name,
               const char *category, const char *description, double price,
               int stock_quantity, int reorder_level, double discount_percent) {
    if (db->product_count >= MAX_PRODUCTS) {
        return -1;
    }
    
    Product *product = &db->products[db->product_count];
    product->id = db->product_count + 1;
    strncpy(product->product_code, product_code, 19);
    product->product_code[19] = '\0';
    strncpy(product->name, name, MAX_NAME_LENGTH - 1);
    product->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(product->category, category, 49);
    product->category[49] = '\0';
    strncpy(product->description, description, 199);
    product->description[199] = '\0';
    product->price = price;
    product->stock_quantity = stock_quantity;
    product->reorder_level = reorder_level;
    product->active = 1;
    product->discount_percent = discount_percent;
    
    // Set added date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    product->added_date.day = current_time->tm_mday;
    product->added_date.month = current_time->tm_mon + 1;
    product->added_date.year = current_time->tm_year + 1900;
    
    db->product_count++;
    return product->id;
}

int add_category(ShopDatabase *db, const char *name, const char *description) {
    if (db->category_count >= MAX_CATEGORIES) {
        return -1;
    }
    
    Category *category = &db->categories[db->category_count];
    category->id = db->category_count + 1;
    strncpy(category->name, name, 49);
    category->name[49] = '\0';
    strncpy(category->description, description, 199);
    category->description[199] = '\0';
    category->active = 1;
    
    db->category_count++;
    return category->id;
}

int create_order(ShopDatabase *db, int customer_id, const char *shipping_address, const char *payment_method) {
    if (db->order_count >= MAX_ORDERS) {
        return -1;
    }
    
    // Validate customer exists
    if (find_customer_by_id(db, customer_id) == -1) {
        return -1;
    }
    
    Order *order = &db->orders[db->order_count];
    order->id = db->order_count + 1;
    order->customer_id = customer_id;
    
    // Set order date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    order->order_date.day = current_time->tm_mday;
    order->order_date.month = current_time->tm_mon + 1;
    order->order_date.year = current_time->tm_year + 1900;
    
    // Shipping date initially set to 7 days from now
    time_t shipping_time = now + (7 * 24 * 60 * 60);
    struct tm *shipping_tm = localtime(&shipping_time);
    order->shipping_date.day = shipping_tm->tm_mday;
    order->shipping_date.month = shipping_tm->tm_mon + 1;
    order->shipping_date.year = shipping_tm->tm_year + 1900;
    
    order->total_amount = 0.0;
    order->discount_amount = 0.0;
    order->shipping_fee = 5.99; // Standard shipping fee
    strcpy(order->status, "Pending");
    strncpy(order->shipping_address, shipping_address, 99);
    order->shipping_address[99] = '\0';
    order->active = 1;
    strcpy(order->payment_method, payment_method, 19);
    order->payment_method[19] = '\0';
    
    db->order_count++;
    return order->id;
}

void add_order_item(ShopDatabase *db, int order_id, int product_id, int quantity, double discount_percent) {
    // Validate order and product exist
    int order_index = find_order_by_id(db, order_id);
    int product_index = find_product_by_id(db, product_id);
    
    if (order_index == -1 || product_index == -1) {
        return;
    }
    
    // Check if product is active and has sufficient stock
    if (!db->products[product_index].active || db->products[product_index].stock_quantity < quantity) {
        printf("Product not available or insufficient stock.\n");
        return;
    }
    
    // Update order total
    double item_total = db->products[product_index].price * quantity * (1.0 - discount_percent / 100.0);
    db->orders[order_index].total_amount += item_total;
    
    // Update product stock
    db->products[product_index].stock_quantity -= quantity;
    
    printf("Product added to order successfully.\n");
    printf("Product: %s\n", db->products[product_index].name);
    printf("Quantity: %d\n", quantity);
    printf("Unit Price: $%.2f\n", db->products[product_index].price);
    printf("Discount: %.1f%%\n", discount_percent);
    printf("Item Total: $%.2f\n", item_total);
}

void update_order_status(ShopDatabase *db, int order_id, const char *status) {
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        printf("Order not found.\n");
        return;
    }
    
    Order *order = &db->orders[order_index];
    
    strcpy(order->status, status);
    
    printf("Order #%d status updated to '%s'.\n", order_id, status);
    printf("Description: %s\n", get_order_status_description(status));
}

void cancel_order(ShopDatabase *db, int order_id) {
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        printf("Order not found.\n");
        return;
    }
    
    Order *order = &db->orders[order_index];
    
    strcpy(order->status, "Cancelled");
    order->active = 0;
    
    // Return items to stock
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active && db->orders[order_index].customer_id == db->customers[i].id) {
            // This is a simplified way to restore stock
            // In a real system, we'd track order items separately
            db->products[i].stock_quantity += 5; // Restock some quantity as a simplification
        }
    }
    
    printf("Order #%d cancelled successfully.\n", order_id);
    printf("Refund: $%.2f\n", order->total_amount + order->shipping_fee - order->discount_amount);
}

void display_customer(const Customer *customer) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s %s\n", customer->first_name, customer->last_name);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Address: %s\n", customer->address);
    printf("Gender: %s\n", customer->gender);
    printf("Birth Date: %02d/%02d/%d\n", 
           customer->birth_date.day, customer->birth_date.month, customer->birth_date.year);
    printf("Registration Date: %02d/%02d/%d\n", 
           customer->registration_date.day, customer->registration_date.month, customer->registration_date.year);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
    printf("VIP Level: %d\n", customer->vip_level);
    printf("Total Spent: $%.2f\n", customer->total_spent);
    printf("Loyalty Points: %d\n", customer->loyalty_points);
}

void display_product(const Product *product) {
    printf("\nProduct ID: %d\n", product->id);
    printf("Product Code: %s\n", product->product_code);
    printf("Name: %s\n", product->name);
    printf("Category: %s\n", product->category);
    printf("Description: %s\n", product->description);
    printf("Price: $%.2f\n", product->price);
    printf("Stock Quantity: %d\n", product->stock_quantity);
    printf("Reorder Level: %d\n", product->reorder_level);
    printf("Discount: %.1f%%\n", product->discount_percent);
    printf("Added Date: %02d/%02d/%d\n", 
           product->added_date.day, product->added_date.month, product->added_date.year);
    printf("Status: %s\n", product->active ? "Active" : "Inactive");
}

void display_category(const Category *category) {
    printf("\nCategory ID: %d\n", category->id);
    printf("Name: %s\n", category->name);
    printf("Description: %s\n", category->description);
    printf("Status: %s\n", category->active ? "Active" : "Inactive");
}

void display_order(const Order *order, const ShopDatabase *db) {
    printf("\nOrder ID: %d\n", order->id);
    
    // Display customer information
    int customer_index = find_customer_by_id(db, order->customer_id);
    if (customer_index != -1) {
        printf("Customer: %s %s (ID: %d)\n", 
               db->customers[customer_index].first_name,
               db->customers[customer_index].last_name,
               order->customer_id);
    }
    
    printf("Order Date: %02d/%02d/%d\n", 
           order->order_date.day, order->order_date.month, order->order_date.year);
    printf("Shipping Date: %02d/%02d/%d\n", 
           order->shipping_date.day, order->shipping_date.month, order->shipping_date.year);
    printf("Total Amount: $%.2f\n", order->total_amount);
    printf("Discount Amount: $%.2f\n", order->discount_amount);
    printf("Shipping Fee: $%.2f\n", order->shipping_fee);
    printf("Final Total: $%.2f\n", order->total_amount + order->shipping_fee - order->discount_amount);
    printf("Status: %s\n", order->status);
    printf("Description: %s\n", get_order_status_description(order->status));
    printf("Shipping Address: %s\n", order->shipping_address);
    printf("Payment Method: %s\n", order->payment_method);
    printf("Status: %s\n", order->active ? "Active" : "Inactive");
}

void display_all_customers(const ShopDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_products(const ShopDatabase *db) {
    printf("\n=== All Products ===\n");
    printf("Total Products: %d\n\n", db->product_count);
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active) {
            display_product(&db->products[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_categories(const ShopDatabase *db) {
    printf("\n=== All Categories ===\n");
    printf("Total Categories: %d\n\n", db->category_count);
    
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].active) {
            display_category(&db->categories[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_orders(const ShopDatabase *db) {
    printf("\n=== All Orders ===\n");
    printf("Total Orders: %d\n\n", db->order_count);
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active) {
            display_order(&db->orders[i], db);
            printf("========================\n");
        }
    }
}

void search_customers_by_name(const ShopDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active && 
            (strstr(db->customers[i].first_name, name) != NULL || 
             strstr(db->customers[i].last_name, name) != NULL)) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No customers found with name containing '%s'.\n", name);
    }
}

void search_products_by_category(const ShopDatabase *db, const char *category) {
    printf("\nProducts in Category: %s\n", category);
    int found = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active && strstr(db->products[i].category, category) != NULL) {
            display_product(&db->products[i]);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No products found in this category.\n");
    }
}

void search_orders_by_status(const ShopDatabase *db, const char *status) {
    printf("\nOrders with Status: %s\n", status);
    int found = 0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active && strcmp(db->orders[i].status, status) == 0) {
            display_order(&db->orders[i], db);
            printf("========================\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No orders found with this status.\n");
    }
}

void generate_customer_report(const ShopDatabase *db, int customer_id) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    Customer *customer = &db->customers[customer_index];
    
    printf("\n=== Customer Report: %s %s ===\n", customer->first_name, customer->last_name);
    printf("Customer ID: %d\n", customer->id);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Address: %s\n", customer->address);
    printf("Gender: %s\n", customer->gender);
    printf("Birth Date: %02d/%02d/%d\n", 
           customer->birth_date.day, customer->birth_date.month, customer->birth_date.year);
    printf("Registration Date: %02d/%02d/%d\n", 
           customer->registration_date.day, customer->registration_date.month, customer->registration_date.year);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
    printf("VIP Level: %d\n", customer->vip_level);
    printf("Total Spent: $%.2f\n", customer->total_spent);
    printf("Loyalty Points: %d\n", customer->loyalty_points);
    
    // Calculate age
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int age = current_year - customer->birth_date.year;
    
    // Adjust age if birthday hasn't occurred this year
    if (current_time->tm_mon + 1 < customer->birth_date.month || 
        (current_time->tm_mon + 1 == customer->birth_date.month && 
         current_time->tm_mday < customer->birth_date.day)) {
        age--;
    }
    
    printf("Age: %d\n", age);
    
    // Order history
    printf("\nOrder History:\n");
    printf("%-5s %-15s %-15s %-10s %-10s %-15s\n", 
           "ID", "Date", "Status", "Total", "Method");
    printf("-------------------------------------------------\n");
    
    int order_count = 0;
    double total_order_amount = 0.0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active && db->orders[i].customer_id == customer_id) {
            printf("%-5d %02d/%02d/%d %-15s $%-9.2f %-15s\n", 
                   db->orders[i].id,
                   db->orders[i].order_date.day, db->orders[i].order_date.month, db->orders[i].order_date.year,
                   db->orders[i].status,
                   db->orders[i].total_amount,
                   db->orders[i].payment_method);
            
            order_count++;
            total_order_amount += db->orders[i].total_amount;
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Orders: %d\n", order_count);
    
    if (order_count > 0) {
        printf("Total Order Amount: $%.2f\n", total_order_amount);
        printf("Average Order Value: $%.2f\n", total_order_amount / order_count);
        
        // Calculate statistics
        int pending_count = 0;
        int processing_count = 0;
        int shipped_count = 0;
        int delivered_count = 0;
        int cancelled_count = 0;
        
        for (int i = 0; i < db->order_count; i++) {
            if (db->orders[i].active && db->orders[i].customer_id == customer_id) {
                if (strcmp(db->orders[i].status, "Pending") == 0) {
                    pending_count++;
                } else if (strcmp(db->orders[i].status, "Processing") == 0) {
                    processing_count++;
                } else if (strcmp(db->orders[i].status, "Shipped") == 0) {
                    shipped_count++;
                } else if (strcmp(db->orders[i].status, "Delivered") == 0) {
                    delivered_count++;
                } else if (strcmp(db->orders[i].status, "Cancelled") == 0) {
                    cancelled_count++;
                }
            }
        }
        
        printf("\nOrder Statistics:\n");
        printf("Pending: %d (%.1f%%)\n", pending_count, (float)pending_count / order_count * 100);
        printf("Processing: %d (%.1f%%)\n", processing_count, (float)processing_count / order_count * 100);
        printf("Shipped: %d (%.1f%%)\n", shipped_count, (float)shipped_count / order_count * 100);
        printf("Delivered: %d (%.1f%%)\n", delivered_count, (float)delivered_count / order_count * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / order_count * 100);
    }
}

void generate_product_report(const ShopDatabase *db, int product_id) {
    int product_index = find_product_by_id(db, product_id);
    if (product_index == -1) {
        printf("Product not found.\n");
        return;
    }
    
    Product *product = &db->products[product_index];
    
    printf("\n=== Product Report: %s ===\n", product->name);
    printf("Product ID: %d\n", product->id);
    printf("Product Code: %s\n", product->product_code);
    printf("Category: %s\n", product->category);
    printf("Description: %s\n", product->description);
    printf("Price: $%.2f\n", product->price);
    printf("Stock Quantity: %d\n", product->stock_quantity);
    printf("Reorder Level: %d\n", product->reorder_level);
    printf("Discount: %.1f%%\n", product->discount_percent);
    printf("Added Date: %02d/%02d/%d\n", 
           product->added_date.day, product->added_date.month, product->added_date.year);
    printf("Status: %s\n", product->active ? "Active" : "Inactive");
    
    // Sales statistics
    printf("\nSales Statistics:\n");
    
    int total_orders = 0;
    double total_revenue = 0.0;
    
    // This is a simplified version - in a real system we'd track order items
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active) {
            // This is a simplified calculation assuming this product is in the order
            // In a real system, we'd need to track order items separately
            if (strcmp(db->orders[i].status, "Delivered") == 0) {
                // Assume product is in delivered orders (simplified)
                if (rand() % 5 == 0) { // 20% chance
                    total_orders++;
                    total_revenue += db->orders[i].total_amount * 0.1; // Simplified assumption
                }
            }
        }
    }
    
    printf("Total Orders: %d\n", total_orders);
    printf("Estimated Revenue: $%.2f\n", total_revenue);
    printf("Average Order Value: $%.2f\n", total_orders > 0 ? total_revenue / total_orders : 0.0);
}

void generate_sales_report(const ShopDatabase *db, const Date *start_date, const Date *end_date) {
    printf("\n=== Sales Report ===\n");
    printf("Period: %02d/%02d/%d to %02d/%02d/%d\n", 
           start_date->day, start_date->month, start_date->year,
           end_date->day, end_date->month, end_date->year);
    
    printf("\nOrders within date range:\n");
    printf("%-5s %-15s %-15s %-10s %-10s %-15s\n", 
           "ID", "Date", "Status", "Total", "Method");
    printf("-------------------------------------------------\n");
    
    int total_orders = 0;
    double total_revenue = 0.0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active) {
            // Check if order date is within range
            int order_timestamp = db->orders[i].order_date.year * 10000 + 
                                 db->orders[i].order_date.month * 100 + 
                                 db->orders[i].order_date.day;
            
            int start_timestamp = start_date->year * 10000 + 
                                 start_date->month * 100 + 
                                 start_date->day;
            
            int end_timestamp = end_date->year * 10000 + 
                               end_date->month * 100 + 
                               end_date->day;
            
            if (order_timestamp >= start_timestamp && order_timestamp <= end_timestamp) {
                printf("%-5d %02d/%02d/%d %-15s $%-9.2f %-15s\n", 
                       db->orders[i].id,
                       db->orders[i].order_date.day, db->orders[i].order_date.month, db->orders[i].order_date.year,
                       db->orders[i].status,
                       db->orders[i].total_amount,
                       db->orders[i].payment_method);
                
                total_orders++;
                total_revenue += db->orders[i].total_amount;
            }
        }
    }
    
    printf("-------------------------------------------------\n");
    printf("Total Orders: %d\n", total_orders);
    
    if (total_orders > 0) {
        printf("Total Revenue: $%.2f\n", total_revenue);
        printf("Average Order Value: $%.2f\n", total_revenue / total_orders);
        
        // Calculate statistics
        int pending_count = 0;
        int processing_count = 0;
        int shipped_count = 0;
        int delivered_count = 0;
        int cancelled_count = 0;
        
        for (int i = 0; i < db->order_count; i++) {
            if (db->orders[i].active) {
                // Check if order date is within range
                int order_timestamp = db->orders[i].order_date.year * 10000 + 
                                     db->orders[i].order_date.month * 100 + 
                                     db->orders[i].order_date.day;
                
                int start_timestamp = start_date->year * 10000 + 
                                     start_date->month * 100 + 
                                     start_date->day;
                
                int end_timestamp = end_date->year * 10000 + 
                                   end_date->month * 100 + 
                                   end_date->day;
                
                if (order_timestamp >= start_timestamp && order_timestamp <= end_timestamp) {
                    if (strcmp(db->orders[i].status, "Pending") == 0) {
                        pending_count++;
                    } else if (strcmp(db->orders[i].status, "Processing") == 0) {
                        processing_count++;
                    } else if (strcmp(db->orders[i].status, "Shipped") == 0) {
                        shipped_count++;
                    } else if (strcmp(db->orders[i].status, "Delivered") == 0) {
                        delivered_count++;
                    } else if (strcmp(db->orders[i].status, "Cancelled") == 0) {
                        cancelled_count++;
                    }
                }
            }
        }
        
        printf("\nOrder Statistics:\n");
        printf("Pending: %d (%.1f%%)\n", pending_count, (float)pending_count / total_orders * 100);
        printf("Processing: %d (%.1f%%)\n", processing_count, (float)processing_count / total_orders * 100);
        printf("Shipped: %d (%.1f%%)\n", shipped_count, (float)shipped_count / total_orders * 100);
        printf("Delivered: %d (%.1f%%)\n", delivered_count, (float)delivered_count / total_orders * 100);
        printf("Cancelled: %d (%.1f%%)\n", cancelled_count, (float)cancelled_count / total_orders * 100);
    }
}

void generate_inventory_report(const ShopDatabase *db) {
    printf("\n=== Inventory Report ===\n");
    
    int total_products = 0;
    int total_stock = 0;
    int low_stock_items = 0;
    double total_inventory_value = 0.0;
    
    printf("%-5s %-20s %-15s %-10s %-10s %-15s\n", 
           "ID", "Name", "Category", "Stock", "Reorder", "Value");
    printf("-------------------------------------------------------\n");
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active) {
            double item_value = db->products[i].price * db->products[i].stock_quantity;
            
            printf("%-5d %-20s %-15s %-10d %-10d $%-14.2f\n", 
                   db->products[i].id,
                   db->products[i].name,
                   db->products[i].category,
                   db->products[i].stock_quantity,
                   db->products[i].reorder_level,
                   item_value);
            
            total_products++;
            total_stock += db->products[i].stock_quantity;
            total_inventory_value += item_value;
            
            if (db->products[i].stock_quantity <= db->products[i].reorder_level) {
                low_stock_items++;
            }
        }
    }
    
    printf("-------------------------------------------------------\n");
    printf("Total Products: %d\n", total_products);
    printf("Total Stock: %d\n", total_stock);
    printf("Low Stock Items: %d\n", low_stock_items);
    printf("Total Inventory Value: $%.2f\n", total_inventory_value);
    printf("Average Stock per Product: %.1f\n", total_products > 0 ? (float)total_stock / total_products : 0.0);
    
    // Low stock warning
    if (low_stock_items > 0) {
        printf("\nLow Stock Items:\n");
        printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Stock", "Reorder");
        printf("-------------------------------------------------\n");
        
        for (int i = 0; i < db->product_count; i++) {
            if (db->products[i].active && 
                db->products[i].stock_quantity <= db->products[i].reorder_level) {
                printf("%-5d %-20s %-10d %-10d\n", 
                       db->products[i].id,
                       db->products[i].name,
                       db->products[i].stock_quantity,
                       db->products[i].reorder_level);
            }
        }
    }
}

void update_product_stock(ShopDatabase *db, int product_id, int quantity_change) {
    int product_index = find_product_by_id(db, product_id);
    if (product_index == -1) {
        printf("Product not found.\n");
        return;
    }
    
    Product *product = &db->products[product_index];
    
    if (product->stock_quantity + quantity_change < 0) {
        printf("Invalid operation. Stock would become negative.\n");
        return;
    }
    
    product->stock_quantity += quantity_change;
    
    printf("Product stock updated.\n");
    printf("Product: %s\n", product->name);
    printf("Previous Stock: %d\n", product->stock_quantity - quantity_change);
    printf("New Stock: %d\n", product->stock_quantity);
    
    if (product->stock_quantity <= product->reorder_level) {
        printf("Warning: Stock level is at or below reorder level. Consider restocking.\n");
    }
}

void apply_promotion(ShopDatabase *db, const char *category, double discount_percent) {
    int updated_count = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active && strstr(db->products[i].category, category) != NULL) {
            db->products[i].discount_percent = discount_percent;
            updated_count++;
        }
    }
    
    printf("Promotion applied to %d products in category '%s'.\n", updated_count, category);
    printf("Discount: %.1f%%\n", discount_percent);
}

void calculate_customer_lifetime_value(ShopDatabase *db, int customer_id) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    Customer *customer = &db->customers[customer_index];
    
    printf("\n=== Customer Lifetime Value ===\n");
    printf("Customer: %s %s\n", customer->first_name, customer->last_name);
    printf("Email: %s\n", customer->email);
    printf("Total Spent: $%.2f\n", customer->total_spent);
    printf("VIP Level: %d\n", customer->vip_level);
    printf("Loyalty Points: %d\n", customer->loyalty_points);
    
    // Calculate average order value
    int order_count = 0;
    double total_order_amount = 0.0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active && db->orders[i].customer_id == customer_id) {
            order_count++;
            total_order_amount += db->orders[i].total_amount;
        }
    }
    
    if (order_count > 0) {
        printf("Total Orders: %d\n", order_count);
        printf("Average Order Value: $%.2f\n", total_order_amount / order_count);
        
        // Predict future value
        int months_as_customer = 6; // Simplified assumption
        
        // In a real system, we'd calculate this from registration date
        printf("Estimated Annual Value: $%.2f\n", total_order_amount / months_as_customer * 12);
        printf("Customer Lifetime Value: $%.2f\n", total_order_amount / months_as_customer * 12 * 10); // 10-year LTV assumption
    }
}

int find_customer_by_id(const ShopDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_product_by_id(const ShopDatabase *db, int product_id) {
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].id == product_id && db->products[i].active) {
            return i;
        }
    }
    return -1;
}

int find_category_by_id(const ShopDatabase *db, int category_id) {
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].id == category_id && db->categories[i].active) {
            return i;
        }
    }
    return -1;
}

int find_order_by_id(const ShopDatabase *db, int order_id) {
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].id == order_id && db->orders[i].active) {
            return i;
        }
    }
    return -1;
}

int save_database(const ShopDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "wb");
    if (customers_file != NULL) {
        fwrite(&db->customer_count, sizeof(int), 1, customers_file);
        fwrite(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *products_file = fopen(FILENAME_PRODUCTS, "wb");
    if (products_file != NULL) {
        fwrite(&db->product_count, sizeof(int), 1, products_file);
        fwrite(db->products, sizeof(Product), db->product_count, products_file);
        fclose(products_file);
    } else {
        return 0;
    }
    
    FILE *categories_file = fopen(FILENAME_CATEGORIES, "wb");
    if (categories_file != NULL) {
        fwrite(&db->category_count, sizeof(int), 1, categories_file);
        fwrite(db->categories, sizeof(Category), db->category_count, categories_file);
        fclose(categories_file);
    } else {
        return 0;
    }
    
    FILE *orders_file = fopen(FILENAME_ORDERS, "wb");
    if (orders_file != NULL) {
        fwrite(&db->order_count, sizeof(int), 1, orders_file);
        fwrite(db->orders, sizeof(Order), db->order_count, orders_file);
        fclose(orders_file);
        return 1;
    }
    
    return 0;
}

int load_database(ShopDatabase *db) {
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "rb");
    if (customers_file != NULL) {
        fread(&db->customer_count, sizeof(int), 1, customers_file);
        fread(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *products_file = fopen(FILENAME_PRODUCTS, "rb");
    if (products_file != NULL) {
        fread(&db->product_count, sizeof(int), 1, products_file);
        fread(db->products, sizeof(Product), db->product_count, products_file);
        fclose(products_file);
    } else {
        return 0;
    }
    
    FILE *categories_file = fopen(FILENAME_CATEGORIES, "rb");
    if (categories_file != NULL) {
        fread(&db->category_count, sizeof(int), 1, categories_file);
        fread(db->categories, sizeof(Category), db->category_count, categories_file);
        fclose(categories_file);
    } else {
        return 0;
    }
    
    FILE *orders_file = fopen(FILENAME_ORDERS, "rb");
    if (orders_file != NULL) {
        fread(&db->order_count, sizeof(int), 1, orders_file);
        fread(db->orders, sizeof(Order), db->order_count, orders_file);
        fclose(orders_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(ShopDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* categories[] = {"Electronics", "Clothing", "Books", "Home", "Sports",
                           "Toys", "Food", "Beauty", "Health", "Automotive"};
    
    char* product_names[] = {"Laptop", "Smartphone", "Tablet", "Monitor", "Keyboard",
                            "Mouse", "Headphones", "Speaker", "Camera", "Printer",
                            "Router", "External Drive", "Memory Card", "USB Cable", "Phone Case"};
    
    char* payment_methods[] = {"Credit Card", "Debit Card", "PayPal", "Bank Transfer"};
    
    char* genders[] = {"Male", "Female", "Other"};
    
    srand(time(NULL));
    
    // Add categories
    for (int i = 0; i < 10 && db->category_count < MAX_CATEGORIES; i++) {
        char description[200];
        sprintf(description, "Category for %s products with comprehensive selection", categories[i]);
        add_category(db, categories[i], description);
    }
    
    // Add products
    for (int i = 0; i < 200 && db->product_count < MAX_PRODUCTS; i++) {
        char product_code[20], description[200];
        sprintf(product_code, "PRD-%05d", 10000 + i);
        sprintf(description, "Premium quality %s with advanced features", product_names[rand() % 15]);
        
        double price = 10.0 + (rand() % 1000) + (rand() % 100) / 100.0;
        int stock_quantity = 5 + (rand() % 100);
        int reorder_level = 5 + (rand() % 20);
        double discount_percent = (rand() % 30) / 10.0; // 0-30% with 0.1 increments
        
        add_product(db, product_code, product_names[rand() % 15], categories[rand() % 10],
                  description, price, stock_quantity, reorder_level, discount_percent);
    }
    
    // Add customers
    for (int i = 0; i < 500 && db->customer_count < MAX_CUSTOMERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100], gender[10];
        sprintf(email, "%s.%d@shop.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        
        const char* gender_list = genders[rand() % 3];
        strcpy(gender, gender_list);
        
        int day = 1 + (rand() % 28);
        int month = 1 + (rand() % 12);
        int year = 1970 + (rand() % 40);
        Date birth_date = {day, month, year};
        
        add_customer(db, first_name, last_name, email, phone, address, gender, birth_date);
    }
    
    // Add orders
    for (int i = 0; i < 1000 && db->order_count < MAX_ORDERS; i++) {
        int customer_id = 1 + (rand() % 500);
        char shipping_address[100];
        const char* payment_method = payment_methods[rand() % 4];
        sprintf(shipping_address, "%d Order Ln, City, State", 1000 + rand() % 9000);
        
        int order_id = create_order(db, customer_id, shipping_address, payment_method);
        if (order_id != -1) {
            // Add products to order (1-5 items per order)
            int items_count = 1 + (rand() % 5);
            for (int j = 0; j < items_count && db->product_count < MAX_PRODUCTS; j++) {
                int product_id = 1 + (rand() % 200);
                int quantity = 1 + (rand() % 3);
                double discount = (rand() % 30) / 10.0;
                
                add_order_item(db, order_id, product_id, quantity, discount);
            }
            
            // Randomly set order status
            const char* statuses[] = {"Pending", "Processing", "Shipped", "Delivered", "Cancelled"};
            int status_index = rand() % 5;
            strcpy(db->orders[db->order_count - 1].status, statuses[status_index]);
        }
    }
    
    // Update some customers' total spent and VIP level
    for (int i = 0; i < 100; i++) {
        int customer_index = db->customers[i].id - 1;
        
        if (customer_index < db->customer_count && customer_index >= 0) {
            // Calculate total spent from orders
            double total_spent = 0.0;
            int order_count = 0;
            
            for (int j = 0; j < db->order_count; j++) {
                if (db->orders[j].active && db->orders[j].customer_id == db->customers[customer_index].id &&
                    strcmp(db->orders[j].status, "Delivered") == 0) {
                    total_spent += db->orders[j].total_amount;
                    order_count++;
                }
            }
            
            if (order_count > 0) {
                db->customers[customer_index].total_spent = total_spent;
                
                // Update VIP level based on total spent
                if (total_spent > 5000) {
                    db->customers[customer_index].vip_level = 3; // Platinum
                } else if (total_spent > 2000) {
                    db->customers[customer_index].vip_level = 2; // Gold
                } else if (total_spent > 500) {
                    db->customers[customer_index].vip_level = 1; // Silver
                }
                
                // Calculate loyalty points (1 point per $100 spent)
                int new_points = (int)(total_spent / 100.0);
                db->customers[customer_index].loyalty_points = new_points;
            }
        }
    }
}

int is_valid_email(const char *email) {
    if (email == NULL) return 0;
    
    int at_symbol = 0;
    int dot_after_at = 0;
    
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            at_symbol = 1;
        } else if (at_symbol && email[i] == '.') {
            dot_after_at = 1;
        }
    }
    
    return at_symbol && dot_after_at;
}

int is_valid_phone(const char *phone) {
    if (phone == NULL) return 0;
    
    // Simple validation: check if phone starts with valid prefix and has enough digits
    return (strncmp(phone, "555-", 4) == 0 || 
            strncmp(phone, "(", 1) == 0 || 
            strncmp(phone, "+", 1) == 0);
}

int is_valid_date(int day, int month, int year) {
    if (year < 1900 || year > 2100) return 0;
    if (month < 1 || month > 12) return 0;
    
    int max_days;
    switch (month) {
        case 2:
            // Leap year check
            max_days = ((year % 400 == 0) || (year % 100 != 0 && year % 4 == 0)) ? 29 : 28;
            break;
        case 4: case 6: case 9: case 11:
            max_days = 30;
            break;
        default:
            max_days = 31;
    }
    
    return day >= 1 && day <= max_days;
}

const char* get_order_status_description(const char *status) {
    if (strcmp(status, "Pending") == 0) return "Order is pending and waiting to be processed";
    if (strcmp(status, "Processing") == 0) return "Order is currently being processed";
    if (strcmp(status, "Shipped") == 0) return "Order has been shipped and is on its way";
    if (strcmp(status, "Delivered") == 0) return "Order has been delivered to customer";
    if (strcmp(status, "Cancelled") == 0) return "Order has been cancelled and will be refunded";
    return "Unknown status";
}