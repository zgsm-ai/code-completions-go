#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_PRODUCTS 500
#define MAX_ORDERS 1000
#define MAX_ORDER_ITEMS 3000
#define MAX_CUSTOMERS 500
#define MAX_CATEGORIES 50
#define MAX_NAME_LENGTH 50
#define MAX_DESC_LENGTH 200
#define FILENAME_PRODUCTS "products.dat"
#define FILENAME_ORDERS "orders.dat"
#define FILENAME_CUSTOMERS "customers.dat"
#define FILENAME_CATEGORIES "categories.dat"

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    float price;
    int stock_quantity;
    int reorder_level;
    int category_id;
    char supplier[MAX_NAME_LENGTH];
    int discontinued;
    Date created_date;
} Product;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESC_LENGTH];
    int parent_id; // 0 for top-level categories
} Category;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char email[50];
    char phone[15];
    char shipping_address[MAX_DESC_LENGTH];
    char billing_address[MAX_DESC_LENGTH];
    Date registration_date;
    float total_purchases;
    int active;
} Customer;

typedef struct {
    int id;
    int customer_id;
    Date order_date;
    Date ship_date;
    char status[20]; // "Pending", "Processing", "Shipped", "Delivered", "Cancelled"
    float total_amount;
    float tax_amount;
    float shipping_cost;
    char payment_method[20];
    int tracking_number;
} Order;

typedef struct {
    int id;
    int order_id;
    int product_id;
    int quantity;
    float unit_price;
    float discount;
} OrderItem;

typedef struct {
    Product products[MAX_PRODUCTS];
    int product_count;
    Category categories[MAX_CATEGORIES];
    int category_count;
    Customer customers[MAX_CUSTOMERS];
    int customer_count;
    Order orders[MAX_ORDERS];
    int order_count;
    OrderItem order_items[MAX_ORDER_ITEMS];
    int order_item_count;
} InventoryDatabase;

// Function prototypes
void initialize_database(InventoryDatabase *db);
int add_product(InventoryDatabase *db, const char *name, const char *description, 
                float price, int stock_quantity, int reorder_level, int category_id, 
                const char *supplier);
int add_category(InventoryDatabase *db, const char *name, const char *description, 
                 int parent_id);
int add_customer(InventoryDatabase *db, const char *name, const char *email, 
                  const char *phone, const char *shipping_address, 
                  const char *billing_address);
int create_order(InventoryDatabase *db, int customer_id, const char *payment_method);
int add_order_item(InventoryDatabase *db, int order_id, int product_id, 
                    int quantity, float discount);
void complete_order(InventoryDatabase *db, int order_id);
void ship_order(InventoryDatabase *db, int order_id);
void display_product(const Product *product, const InventoryDatabase *db);
void display_category(const Category *category, const InventoryDatabase *db, int level);
void display_customer(const Customer *customer);
void display_order(const Order *order, const InventoryDatabase *db);
void display_order_items(const InventoryDatabase *db, int order_id);
void display_all_products(const InventoryDatabase *db);
void display_all_categories(const InventoryDatabase *db);
void display_all_customers(const InventoryDatabase *db);
void display_all_orders(const InventoryDatabase *db);
int find_product_by_id(const InventoryDatabase *db, int product_id);
int find_category_by_id(const InventoryDatabase *db, int category_id);
int find_customer_by_id(const InventoryDatabase *db, int customer_id);
int find_order_by_id(const InventoryDatabase *db, int order_id);
void search_products_by_name(const InventoryDatabase *db, const char *name);
void search_products_by_category(const InventoryDatabase *db, int category_id);
void generate_inventory_report(const InventoryDatabase *db);
void generate_sales_report(const InventoryDatabase *db, int month, int year);
void update_stock(InventoryDatabase *db, int product_id, int quantity_change);
void check_reorder_levels(const InventoryDatabase *db);
void apply_discount_to_category(InventoryDatabase *db, int category_id, float discount_percentage);
int save_database(const InventoryDatabase *db);
int load_database(InventoryDatabase *db);
void generate_sample_data(InventoryDatabase *db);
void display_top_selling_products(const InventoryDatabase *db);
void display_customer_purchases(const InventoryDatabase *db, int customer_id);
void calculate_monthly_revenue(const InventoryDatabase *db, int year);
int is_valid_date(int day, int month, int year);

