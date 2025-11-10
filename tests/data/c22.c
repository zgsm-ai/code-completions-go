#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_CUSTOMERS 1000
#define MAX_ORDERS 5000
#define MAX_PRODUCTS 2000
#define MAX_CATEGORIES 50
#define MAX_CART_ITEMS 50
#define MAX_NAME_LENGTH 50
#define FILENAME_CUSTOMERS "customers.dat"
#define FILENAME_ORDERS "orders.dat"
#define FILENAME_PRODUCTS "products.dat"
#define FILENAME_CATEGORIES "categories.dat"

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
    char username[30];
    char password[30];
    int active;
    int vip_level;
    double total_spent;
    int loyalty_points;
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
} Order;

typedef struct {
    int order_id;
    int product_id;
    int quantity;
    double unit_price;
    double discount_percent;
} OrderItem;

typedef struct {
    int customer_id;
    int product_id;
    int quantity;
    Date added_date;
} CartItem;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Product products[MAX_PRODUCTS];
    int product_count;
    Category categories[MAX_CATEGORIES];
    int category_count;
    Order orders[MAX_ORDERS];
    int order_count;
    OrderItem order_items[MAX_ORDERS * 10];
    int order_item_count;
    CartItem cart_items[MAX_CART_ITEMS];
    int cart_item_count;
} ECommerceDatabase;

// Function prototypes
void initialize_database(ECommerceDatabase *db);
int add_customer(ECommerceDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *username, const char *password);
int add_product(ECommerceDatabase *db, const char *product_code, const char *name,
               const char *category, const char *description, double price,
               int stock_quantity, int reorder_level, double discount_percent);
int add_category(ECommerceDatabase *db, const char *name, const char *description);
int create_order(ECommerceDatabase *db, int customer_id, const char *shipping_address);
int add_order_item(ECommerceDatabase *db, int order_id, int product_id, int quantity, double discount_percent);
int add_to_cart(ECommerceDatabase *db, int customer_id, int product_id, int quantity);
void checkout_cart(ECommerceDatabase *db, int customer_id, const char *shipping_address);
void display_customer(const Customer *customer);
void display_product(const Product *product);
void display_category(const Category *category);
void display_order(const Order *order, const ECommerceDatabase *db);
void display_all_customers(const ECommerceDatabase *db);
void display_all_products(const ECommerceDatabase *db);
void display_all_categories(const ECommerceDatabase *db);
void display_all_orders(const ECommerceDatabase *db);
void search_customers_by_name(const ECommerceDatabase *db, const char *name);
void search_products_by_category(const ECommerceDatabase *db, const char *category);
void search_orders_by_status(const ECommerceDatabase *db, const char *status);
void generate_customer_report(const ECommerceDatabase *db, int customer_id);
void generate_product_report(const ECommerceDatabase *db, int product_id);
void generate_sales_report(const ECommerceDatabase *db, const Date *start_date, const Date *end_date);
void generate_inventory_report(const ECommerceDatabase *db);
void update_product_stock(ECommerceDatabase *db, int product_id, int quantity_change);
void update_order_status(ECommerceDatabase *db, int order_id, const char *status);
void apply_promotion(ECommerceDatabase *db, const char *category, double discount_percent);
int find_customer_by_id(const ECommerceDatabase *db, int customer_id);
int find_product_by_id(const ECommerceDatabase *db, int product_id);
int find_category_by_id(const ECommerceDatabase *db, int category_id);
int find_order_by_id(const ECommerceDatabase *db, int order_id);
int save_database(const ECommerceDatabase *db);
int load_database(ECommerceDatabase *db);
void generate_sample_data(ECommerceDatabase *db);
void process_pending_orders(ECommerceDatabase *db);
void calculate_revenue_statistics(const ECommerceDatabase *db);
int is_valid_email(const char *email);
int is_valid_username(const char *username);
int is_valid_password(const char *password);
const char* get_order_status_description(const char *status);

