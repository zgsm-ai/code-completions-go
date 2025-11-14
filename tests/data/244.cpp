#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <stack>
#include <queue>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <functional>
#include <cmath>

/**
 * E-commerce Shopping System
 * 
 * This C++ program implements a comprehensive e-commerce shopping system
 * with product management, shopping cart functionality, and order processing.
 */

// Enum for product categories
enum class ProductCategory {
    ELECTRONICS,
    CLOTHING,
    BOOKS,
    HOME_APPLIANCES,
    SPORTS,
    TOYS,
    FOOD,
    BEAUTY
};

// Utility function to convert category to string
std::string categoryToString(ProductCategory category) {
    switch (category) {
        case ProductCategory::ELECTRONICS: return "Electronics";
        case ProductCategory::CLOTHING: return "Clothing";
        case ProductCategory::BOOKS: return "Books";
        case ProductCategory::HOME_APPLIANCES: return "Home Appliances";
        case ProductCategory::SPORTS: return "Sports";
        case ProductCategory::TOYS: return "Toys";
        case ProductCategory::FOOD: return "Food";
        case ProductCategory::BEAUTY: return "Beauty";
        default: return "Unknown";
    }
}

// Product class
class Product {
private:
    std::string productId;
    std::string name;
    std::string description;
    ProductCategory category;
    float price;
    int stockQuantity;
    std::string brand;
    std::vector<std::string> tags;
    float averageRating;
    int numReviews;
    
public:
    Product(const std::string& id, const std::string& name, 
            const std::string& description, ProductCategory category,
            float price, int stockQuantity, const std::string& brand = "")
        : productId(id), name(name), description(description), 
          category(category), price(price), stockQuantity(stockQuantity), 
          brand(brand), averageRating(0.0f), numReviews(0) {}
    