int main() {
    InventoryDatabase db;
    initialize_database(&db);
    
    printf("Inventory Management System\n");
    printf("1. Add Product\n");
    printf("2. Add Category\n");
    printf("3. Add Customer\n");
    printf("4. Create Order\n");
    printf("5. Add Order Item\n");
    printf("6. Complete Order\n");
    printf("7. Ship Order\n");
    printf("8. Display All Products\n");
    printf("9. Display All Categories\n");
    printf("10. Display All Customers\n");
    printf("11. Display All Orders\n");
    printf("12. Search Products by Name\n");
    printf("13. Search Products by Category\n");
    printf("14. Generate Inventory Report\n");
    printf("15. Generate Sales Report\n");
    printf("16. Update Stock\n");
    printf("17. Check Reorder Levels\n");
    printf("18. Apply Discount to Category\n");
    printf("19. Save Database\n");
    printf("20. Load Database\n");
    printf("21. Generate Sample Data\n");
    printf("22. Display Top Selling Products\n");
    printf("23. Display Customer Purchases\n");
    printf("24. Calculate Monthly Revenue\n");
    printf("0. Exit\n");
    
    int choice;
    do {
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH], description[MAX_DESC_LENGTH], supplier[MAX_NAME_LENGTH];
                float price;
                int stock_quantity, reorder_level, category_id;
                
                printf("Enter product name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter price: ");
                scanf("%f", &price);
                printf("Enter stock quantity: ");
                scanf("%d", &stock_quantity);
                printf("Enter reorder level: ");
                scanf("%d", &reorder_level);
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                printf("Enter supplier: ");
                scanf(" %[^\n]", supplier);
                
                int product_id = add_product(&db, name, description, price, stock_quantity, 
                                            reorder_level, category_id, supplier);
                if (product_id != -1) {
                    printf("Product added with ID: %d\n", product_id);
                } else {
                    printf("Failed to add product. Database is full or category not found.\n");
                }
                break;
            }
            case 2: {
                char name[MAX_NAME_LENGTH], description[MAX_DESC_LENGTH];
                int parent_id;
                
                printf("Enter category name: ");
                scanf(" %[^\n]", name);
                printf("Enter description: ");
                scanf(" %[^\n]", description);
                printf("Enter parent category ID (0 for top-level): ");
                scanf("%d", &parent_id);
                
                int category_id = add_category(&db, name, description, parent_id);
                if (category_id != -1) {
                    printf("Category added with ID: %d\n", category_id);
                } else {
                    printf("Failed to add category. Database is full.\n");
                }
                break;
            }
            case 3: {
                char name[MAX_NAME_LENGTH], email[50], phone[15];
                char shipping_address[MAX_DESC_LENGTH], billing_address[MAX_DESC_LENGTH];
                
                printf("Enter customer name: ");
                scanf(" %[^\n]", name);
                printf("Enter email: ");
                scanf("%s", email);
                printf("Enter phone: ");
                scanf("%s", phone);
                printf("Enter shipping address: ");
                scanf(" %[^\n]", shipping_address);
                printf("Enter billing address (press Enter to use shipping): ");
                getchar();
                fgets(billing_address, MAX_DESC_LENGTH, stdin);
                billing_address[strcspn(billing_address, "\n")] = '\0';
                
                if (strlen(billing_address) == 0) {
                    strcpy(billing_address, shipping_address);
                }
                
                int customer_id = add_customer(&db, name, email, phone, 
                                               shipping_address, billing_address);
                if (customer_id != -1) {
                    printf("Customer added with ID: %d\n", customer_id);
                } else {
                    printf("Failed to add customer. Database is full.\n");
                }
                break;
            }
            case 4: {
                int customer_id;
                char payment_method[20];
                
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                printf("Enter payment method (Credit Card, PayPal, Cash, etc.): ");
                scanf(" %[^\n]", payment_method);
                
                int order_id = create_order(&db, customer_id, payment_method);
                if (order_id != -1) {
                    printf("Order created with ID: %d\n", order_id);
                } else {
                    printf("Failed to create order. Customer not found.\n");
                }
                break;
            }
            case 5: {
                int order_id, product_id, quantity;
                float discount;
                
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                printf("Enter discount percentage (0-100): ");
                scanf("%f", &discount);
                
                if (add_order_item(&db, order_id, product_id, quantity, discount)) {
                    printf("Order item added successfully.\n");
                } else {
                    printf("Failed to add order item. Check IDs and stock availability.\n");
                }
                break;
            }
            case 6: {
                int order_id;
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                complete_order(&db, order_id);
                break;
            }
            case 7: {
                int order_id;
                printf("Enter order ID: ");
                scanf("%d", &order_id);
                ship_order(&db, order_id);
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
                display_all_customers(&db);
                break;
            }
            case 11: {
                display_all_orders(&db);
                break;
            }
            case 12: {
                char name[MAX_NAME_LENGTH];
                printf("Enter product name to search: ");
                scanf(" %[^\n]", name);
                search_products_by_name(&db, name);
                break;
            }
            case 13: {
                int category_id;
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                search_products_by_category(&db, category_id);
                break;
            }
            case 14: {
                generate_inventory_report(&db);
                break;
            }
            case 15: {
                int month, year;
                printf("Enter month (1-12): ");
                scanf("%d", &month);
                printf("Enter year: ");
                scanf("%d", &year);
                generate_sales_report(&db, month, year);
                break;
            }
            case 16: {
                int product_id, quantity_change;
                printf("Enter product ID: ");
                scanf("%d", &product_id);
                printf("Enter quantity change (positive to add, negative to remove): ");
                scanf("%d", &quantity_change);
                update_stock(&db, product_id, quantity_change);
                break;
            }
            case 17: {
                check_reorder_levels(&db);
                break;
            }
            case 18: {
                int category_id;
                float discount_percentage;
                printf("Enter category ID: ");
                scanf("%d", &category_id);
                printf("Enter discount percentage (0-100): ");
                scanf("%f", &discount_percentage);
                apply_discount_to_category(&db, category_id, discount_percentage);
                break;
            }
            case 19: {
                if (save_database(&db)) {
                    printf("Database saved successfully.\n");
                } else {
                    printf("Failed to save database.\n");
                }
                break;
            }
            case 20: {
                if (load_database(&db)) {
                    printf("Database loaded successfully.\n");
                } else {
                    printf("Failed to load database.\n");
                }
                break;
            }
            case 21: {
                generate_sample_data(&db);
                printf("Sample data generated.\n");
                break;
            }
            case 22: {
                display_top_selling_products(&db);
                break;
            }
            case 23: {
                int customer_id;
                printf("Enter customer ID: ");
                scanf("%d", &customer_id);
                display_customer_purchases(&db, customer_id);
                break;
            }
            case 24: {
                int year;
                printf("Enter year: ");
                scanf("%d", &year);
                calculate_monthly_revenue(&db, year);
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

void initialize_database(InventoryDatabase *db) {
    <｜fim▁hole｜>db->product_count = 0;
    db->category_count = 0;
    db->customer_count = 0;
    db->order_count = 0;
    db->order_item_count = 0;
}

int add_product(InventoryDatabase *db, const char *name, const char *description, 
                float price, int stock_quantity, int reorder_level, int category_id, 
                const char *supplier) {
    if (db->product_count >= MAX_PRODUCTS) {
        return -1;
    }
    
    // Validate category exists
    int category_index = find_category_by_id(db, category_id);
    if (category_index == -1) {
        printf("Category not found.\n");
        return -1;
    }
    
    Product *product = &db->products[db->product_count];
    product->id = db->product_count + 1;
    strncpy(product->name, name, MAX_NAME_LENGTH - 1);
    product->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(product->description, description, MAX_DESC_LENGTH - 1);
    product->description[MAX_DESC_LENGTH - 1] = '\0';
    product->price = price;
    product->stock_quantity = stock_quantity;
    product->reorder_level = reorder_level;
    product->category_id = category_id;
    strncpy(product->supplier, supplier, MAX_NAME_LENGTH - 1);
    product->supplier[MAX_NAME_LENGTH - 1] = '\0';
    product->discontinued = 0;
    
    // Set created date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    product->created_date.day = current_time->tm_mday;
    product->created_date.month = current_time->tm_mon + 1;
    product->created_date.year = current_time->tm_year + 1900;
    
    db->product_count++;
    return product->id;
}

int add_category(InventoryDatabase *db, const char *name, const char *description, 
                 int parent_id) {
    if (db->category_count >= MAX_CATEGORIES) {
        return -1;
    }
    
    // Validate parent category exists (if not top-level)
    if (parent_id != 0 && find_category_by_id(db, parent_id) == -1) {
        printf("Parent category not found.\n");
        return -1;
    }
    
    Category *category = &db->categories[db->category_count];
    category->id = db->category_count + 1;
    strncpy(category->name, name, MAX_NAME_LENGTH - 1);
    category->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(category->description, description, MAX_DESC_LENGTH - 1);
    category->description[MAX_DESC_LENGTH - 1] = '\0';
    category->parent_id = parent_id;
    
    db->category_count++;
    return category->id;
}

int add_customer(InventoryDatabase *db, const char *name, const char *email, 
                  const char *phone, const char *shipping_address, 
                  const char *billing_address) {
    if (db->customer_count >= MAX_CUSTOMERS) {
        return -1;
    }
    
    Customer *customer = &db->customers[db->customer_count];
    customer->id = db->customer_count + 1;
    strncpy(customer->name, name, MAX_NAME_LENGTH - 1);
    customer->name[MAX_NAME_LENGTH - 1] = '\0';
    strncpy(customer->email, email, 49);
    customer->email[49] = '\0';
    strncpy(customer->phone, phone, 14);
    customer->phone[14] = '\0';
    strncpy(customer->shipping_address, shipping_address, MAX_DESC_LENGTH - 1);
    customer->shipping_address[MAX_DESC_LENGTH - 1] = '\0';
    strncpy(customer->billing_address, billing_address, MAX_DESC_LENGTH - 1);
    customer->billing_address[MAX_DESC_LENGTH - 1] = '\0';
    customer->total_purchases = 0.0f;
    customer->active = 1;
    
    // Set registration date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    customer->registration_date.day = current_time->tm_mday;
    customer->registration_date.month = current_time->tm_mon + 1;
    customer->registration_date.year = current_time->tm_year + 1900;
    
    db->customer_count++;
    return customer->id;
}

int create_order(InventoryDatabase *db, int customer_id, const char *payment_method) {
    if (db->order_count >= MAX_ORDERS) {
        return -1;
    }
    
    // Validate customer exists
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        return -1;
    }
    
    Order *order = &db->orders[db->order_count];
    order->id = db->order_count + 1;
    order->customer_id = customer_id;
    strcpy(order->status, "Pending");
    order->total_amount = 0.0f;
    order->tax_amount = 0.0f;
    order->shipping_cost = 5.99f; // Default shipping cost
    strncpy(order->payment_method, payment_method, 19);
    order->payment_method[19] = '\0';
    order->tracking_number = 0;
    
    // Set order date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    order->order_date.day = current_time->tm_mday;
    order->order_date.month = current_time->tm_mon + 1;
    order->order_date.year = current_time->tm_year + 1900;
    
    // Initialize ship date to 0 (not shipped yet)
    order->ship_date.day = 0;
    order->ship_date.month = 0;
    order->ship_date.year = 0;
    
    db->order_count++;
    return order->id;
}

