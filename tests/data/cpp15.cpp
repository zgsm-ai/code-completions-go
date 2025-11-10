#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <memory>
#include <cmath>
#include <queue>
#include <stack>
#include <limits>
#include <random>

using namespace std;

#define MAX_PRODUCTS 1000
#define MAX_ORDERS 2000
#define MAX_CUSTOMERS 500
#define MAX_CATEGORIES 50

struct date {
    int year;
    int month;
    int day;
};

class Customer;
class Order;
class OrderItem;
class Product;
class Category;

class Product {
private:
    int productId;
    string name;
    string description;
    double price;
    int quantity;
    string sku;
    int categoryId;
    string manufacturer;
    date addedDate;
    bool isActive;
    double discount;
    int reorderLevel;
    
public:
    Product(int productId = 0, const string& name = "", const string& description = "", 
            double price = 0.0, int quantity = 0, const string& sku = "", 
            int categoryId = 0, const string& manufacturer = "", 
            date addedDate = {0, 0, 0}, bool isActive = true, 
            double discount = 0.0, int reorderLevel = 10)
        : productId(productId), name(name), description(description), price(price), 
          quantity(quantity), sku(sku), categoryId(categoryId), manufacturer(manufacturer),
          addedDate(addedDate), isActive(isActive), discount(discount), reorderLevel(reorderLevel) {}
    