    // Getters
    std::string getId() const { return productId; }
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }
    ProductCategory getCategory() const { return category; }
    float getPrice() const { return price; }
    int getStockQuantity() const { return stockQuantity; }
    std::string getBrand() const { return brand; }
    std::vector<std::string> getTags() const { return tags; }
    float getAverageRating() const { return averageRating; }
    int getNumReviews() const { return numReviews; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setDescription(const std::string& newDesc) { description = newDesc; }
    void setPrice(float newPrice) { price = newPrice; }
    void setStockQuantity(int quantity) { stockQuantity = quantity; }
    void setBrand(const std::string& newBrand) { brand = newBrand; }
    
    // Tag management
    void addTag(const std::string& tag) {
        if (std::find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }
    
    void removeTag(const std::string& tag) {
        tags.erase(std::remove(tags.begin(), tags.end(), tag), tags.end());
    }
    
    // Rating management
    void addRating(int rating) {
        if (rating >= 1 && rating <= 5) {
            float totalRating = averageRating * numReviews + rating;
            numReviews++;
            averageRating = totalRating / numReviews;
        }
    }
    
    // Stock management
    bool decreaseStock(int quantity) {
        if (stockQuantity >= quantity) {
            stockQuantity -= quantity;
            return true;
        }
        return false;
    }
    
    void increaseStock(int quantity) {
        stockQuantity += quantity;
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "Product ID: " << productId << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Brand: " << (brand.empty() ? "N/A" : brand) << std::endl;
        std::cout << "Category: " << categoryToString(category) << std::endl;
        std::cout << "Price: $" << price << std::endl;
        std::cout << "Stock: " << stockQuantity << " units" << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Tags: ";
        for (const auto& tag : tags) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
        std::cout << "Rating: " << averageRating << "/5 (" << numReviews << " reviews)" << std::endl;
    }
    
    std::string getSummary() const {
        return productId + ": " + name + " - $" + std::to_string(price) + 
               " (" + std::to_string(stockQuantity) + " in stock)";
    }
};

// Cart item class
class CartItem {
private:
    std::shared_ptr<Product> product;
    int quantity;
    float totalPrice;
    
public:
    CartItem(std::shared_ptr<Product> prod, int qty = 1) 
        : product(prod), quantity(qty) {
        calculateTotal();
    }
    
    std::shared_ptr<Product> getProduct() const { return product; }
    int getQuantity() const { return quantity; }
    float getTotalPrice() const { return totalPrice; }
    
    void setQuantity(int qty) {
        quantity = qty;
        calculateTotal();
    }
    
    void increaseQuantity(int amount = 1) {
        quantity += amount;
        calculateTotal();
    }
    
    void decreaseQuantity(int amount = 1) {
        if (quantity > amount) {
            quantity -= amount;
        } else {
            quantity = 0;
        }
        calculateTotal();
    }
    
    void calculateTotal() {
        if (product) {
            totalPrice = product->getPrice() * quantity;
        } else {
            totalPrice = 0.0f;
        }
    }
    
    void displayDetails() const {
        if (product) {
            std::cout << product->getName() << " x" << quantity 
                      << " - $" << totalPrice << std::endl;
            std::cout << "  Unit price: $" << product->getPrice() << std::endl;
        }
    }
};

// Shopping cart class
class ShoppingCart {
private:
    std::vector<std::shared_ptr<CartItem>> items;
    float discountPercentage;
    
public:
    ShoppingCart() : discountPercentage(0.0f) {}
    
    // Cart operations
    bool addItem(std::shared_ptr<Product> product, int quantity = 1) {
        if (!product || quantity <= 0) {
            return false;
        }
        
        // Check if product already in cart
        for (auto& item : items) {
            if (item->getProduct()->getId() == product->getId()) {
                // Increase quantity of existing item
                item->increaseQuantity(quantity);
                return true;
            }
        }
        
        // Add new item to cart
        items.push_back(std::make_shared<CartItem>(product, quantity));
        return true;
    }
    
    bool removeItem(const std::string& productId) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if ((*it)->getProduct()->getId() == productId) {
                items.erase(it);
                return true;
            }
        }
        return false;
    }
    
    bool updateItemQuantity(const std::string& productId, int quantity) {
        if (quantity <= 0) {
            return removeItem(productId);
        }
        
        for (auto& item : items) {
            if (item->getProduct()->getId() == productId) {
                item->setQuantity(quantity);
                return true;
            }
        }
        return false;
    }
    
    // Cart information
    float getSubtotal() const {
        float subtotal = 0.0f;
        for (const auto& item : items) {
            subtotal += item->getTotalPrice();
        }
        return subtotal;
    }
    
    float getDiscountAmount() const {
        return getSubtotal() * (discountPercentage / 100.0f);
    }
    
    float getTotal() const {
        return getSubtotal() - getDiscountAmount();
    }
    
    int getItemCount() const {
        return items.size();
    }
    
    int getTotalQuantity() const {
        int totalQty = 0;
        for (const auto& item : items) {
            totalQty += item->getQuantity();
        }
        return totalQty;
    }
    
    void setDiscount(float percentage) {
        discountPercentage = std::max(0.0f, std::min(100.0f, percentage));
    }
    
    float getDiscountPercentage() const {
        return discountPercentage;
    }
    
    bool isEmpty() const {
        return items.empty();
    }
    
    void clear() {
        items.clear();
        discountPercentage = 0.0f;
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "=== Shopping Cart ===" << std::endl;
        std::cout << "Items: " << getItemCount() << " (Total Quantity: " << getTotalQuantity() << ")" << std::endl;
        
        for (const auto& item : items) {
            item->displayDetails();
            std::cout << std::endl;
        }
        
        std::cout << "Subtotal: $" << getSubtotal() << std::endl;
        if (discountPercentage > 0) {
            std::cout << "Discount: " << discountPercentage << "% (-$" << getDiscountAmount() << ")" << std::endl;
        }
        std::cout << "Total: $" << getTotal() << std::endl;
    }
    
    // Cart operations for checkout
    bool checkStockAvailability() const {
        for (const auto& item : items) {
            if (item->getProduct()->getStockQuantity() < item->getQuantity()) {
                return false;
            }
        }
        return true;
    }
    
    std::vector<std::shared_ptr<CartItem>> getItems() const {
        return items;
    }
};

// Order status enum
enum class OrderStatus {
    PENDING,
    PROCESSING,
    SHIPPED,
    DELIVERED,
    CANCELLED
};

// Utility function to convert order status to string
std::string orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING: return "Pending";
        case OrderStatus::PROCESSING: return "Processing";
        case OrderStatus::SHIPPED: return "Shipped";
        case OrderStatus::DELIVERED: return "Delivered";
        case OrderStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