int add_order_item(InventoryDatabase *db, int order_id, int product_id, 
                    int quantity, float discount) {
    if (db->order_item_count >= MAX_ORDER_ITEMS) {
        return 0;
    }
    
    // Validate order exists and is in a state that allows adding items
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        return 0;
    }
    
    Order *order = &db->orders[order_index];
    if (strcmp(order->status, "Pending") != 0) {
        printf("Cannot add items to an order that is not pending.\n");
        return 0;
    }
    
    // Validate product exists
    int product_index = find_product_by_id(db, product_id);
    if (product_index == -1) {
        return 0;
    }
    
    Product *product = &db->products[product_index];
    
    // Check if there's enough stock
    if (product->stock_quantity < quantity) {
        printf("Insufficient stock. Only %d units available.\n", product->stock_quantity);
        return 0;
    }
    
    // Validate discount
    if (discount < 0 || discount > 100) {
        printf("Invalid discount percentage. Must be between 0 and 100.\n");
        return 0;
    }
    
    OrderItem *item = &db->order_items[db->order_item_count];
    item->id = db->order_item_count + 1;
    item->order_id = order_id;
    item->product_id = product_id;
    item->quantity = quantity;
    item->unit_price = product->price;
    item->discount = discount;
    
    // Update order total
    float item_total = item->unit_price * item->quantity * (1.0f - item->discount / 100.0f);
    order->total_amount += item_total;
    
    // Update tax (assuming 8% sales tax)
    order->tax_amount = order->total_amount * 0.08f;
    
    db->order_item_count++;
    return 1;
}