int main() {
    ECommerceDatabase db;
    initialize_database(&db);
    
    printf("E-Commerce Management System\n");
    printf("1. Add Customer\n");
    printf("2. Add Product\n");
    printf("3. Add Category\n");
    printf("4. Create Order\n");
    printf("5. Add Product to Cart\n");
    printf("6. Checkout Cart\n");
    printf("7. Display All Customers\n");
    printf("8. Display All Products\n");
    printf("9. Display All Categories\n");
    printf("10. Display All Orders\n");
    printf("11. Search Customers by Name\n");
    printf("12. Search Products by Category\n");
    printf("13. Search Orders by Status\n");
    printf("14. Generate Customer Report\n");
    printf("15. Generate Product Report\n");
    printf("16. Generate Sales Report\n");
    printf("17. Generate Inventory Report\n");
    printf("18. Update Product Stock\n");
    printf("19. Update Order Status\n");
    printf("20. Apply Promotion\n");
    printf("21. Process Pending Orders\n");
    printf("22. Calculate Revenue Statistics\n");
    printf("23. Save Database\n");
    printf("24. Load Database\n");
    printf("25. Generate Sample Data\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
                char email[50], phone[15], address[100], username[30], password[30];
                
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
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);
                
                if (!is_valid_email(email)) {
                    printf("Invalid email format.\n");
                    break;
                }
                
                if (!is_valid_username(username)) {
                    printf("Invalid username. Must be 3-30 characters and alphanumeric.\n");
                    break;
                }
                
                if (!is_valid_password(password)) {
                    printf("Invalid password. Must be 6-30 characters.\n");
                    break;
                }
                
                int customer_id = add_customer(&db, first_name, last_name, email, phone, 
                                              address, username, password);
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
                char shipping_address[100];
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter shipping address: ");
                scanf(" %[^\n]", shipping_address);
                
                int order_id = create_order(&db, customer_id, shipping_address);
                if (order_id != -1) {
                    printf("Order created with ID: %d\n", order_id);
                } else {
                    printf("Failed to create order. Check customer ID.\n");
                }
                break;
            }
            case 5: {
                int customer_id, product_id, quantity;
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                
                if (add_to_cart(&db, customer_id, product_id, quantity)) {
                    printf("Product added to cart successfully.\n");
                } else {
                    printf("Failed to add product to cart. Check IDs and stock availability.\n");
                }
                break;
            }
            case 6: {
                int customer_id;
                char shipping_address[100];
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter shipping address: ");
                scanf(" %[^\n]", shipping_address);
                
                checkout_cart(&db, customer_id, shipping_address);
                break;
            }
            case 7: {
                display_all_customers(&db);
                break;
            }
            case 8: {
                display_all_products(&db);
                break;
            }
            case 9: {
                display_all_categories(&db);
                break;
            }
            case 10: {
                display_all_orders(&db);
                break;
            }
            case 11: {
                char name[MAX_NAME_LENGTH];
                printf("Enter customer name to search: ");
                scanf(" %[^\n]", name);
                search_customers_by_name(&db, name);
                break;
            }
            case 12: {
                char category[50];
                printf("Enter product category to search: ");
                scanf(" %[^\n]", category);
                search_products_by_category(&db, category);
                break;
            }
            case 13: {
                char status[20];
                printf("Enter order status to search (Pending, Processing, Shipped, Delivered, Cancelled): ");
                scanf(" %[^\n]", status);
                search_orders_by_status(&db, status);
                break;
            }
            case 14: {
                int customer_id;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                generate_customer_report(&db, customer_id);
                break;
            }
            case 15: {
                int product_id;
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                generate_product_report(&db, product_id);
                break;
            }
            case 16: {
                int start_day, start_month, start_year;
                int end_day, end_month, end_year;
                
                printf("Enter start date (DD MM YYYY): ");
                scanf("%d %d %d", &start_day, &start_month, &start_year);
                printf("Enter end date (DD MM YYYY): ");
                scanf("%d %d %d", &end_day, &end_month, &end_year);
                
                Date start_date = {start_day, start_month, start_year};
                Date end_date = {end_day, end_month, end_year};
                
                generate_sales_report(&db, &start_date, &end_date);
                break;
            }
            case 17: {
                generate_inventory_report(&db);
                break;
            }
            case 18: {
                int product_id, quantity_change;
                
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity change (positive for restock, negative for sale): ");
                scanf("%d", &quantity_change);
                
                update_product_stock(&db, product_id, quantity_change);
                break;
            }
            case 19: {
                int order_id;
                char status[20];
                
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter new status (Pending, Processing, Shipped, Delivered, Cancelled): ");
                scanf(" %[^\n]", status);
                
                update_order_status(&db, order_id, status);
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
                process_pending_orders(&db);
                break;
            }
            case 22: {
                calculate_revenue_statistics(&db);
                break;
            }
            case 23: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 24: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 25: {
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

void initialize_database(ECommerceDatabase *db) {
    <｜fim▁hole｜>db->customer_count = 0;
    db->product_count = 0;
    db->category_count = 0;
    db->order_count = 0;
    db->order_item_count = 0;
    db->cart_item_count = 0;
}

int add_customer(ECommerceDatabase *db, const char *first_name, const char *last_name,
                 const char *email, const char *phone, const char *address,
                 const char *username, const char *password) {
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
    strncpy(customer->username, username, 29);
    customer->username[29] = '\0';
    strncpy(customer->password, password, 29);
    customer->password[29] = '\0';
    
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

int add_product(ECommerceDatabase *db, const char *product_code, const char *name,
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

int add_category(ECommerceDatabase *db, const char *name, const char *description) {
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

int create_order(ECommerceDatabase *db, int customer_id, const char *shipping_address) {
    if (db->order_count >= MAX_ORDERS) {
        return -1;
    }
    
    // Check if customer exists
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
    
    db->order_count++;
    return order->id;
}

int add_order_item(ECommerceDatabase *db, int order_id, int product_id, int quantity, double discount_percent) {
    if (db->order_item_count >= MAX_ORDERS * 10) {
        return 0;
    }
    
    // Validate order and product exist
    int order_index = find_order_by_id(db, order_id);
    int product_index = find_product_by_id(db, product_id);
    
    if (order_index == -1 || product_index == -1) {
        return 0;
    }
    
    // Check if product is active and has sufficient stock
    if (!db->products[product_index].active || db->products[product_index].stock_quantity < quantity) {
        return 0;
    }
    
    // Create order item
    OrderItem *order_item = &db->order_items[db->order_item_count];
    order_item->order_id = order_id;
    order_item->product_id = product_id;
    order_item->quantity = quantity;
    order_item->unit_price = db->products[product_index].price;
    order_item->discount_percent = discount_percent;
    
    // Update order total
    double item_total = order_item->unit_price * quantity * (1.0 - discount_percent / 100.0);
    db->orders[order_index].total_amount += item_total;
    
    // Update product stock
    db->products[product_index].stock_quantity -= quantity;
    
    db->order_item_count++;
    return 1;
}

int add_to_cart(ECommerceDatabase *db, int customer_id, int product_id, int quantity) {
    if (db->cart_item_count >= MAX_CART_ITEMS) {
        return 0;
    }
    
    // Validate customer and product exist
    if (find_customer_by_id(db, customer_id) == -1 || find_product_by_id(db, product_id) == -1) {
        return 0;
    }
    
    // Check if product is active and has sufficient stock
    int product_index = find_product_by_id(db, product_id);
    if (!db->products[product_index].active || db->products[product_index].stock_quantity < quantity) {
        return 0;
    }
    
    // Check if item already in cart
    for (int i = 0; i < db->cart_item_count; i++) {
        if (db->cart_items[i].customer_id == customer_id && db->cart_items[i].product_id == product_id) {
            // Update existing cart item
            if (db->products[product_index].stock_quantity >= db->cart_items[i].quantity + quantity) {
                db->cart_items[i].quantity += quantity;
                return 1;
            } else {
                return 0;
            }
        }
    }
    
    // Create new cart item
    CartItem *cart_item = &db->cart_items[db->cart_item_count];
    cart_item->customer_id = customer_id;
    cart_item->product_id = product_id;
    cart_item->quantity = quantity;
    
    // Set added date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    cart_item->added_date.day = current_time->tm_mday;
    cart_item->added_date.month = current_time->tm_mon + 1;
    cart_item->added_date.year = current_time->tm_year + 1900;
    
    db->cart_item_count++;
    return 1;
}

void checkout_cart(ECommerceDatabase *db, int customer_id, const char *shipping_address) {
    // Create order
    int order_id = create_order(db, customer_id, shipping_address);
    if (order_id == -1) {
        printf("Failed to create order during checkout.\n");
        return;
    }
    
    int order_index = find_order_by_id(db, order_id);
    
    // Move cart items to order items
    int items_added = 0;
    for (int i = 0; i < db->cart_item_count; i++) {
        if (db->cart_items[i].customer_id == customer_id) {
            int product_index = find_product_by_id(db, db->cart_items[i].product_id);
            if (product_index != -1 && db->products[product_index].active && 
                db->products[product_index].stock_quantity >= db->cart_items[i].quantity) {
                
                if (add_order_item(db, order_id, db->cart_items[i].product_id, 
                                  db->cart_items[i].quantity, 
                                  db->products[product_index].discount_percent)) {
                    items_added++;
                }
            }
        }
    }
    
    if (items_added > 0) {
        // Remove cart items for this customer
        for (int i = 0; i < db->cart_item_count; i++) {
            if (db->cart_items[i].customer_id == customer_id) {
                // Shift remaining items
                for (int j = i; j < db->cart_item_count - 1; j++) {
                    db->cart_items[j] = db->cart_items[j + 1];
                }
                db->cart_item_count--;
                i--; // Adjust index since we shifted
            }
        }
        
        // Update customer total spent
        int customer_index = find_customer_by_id(db, customer_id);
        if (customer_index != -1) {
            db->customers[customer_index].total_spent += db->orders[order_index].total_amount;
            
            // Calculate loyalty points (1 point per $10 spent)
            int new_points = (int)(db->orders[order_index].total_amount / 10.0);
            db->customers[customer_index].loyalty_points += new_points;
            
            // Update VIP level based on total spent
            double total_spent = db->customers[customer_index].total_spent;
            if (total_spent > 5000) {
                db->customers[customer_index].vip_level = 3; // Platinum
            } else if (total_spent > 2000) {
                db->customers[customer_index].vip_level = 2; // Gold
            } else if (total_spent > 500) {
                db->customers[customer_index].vip_level = 1; // Silver
            }
        }
        
        printf("Order #%d created successfully with %d items.\n", order_id, items_added);
        printf("Order Total: $%.2f\n", db->orders[order_index].total_amount + db->orders[order_index].shipping_fee);
        printf("Shipping Date: %02d/%02d/%d\n", 
               db->orders[order_index].shipping_date.day,
               db->orders[order_index].shipping_date.month,
               db->orders[order_index].shipping_date.year);
    } else {
        printf("No valid items in cart or insufficient stock.\n");
        // Remove the empty order
        db->order_count--;
    }
}

void display_customer(const Customer *customer) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s %s\n", customer->first_name, customer->last_name);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Address: %s\n", customer->address);
    printf("Username: %s\n", customer->username);
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

void display_order(const Order *order, const ECommerceDatabase *db) {
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
    printf("Shipping Address: %s\n", order->shipping_address);
    printf("Status: %s\n", order->active ? "Active" : "Inactive");
    
    // Display order items
    printf("\nOrder Items:\n");
    printf("%-5s %-20s %-10s %-10s %-15s\n", "ID", "Product", "Quantity", "Price", "Total");
    printf("--------------------------------------------------\n");
    
    for (int i = 0; i < db->order_item_count; i++) {
        if (db->order_items[i].order_id == order->id) {
            int product_index = find_product_by_id(db, db->order_items[i].product_id);
            if (product_index != -1) {
                double total = db->order_items[i].unit_price * db->order_items[i].quantity * 
                              (1.0 - db->order_items[i].discount_percent / 100.0);
                printf("%-5d %-20s %-10d $%-9.2f $%-14.2f\n", 
                       db->order_items[i].product_id,
                       db->products[product_index].name,
                       db->order_items[i].quantity,
                       db->order_items[i].unit_price,
                       total);
            }
        }
    }
}

void display_all_customers(const ECommerceDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_products(const ECommerceDatabase *db) {
    printf("\n=== All Products ===\n");
    printf("Total Products: %d\n\n", db->product_count);
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active) {
            display_product(&db->products[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_categories(const ECommerceDatabase *db) {
    printf("\n=== All Categories ===\n");
    printf("Total Categories: %d\n\n", db->category_count);
    
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].active) {
            display_category(&db->categories[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_orders(const ECommerceDatabase *db) {
    printf("\n=== All Orders ===\n");
    printf("Total Orders: %d\n\n", db->order_count);
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active) {
            display_order(&db->orders[i], db);
            printf("========================\n");
        }
    }
}

void search_customers_by_name(const ECommerceDatabase *db, const char *name) {
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

void search_products_by_category(const ECommerceDatabase *db, const char *category) {
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

void search_orders_by_status(const ECommerceDatabase *db, const char *status) {
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

void generate_customer_report(const ECommerceDatabase *db, int customer_id) {
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
    printf("Username: %s\n", customer->username);
    printf("Registration Date: %02d/%02d/%d\n", 
           customer->registration_date.day, customer->registration_date.month, customer->registration_date.year);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
    printf("VIP Level: %d\n", customer->vip_level);
    printf("Total Spent: $%.2f\n", customer->total_spent);
    printf("Loyalty Points: %d\n", customer->loyalty_points);
    
    printf("\nOrder History:\n");
    printf("%-5s %-10s %-10s %-15s\n", "ID", "Date", "Status", "Total");
    printf("-------------------------------------\n");
    
    int order_count = 0;
    double total_order_amount = 0.0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].customer_id == customer_id && db->orders[i].active) {
            printf("%-5d %02d/%02d/%d %-10s $%-14.2f\n", 
                   db->orders[i].id,
                   db->orders[i].order_date.day, db->orders[i].order_date.month, db->orders[i].order_date.year,
                   db->orders[i].status,
                   db->orders[i].total_amount + db->orders[i].shipping_fee - db->orders[i].discount_amount);
            
            order_count++;
            total_order_amount += db->orders[i].total_amount;
        }
    }
    
    printf("-------------------------------------\n");
    printf("Total Orders: %d\n", order_count);
    printf("Total Order Amount: $%.2f\n", total_order_amount);
    printf("Average Order Value: $%.2f\n", order_count > 0 ? total_order_amount / order_count : 0.0);
}

void generate_product_report(const ECommerceDatabase *db, int product_id) {
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
    
    printf("\nSales Statistics:\n");
    
    int total_orders = 0;
    int total_quantity = 0;
    double total_revenue = 0.0;
    
    for (int i = 0; i < db->order_item_count; i++) {
        if (db->order_items[i].product_id == product_id) {
            int order_index = find_order_by_id(db, db->order_items[i].order_id);
            if (order_index != -1 && db->orders[order_index].active) {
                total_orders++;
                total_quantity += db->order_items[i].quantity;
                total_revenue += db->order_items[i].unit_price * db->order_items[i].quantity * 
                                 (1.0 - db->order_items[i].discount_percent / 100.0);
            }
        }
    }
    
    printf("Total Orders: %d\n", total_orders);
    printf("Total Quantity Sold: %d\n", total_quantity);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Average Quantity per Order: %.1f\n", total_orders > 0 ? (float)total_quantity / total_orders : 0.0);
    printf("Average Revenue per Order: $%.2f\n", total_orders > 0 ? total_revenue / total_orders : 0.0);
    
    // Check if need to reorder
    if (product->stock_quantity <= product->reorder_level) {
        printf("\nWarning: Stock level is at or below reorder level. Consider restocking.\n");
    }
}

void generate_sales_report(const ECommerceDatabase *db, const Date *start_date, const Date *end_date) {
    printf("\n=== Sales Report ===\n");
    printf("Period: %02d/%02d/%d to %02d/%02d/%d\n", 
           start_date->day, start_date->month, start_date->year,
           end_date->day, end_date->month, end_date->year);
    
    int total_orders = 0;
    double total_revenue = 0.0;
    double total_shipping_fees = 0.0;
    double total_discounts = 0.0;
    
    printf("\nOrders within date range:\n");
    printf("%-5s %-10s %-15s %-10s\n", "ID", "Date", "Total", "Status");
    printf("--------------------------------------------------\n");
    
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
                double final_total = db->orders[i].total_amount + db->orders[i].shipping_fee - db->orders[i].discount_amount;
                
                printf("%-5d %02d/%02d/%d $%-14.2f %-10s\n", 
                       db->orders[i].id,
                       db->orders[i].order_date.day, db->orders[i].order_date.month, db->orders[i].order_date.year,
                       final_total,
                       db->orders[i].status);
                
                total_orders++;
                total_revenue += db->orders[i].total_amount;
                total_shipping_fees += db->orders[i].shipping_fee;
                total_discounts += db->orders[i].discount_amount;
            }
        }
    }
    
    printf("--------------------------------------------------\n");
    printf("Total Orders: %d\n", total_orders);
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Total Shipping Fees: $%.2f\n", total_shipping_fees);
    printf("Total Discounts: $%.2f\n", total_discounts);
    printf("Net Revenue: $%.2f\n", total_revenue + total_shipping_fees - total_discounts);
    printf("Average Order Value: $%.2f\n", total_orders > 0 ? (total_revenue + total_shipping_fees - total_discounts) / total_orders : 0.0);
    
    // Top products by revenue
    printf("\nTop Products by Revenue:\n");
    printf("%-5s %-20s %-10s %-15s\n", "ID", "Product", "Quantity", "Revenue");
    printf("--------------------------------------------------\n");
    
    int product_sales_count = 0;
    typedef struct {
        int product_id;
        int quantity;
        double revenue;
    } ProductSales;
    
    ProductSales product_sales[MAX_PRODUCTS];
    
    // Initialize product sales
    for (int i = 0; i < db->product_count; i++) {
        product_sales[i].product_id = db->products[i].id;
        product_sales[i].quantity = 0;
        product_sales[i].revenue = 0.0;
    }
    
    // Calculate sales for each product
    for (int i = 0; i < db->order_item_count; i++) {
        int order_index = find_order_by_id(db, db->order_items[i].order_id);
        if (order_index != -1 && db->orders[order_index].active) {
            // Check if order date is within range
            int order_timestamp = db->orders[order_index].order_date.year * 10000 + 
                                 db->orders[order_index].order_date.month * 100 + 
                                 db->orders[order_index].order_date.day;
            
            int start_timestamp = start_date->year * 10000 + 
                                 start_date->month * 100 + 
                                 start_date->day;
            
            int end_timestamp = end_date->year * 10000 + 
                               end_date->month * 100 + 
                               end_date->day;
            
            if (order_timestamp >= start_timestamp && order_timestamp <= end_timestamp) {
                int product_index = find_product_by_id(db, db->order_items[i].product_id);
                if (product_index != -1) {
                    product_sales[product_index].quantity += db->order_items[i].quantity;
                    product_sales[product_index].revenue += db->order_items[i].unit_price * db->order_items[i].quantity * 
                                                             (1.0 - db->order_items[i].discount_percent / 100.0);
                }
            }
        }
    }
    
    // Sort by revenue (descending)
    for (int i = 0; i < db->product_count - 1; i++) {
        for (int j = 0; j < db->product_count - i - 1; j++) {
            if (product_sales[j].revenue < product_sales[j + 1].revenue) {
                ProductSales temp = product_sales[j];
                product_sales[j] = product_sales[j + 1];
                product_sales[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 products
    for (int i = 0; i < 10 && i < db->product_count; i++) {
        if (product_sales[i].revenue > 0) {
            int product_index = find_product_by_id(db, product_sales[i].product_id);
            if (product_index != -1) {
                printf("%-5d %-20s %-10d $%-14.2f\n", 
                       product_sales[i].product_id,
                       db->products[product_index].name,
                       product_sales[i].quantity,
                       product_sales[i].revenue);
                product_sales_count++;
            }
        }
    }
    
    if (product_sales_count == 0) {
        printf("No product sales in the specified period.\n");
    }
}

void generate_inventory_report(const ECommerceDatabase *db) {
    printf("\n=== Inventory Report ===\n");
    
    int total_products = 0;
    int total_stock = 0;
    int low_stock_items = 0;
    double total_inventory_value = 0.0;
    
    printf("%-5s %-20s %-10s %-10s %-15s\n", "ID", "Product", "Stock", "Reorder", "Value");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].active) {
            double item_value = db->products[i].price * db->products[i].stock_quantity;
            
            printf("%-5d %-20s %-10d %-10d $%-14.2f\n", 
                   db->products[i].id,
                   db->products[i].name,
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
    
    printf("----------------------------------------------------------\n");
    printf("Total Products: %d\n", total_products);
    printf("Total Stock: %d\n", total_stock);
    printf("Low Stock Items: %d\n", low_stock_items);
    printf("Total Inventory Value: $%.2f\n", total_inventory_value);
    printf("Average Stock per Product: %.1f\n", total_products > 0 ? (float)total_stock / total_products : 0.0);
    
    // Low stock warning
    if (low_stock_items > 0) {
        printf("\nLow Stock Items:\n");
        printf("%-5s %-20s %-10s %-10s\n", "ID", "Product", "Stock", "Reorder");
        printf("--------------------------------------------------\n");
        
        for (int i = 0; i < db->product_count; i++) {
            if (db->products[i].active && db->products[i].stock_quantity <= db->products[i].reorder_level) {
                printf("%-5d %-20s %-10d %-10d\n", 
                       db->products[i].id,
                       db->products[i].name,
                       db->products[i].stock_quantity,
                       db->products[i].reorder_level);
            }
        }
    }
}

void update_product_stock(ECommerceDatabase *db, int product_id, int quantity_change) {
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

void update_order_status(ECommerceDatabase *db, int order_id, const char *status) {
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        printf("Order not found.\n");
        return;
    }
    
    Order *order = &db->orders[order_index];
    
    // Validate status
    if (strcmp(status, "Pending") != 0 && strcmp(status, "Processing") != 0 &&
        strcmp(status, "Shipped") != 0 && strcmp(status, "Delivered") != 0 &&
        strcmp(status, "Cancelled") != 0) {
        printf("Invalid status. Must be one of: Pending, Processing, Shipped, Delivered, Cancelled.\n");
        return;
    }
    
    strcpy(order->status, status);
    
    printf("Order status updated.\n");
    printf("Order ID: %d\n", order_id);
    printf("New Status: %s\n", status);
    
    if (strcmp(status, "Delivered") == 0) {
        // Set shipping date to today when delivered
        time_t now = time(NULL);
        struct tm *current_time = localtime(&now);
        order->shipping_date.day = current_time->tm_mday;
        order->shipping_date.month = current_time->tm_mon + 1;
        order->shipping_date.year = current_time->tm_year + 1900;
    }
}

void apply_promotion(ECommerceDatabase *db, const char *category, double discount_percent) {
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

void process_pending_orders(ECommerceDatabase *db) {
    int processed_count = 0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active && strcmp(db->orders[i].status, "Pending") == 0) {
            // Update status to Processing
            strcpy(db->orders[i].status, "Processing");
            processed_count++;
        }
    }
    
    printf("Processed %d pending orders.\n", processed_count);
    
    if (processed_count > 0) {
        printf("Orders status changed from 'Pending' to 'Processing'.\n");
    } else {
        printf("No pending orders found.\n");
    }
}

void calculate_revenue_statistics(const ECommerceDatabase *db) {
    printf("\n=== Revenue Statistics ===\n");
    
    int total_orders = 0;
    int pending_orders = 0;
    int processing_orders = 0;
    int shipped_orders = 0;
    int delivered_orders = 0;
    int cancelled_orders = 0;
    
    double total_revenue = 0.0;
    double total_shipping_fees = 0.0;
    double total_discounts = 0.0;
    
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].active) {
            total_orders++;
            total_revenue += db->orders[i].total_amount;
            total_shipping_fees += db->orders[i].shipping_fee;
            total_discounts += db->orders[i].discount_amount;
            
            if (strcmp(db->orders[i].status, "Pending") == 0) {
                pending_orders++;
            } else if (strcmp(db->orders[i].status, "Processing") == 0) {
                processing_orders++;
            } else if (strcmp(db->orders[i].status, "Shipped") == 0) {
                shipped_orders++;
            } else if (strcmp(db->orders[i].status, "Delivered") == 0) {
                delivered_orders++;
            } else if (strcmp(db->orders[i].status, "Cancelled") == 0) {
                cancelled_orders++;
            }
        }
    }
    
    printf("Total Orders: %d\n", total_orders);
    printf("Pending Orders: %d (%.1f%%)\n", pending_orders, total_orders > 0 ? (float)pending_orders / total_orders * 100 : 0.0);
    printf("Processing Orders: %d (%.1f%%)\n", processing_orders, total_orders > 0 ? (float)processing_orders / total_orders * 100 : 0.0);
    printf("Shipped Orders: %d (%.1f%%)\n", shipped_orders, total_orders > 0 ? (float)shipped_orders / total_orders * 100 : 0.0);
    printf("Delivered Orders: %d (%.1f%%)\n", delivered_orders, total_orders > 0 ? (float)delivered_orders / total_orders * 100 : 0.0);
    printf("Cancelled Orders: %d (%.1f%%)\n", cancelled_orders, total_orders > 0 ? (float)cancelled_orders / total_orders * 100 : 0.0);
    
    printf("\nRevenue:\n");
    printf("Total Revenue: $%.2f\n", total_revenue);
    printf("Total Shipping Fees: $%.2f\n", total_shipping_fees);
    printf("Total Discounts: $%.2f\n", total_discounts);
    printf("Net Revenue: $%.2f\n", total_revenue + total_shipping_fees - total_discounts);
    printf("Average Order Value: $%.2f\n", total_orders > 0 ? (total_revenue + total_shipping_fees - total_discounts) / total_orders : 0.0);
    
    // Monthly statistics
    printf("\nMonthly Revenue (Last 12 Months):\n");
    
    int monthly_revenues[12] = {0};
    
    // Get current month
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    int current_month = current_time->tm_mon;
    int current_year = current_time->tm_year + 1900;
    
    for (int i = 0; i < 12; i++) {
        int month = (current_month - i + 12) % 12;
        int year = current_month - i < 0 ? current_year - 1 : current_year;
        
        for (int j = 0; j < db->order_count; j++) {
            if (db->orders[j].active && db->orders[j].order_date.month == month + 1 && 
                db->orders[j].order_date.year == year) {
                monthly_revenues[i] += (int)(db->orders[j].total_amount + db->orders[j].shipping_fee - db->orders[j].discount_amount);
            }
        }
    }
    
    printf("Month      | Revenue\n");
    printf("-----------|----------\n");
    
    for (int i = 0; i < 12; i++) {
        int month = (current_month - i + 12) % 12;
        int year = current_month - i < 0 ? current_year - 1 : current_year;
        
        const char* month_names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
        
        printf("%-3s %5d | $%8.2f\n", month_names[month], year, (float)monthly_revenues[i]);
    }
}

int find_customer_by_id(const ECommerceDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_product_by_id(const ECommerceDatabase *db, int product_id) {
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].id == product_id && db->products[i].active) {
            return i;
        }
    }
    return -1;
}

int find_category_by_id(const ECommerceDatabase *db, int category_id) {
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].id == category_id && db->categories[i].active) {
            return i;
        }
    }
    return -1;
}