// Order class
class Order {
private:
    std::string orderId;
    std::vector<std::shared_ptr<CartItem>> items;
    OrderStatus status;
    std::string customerName;
    std::string shippingAddress;
    std::string billingAddress;
    std::string paymentMethod;
    float totalAmount;
    float shippingCost;
    float tax;
    std::string orderDate;
    std::string deliveryDate;
    
public:
    Order(const std::string& id, const std::vector<std::shared_ptr<CartItem>>& cartItems,
          const std::string& customer, const std::string& shippingAddr,
          const std::string& billingAddr, const std::string& payment,
          float shipping = 5.99f, float taxRate = 0.08f)
        : orderId(id), items(cartItems), status(OrderStatus::PENDING),
          customerName(customer), shippingAddress(shippingAddr),
          billingAddress(billingAddr), paymentMethod(payment),
          shippingCost(shipping) {
        
        // Calculate total amount
        float subtotal = 0.0f;
        for (const auto& item : items) {
            subtotal += item->getTotalPrice();
        }
        tax = subtotal * taxRate;
        totalAmount = subtotal + tax + shippingCost;
        
        // Set order date to current date (simplified)
        orderDate = "2025-11-12";
    }
    
    // Getters
    std::string getOrderId() const { return orderId; }
    OrderStatus getStatus() const { return status; }
    std::string getCustomerName() const { return customerName; }
    float getTotalAmount() const { return totalAmount; }
    std::string getOrderDate() const { return orderDate; }
    std::string getDeliveryDate() const { return deliveryDate; }
    
    // Setters
    void setStatus(OrderStatus newStatus) {
        status = newStatus;
        
        // Set delivery date when order is delivered
        if (newStatus == OrderStatus::DELIVERED && deliveryDate.empty()) {
            deliveryDate = "2025-11-15"; // Simplified
        }
    }
    
    void setDeliveryDate(const std::string& date) {
        deliveryDate = date;
    }
    
    // Order operations
    bool cancel() {
        if (status == OrderStatus::PENDING || status == OrderStatus::PROCESSING) {
            status = OrderStatus::CANCELLED;
            return true;
        }
        return false;
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "=== Order Details ===" << std::endl;
        std::cout << "Order ID: " << orderId << std::endl;
        std::cout << "Status: " << orderStatusToString(status) << std::endl;
        std::cout << "Customer: " << customerName << std::endl;
        std::cout << "Order Date: " << orderDate << std::endl;
        if (!deliveryDate.empty()) {
            std::cout << "Delivery Date: " << deliveryDate << std::endl;
        }
        
        std::cout << "\nItems:" << std::endl;
        for (const auto& item : items) {
            item->displayDetails();
        }
        
        std::cout << "\nAddress Information:" << std::endl;
        std::cout << "Shipping: " << shippingAddress << std::endl;
        std::cout << "Billing: " << billingAddress << std::endl;
        
        std::cout << "\nPayment Information:" << std::endl;
        std::cout << "Payment Method: " << paymentMethod << std::endl;
        
        std::cout << "\nCost Breakdown:" << std::endl;
        float subtotal = 0.0f;
        for (const auto& item : items) {
            subtotal += item->getTotalPrice();
        }
        std::cout << "Subtotal: $" << subtotal << std::endl;
        std::cout << "Tax: $" << tax << std::endl;
        std::cout << "Shipping: $" << shippingCost << std::endl;
        std::cout << "Total: $" << totalAmount << std::endl;
    }
    
    std::string getSummary() const {
        return orderId + ": " + customerName + " - " + orderStatusToString(status) + " - $" + std::to_string(totalAmount);
    }
};

// Customer class
class Customer {
private:
    std::string customerId;
    std::string name;
    std::string email;
    std::string phone;
    std::string defaultAddress;
    std::vector<std::shared_ptr<Order>> orders;
    
public:
    Customer(const std::string& id, const std::string& name, 
             const std::string& email, const std::string& phone,
             const std::string& address = "")
        : customerId(id), name(name), email(email), 
          phone(phone), defaultAddress(address) {}
    