void complete_order(InventoryDatabase *db, int order_id) {
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        printf("Order not found.\n");
        return;
    }
    
    Order *order = &db->orders[order_index];
    
    if (strcmp(order->status, "Pending") != 0) {
        printf("Only pending orders can be completed.\n");
        return;
    }
    
    // Check if order has items
    int has_items = 0;
    for (int i = 0; i < db->order_item_count; i++) {
        if (db->order_items[i].order_id == order_id) {
            has_items = 1;
            break;
        }
    }
    
    if (!has_items) {
        printf("Cannot complete an order with no items.\n");
        return;
    }
    
    // Update product stock and customer purchases
    for (int i = 0; i < db->order_item_count; i++) {
        if (db->order_items[i].order_id == order_id) {
            OrderItem *item = &db->order_items[i];
            
            // Find the product and update stock
            int product_index = find_product_by_id(db, item->product_id);
            if (product_index != -1) {
                Product *product = &db->products[product_index];
                product->stock_quantity -= item->quantity;
            }
            
            // Update customer total purchases
            int customer_index = find_customer_by_id(db, order->customer_id);
            if (customer_index != -1) {
                float item_total = item->unit_price * item->quantity * 
                                  (1.0f - item->discount / 100.0f);
                db->customers[customer_index].total_purchases += item_total;
            }
        }
    }
    
    // Update order status
    strcpy(order->status, "Processing");
    
    // Add tax and shipping to total
    order->total_amount += order->tax_amount + order->shipping_cost;
    
    printf("Order %d has been completed and is now being processed.\n", order_id);
}

void ship_order(InventoryDatabase *db, int order_id) {
    int order_index = find_order_by_id(db, order_id);
    if (order_index == -1) {
        printf("Order not found.\n");
        return;
    }
    
    Order *order = &db->orders[order_index];
    
    if (strcmp(order->status, "Processing") != 0) {
        printf("Only processing orders can be shipped.\n");
        return;
    }
    
    // Update order status
    strcpy(order->status, "Shipped");
    
    // Set ship date to today
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    order->ship_date.day = current_time->tm_mday;
    order->ship_date.month = current_time->tm_mon + 1;
    order->ship_date.year = current_time->tm_year + 1900;
    
    // Generate tracking number (simple random number for demonstration)
    order->tracking_number = 1000000 + rand() % 9000000;
    
    printf("Order %d has been shipped. Tracking number: %d\n", 
           order_id, order->tracking_number);
}

void display_product(const Product *product, const InventoryDatabase *db) {
    printf("\nProduct ID: %d\n", product->id);
    printf("Name: %s\n", product->name);
    printf("Description: %s\n", product->description);
    printf("Price: $%.2f\n", product->price);
    printf("Stock Quantity: %d\n", product->stock_quantity);
    printf("Reorder Level: %d\n", product->reorder_level);
    
    // Display category
    int category_index = find_category_by_id(db, product->category_id);
    if (category_index != -1) {
        printf("Category: %s\n", db->categories[category_index].name);
    } else {
        printf("Category: Unknown\n");
    }
    
    printf("Supplier: %s\n", product->supplier);
    printf("Created Date: %02d/%02d/%d\n", 
           product->created_date.day, product->created_date.month, product->created_date.year);
    printf("Status: %s\n", product->discontinued ? "Discontinued" : "Active");
    
    // Show warning if stock is low
    if (product->stock_quantity <= product->reorder_level) {
        printf("WARNING: Stock level at or below reorder point!\n");
    }
}

void display_category(const Category *category, const InventoryDatabase *db, int level) {
    // Indent based on level for hierarchy visualization
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    printf("\nCategory ID: %d\n", category->id);
    
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    printf("Name: %s\n", category->name);
    
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    
    printf("Description: %s\n", category->description);
    
    // Display subcategories
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].parent_id == category->id) {
            display_category(&db->categories[i], db, level + 1);
        }
    }
}

void display_customer(const Customer *customer) {
    printf("\nCustomer ID: %d\n", customer->id);
    printf("Name: %s\n", customer->name);
    printf("Email: %s\n", customer->email);
    printf("Phone: %s\n", customer->phone);
    printf("Shipping Address: %s\n", customer->shipping_address);
    printf("Billing Address: %s\n", customer->billing_address);
    printf("Registration Date: %02d/%02d/%d\n", 
           customer->registration_date.day, customer->registration_date.month, 
           customer->registration_date.year);
    printf("Total Purchases: $%.2f\n", customer->total_purchases);
    printf("Status: %s\n", customer->active ? "Active" : "Inactive");
}

void display_order(const Order *order, const InventoryDatabase *db) {
    printf("\nOrder ID: %d\n", order->id);
    
    // Display customer information
    int customer_index = find_customer_by_id(db, order->customer_id);
    if (customer_index != -1) {
        printf("Customer: %s (ID: %d)\n", 
               db->customers[customer_index].name, order->customer_id);
    }
    
    printf("Order Date: %02d/%02d/%d\n", 
           order->order_date.day, order->order_date.month, order->order_date.year);
    
    if (order->ship_date.year != 0) {
        printf("Ship Date: %02d/%02d/%d\n", 
               order->ship_date.day, order->ship_date.month, order->ship_date.year);
    } else {
        printf("Ship Date: Not shipped yet\n");
    }
    
    printf("Status: %s\n", order->status);
    printf("Subtotal: $%.2f\n", order->total_amount - order->tax_amount - order->shipping_cost);
    printf("Tax: $%.2f\n", order->tax_amount);
    printf("Shipping: $%.2f\n", order->shipping_cost);
    printf("Total: $%.2f\n", order->total_amount);
    printf("Payment Method: %s\n", order->payment_method);
    
    if (order->tracking_number != 0) {
        printf("Tracking Number: %d\n", order->tracking_number);
    }
}