int find_order_by_id(const ECommerceDatabase *db, int order_id) {
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].id == order_id && db->orders[i].active) {
            return i;
        }
    }
    return -1;
}

int save_database(const ECommerceDatabase *db) {
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
    } else {
        return 0;
    }
    
    FILE *order_items_file = fopen("order_items.dat", "wb");
    if (order_items_file != NULL) {
        fwrite(&db->order_item_count, sizeof(int), 1, order_items_file);
        fwrite(db->order_items, sizeof(OrderItem), db->order_item_count, order_items_file);
        fclose(order_items_file);
    } else {
        return 0;
    }
    
    FILE *cart_items_file = fopen("cart_items.dat", "wb");
    if (cart_items_file != NULL) {
        fwrite(&db->cart_item_count, sizeof(int), 1, cart_items_file);
        fwrite(db->cart_items, sizeof(CartItem), db->cart_item_count, cart_items_file);
        fclose(cart_items_file);
        return 1;
    }
    
    return 0;
}

int load_database(ECommerceDatabase *db) {
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
    } else {
        return 0;
    }
    
    FILE *order_items_file = fopen("order_items.dat", "rb");
    if (order_items_file != NULL) {
        fread(&db->order_item_count, sizeof(int), 1, order_items_file);
        fread(db->order_items, sizeof(OrderItem), db->order_item_count, order_items_file);
        fclose(order_items_file);
    } else {
        return 0;
    }
    
    FILE *cart_items_file = fopen("cart_items.dat", "rb");
    if (cart_items_file != NULL) {
        fread(&db->cart_item_count, sizeof(int), 1, cart_items_file);
        fread(db->cart_items, sizeof(CartItem), db->cart_item_count, cart_items_file);
        fclose(cart_items_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(ECommerceDatabase *db) {
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda",
                          "Daniel", "Jennifer", "Matthew", "Melissa", "Christopher", "Laura",
                          "Joseph", "Karen", "Thomas", "Nancy", "Charles", "Lisa"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson",
                          "Taylor", "Thomas", "Moore", "Jackson", "Martin", "Lee"};
    
    char* product_names[] = {"Laptop", "Smartphone", "Tablet", "Monitor", "Keyboard",
                            "Mouse", "Headphones", "Speaker", "Camera", "Printer",
                            "Router", "External Drive", "Memory Card", "USB Cable", "Phone Case"};
    
    char* category_names[] = {"Electronics", "Computers", "Accessories", "Audio", "Photography",
                             "Networking", "Storage", "Mobile"};
    
    srand(time(NULL));
    
    // Add categories
    for (int i = 0; i < 8 && db->category_count < MAX_CATEGORIES; i++) {
        char description[200];
        sprintf(description, "High-quality %s products for your daily needs", category_names[i]);
        add_category(db, category_names[i], description);
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
        
        add_product(db, product_code, product_names[rand() % 15], category_names[rand() % 8],
                  description, price, stock_quantity, reorder_level, discount_percent);
    }
    
    // Add customers
    for (int i = 0; i < 500 && db->customer_count < MAX_CUSTOMERS; i++) {
        char first_name[MAX_NAME_LENGTH], last_name[MAX_NAME_LENGTH];
        strcpy(first_name, first_names[rand() % 24]);
        strcpy(last_name, last_names[rand() % 18]);
        
        char email[50], phone[15], address[100], username[30], password[30];
        sprintf(email, "%s.%d@example.com", first_name, 1000 + i);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(address, "%d Main St, City, State", 100 + rand() % 900);
        sprintf(username, "%s%d", first_name, rand() % 1000);
        sprintf(password, "pass%d", rand() % 10000);
        
        add_customer(db, first_name, last_name, email, phone, address, username, password);
    }
    
    // Create orders
    for (int i = 0; i < 1000 && db->order_count < MAX_ORDERS; i++) {
        int customer_id = 1 + (rand() % 500);
        char shipping_address[100];
        sprintf(shipping_address, "%d Shipping Ln, City, State", 1000 + rand() % 9000);
        
        int order_id = create_order(db, customer_id, shipping_address);
        if (order_id != -1) {
            // Add order items (1-5 items per order)
            int items_count = 1 + (rand() % 5);
            for (int j = 0; j < items_count && db->order_item_count < MAX_ORDERS * 10; j++) {
                int product_id = 1 + (rand() % 200);
                int quantity = 1 + (rand() % 5);
                double discount = (rand() % 30) / 10.0;
                
                add_order_item(db, order_id, product_id, quantity, discount);
            }
            
            // Update order status randomly
            int status_choice = rand() % 5;
            switch (status_choice) {
                case 0: update_order_status(db, order_id, "Pending"); break;
                case 1: update_order_status(db, order_id, "Processing"); break;
                case 2: update_order_status(db, order_id, "Shipped"); break;
                case 3: update_order_status(db, order_id, "Delivered"); break;
                case 4: update_order_status(db, order_id, "Cancelled"); break;
            }
        }
    }
    
    // Add some items to cart
    for (int i = 0; i < 100 && db->cart_item_count < MAX_CART_ITEMS; i++) {
        int customer_id = 1 + (rand() % 500);
        int product_id = 1 + (rand() % 200);
        int quantity = 1 + (rand() % 5);
        
        add_to_cart(db, customer_id, product_id, quantity);
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

int is_valid_username(const char *username) {
    if (username == NULL) return 0;
    
    int length = 0;
    for (int i = 0; username[i] != '\0'; i++) {
        length++;
        if (length > 30) return 0; // Too long
        if (!isalnum(username[i])) return 0; // Not alphanumeric
    }
    
    return length >= 3; // At least 3 characters
}

int is_valid_password(const char *password) {
    if (password == NULL) return 0;
    
    int length = 0;
    for (int i = 0; password[i] != '\0'; i++) {
        length++;
        if (length > 30) return 0; // Too long
    }
    
    return length >= 6; // At least 6 characters
}

const char* get_order_status_description(const char *status) {
    if (strcmp(status, "Pending") == 0) return "Order received and waiting for processing";
    if (strcmp(status, "Processing") == 0) return "Order is being prepared for shipment";
    if (strcmp(status, "Shipped") == 0) return "Order has been shipped and is on its way";
    if (strcmp(status, "Delivered") == 0) return "Order has been delivered to the customer";
    if (strcmp(status, "Cancelled") == 0) return "Order has been cancelled";
    return "Unknown status";
}