    // Getters
    std::string getId() const { return customerId; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    std::string getPhone() const { return phone; }
    std::string getDefaultAddress() const { return defaultAddress; }
    std::vector<std::shared_ptr<Order>> getOrders() const { return orders; }
    
    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setEmail(const std::string& newEmail) { email = newEmail; }
    void setPhone(const std::string& newPhone) { phone = newPhone; }
    void setDefaultAddress(const std::string& address) { defaultAddress = address; }
    
    // Order management
    void addOrder(std::shared_ptr<Order> order) {
        orders.push_back(order);
    }
    
    std::shared_ptr<Order> findOrder(const std::string& orderId) {
        for (auto& order : orders) {
            if (order->getOrderId() == orderId) {
                return order;
            }
        }
        return nullptr;
    }
    
    // Display methods
    void displayDetails() const {
        std::cout << "=== Customer Details ===" << std::endl;
        std::cout << "Customer ID: " << customerId << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Email: " << email << std::endl;
        std::cout << "Phone: " << phone << std::endl;
        std::cout << "Default Address: " << defaultAddress << std::endl;
        
        std::cout << "\nOrder History (" << orders.size() << " orders):" << std::endl;
        for (const auto& order : orders) {
            std::cout << "- " << order->getSummary() << std::endl;
        }
    }
};

// E-commerce system class
class ECommerceSystem {
private:
    std::unordered_map<std::string, std::shared_ptr<Product>> products;
    std::unordered_map<std::string, std::shared_ptr<Customer>> customers;
    std::vector<std::shared_ptr<Order>> orders;
    
public:
    // Product management
    void addProduct(std::shared_ptr<Product> product) {
        if (product) {
            products[product->getId()] = product;
            std::cout << "Added product: " << product->getName() << std::endl;
        }
    }
    
    void removeProduct(const std::string& productId) {
        if (products.find(productId) != products.end()) {
            std::cout << "Removed product: " << products[productId]->getName() << std::endl;
            products.erase(productId);
        }
    }
    
    std::shared_ptr<Product> getProduct(const std::string& productId) {
        if (products.find(productId) != products.end()) {
            return products[productId];
        }
        return nullptr;
    }
    
    // Customer management
    void addCustomer(std::shared_ptr<Customer> customer) {
        if (customer) {
            customers[customer->getId()] = customer;
            std::cout << "Added customer: " << customer->getName() << std::endl;
        }
    }
    
    std::shared_ptr<Customer> getCustomer(const std::string& customerId) {
        if (customers.find(customerId) != customers.end()) {
            return customers[customerId];
        }
        return nullptr;
    }
    
    // Order processing
    std::shared_ptr<Order> createOrder(const std::string& customerId, 
                                      std::shared_ptr<ShoppingCart> cart,
                                      const std::string& shippingAddress,
                                      const std::string& paymentMethod) {
        auto customer = getCustomer(customerId);
        if (!customer || !cart || cart->isEmpty()) {
            return nullptr;
        }
        
        // Check stock availability
        if (!cart->checkStockAvailability()) {
            std::cout << "Cannot create order: Not enough stock for some items" << std::endl;
            return nullptr;
        }
        
        // Generate order ID (simplified)
        static int orderCounter = 1000;
        std::string orderId = "ORD" + std::to_string(orderCounter++);
        
        // Create order
        auto order = std::make_shared<Order>(orderId, cart->getItems(), 
                                            customer->getName(), shippingAddress,
                                            customer->getDefaultAddress(), paymentMethod);
        
        // Update stock levels
        for (const auto& item : cart->getItems()) {
            item->getProduct()->decreaseStock(item->getQuantity());
        }
        
        // Add order to system and customer
        orders.push_back(order);
        customer->addOrder(order);
        
        std::cout << "Created order: " << orderId << " for customer " << customer->getName() << std::endl;
        
        return order;
    }
    
    // Search functionality
    std::vector<std::shared_ptr<Product>> searchByName(const std::string& name) {
        std::vector<std::shared_ptr<Product>> results;
        
        for (const auto& pair : products) {
            if (pair.second->getName().find(name) != std::string::npos) {
                results.push_back(pair.second);
            }
        }
        
        return results;
    }
    
    std::vector<std::shared_ptr<Product>> searchByCategory(ProductCategory category) {
        std::vector<std::shared_ptr<Product>> results;
        
        for (const auto& pair : products) {
            if (pair.second->getCategory() == category) {
                results.push_back(pair.second);
            }
        }
        
        return results;
    }
    