void display_order_items(const InventoryDatabase *db, int order_id) {
    printf("\nOrder Items for Order ID %d:\n", order_id);
    
    int found = 0;
    float subtotal = 0.0f;
    
    for (int i = 0; i < db->order_item_count; i++) {
        if (db->order_items[i].order_id == order_id) {
            OrderItem *item = &db->order_items[i];
            
            // Find product information
            int product_index = find_product_by_id(db, item->product_id);
            if (product_index != -1) {
                Product *product = &db->products[product_index];
                
                printf("  Product: %s\n", product->name);
                printf("    Quantity: %d\n", item->quantity);
                printf("    Unit Price: $%.2f\n", item->unit_price);
                printf("    Discount: %.1f%%\n", item->discount);
                
                float item_total = item->unit_price * item->quantity * 
                                  (1.0f - item->discount / 100.0f);
                printf("    Total: $%.2f\n", item_total);
                
                subtotal += item_total;
                found++;
            }
        }
    }
    
    if (found == 0) {
        printf("No items found for this order.\n");
    } else {
        printf("\nSubtotal: $%.2f\n", subtotal);
    }
}

void display_all_products(const InventoryDatabase *db) {
    printf("\n=== All Products ===\n");
    printf("Total Products: %d\n\n", db->product_count);
    
    for (int i = 0; i < db->product_count; i++) {
        if (!db->products[i].discontinued) {
            display_product(&db->products[i], db);
            printf("------------------------\n");
        }
    }
}

void display_all_categories(const InventoryDatabase *db) {
    printf("\n=== All Categories ===\n");
    printf("Total Categories: %d\n\n", db->category_count);
    
    // Display top-level categories first
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].parent_id == 0) {
            display_category(&db->categories[i], db, 0);
            printf("------------------------\n");
        }
    }
}

void display_all_customers(const InventoryDatabase *db) {
    printf("\n=== All Customers ===\n");
    printf("Total Customers: %d\n\n", db->customer_count);
    
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].active) {
            display_customer(&db->customers[i]);
            printf("------------------------\n");
        }
    }
}

void display_all_orders(const InventoryDatabase *db) {
    printf("\n=== All Orders ===\n");
    printf("Total Orders: %d\n\n", db->order_count);
    
    for (int i = 0; i < db->order_count; i++) {
        display_order(&db->orders[i], db);
        display_order_items(db, db->orders[i].id);
        printf("========================\n");
    }
}

int find_product_by_id(const InventoryDatabase *db, int product_id) {
    for (int i = 0; i < db->product_count; i++) {
        if (db->products[i].id == product_id && !db->products[i].discontinued) {
            return i;
        }
    }
    return -1;
}

int find_category_by_id(const InventoryDatabase *db, int category_id) {
    for (int i = 0; i < db->category_count; i++) {
        if (db->categories[i].id == category_id) {
            return i;
        }
    }
    return -1;
}

int find_customer_by_id(const InventoryDatabase *db, int customer_id) {
    for (int i = 0; i < db->customer_count; i++) {
        if (db->customers[i].id == customer_id && db->customers[i].active) {
            return i;
        }
    }
    return -1;
}

int find_order_by_id(const InventoryDatabase *db, int order_id) {
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].id == order_id) {
            return i;
        }
    }
    return -1;
}