    int getProductId() const { return productId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    string getSku() const { return sku; }
    int getCategoryId() const { return categoryId; }
    string getManufacturer() const { return manufacturer; }
    date getAddedDate() const { return addedDate; }
    bool getIsActive() const { return isActive; }
    double getDiscount() const { return discount; }
    int getReorderLevel() const { return reorderLevel; }
    
    void setProductId(int id) { productId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setSku(const string& sku) { this->sku = sku; }
    void setCategoryId(int categoryId) { this->categoryId = categoryId; }
    void setManufacturer(const string& manufacturer) { this->manufacturer = manufacturer; }
    void setAddedDate(date date) { addedDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    void setDiscount(double discount) { this->discount = discount; }
    void setReorderLevel(int reorderLevel) { this->reorderLevel = reorderLevel; }
    
    double getEffectivePrice() const {
        return price * (1.0 - discount / 100.0);
    }
    
    bool needsReorder() const {
        return quantity <= reorderLevel;
    }
    
    bool isAvailable() const {
        return isActive && quantity > 0;
    }
    
    void display() const {
        cout << "Product ID: " << productId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Price: $" << fixed << setprecision(2) << price << endl;
        if (discount > 0.0) {
            cout << "Discount: " << discount << "%" << endl;
            cout << "Effective Price: $" << fixed << setprecision(2) << getEffectivePrice() << endl;
        }
        cout << "Quantity: " << quantity << endl;
        cout << "SKU: " << sku << endl;
        cout << "Category ID: " << categoryId << endl;
        cout << "Manufacturer: " << manufacturer << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = addedDate.year - 1900;
        tm.tm_mon = addedDate.month - 1;
        tm.tm_mday = addedDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Added Date: " << dateStr << endl;
        
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        
        if (needsReorder()) {
            cout << "WARNING: Product needs to be reordered!" << endl;
        }
        
        cout << "Reorder Level: " << reorderLevel << endl;
    }
};

class Category {
private:
    int categoryId;
    string name;
    string description;
    int parentId;
    bool isActive;
    
public:
    Category(int categoryId = 0, const string& name = "", const string& description = "", 
             int parentId = 0, bool isActive = true)
        : categoryId(categoryId), name(name), description(description), 
          parentId(parentId), isActive(isActive) {}
    
    int getCategoryId() const { return categoryId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    int getParentId() const { return parentId; }
    bool getIsActive() const { return isActive; }
    
    void setCategoryId(int id) { categoryId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setParentId(int parentId) { this->parentId = parentId; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    
    void display() const {
        cout << "Category ID: " << categoryId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Parent Category ID: " << (parentId == 0 ? "None (Root)" : to_string(parentId)) << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
    }
};

class Customer {
private:
    int customerId;
    string firstName;
    string lastName;
    string email;
    string phone;
    string address;
    date registrationDate;
    bool isActive;
    double totalPurchases;
    string membershipLevel;
    int loyaltyPoints;
    
public:
    Customer(int customerId = 0, const string& firstName = "", const string& lastName = "", 
             const string& email = "", const string& phone = "", const string& address = "",
             date registrationDate = {0, 0, 0}, bool isActive = true,
             double totalPurchases = 0.0, const string& membershipLevel = "Bronze", 
             int loyaltyPoints = 0)
        : customerId(customerId), firstName(firstName), lastName(lastName), email(email), 
          phone(phone), address(address), registrationDate(registrationDate),
          isActive(isActive), totalPurchases(totalPurchases), 
          membershipLevel(membershipLevel), loyaltyPoints(loyaltyPoints) {}
    
    int getCustomerId() const { return customerId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    date getRegistrationDate() const { return registrationDate; }
    bool getIsActive() const { return isActive; }
    double getTotalPurchases() const { return totalPurchases; }
    string getMembershipLevel() const { return membershipLevel; }
    int getLoyaltyPoints() const { return loyaltyPoints; }
    
    void setCustomerId(int id) { customerId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setAddress(const string& address) { this->address = address; }
    void setRegistrationDate(date date) { registrationDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    void setTotalPurchases(double totalPurchases) { this->totalPurchases = totalPurchases; }
    void setMembershipLevel(const string& membershipLevel) { this->membershipLevel = membershipLevel; }
    void setLoyaltyPoints(int points) { loyaltyPoints = points; }
    
    void addPurchase(double amount) {
        totalPurchases += amount;
        
        // Update membership level based on total purchases
        if (totalPurchases >= 5000.0) {
            membershipLevel = "Platinum";
        } else if (totalPurchases >= 2500.0) {
            membershipLevel = "Gold";
        } else if (totalPurchases >= 1000.0) {
            membershipLevel = "Silver";
        } else {
            membershipLevel = "Bronze";
        }
        
        // Add loyalty points (1 point per dollar)
        loyaltyPoints += static_cast<int>(amount);
    }
    
    void redeemPoints(int points) {
        if (points <= loyaltyPoints) {
            loyaltyPoints -= points;
        }
    }
    
    void display() const {
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address << endl;
        
        // Format date
        tm tm = {};
        tm.tm_year = registrationDate.year - 1900;
        tm.tm_mon = registrationDate.month - 1;
        tm.tm_mday = registrationDate.day;
        
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Registration Date: " << dateStr << endl;
        
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Total Purchases: $" << fixed << setprecision(2) << totalPurchases << endl;
        cout << "Membership Level: " << membershipLevel << endl;
        cout << "Loyalty Points: " << loyaltyPoints << endl;
    }
};

class OrderItem {
private:
    int orderItemId;
    int orderId;
    int productId;
    int quantity;
    double unitPrice;
    double discount;
    
public:
    OrderItem(int orderItemId = 0, int orderId = 0, int productId = 0, 
              int quantity = 0, double unitPrice = 0.0, double discount = 0.0)
        : orderItemId(orderItemId), orderId(orderId), productId(productId), 
          quantity(quantity), unitPrice(unitPrice), discount(discount) {}
    
    int getOrderItemId() const { return orderItemId; }
    int getOrderId() const { return orderId; }
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    double getDiscount() const { return discount; }
    
    void setOrderItemId(int id) { orderItemId = id; }
    void setOrderId(int orderId) { this->orderId = orderId; }
    void setProductId(int productId) { this->productId = productId; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
    void setDiscount(double discount) { this->discount = discount; }
    
    double getSubtotal() const {
        return quantity * unitPrice * (1.0 - discount / 100.0);
    }
    
    void display() const {
        cout << "Order Item ID: " << orderItemId << endl;
        cout << "Order ID: " << orderId << endl;
        cout << "Product ID: " << productId << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Unit Price: $" << fixed << setprecision(2) << unitPrice << endl;
        cout << "Discount: " << discount << "%" << endl;
        cout << "Subtotal: $" << fixed << setprecision(2) << getSubtotal() << endl;
    }
};

class Order {
private:
    int orderId;
    int customerId;
    date orderDate;
    date shippingDate;
    date deliveryDate;
    string status;
    string shippingAddress;
    string paymentMethod;
    double totalAmount;
    double shippingCost;
    double tax;
    vector<OrderItem> orderItems;
    
public:
    Order(int orderId = 0, int customerId = 0, date orderDate = {0, 0, 0}, 
          date shippingDate = {0, 0, 0}, date deliveryDate = {0, 0, 0},
          const string& status = "Pending", const string& shippingAddress = "", 
          const string& paymentMethod = "", double totalAmount = 0.0, 
          double shippingCost = 0.0, double tax = 0.0)
        : orderId(orderId), customerId(customerId), orderDate(orderDate), 
          shippingDate(shippingDate), deliveryDate(deliveryDate), status(status), 
          shippingAddress(shippingAddress), paymentMethod(paymentMethod), 
          totalAmount(totalAmount), shippingCost(shippingCost), tax(tax) {}
    
    int getOrderId() const { return orderId; }
    int getCustomerId() const { return customerId; }
    date getOrderDate() const { return orderDate; }
    date getShippingDate() const { return shippingDate; }
    date getDeliveryDate() const { return deliveryDate; }
    string getStatus() const { return status; }
    string getShippingAddress() const { return shippingAddress; }
    string getPaymentMethod() const { return paymentMethod; }
    double getTotalAmount() const { return totalAmount; }
    double getShippingCost() const { return shippingCost; }
    double getTax() const { return tax; }
    vector<OrderItem> getOrderItems() const { return orderItems; }
    
    void setOrderId(int id) { orderId = id; }
    void setCustomerId(int customerId) { this->customerId = customerId; }
    void setOrderDate(date date) { orderDate = date; }
    void setShippingDate(date date) { shippingDate = date; }
    void setDeliveryDate(date date) { deliveryDate = date; }
    void setStatus(const string& status) { this->status = status; }
    void setShippingAddress(const string& address) { shippingAddress = address; }
    void setPaymentMethod(const string& method) { paymentMethod = method; }
    void setTotalAmount(double amount) { totalAmount = amount; }
    void setShippingCost(double cost) { shippingCost = cost; }
    void setTax(double tax) { this->tax = tax; }
    void setOrderItems(const vector<OrderItem>& items) { orderItems = items; }
    
    void addOrderItem(const OrderItem& item) {
        orderItems.push_back(item);
        calculateTotal();
    }
    
    void calculateTotal() {
        double subtotal = 0.0;
        for (const auto& item : orderItems) {
            subtotal += item.getSubtotal();
        }
        
        tax = subtotal * 0.08; // 8% tax
        shippingCost = (subtotal > 100.0) ? 0.0 : 10.0; // Free shipping for orders over $100
        totalAmount = subtotal + tax + shippingCost;
    }
    
    void display() const {
        cout << "Order ID: " << orderId << endl;
        cout << "Customer ID: " << customerId << endl;
        
        // Format dates
        tm tm = {};
        
        tm.tm_year = orderDate.year - 1900;
        tm.tm_mon = orderDate.month - 1;
        tm.tm_mday = orderDate.day;
        char dateStr[100];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
        cout << "Order Date: " << dateStr << endl;
        
        if (shippingDate.year > 0) {
            tm.tm_year = shippingDate.year - 1900;
            tm.tm_mon = shippingDate.month - 1;
            tm.tm_mday = shippingDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Shipping Date: " << dateStr << endl;
        }
        
        if (deliveryDate.year > 0) {
            tm.tm_year = deliveryDate.year - 1900;
            tm.tm_mon = deliveryDate.month - 1;
            tm.tm_mday = deliveryDate.day;
            strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &tm);
            cout << "Delivery Date: " << dateStr << endl;
        }
        
        cout << "Status: " << status << endl;
        cout << "Shipping Address: " << shippingAddress << endl;
        cout << "Payment Method: " << paymentMethod << endl;
        cout << "Order Items:" << endl;
        
        for (const auto& item : orderItems) {
            cout << "  Product ID: " << item.getProductId() 
                 << ", Quantity: " << item.getQuantity() 
                 << ", Unit Price: $" << fixed << setprecision(2) << item.getUnitPrice()
                 << ", Subtotal: $" << fixed << setprecision(2) << item.getSubtotal() << endl;
        }
        
        cout << "Subtotal: $" << fixed << setprecision(2) << (totalAmount - tax - shippingCost) << endl;
        cout << "Tax: $" << fixed << setprecision(2) << tax << endl;
        cout << "Shipping Cost: $" << fixed << setprecision(2) << shippingCost << endl;
        cout << "Total Amount: $" << fixed << setprecision(2) << totalAmount << endl;
    }
};

class InventorySystem {
private:
    vector<Product> products;
    vector<Category> categories;
    vector<Customer> customers;
    vector<Order> orders;
    vector<OrderItem> orderItems;
    
    int nextProductId;
    int nextCategoryId;
    int nextCustomerId;
    int nextOrderId;
    int nextOrderItemId;
    
public:
    InventorySystem() {
        nextProductId = 1;
        nextCategoryId = 1;
        nextCustomerId = 1;
        nextOrderId = 1;
        nextOrderItemId = 1;
    }
    
    // Product management
    Product* findProduct(int productId) {
        for (auto& product : products) {
            if (product.getProductId() == productId) {
                return &product;
            }
        }
        return nullptr;
    }
    
    bool addProduct(const Product& product) {
        if (products.size() >= MAX_PRODUCTS) {
            return false;
        }
        
        products.push_back(product);
        return true;
    }
    
    bool removeProduct(int productId) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->getProductId() == productId) {
                products.erase(it);
                return true;
            }
        }
        return false;
    }
    
    bool updateProductQuantity(int productId, int quantityChange) {
        Product* product = findProduct(productId);
        if (product) {
            int newQuantity = product->getQuantity() + quantityChange;
            if (newQuantity >= 0) {
                product->setQuantity(newQuantity);
                return true;
            }
        }
        return false;
    }
    
    vector<Product*> getAllProducts() {
        vector<Product*> result;
        for (auto& product : products) {
            result.push_back(&product);
        }
        return result;
    }
    
    vector<Product*> searchProductsByName(const string& name) {
        vector<Product*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& product : products) {
            string productName = product.getName();
            transform(productName.begin(), productName.end(), productName.begin(), ::tolower);
            
            if (productName.find(lowerName) != string::npos) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    vector<Product*> searchProductsByCategory(int categoryId) {
        vector<Product*> result;
        for (auto& product : products) {
            if (product.getCategoryId() == categoryId) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    vector<Product*> getProductsNeedingReorder() {
        vector<Product*> result;
        for (auto& product : products) {
            if (product.needsReorder() && product.getIsActive()) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    vector<Product*> getAvailableProducts() {
        vector<Product*> result;
        for (auto& product : products) {
            if (product.isAvailable()) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    int getNextProductId() { return nextProductId++; }
    
    // Category management
    Category* findCategory(int categoryId) {
        for (auto& category : categories) {
            if (category.getCategoryId() == categoryId) {
                return &category;
            }
        }
        return nullptr;
    }
    
    bool addCategory(const Category& category) {
        if (categories.size() >= MAX_CATEGORIES) {
            return false;
        }
        
        categories.push_back(category);
        return true;
    }
    
    bool removeCategory(int categoryId) {
        for (auto it = categories.begin(); it != categories.end(); ++it) {
            if (it->getCategoryId() == categoryId) {
                categories.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Category*> getAllCategories() {
        vector<Category*> result;
        for (auto& category : categories) {
            result.push_back(&category);
        }
        return result;
    }
    
    int getNextCategoryId() { return nextCategoryId++; }
    
    // Customer management
    Customer* findCustomer(int customerId) {
        for (auto& customer : customers) {
            if (customer.getCustomerId() == customerId) {
                return &customer;
            }
        }
        return nullptr;
    }
    
    bool addCustomer(const Customer& customer) {
        if (customers.size() >= MAX_CUSTOMERS) {
            return false;
        }
        
        customers.push_back(customer);
        return true;
    }
    
    bool removeCustomer(int customerId) {
        for (auto it = customers.begin(); it != customers.end(); ++it) {
            if (it->getCustomerId() == customerId) {
                customers.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Customer*> getAllCustomers() {
        vector<Customer*> result;
        for (auto& customer : customers) {
            result.push_back(&customer);
        }
        return result;
    }
    
    vector<Customer*> searchCustomersByName(const string& name) {
        vector<Customer*> result;
        string lowerName = name;
        transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        
        for (auto& customer : customers) {
            string fullName = customer.getFullName();
            transform(fullName.begin(), fullName.end(), fullName.begin(), ::tolower);
            
            if (fullName.find(lowerName) != string::npos) {
                result.push_back(&customer);
            }
        }
        return result;
    }
    
    int getNextCustomerId() { return nextCustomerId++; }
    
    // Order management
    Order* findOrder(int orderId) {
        for (auto& order : orders) {
            if (order.getOrderId() == orderId) {
                return &order;
            }
        }
        return nullptr;
    }
    
    bool addOrder(const Order& order) {
        if (orders.size() >= MAX_ORDERS) {
            return false;
        }
        
        orders.push_back(order);
        return true;
    }
    
    bool removeOrder(int orderId) {
        for (auto it = orders.begin(); it != orders.end(); ++it) {
            if (it->getOrderId() == orderId) {
                orders.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Order*> getAllOrders() {
        vector<Order*> result;
        for (auto& order : orders) {
            result.push_back(&order);
        }
        return result;
    }
    
    vector<Order*> getOrdersByCustomer(int customerId) {
        vector<Order*> result;
        for (auto& order : orders) {
            if (order.getCustomerId() == customerId) {
                result.push_back(&order);
            }
        }
        return result;
    }
    
    vector<Order*> getOrdersByStatus(const string& status) {
        vector<Order*> result;
        for (auto& order : orders) {
            if (order.getStatus() == status) {
                result.push_back(&order);
            }
        }
        return result;
    }
    
    int getNextOrderId() { return nextOrderId++; }
    
    // OrderItem management
    OrderItem* findOrderItem(int orderItemId) {
        for (auto& orderItem : orderItems) {
            if (orderItem.getOrderItemId() == orderItemId) {
                return &orderItem;
            }
        }
        return nullptr;
    }
    
    bool addOrderItem(const OrderItem& orderItem) {
        if (orderItems.size() >= MAX_ORDERS * 10) { // Assume max 10 items per order
            return false;
        }
        
        orderItems.push_back(orderItem);
        return true;
    }
    
    vector<OrderItem*> getOrderItemsByOrder(int orderId) {
        vector<OrderItem*> result;
        for (auto& orderItem : orderItems) {
            if (orderItem.getOrderId() == orderId) {
                result.push_back(&orderItem);
            }
        }
        return result;
    }
    
    vector<OrderItem*> getOrderItemsByProduct(int productId) {
        vector<OrderItem*> result;
        for (auto& orderItem : orderItems) {
            if (orderItem.getProductId() == productId) {
                result.push_back(&orderItem);
            }
        }
        return result;
    }
    
    int getNextOrderItemId() { return nextOrderItemId++; }
    
    // Business operations
    bool createOrder(int customerId, const string& shippingAddress, const string& paymentMethod,
                   const vector<pair<int, int>>& productQuantities, int& newOrderId) {
        // Check if customer exists
        Customer* customer = findCustomer(customerId);
        if (!customer) {
            return false;
        }
        
        // Create order
        newOrderId = getNextOrderId();
        date today;
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        today.year = timeinfo->tm_year + 1900;
        today.month = timeinfo->tm_mon + 1;
        today.day = timeinfo->tm_mday;
        
        Order order(newOrderId, customerId, today);
        order.setShippingAddress(shippingAddress);
        order.setPaymentMethod(paymentMethod);
        
        // Add order items and update inventory
        for (const auto& pq : productQuantities) {
            int productId = pq.first;
            int quantity = pq.second;
            
            Product* product = findProduct(productId);
            if (!product || !product->isAvailable() || product->getQuantity() < quantity) {
                return false; // Product not available or insufficient quantity
            }
            
            // Create order item
            OrderItem orderItem(getNextOrderItemId(), newOrderId, productId, 
                               quantity, product->getPrice(), product->getDiscount());
            
            order.addOrderItem(orderItem);
            addOrderItem(orderItem);
            
            // Update product quantity
            updateProductQuantity(productId, -quantity);
        }
        
        // Calculate total
        order.calculateTotal();
        
        // Add order to system
        if (!addOrder(order)) {
            return false;
        }
        
        // Update customer's purchase history
        customer->addPurchase(order.getTotalAmount());
        
        return true;
    }
    
    bool processOrder(int orderId) {
        Order* order = findOrder(orderId);
        if (!order || order->getStatus() != "Pending") {
            return false;
        }
        
        // Get current date
        date today;
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        today.year = timeinfo->tm_year + 1900;
        today.month = timeinfo->tm_mon + 1;
        today.day = timeinfo->tm_mday;
        
        // Set shipping date
        order->setShippingDate(today);
        order->setStatus("Shipped");
        
        // Set estimated delivery date (5 days from now)
        timeinfo->tm_mday += 5;
        mktime(timeinfo); // Normalize the date
        date deliveryDate;
        deliveryDate.year = timeinfo->tm_year + 1900;
        deliveryDate.month = timeinfo->tm_mon + 1;
        deliveryDate.day = timeinfo->tm_mday;
        
        order->setDeliveryDate(deliveryDate);
        
        return true;
    }
    
    bool completeOrder(int orderId) {
        Order* order = findOrder(orderId);
        if (!order || order->getStatus() != "Shipped") {
            return false;
        }
        
        order->setStatus("Completed");
        
        // Get current date
        date today;
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        today.year = timeinfo->tm_year + 1900;
        today.month = timeinfo->tm_mon + 1;
        today.day = timeinfo->tm_mday;
        
        order->setDeliveryDate(today);
        
        return true;
    }
    
    bool cancelOrder(int orderId) {
        Order* order = findOrder(orderId);
        if (!order || (order->getStatus() != "Pending" && order->getStatus() != "Shipped")) {
            return false;
        }
        
        // Return products to inventory
        vector<OrderItem*> items = getOrderItemsByOrder(orderId);
        for (OrderItem* item : items) {
            updateProductQuantity(item->getProductId(), item->getQuantity());
        }
        
        order->setStatus("Cancelled");
        
        return true;
    }
    
    // Reporting
    void generateSalesReport(const string& startDate, const string& endDate) {
        cout << "Sales Report" << endl;
        cout << "============" << endl;
        cout << "Period: " << startDate << " to " << endDate << endl;
        cout << endl;
        
        // Parse dates (simplified)
        int startYear, startMonth, startDay;
        int endYear, endMonth, endDay;
        
        sscanf(startDate.c_str(), "%d-%d-%d", &startYear, &startMonth, &startDay);
        sscanf(endDate.c_str(), "%d-%d-%d", &endYear, &endMonth, &endDay);
        
        double totalSales = 0.0;
        int totalOrders = 0;
        map<string, int> productSales;
        
        for (const auto& order : orders) {
            if (order.getStatus() == "Completed") {
                date orderDate = order.getOrderDate();
                
                // Check if order is within the date range
                if ((orderDate.year > startYear || 
                     (orderDate.year == startYear && orderDate.month > startMonth) ||
                     (orderDate.year == startYear && orderDate.month == startMonth && orderDate.day >= startDay)) &&
                    (orderDate.year < endYear || 
                     (orderDate.year == endYear && orderDate.month < endMonth) ||
                     (orderDate.year == endYear && orderDate.month == endMonth && orderDate.day <= endDay))) {
                    
                    totalSales += order.getTotalAmount();
                    totalOrders++;
                    
                    // Count product sales
                    vector<OrderItem*> items = getOrderItemsByOrder(order.getOrderId());
                    for (OrderItem* item : items) {
                        Product* product = findProduct(item->getProductId());
                        if (product) {
                            productSales[product->getName()] += item->getQuantity();
                        }
                    }
                }
            }
        }
        
        cout << "Total Sales: $" << fixed << setprecision(2) << totalSales << endl;
        cout << "Total Orders: " << totalOrders << endl;
        
        if (totalOrders > 0) {
            cout << "Average Order Value: $" << fixed << setprecision(2) << (totalSales / totalOrders) << endl;
        } else {
            cout << "Average Order Value: $0.00" << endl;
        }
        
        cout << endl;
        cout << "Top Selling Products:" << endl;
        
        // Sort products by sales
        vector<pair<string, int>> sortedProducts(productSales.begin(), productSales.end());
        sort(sortedProducts.begin(), sortedProducts.end(), 
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        int displayCount = min(5, static_cast<int>(sortedProducts.size()));
        for (int i = 0; i < displayCount; i++) {
            cout << (i + 1) << ". " << sortedProducts[i].first << ": " << sortedProducts[i].second << " units" << endl;
        }
    }
    
    void generateInventoryReport() {
        cout << "Inventory Report" << endl;
        cout << "===============" << endl;
        cout << endl;
        
        int totalProducts = 0;
        int totalQuantity = 0;
        double totalValue = 0.0;
        int activeProducts = 0;
        int outOfStockProducts = 0;
        int lowStockProducts = 0;
        
        vector<Product*> reorderProducts = getProductsNeedingReorder();
        
        for (const auto& product : products) {
            totalProducts++;
            totalQuantity += product.getQuantity();
            totalValue += product.getQuantity() * product.getEffectivePrice();
            
            if (product.getIsActive()) {
                activeProducts++;
            }
            
            if (product.getQuantity() == 0) {
                outOfStockProducts++;
            }
            
            if (product.needsReorder()) {
                lowStockProducts++;
            }
        }
        
        cout << "Total Products: " << totalProducts << endl;
        cout << "Active Products: " << activeProducts << endl;
        cout << "Total Quantity in Stock: " << totalQuantity << endl;
        cout << "Total Inventory Value: $" << fixed << setprecision(2) << totalValue << endl;
        cout << "Out of Stock Products: " << outOfStockProducts << endl;
        cout << "Products Needing Reorder: " << lowStockProducts << endl;
        cout << endl;
        
        if (!reorderProducts.empty()) {
            cout << "Products Requiring Reorder:" << endl;
            for (Product* product : reorderProducts) {
                cout << "  ID: " << product->getProductId() 
                     << ", Name: " << product->getName() 
                     << ", Current Quantity: " << product->getQuantity()
                     << ", Reorder Level: " << product->getReorderLevel() << endl;
            }
        }
    }
    
    void generateCustomerReport() {
        cout << "Customer Report" << endl;
        cout << "===============" << endl;
        cout << endl;
        
        int totalCustomers = static_cast<int>(customers.size());
        int activeCustomers = 0;
        double totalPurchases = 0.0;
        int totalLoyaltyPoints = 0;
        map<string, int> membershipCounts;
        
        for (const auto& customer : customers) {
            if (customer.getIsActive()) {
                activeCustomers++;
            }
            
            totalPurchases += customer.getTotalPurchases();
            totalLoyaltyPoints += customer.getLoyaltyPoints();
            membershipCounts[customer.getMembershipLevel()]++;
        }
        
        cout << "Total Customers: " << totalCustomers << endl;
        cout << "Active Customers: " << activeCustomers << endl;
        cout << "Total Purchases: $" << fixed << setprecision(2) << totalPurchases << endl;
        
        if (totalCustomers > 0) {
            cout << "Average Customer Value: $" << fixed << setprecision(2) << (totalPurchases / totalCustomers) << endl;
        } else {
            cout << "Average Customer Value: $0.00" << endl;
        }
        
        cout << "Total Loyalty Points Issued: " << totalLoyaltyPoints << endl;
        cout << endl;
        
        cout << "Membership Distribution:" << endl;
        for (const auto& pair : membershipCounts) {
            cout << "  " << pair.first << ": " << pair.second << " customers" << endl;
        }
        
        // Find top customers
        vector<Customer> sortedCustomers(customers);
        sort(sortedCustomers.begin(), sortedCustomers.end(), 
             [](const Customer& a, const Customer& b) {
                 return a.getTotalPurchases() > b.getTotalPurchases();
             });
        
        cout << endl;
        cout << "Top Customers:" << endl;
        
        int displayCount = min(5, static_cast<int>(sortedCustomers.size()));
        for (int i = 0; i < displayCount; i++) {
            cout << (i + 1) << ". " << sortedCustomers[i].getFullName() 
                 << ": $" << fixed << setprecision(2) << sortedCustomers[i].getTotalPurchases() << endl;
        }
    }
    
    // File operations
    bool saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (!outFile) {
            return false;
        }
        
        // Save next IDs
        outFile << "NEXT_IDS" << endl;
        outFile << nextProductId << "|" << nextCategoryId << "|" << nextCustomerId << "|" 
                << nextOrderId << "|" << nextOrderItemId << endl;
        
        // Save products
        outFile << "PRODUCTS" << endl;
        for (const auto& product : products) {
            outFile << product.getProductId() << "|" 
                    << product.getName() << "|" 
                    << product.getDescription() << "|" 
                    << product.getPrice() << "|" 
                    << product.getQuantity() << "|" 
                    << product.getSku() << "|" 
                    << product.getCategoryId() << "|" 
                    << product.getManufacturer() << "|" 
                    << product.getAddedDate().year << "|" 
                    << product.getAddedDate().month << "|" 
                    << product.getAddedDate().day << "|" 
                    << (product.getIsActive() ? "1" : "0") << "|" 
                    << product.getDiscount() << "|" 
                    << product.getReorderLevel() << endl;
        }
        
        // Save categories
        outFile << "CATEGORIES" << endl;
        for (const auto& category : categories) {
            outFile << category.getCategoryId() << "|" 
                    << category.getName() << "|" 
                    << category.getDescription() << "|" 
                    << category.getParentId() << "|" 
                    << (category.getIsActive() ? "1" : "0") << endl;
        }
        
        // Save customers
        outFile << "CUSTOMERS" << endl;
        for (const auto& customer : customers) {
            outFile << customer.getCustomerId() << "|" 
                    << customer.getFirstName() << "|" 
                    << customer.getLastName() << "|" 
                    << customer.getEmail() << "|" 
                    << customer.getPhone() << "|" 
                    << customer.getAddress() << "|" 
                    << customer.getRegistrationDate().year << "|" 
                    << customer.getRegistrationDate().month << "|" 
                    << customer.getRegistrationDate().day << "|" 
                    << (customer.getIsActive() ? "1" : "0") << "|" 
                    << customer.getTotalPurchases() << "|" 
                    << customer.getMembershipLevel() << "|" 
                    << customer.getLoyaltyPoints() << endl;
        }
        
        outFile.close();
        return true;
    }
    
    bool loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (!inFile) {
            return false;
        }
        
        string line;
        string section = "";
        
        while (getline(inFile, line)) {
            if (line == "NEXT_IDS") {
                section = "NEXT_IDS";
                continue;
            } else if (line == "PRODUCTS") {
                section = "PRODUCTS";
                continue;
            } else if (line == "CATEGORIES") {
                section = "CATEGORIES";
                continue;
            } else if (line == "CUSTOMERS") {
                section = "CUSTOMERS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 5) {
                nextProductId = stoi(tokens[0]);
                nextCategoryId = stoi(tokens[1]);
                nextCustomerId = stoi(tokens[2]);
                nextOrderId = stoi(tokens[3]);
                nextOrderItemId = stoi(tokens[4]);
            } else if (section == "PRODUCTS" && tokens.size() >= 14) {
                int productId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                double price = stod(tokens[3]);
                int quantity = stoi(tokens[4]);
                string sku = tokens[5];
                int categoryId = stoi(tokens[6]);
                string manufacturer = tokens[7];
                
                date addedDate;
                addedDate.year = stoi(tokens[8]);
                addedDate.month = stoi(tokens[9]);
                addedDate.day = stoi(tokens[10]);
                
                bool isActive = (tokens[11] == "1");
                double discount = stod(tokens[12]);
                int reorderLevel = stoi(tokens[13]);
                
                Product product(productId, name, description, price, quantity, sku, 
                              categoryId, manufacturer, addedDate, isActive, discount, reorderLevel);
                
                products.push_back(product);
            } else if (section == "CATEGORIES" && tokens.size() >= 5) {
                int categoryId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                int parentId = stoi(tokens[3]);
                bool isActive = (tokens[4] == "1");
                
                Category category(categoryId, name, description, parentId, isActive);
                categories.push_back(category);
            } else if (section == "CUSTOMERS" && tokens.size() >= 14) {
                int customerId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string phone = tokens[4];
                string address = tokens[5];
                
                date registrationDate;
                registrationDate.year = stoi(tokens[6]);
                registrationDate.month = stoi(tokens[7]);
                registrationDate.day = stoi(tokens[8]);
                
                bool isActive = (tokens[9] == "1");
                double totalPurchases = stod(tokens[10]);
                string membershipLevel = tokens[11];
                int loyaltyPoints = stoi(tokens[12]);
                
                Customer customer(customerId, firstName, lastName, email, phone, address,
                               registrationDate, isActive, totalPurchases, membershipLevel, loyaltyPoints);
                
                customers.push_back(customer);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        products.clear();
        categories.clear();
        customers.clear();
        orders.clear();
        orderItems.clear();
        
        nextProductId = 1;
        nextCategoryId = 1;
        nextCustomerId = 1;
        nextOrderId = 1;
        nextOrderItemId = 1;
        
        // Create sample categories
        vector<Category> sampleCategories = {
            Category(getNextCategoryId(), "Electronics", "Electronic devices and accessories", 0),
            Category(getNextCategoryId(), "Computers", "Laptops, desktops, and computer accessories", 1),
            Category(getNextCategoryId(), "Smartphones", "Mobile phones and accessories", 1),
            Category(getNextCategoryId(), "Clothing", "Apparel and fashion items", 0),
            Category(getNextCategoryId(), "Men's Clothing", "Clothing for men", 4),
            Category(getNextCategoryId(), "Women's Clothing", "Clothing for women", 4)
        };
        
        for (const auto& category : sampleCategories) {
            addCategory(category);
        }
        
        // Create sample products
        vector<Product> sampleProducts = {
            Product(getNextProductId(), "Laptop Pro 15", "High-performance laptop with 15-inch display", 1299.99, 50, "LP15-001", 2, "TechBrand", {2023, 1, 15}, true, 5.0, 10),
            Product(getNextProductId(), "Smartphone X", "Latest smartphone with advanced camera", 799.99, 120, "SX-2023", 3, "TechBrand", {2023, 2, 10}, true, 0.0, 20),
            Product(getNextProductId(), "Wireless Headphones", "Noise-cancelling wireless headphones", 199.99, 80, "WH-NC", 1, "AudioBrand", {2023, 1, 20}, true, 10.0, 15),
            Product(getNextProductId(), "Men's T-Shirt", "Comfortable cotton t-shirt", 24.99, 200, "MTS-001", 5, "ClothingCo", {2023, 3, 5}, true, 20.0, 50),
            Product(getNextProductId(), "Women's Jeans", "Stylish denim jeans", 59.99, 150, "WJN-002", 6, "ClothingCo", {2023, 2, 15}, true, 15.0, 30),
            Product(getNextProductId(), "Tablet Pro", "10-inch tablet with stylus", 549.99, 40, "TP-10", 2, "TechBrand", {2023, 3, 1}, true, 0.0, 10),
            Product(getNextProductId(), "Smartwatch", "Fitness and health tracking smartwatch", 299.99, 60, "SW-FIT", 3, "TechBrand", {2023, 1, 25}, true, 5.0, 15),
            Product(getNextProductId(), "Gaming Mouse", "High-precision gaming mouse", 79.99, 5, "GM-PRO", 2, "GamingBrand", {2023, 3, 10}, true, 0.0, 10)
        };
        
        for (const auto& product : sampleProducts) {
            addProduct(product);
        }
        
        // Create sample customers
        vector<Customer> sampleCustomers = {
            Customer(getNextCustomerId(), "John", "Doe", "john.doe@example.com", "555-1234", "123 Main St", {2022, 1, 15}, true, 1250.50, "Silver", 1250),
            Customer(getNextCustomerId(), "Jane", "Smith", "jane.smith@example.com", "555-5678", "456 Oak Ave", {2022, 2, 20}, true, 3450.75, "Gold", 3450),
            Customer(getNextCustomerId(), "Bob", "Johnson", "bob.johnson@example.com", "555-9012", "789 Pine Rd", {2022, 3, 10}, true, 800.25, "Bronze", 800),
            Customer(getNextCustomerId(), "Alice", "Williams", "alice.williams@example.com", "555-3456", "321 Elm St", {2022, 4, 5}, true, 5200.00, "Gold", 5200),
            Customer(getNextCustomerId(), "Charlie", "Brown", "charlie.brown@example.com", "555-7890", "654 Maple Dr", {2022, 5, 12}, true, 150.00, "Bronze", 150)
        };
        
        for (const auto& customer : sampleCustomers) {
            addCustomer(customer);
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Categories: " << categories.size() << endl;
        cout << "Products: " << products.size() << endl;
        cout << "Customers: " << customers.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(InventorySystem& system, int choice);
void productMenu(InventorySystem& system);
void categoryMenu(InventorySystem& system);
void customerMenu(InventorySystem& system);
void orderMenu(InventorySystem& system);
void reportMenu(InventorySystem& system);
void fileMenu(InventorySystem& system);

int main() {
    InventorySystem system;
    
    cout << "Inventory Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("inventory_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("inventory_data.txt");
        cout << "Sample data generated and saved." << endl;
    } else {
        cout << "Existing data loaded successfully." << endl;
    }
    
    int choice;
    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        handleMenuChoice(system, choice);
        
        cout << endl;
        system("pause");
        system("cls");
    } while (choice != 0);
    
    // Save data before exiting
    system.saveToFile("inventory_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "Inventory Management System" << endl;
    cout << "1. Product Management" << endl;
    cout << "2. Category Management" << endl;
    cout << "3. Customer Management" << endl;
    cout << "4. Order Management" << endl;
    cout << "5. Reports" << endl;
    cout << "6. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(InventorySystem& system, int choice) {
    switch (choice) {
        case 1:
            productMenu(system);
            break;
        case 2:
            categoryMenu(system);
            break;
        case 3:
            customerMenu(system);
            break;
        case 4:
            orderMenu(system);
            break;
        case 5:
            reportMenu(system);
            break;
        case 6:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void productMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "Product Management" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Remove Product" << endl;
        cout << "3. Update Product Quantity" << endl;
        cout << "4. List All Products" << endl;
        cout << "5. Search Products by Name" << endl;
        cout << "6. Search Products by Category" << endl;
        cout << "7. List Products Needing Reorder" << endl;
        cout << "8. List Available Products" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description, sku, manufacturer;
                double price, discount;
                int quantity, categoryId, reorderLevel;
                
                cout << "Enter Product Name: ";
                getline(cin, name);
                
                cout << "Enter Product Description: ";
                getline(cin, description);
                
                cout << "Enter Price: ";
                cin >> price;
                
                cout << "Enter Quantity: ";
                cin >> quantity;
                
                cout << "Enter SKU: ";
                cin >> sku;
                
                cout << "Enter Category ID: ";
                cin >> categoryId;
                
                cout << "Enter Manufacturer: ";
                cin >> manufacturer;
                
                cout << "Enter Discount (%): ";
                cin >> discount;
                
                cout << "Enter Reorder Level: ";
                cin >> reorderLevel;
                
                // Get current date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date addedDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                Product product(system.getNextProductId(), name, description, price, quantity, 
                               sku, categoryId, manufacturer, addedDate, true, discount, reorderLevel);
                
                if (system.addProduct(product)) {
                    cout << "Product added successfully." << endl;
                } else {
                    cout << "Failed to add product. Maximum number of products reached." << endl;
                }
                break;
            }
            case 2: {
                int productId;
                cout << "Enter Product ID to remove: ";
                cin >> productId;
                
                if (system.removeProduct(productId)) {
                    cout << "Product removed successfully." << endl;
                } else {
                    cout << "Failed to remove product. Product ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                int productId, quantityChange;
                cout << "Enter Product ID: ";
                cin >> productId;
                
                cout << "Enter Quantity Change (positive to add, negative to remove): ";
                cin >> quantityChange;
                
                if (system.updateProductQuantity(productId, quantityChange)) {
                    cout << "Product quantity updated successfully." << endl;
                } else {
                    cout << "Failed to update product quantity. Product ID may not exist or insufficient quantity." << endl;
                }
                break;
            }
            case 4: {
                vector<Product*> products = system.getAllProducts();
                if (products.empty()) {
                    cout << "No products found." << endl;
                } else {
                    cout << "Products (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice()
                             << ", Quantity: " << product->getQuantity() << endl;
                    }
                }
                break;
            }
            case 5: {
                string name;
                cout << "Enter product name to search: ";
                getline(cin, name);
                
                vector<Product*> products = system.searchProductsByName(name);
                if (products.empty()) {
                    cout << "No products found matching the name." << endl;
                } else {
                    cout << "Products matching '" << name << "' (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice()
                             << ", Quantity: " << product->getQuantity() << endl;
                    }
                }
                break;
            }
            case 6: {
                int categoryId;
                cout << "Enter Category ID: ";
                cin >> categoryId;
                
                vector<Product*> products = system.searchProductsByCategory(categoryId);
                if (products.empty()) {
                    cout << "No products found in the category." << endl;
                } else {
                    cout << "Products in Category " << categoryId << " (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice()
                             << ", Quantity: " << product->getQuantity() << endl;
                    }
                }
                break;
            }
            case 7: {
                vector<Product*> products = system.getProductsNeedingReorder();
                if (products.empty()) {
                    cout << "No products need to be reordered." << endl;
                } else {
                    cout << "Products Needing Reorder (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Current Quantity: " << product->getQuantity()
                             << ", Reorder Level: " << product->getReorderLevel() << endl;
                    }
                }
                break;
            }
            case 8: {
                vector<Product*> products = system.getAvailableProducts();
                if (products.empty()) {
                    cout << "No available products found." << endl;
                } else {
                    cout << "Available Products (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getEffectivePrice()
                             << ", Quantity: " << product->getQuantity() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void categoryMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "Category Management" << endl;
        cout << "1. Add Category" << endl;
        cout << "2. Remove Category" << endl;
        cout << "3. List All Categories" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description;
                int parentId;
                
                cout << "Enter Category Name: ";
                getline(cin, name);
                
                cout << "Enter Category Description: ";
                getline(cin, description);
                
                cout << "Enter Parent Category ID (0 for root): ";
                cin >> parentId;
                
                Category category(system.getNextCategoryId(), name, description, parentId);
                
                if (system.addCategory(category)) {
                    cout << "Category added successfully." << endl;
                } else {
                    cout << "Failed to add category. Maximum number of categories reached." << endl;
                }
                break;
            }
            case 2: {
                int categoryId;
                cout << "Enter Category ID to remove: ";
                cin >> categoryId;
                
                if (system.removeCategory(categoryId)) {
                    cout << "Category removed successfully." << endl;
                } else {
                    cout << "Failed to remove category. Category ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Category*> categories = system.getAllCategories();
                if (categories.empty()) {
                    cout << "No categories found." << endl;
                } else {
                    cout << "Categories (" << categories.size() << "):" << endl;
                    for (Category* category : categories) {
                        cout << "ID: " << category->getCategoryId()
                             << ", Name: " << category->getName()
                             << ", Parent ID: " << (category->getParentId() == 0 ? "None" : to_string(category->getParentId())) << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void customerMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "Customer Management" << endl;
        cout << "1. Add Customer" << endl;
        cout << "2. Remove Customer" << endl;
        cout << "3. List All Customers" << endl;
        cout << "4. Search Customers by Name" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, email, phone, address;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Address: ";
                getline(cin, address);
                
                // Get current date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date registrationDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                Customer customer(system.getNextCustomerId(), firstName, lastName, email, phone, 
                                address, registrationDate);
                
                if (system.addCustomer(customer)) {
                    cout << "Customer added successfully." << endl;
                } else {
                    cout << "Failed to add customer. Maximum number of customers reached." << endl;
                }
                break;
            }
            case 2: {
                int customerId;
                cout << "Enter Customer ID to remove: ";
                cin >> customerId;
                
                if (system.removeCustomer(customerId)) {
                    cout << "Customer removed successfully." << endl;
                } else {
                    cout << "Failed to remove customer. Customer ID may not exist." << endl;
                }
                break;
            }
            case 3: {
                vector<Customer*> customers = system.getAllCustomers();
                if (customers.empty()) {
                    cout << "No customers found." << endl;
                } else {
                    cout << "Customers (" << customers.size() << "):" << endl;
                    for (Customer* customer : customers) {
                        cout << "ID: " << customer->getCustomerId()
                             << ", Name: " << customer->getFullName()
                             << ", Membership: " << customer->getMembershipLevel()
                             << ", Purchases: $" << fixed << setprecision(2) << customer->getTotalPurchases() << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter customer name to search: ";
                getline(cin, name);
                
                vector<Customer*> customers = system.searchCustomersByName(name);
                if (customers.empty()) {
                    cout << "No customers found matching the name." << endl;
                } else {
                    cout << "Customers matching '" << name << "' (" << customers.size() << "):" << endl;
                    for (Customer* customer : customers) {
                        cout << "ID: " << customer->getCustomerId()
                             << ", Name: " << customer->getFullName()
                             << ", Membership: " << customer->getMembershipLevel()
                             << ", Purchases: $" << fixed << setprecision(2) << customer->getTotalPurchases() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void orderMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "Order Management" << endl;
        cout << "1. Create Order" << endl;
        cout << "2. Process Order" << endl;
        cout << "3. Complete Order" << endl;
        cout << "4. Cancel Order" << endl;
        cout << "5. List All Orders" << endl;
        cout << "6. List Orders by Customer" << endl;
        cout << "7. List Orders by Status" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int customerId, productId, quantity;
                string shippingAddress, paymentMethod;
                vector<pair<int, int>> productQuantities;
                
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cin.ignore();
                
                cout << "Enter Shipping Address: ";
                getline(cin, shippingAddress);
                
                cout << "Enter Payment Method: ";
                getline(cin, paymentMethod);
                
                cout << "Enter products (Product ID and Quantity):" << endl;
                do {
                    cout << "Enter Product ID (0 to finish): ";
                    cin >> productId;
                    
                    if (productId == 0) break;
                    
                    cout << "Enter Quantity: ";
                    cin >> quantity;
                    
                    productQuantities.push_back({productId, quantity});
                } while (productId != 0);
                
                int newOrderId;
                if (system.createOrder(customerId, shippingAddress, paymentMethod, productQuantities, newOrderId)) {
                    cout << "Order created successfully with ID: " << newOrderId << endl;
                } else {
                    cout << "Failed to create order. Customer may not exist or products may not be available." << endl;
                }
                break;
            }
            case 2: {
                int orderId;
                cout << "Enter Order ID to process: ";
                cin >> orderId;
                
                if (system.processOrder(orderId)) {
                    cout << "Order processed successfully." << endl;
                } else {
                    cout << "Failed to process order. Order ID may not exist or order may not be in 'Pending' status." << endl;
                }
                break;
            }
            case 3: {
                int orderId;
                cout << "Enter Order ID to complete: ";
                cin >> orderId;
                
                if (system.completeOrder(orderId)) {
                    cout << "Order completed successfully." << endl;
                } else {
                    cout << "Failed to complete order. Order ID may not exist or order may not be in 'Shipped' status." << endl;
                }
                break;
            }
            case 4: {
                int orderId;
                cout << "Enter Order ID to cancel: ";
                cin >> orderId;
                
                if (system.cancelOrder(orderId)) {
                    cout << "Order cancelled successfully." << endl;
                } else {
                    cout << "Failed to cancel order. Order ID may not exist or order may already be completed." << endl;
                }
                break;
            }
            case 5: {
                vector<Order*> orders = system.getAllOrders();
                if (orders.empty()) {
                    cout << "No orders found." << endl;
                } else {
                    cout << "Orders (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Status: " << order->getStatus()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotalAmount() << endl;
                    }
                }
                break;
            }
            case 6: {
                int customerId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                vector<Order*> orders = system.getOrdersByCustomer(customerId);
                if (orders.empty()) {
                    cout << "No orders found for this customer." << endl;
                } else {
                    cout << "Orders for Customer " << customerId << " (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Status: " << order->getStatus()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotalAmount() << endl;
                    }
                }
                break;
            }
            case 7: {
                string status;
                cout << "Enter Status to filter by: ";
                getline(cin, status);
                
                vector<Order*> orders = system.getOrdersByStatus(status);
                if (orders.empty()) {
                    cout << "No orders found with this status." << endl;
                } else {
                    cout << "Orders with status '" << status << "' (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotalAmount() << endl;
                    }
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void reportMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Sales Report" << endl;
        cout << "2. Inventory Report" << endl;
        cout << "3. Customer Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string startDate, endDate;
                cout << "Enter Start Date (YYYY-MM-DD): ";
                getline(cin, startDate);
                
                cout << "Enter End Date (YYYY-MM-DD): ";
                getline(cin, endDate);
                
                system.generateSalesReport(startDate, endDate);
                break;
            }
            case 2: {
                system.generateInventoryReport();
                break;
            }
            case 3: {
                system.generateCustomerReport();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}

void fileMenu(InventorySystem& system) {
    int choice;
    do {
        cout << "File Operations" << endl;
        cout << "1. Save Data to File" << endl;
        cout << "2. Load Data from File" << endl;
        cout << "3. Generate Sample Data" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string filename;
                cout << "Enter filename to save: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "inventory_data.txt";
                }
                
                if (system.saveToFile(filename)) {
                    cout << "Data saved successfully to " << filename << endl;
                } else {
                    cout << "Failed to save data." << endl;
                }
                break;
            }
            case 2: {
                string filename;
                cout << "Enter filename to load: ";
                getline(cin, filename);
                
                if (filename.empty()) {
                    filename = "inventory_data.txt";
                }
                
                if (system.loadFromFile(filename)) {
                    cout << "Data loaded successfully from " << filename << endl;
                } else {
                    cout << "Failed to load data." << endl;
                }
                break;
            }
            case 3: {
                system.generateSampleData();
                break;
            }
            case 0:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        
        if (choice != 0) {
            cout << endl;
            system("pause");
            system("cls");
        }
    } while (choice != 0);
}
<｜fim▁hole｜>