    std::vector<std::shared_ptr<Product>> searchByPriceRange(float minPrice, float maxPrice) {
        std::vector<std::shared_ptr<Product>> results;
        
        for (const auto& pair : products) {
            float price = pair.second->getPrice();
            if (price >= minPrice && price <= maxPrice) {
                results.push_back(pair.second);
            }
        }
        
        return results;
    }
    
    // Display methods
    void displayAllProducts() const {
        std::cout << "=== All Products ===" << std::endl;
        for (const auto& pair : products) {
            std::cout << "- " << pair.second->getSummary() << std::endl;
        }
    }
    
    void displayAllCustomers() const {
        std::cout << "=== All Customers ===" << std::endl;
        for (const auto& pair : customers) {
            std::cout << "- " << pair.second->getId() << ": " << pair.second->getName() << std::endl;
        }
    }
    
    void displayAllOrders() const {
        std::cout << "=== All Orders ===" << std::endl;
        for (const auto& order : orders) {
            std::cout << "- " << order->getSummary() << std::endl;
        }
    }
};

int main() {
    // Create e-commerce system
    ECommerceSystem system;
    
    // Create products
    auto laptop = std::make_shared<Product>("P001", "Laptop Pro", "High-performance laptop", 
                                          ProductCategory::ELECTRONICS, 1299.99f, 50, "TechBrand");
    laptop->addTag("computer");
    laptop->addTag("portable");
    laptop->addRating(4);
    laptop->addRating(5);
    
    auto tshirt = std::make_shared<Product>("P002", "Cotton T-Shirt", "Comfortable cotton t-shirt", 
                                           ProductCategory::CLOTHING, 19.99f, 100, "FashionCo");
    tshirt->addTag("casual");
    tshirt->addTag("summer");
    tshirt->addRating(3);
    tshirt->addRating(4);
    
    auto book = std::make_shared<Product>("P003", "Programming Guide", "Learn to code", 
                                         ProductCategory::BOOKS, 29.99f, 30, "TechBooks");
    book->addTag("education");
    book->addTag("technology");
    book->addRating(5);
    book->addRating(4);
    book->addRating(5);
    
    // Add products to system
    system.addProduct(laptop);
    system.addProduct(tshirt);
    system.addProduct(book);
    
    // Create customers
    auto john = std::make_shared<Customer>("C001", "John Smith", "john@example.com", 
                                          "555-1234", "123 Main St, City, State");
    auto jane = std::make_shared<Customer>("C002", "Jane Doe", "jane@example.com", 
                                          "555-5678", "456 Oak Ave, City, State");
    
    // Add customers to system
    system.addCustomer(john);
    system.addCustomer(jane);
    
    // Create shopping carts
    auto johnCart = std::make_shared<ShoppingCart>();
    auto janeCart = std::make_shared<ShoppingCart>();
    
    // Add items to carts
    johnCart->addItem(laptop, 1);
    johnCart->addItem(book, 2);
    
    janeCart->addItem(tshirt, 3);
    janeCart->addItem(book, 1);
    
    // Apply discount to John's cart
    johnCart->setDiscount(10.0f);
    
    // Display cart details
    johnCart->displayDetails();
    std::cout << std::endl;
    
    janeCart->displayDetails();
    std::cout << std::endl;
    
    // Create orders
    auto johnOrder = system.createOrder("C001", johnCart, "123 Main St, City, State", "Credit Card");
    auto janeOrder = system.createOrder("C002", janeCart, "456 Oak Ave, City, State", "PayPal");
    
    // Update order status
    if (johnOrder) {
        johnOrder->setStatus(OrderStatus::PROCESSING);
        johnOrder->displayDetails();
    }
    
    // Search products
    std::cout << "\n=== Search Results ===" << std::endl;
    auto techProducts = system.searchByCategory(ProductCategory::ELECTRONICS);
    std::cout << "Electronics products:" << std::endl;
    for (const auto& product : techProducts) {
        std::cout << "- " << product->getName() << std::endl;
    }
    
    auto affordableProducts = system.searchByPriceRange(0.0f, 30.0f);
    std::cout << "\nAffordable products (under $30):" << std::endl;
    for (const auto& product : affordableProducts) {
        std::cout << "- " << product->getName() << " ($" << product->getPrice() << ")" << std::endl;
    }
    
    // Display system information
    <｜fim▁hole｜>system.displayAllOrders();
    
    return 0;
}