void search_products_by_name(const InventoryDatabase *db, const char *name) {
    printf("\nSearch Results for '%s':\n", name);
    int found = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (!db->products[i].discontinued && strstr(db->products[i].name, name) != NULL) {
            display_product(&db->products[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No products found with name containing '%s'.\n", name);
    }
}

void search_products_by_category(const InventoryDatabase *db, int category_id) {
    int category_index = find_category_by_id(db, category_id);
    if (category_index == -1) {
        printf("Category not found.\n");
        return;
    }
    
    printf("\nProducts in Category '%s':\n", db->categories[category_index].name);
    int found = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (!db->products[i].discontinued && db->products[i].category_id == category_id) {
            display_product(&db->products[i], db);
            printf("------------------------\n");
            found++;
        }
    }
    
    if (found == 0) {
        printf("No products found in this category.\n");
    }
}

void generate_inventory_report(const InventoryDatabase *db) {
    printf("\n=== Inventory Report ===\n");
    
    float total_inventory_value = 0.0f;
    int total_products = 0;
    int discontinued_products = 0;
    int low_stock_items = 0;
    int out_of_stock_items = 0;
    
    printf("%-5s %-20s %-15s %-10s %-10s %-10s\n", 
           "ID", "Name", "Category", "Price", "Stock", "Value");
    printf("-------------------------------------------------------------------\n");
    
    for (int i = 0; i < db->product_count; i++) {
        Product *product = &db->products[i];
        float value = product->price * product->stock_quantity;
        
        if (!product->discontinued) {
            // Get category name
            int category_index = find_category_by_id(db, product->category_id);
            char category_name[MAX_NAME_LENGTH] = "Unknown";
            if (category_index != -1) {
                strcpy(category_name, db->categories[category_index].name);
            }
            
            printf("%-5d %-20s %-15s $%-9.2f %-10d $%-9.2f\n", 
                   product->id, product->name, category_name, 
                   product->price, product->stock_quantity, value);
            
            total_inventory_value += value;
            total_products++;
            
            if (product->stock_quantity <= 0) {
                out_of_stock_items++;
            } else if (product->stock_quantity <= product->reorder_level) {
                low_stock_items++;
            }
        } else {
            discontinued_products++;
        }
    }
    
    printf("-------------------------------------------------------------------\n");
    printf("\nInventory Summary:\n");
    printf("Total Active Products: %d\n", total_products);
    printf("Total Discontinued Products: %d\n", discontinued_products);
    printf("Total Inventory Value: $%.2f\n", total_inventory_value);
    printf("Out of Stock Items: %d\n", out_of_stock_items);
    printf("Low Stock Items: %d\n", low_stock_items);
}

void generate_sales_report(const InventoryDatabase *db, int month, int year) {
    if (month < 1 || month > 12 || year < 2000 || year > 2100) {
        printf("Invalid month or year.\n");
        return;
    }
    
    char month_names[][10] = {"January", "February", "March", "April", "May", "June",
                             "July", "August", "September", "October", "November", "December"};
    
    printf("\n=== Sales Report for %s %d ===\n", month_names[month - 1], year);
    
    float total_sales = 0.0f;
    float total_tax = 0.0f;
    float total_shipping = 0.0f;
    int order_count = 0;
    
    // Count sales by product
    int product_sales[MAX_PRODUCTS] = {0};
    float product_revenue[MAX_PRODUCTS] = {0.0f};
    
    for (int i = 0; i < db->order_count; i++) {
        Order *order = &db->orders[i];
        
        // Check if order is in the specified month and year
        if (order->order_date.year == year && order->order_date.month == month) {
            // Check if order is shipped or delivered
            if (strcmp(order->status, "Shipped") == 0 || strcmp(order->status, "Delivered") == 0) {
                total_sales += order->total_amount;
                total_tax += order->tax_amount;
                total_shipping += order->shipping_cost;
                order_count++;
                
                // Count product sales
                for (int j = 0; j < db->order_item_count; j++) {
                    OrderItem *item = &db->order_items[j];
                    if (item->order_id == order->id) {
                        product_sales[item->product_id - 1] += item->quantity;
                        
                        float item_total = item->unit_price * item->quantity * 
                                          (1.0f - item->discount / 100.0f);
                        product_revenue[item->product_id - 1] += item_total;
                    }
                }
            }
        }
    }
    
    printf("Total Orders: %d\n", order_count);
    printf("Total Sales: $%.2f\n", total_sales);
    printf("Total Tax: $%.2f\n", total_tax);
    printf("Total Shipping: $%.2f\n", total_shipping);
    printf("Net Revenue: $%.2f\n", total_sales - total_tax - total_shipping);
    
    // Display top selling products
    printf("\nTop Selling Products:\n");
    printf("%-5s %-20s %-10s %-15s\n", "ID", "Name", "Quantity", "Revenue");
    printf("------------------------------------------------\n");
    
    // Create temporary arrays to sort products by revenue
    int sorted_indices[MAX_PRODUCTS];
    int valid_products = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (product_sales[i] > 0) {
            sorted_indices[valid_products] = i;
            valid_products++;
        }
    }
    
    // Sort by revenue (descending)
    for (int i = 0; i < valid_products - 1; i++) {
        for (int j = 0; j < valid_products - i - 1; j++) {
            if (product_revenue[sorted_indices[j]] < product_revenue[sorted_indices[j + 1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 or fewer
    int display_count = valid_products < 10 ? valid_products : 10;
    for (int i = 0; i < display_count; i++) {
        int index = sorted_indices[i];
        printf("%-5d %-20s %-10d $%-14.2f\n", 
               db->products[index].id, db->products[index].name, 
               product_sales[index], product_revenue[index]);
    }
}

void update_stock(InventoryDatabase *db, int product_id, int quantity_change) {
    int product_index = find_product_by_id(db, product_id);
    if (product_index == -1) {
        printf("Product not found.\n");
        return;
    }
    
    Product *product = &db->products[product_index];
    
    // Check if the new quantity would be negative
    if (product->stock_quantity + quantity_change < 0) {
        printf("Cannot remove more items than are in stock.\n");
        return;
    }
    
    int old_quantity = product->stock_quantity;
    product->stock_quantity += quantity_change;
    
    printf("Stock for product '%s' updated from %d to %d.\n", 
           product->name, old_quantity, product->stock_quantity);
    
    // Check if stock is now at or below reorder level
    if (product->stock_quantity <= product->reorder_level) {
        printf("WARNING: Stock level is now at or below reorder point!\n");
    }
}

void check_reorder_levels(const InventoryDatabase *db) {
    printf("\n=== Reorder Level Report ===\n");
    printf("Products that need to be reordered:\n\n");
    
    int found = 0;
    
    printf("%-5s %-20s %-10s %-10s\n", "ID", "Name", "Current", "Reorder");
    printf("---------------------------------------\n");
    
    for (int i = 0; i < db->product_count; i++) {
        Product *product = &db->products[i];
        if (!product->discontinued && product->stock_quantity <= product->reorder_level) {
            printf("%-5d %-20s %-10d %-10d\n", 
                   product->id, product->name, product->stock_quantity, product->reorder_level);
            found++;
        }
    }
    
    if (found == 0) {
        printf("No products need to be reordered.\n");
    } else {
        printf("\nTotal products needing reorder: %d\n", found);
    }
}

void apply_discount_to_category(InventoryDatabase *db, int category_id, float discount_percentage) {
    if (discount_percentage < 0 || discount_percentage > 100) {
        printf("Invalid discount percentage. Must be between 0 and 100.\n");
        return;
    }
    
    int category_index = find_category_by_id(db, category_id);
    if (category_index == -1) {
        printf("Category not found.\n");
        return;
    }
    
    printf("Applying %.1f%% discount to all products in category '%s'...\n", 
           discount_percentage, db->categories[category_index].name);
    
    int affected_products = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        Product *product = &db->products[i];
        if (!product->discontinued && product->category_id == category_id) {
            float old_price = product->price;
            product->price *= (1.0f - discount_percentage / 100.0f);
            
            printf("Product '%s': $%.2f -> $%.2f\n", 
                   product->name, old_price, product->price);
            affected_products++;
        }
    }
    
    printf("\nDiscount applied to %d products.\n", affected_products);
}

int save_database(const InventoryDatabase *db) {
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
    
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "wb");
    if (customers_file != NULL) {
        fwrite(&db->customer_count, sizeof(int), 1, customers_file);
        fwrite(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *orders_file = fopen(FILENAME_ORDERS, "wb");
    if (orders_file != NULL) {
        fwrite(&db->order_count, sizeof(int), 1, orders_file);
        fwrite(db->orders, sizeof(Order), db->order_count, orders_file);
        fwrite(&db->order_item_count, sizeof(int), 1, orders_file);
        fwrite(db->order_items, sizeof(OrderItem), db->order_item_count, orders_file);
        fclose(orders_file);
        return 1;
    }
    
    return 0;
}

int load_database(InventoryDatabase *db) {
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
    
    FILE *customers_file = fopen(FILENAME_CUSTOMERS, "rb");
    if (customers_file != NULL) {
        fread(&db->customer_count, sizeof(int), 1, customers_file);
        fread(db->customers, sizeof(Customer), db->customer_count, customers_file);
        fclose(customers_file);
    } else {
        return 0;
    }
    
    FILE *orders_file = fopen(FILENAME_ORDERS, "rb");
    if (orders_file != NULL) {
        fread(&db->order_count, sizeof(int), 1, orders_file);
        fread(db->orders, sizeof(Order), db->order_count, orders_file);
        fread(&db->order_item_count, sizeof(int), 1, orders_file);
        fread(db->order_items, sizeof(OrderItem), db->order_item_count, orders_file);
        fclose(orders_file);
        return 1;
    }
    
    return 0;
}

void generate_sample_data(InventoryDatabase *db) {
    char* product_names[] = {
        "Laptop", "Desktop Computer", "Monitor", "Keyboard", "Mouse",
        "Smartphone", "Tablet", "Smart Watch", "Headphones", "Speakers",
        "Printer", "Scanner", "External Hard Drive", "USB Flash Drive", "Memory Card",
        "Router", "Modem", "Network Switch", "Ethernet Cable", "Power Adapter",
        "Software License", "Operating System", "Office Suite", "Antivirus", "VPN Service",
        "Webcam", "Microphone", "Graphics Tablet", "Gaming Console", "VR Headset"
    };
    
    char* category_names[] = {
        "Computers", "Peripherals", "Mobile Devices", "Accessories",
        "Networking", "Storage", "Software", "Gaming", "Electronics"
    };
    
    char* supplier_names[] = {
        "TechSupply Co.", "Global Electronics", "Digital Solutions", "Computer World",
        "Gadget Central", "Network Pro", "Storage Masters", "Software Giants"
    };
    
    char* first_names[] = {"John", "Jane", "Michael", "Sarah", "Robert", "Emily",
                          "David", "Jessica", "William", "Ashley", "James", "Amanda"};
    
    char* last_names[] = {"Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia",
                          "Miller", "Davis", "Rodriguez", "Martinez", "Wilson", "Anderson"};
    
    char* payment_methods[] = {"Credit Card", "PayPal", "Bank Transfer", "Cash", "Check"};
    
    srand(time(NULL));
    
    // Add categories
    for (int i = 0; i < 9 && db->category_count < MAX_CATEGORIES; i++) {
        add_category(db, category_names[i], "Category description", 0);
    }
    
    // Add subcategories for some categories
    for (int i = 0; i < 5 && db->category_count < MAX_CATEGORIES; i++) {
        char subcat_name[50];
        sprintf(subcat_name, "Subcategory %d", i + 1);
        add_category(db, subcat_name, "Subcategory description", 1 + (rand() % 3));
    }
    
    // Add products
    for (int i = 0; i < 100 && db->product_count < MAX_PRODUCTS; i++) {
        char description[MAX_DESC_LENGTH];
        sprintf(description, "High quality %s with advanced features", product_names[i % 30]);
        
        int category_id = 1 + (rand() % db->category_count);
        int supplier_index = rand() % 8;
        float price = 10.0f + (rand() % 1000);
        int stock = 5 + (rand() % 100);
        int reorder_level = 5 + (rand() % 20);
        
        add_product(db, product_names[i % 30], description, price, stock, 
                    reorder_level, category_id, supplier_names[supplier_index]);
    }
    
    // Add customers
    for (int i = 0; i < 50 && db->customer_count < MAX_CUSTOMERS; i++) {
        char name[60], email[60], phone[15];
        char shipping_address[MAX_DESC_LENGTH], billing_address[MAX_DESC_LENGTH];
        
        sprintf(name, "%s %s", first_names[rand() % 12], last_names[rand() % 12]);
        sprintf(email, "%s.%s@email.com", first_names[rand() % 12], last_names[rand() % 12]);
        sprintf(phone, "555-%04d", rand() % 10000);
        sprintf(shipping_address, "%d Main St, City, State", 100 + rand() % 900);
        strcpy(billing_address, shipping_address);
        
        add_customer(db, name, email, phone, shipping_address, billing_address);
    }
    
    // Create orders with items
    for (int i = 0; i < 100 && db->order_count < MAX_ORDERS; i++) {
        int customer_id = 1 + (rand() % db->customer_count);
        const char *payment_method = payment_methods[rand() % 5];
        
        int order_id = create_order(db, customer_id, payment_method);
        
        // Add 1-5 items to each order
        int num_items = 1 + (rand() % 5);
        for (int j = 0; j < num_items && db->order_item_count < MAX_ORDER_ITEMS; j++) {
            int product_id = 1 + (rand() % db->product_count);
            int quantity = 1 + (rand() % 5);
            float discount = (rand() % 20) / 100.0f; // 0-19% discount
            
            add_order_item(db, order_id, product_id, quantity, discount);
        }
        
        // Complete most orders
        if (rand() % 10 < 8) { // 80% chance
            complete_order(db, order_id);
            
            // Ship some of the completed orders
            if (rand() % 10 < 7) { // 70% chance
                ship_order(db, order_id);
            }
        }
    }
}

void display_top_selling_products(const InventoryDatabase *db) {
    printf("\n=== Top Selling Products ===\n");
    
    // Count sales for each product
    int product_sales[MAX_PRODUCTS] = {0};
    float product_revenue[MAX_PRODUCTS] = {0.0f};
    
    for (int i = 0; i < db->order_item_count; i++) {
        OrderItem *item = &db->order_items[i];
        product_sales[item->product_id - 1] += item->quantity;
        
        float item_total = item->unit_price * item->quantity * 
                          (1.0f - item->discount / 100.0f);
        product_revenue[item->product_id - 1] += item_total;
    }
    
    // Create temporary arrays to sort products by quantity sold
    int sorted_indices[MAX_PRODUCTS];
    int valid_products = 0;
    
    for (int i = 0; i < db->product_count; i++) {
        if (product_sales[i] > 0) {
            sorted_indices[valid_products] = i;
            valid_products++;
        }
    }
    
    // Sort by quantity sold (descending)
    for (int i = 0; i < valid_products - 1; i++) {
        for (int j = 0; j < valid_products - i - 1; j++) {
            if (product_sales[sorted_indices[j]] < product_sales[sorted_indices[j + 1]]) {
                int temp = sorted_indices[j];
                sorted_indices[j] = sorted_indices[j + 1];
                sorted_indices[j + 1] = temp;
            }
        }
    }
    
    // Display top 10 or fewer
    int display_count = valid_products < 10 ? valid_products : 10;
    printf("%-5s %-20s %-10s %-15s\n", "ID", "Name", "Quantity", "Revenue");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < display_count; i++) {
        int index = sorted_indices[i];
        printf("%-5d %-20s %-10d $%-14.2f\n", 
               db->products[index].id, db->products[index].name, 
               product_sales[index], product_revenue[index]);
    }
    
    if (valid_products == 0) {
        printf("No sales data available.\n");
    }
}

void display_customer_purchases(const InventoryDatabase *db, int customer_id) {
    int customer_index = find_customer_by_id(db, customer_id);
    if (customer_index == -1) {
        printf("Customer not found.\n");
        return;
    }
    
    Customer *customer = &db->customers[customer_index];
    printf("\n=== Purchase History for %s ===\n", customer->name);
    
    float total_spent = 0.0f;
    int order_count = 0;
    
    // Find all orders for this customer
    for (int i = 0; i < db->order_count; i++) {
        if (db->orders[i].customer_id == customer_id) {
            Order *order = &db->orders[i];
            printf("\nOrder ID: %d\n", order->id);
            printf("Order Date: %02d/%02d/%d\n", 
                   order->order_date.day, order->order_date.month, order->order_date.year);
            printf("Status: %s\n", order->status);
            printf("Total Amount: $%.2f\n", order->total_amount);
            printf("Payment Method: %s\n", order->payment_method);
            
            // Display order items
            for (int j = 0; j < db->order_item_count; j++) {
                OrderItem *item = &db->order_items[j];
                if (item->order_id == order->id) {
                    // Find product information
                    int product_index = find_product_by_id(db, item->product_id);
                    if (product_index != -1) {
                        Product *product = &db->products[product_index];
                        
                        printf("  - %s: %d x $%.2f", 
                               product->name, item->quantity, item->unit_price);
                        
                        if (item->discount > 0) {
                            printf(" (%.1f%% off)", item->discount);
                        }
                        
                        float item_total = item->unit_price * item->quantity * 
                                          (1.0f - item->discount / 100.0f);
                        printf(" = $%.2f\n", item_total);
                    }
                }
            }
            
            total_spent += order->total_amount;
            order_count++;
        }
    }
    
    printf("\nSummary:\n");
    printf("Total Orders: %d\n", order_count);
    printf("Total Amount Spent: $%.2f\n", total_spent);
    printf("Average Order Value: $%.2f\n", order_count > 0 ? total_spent / order_count : 0.0f);
}

void calculate_monthly_revenue(const InventoryDatabase *db, int year) {
    if (year < 2000 || year > 2100) {
        printf("Invalid year.\n");
        return;
    }
    
    printf("\n=== Monthly Revenue for %d ===\n", year);
    
    float monthly_revenue[12] = {0.0f};
    int monthly_orders[12] = {0};
    
    for (int i = 0; i < db->order_count; i++) {
        Order *order = &db->orders[i];
        
        // Check if order is in the specified year
        if (order->order_date.year == year) {
            // Check if order is shipped or delivered
            if (strcmp(order->status, "Shipped") == 0 || strcmp(order->status, "Delivered") == 0) {
                int month = order->order_date.month - 1; // Convert to 0-based index
                monthly_revenue[month] += order->total_amount;
                monthly_orders[month]++;
            }
        }
    }
    
    char month_names[][10] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                             "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    
    printf("%-5s %-15s %-10s %-15s\n", "Month", "Revenue", "Orders", "Avg. Order");
    printf("------------------------------------------------\n");
    
    float total_revenue = 0.0f;
    int total_orders = 0;
    
    for (int i = 0; i < 12; i++) {
        float avg_order = monthly_orders[i] > 0 ? monthly_revenue[i] / monthly_orders[i] : 0.0f;
        printf("%-5s $%-14.2f %-10d $%-14.2f\n", 
               month_names[i], monthly_revenue[i], monthly_orders[i], avg_order);
        
        total_revenue += monthly_revenue[i];
        total_orders += monthly_orders[i];
    }
    
    printf("------------------------------------------------\n");
    printf("Total $%-14.2f %-10d $%-14.2f\n", 
           total_revenue, total_orders, 
           total_orders > 0 ? total_revenue / total_orders : 0.0f);
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