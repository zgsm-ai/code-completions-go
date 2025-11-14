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
#include <regex>

using namespace std;

#define MAX_ORDERS 1000
#define MAX_PRODUCTS 500
#define MAX_CUSTOMERS 500
#define MAX_CATEGORIES 50
#define MAX_CART_ITEMS 50
#define MAX_REVIEWS 2000

struct date {
    int year;
    int month;
    int day;
};

struct time {
    int hour;
    int minute;
    int second;
};

class Product;
class Customer;
class Order;
class Cart;
class CartItem;
class Review;
class Category;

enum OrderStatus {
    PENDING = 1,
    PROCESSING = 2,
    SHIPPED = 3,
    DELIVERED = 4,
    CANCELLED = 5,
    REFUNDED = 6
};

enum PaymentMethod {
    CREDIT_CARD = 1,
    DEBIT_CARD = 2,
    PAYPAL = 3,
    BANK_TRANSFER = 4,
    CASH_ON_DELIVERY = 5
};

enum ProductStatus {
    IN_STOCK = 1,
    OUT_OF_STOCK = 2,
    DISCONTINUED = 3,
    ON_BACKORDER = 4
};

class Category {
private:
    int categoryId;
    string name;
    string description;
    int parentId;
    vector<int> productIds;
    vector<int> subcategoryIds;
    
public:
    Category(int categoryId = 0, const string& name = "", const string& description = "", int parentId = 0)
        : categoryId(categoryId), name(name), description(description), parentId(parentId) {}
    
    int getCategoryId() const { return categoryId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    int getParentId() const { return parentId; }
    vector<int> getProductIds() const { return productIds; }
    vector<int> getSubcategoryIds() const { return subcategoryIds; }
    
    void setCategoryId(int id) { categoryId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setParentId(int parentId) { this->parentId = parentId; }
    
    void addProduct(int productId) {
        if (find(productIds.begin(), productIds.end(), productId) == productIds.end()) {
            productIds.push_back(productId);
        }
    }
    
    void removeProduct(int productId) {
        auto it = find(productIds.begin(), productIds.end(), productId);
        if (it != productIds.end()) {
            productIds.erase(it);
        }
    }
    
    void addSubcategory(int categoryId) {
        if (find(subcategoryIds.begin(), subcategoryIds.end(), categoryId) == subcategoryIds.end()) {
            subcategoryIds.push_back(categoryId);
        }
    }
    
    void removeSubcategory(int categoryId) {
        auto it = find(subcategoryIds.begin(), subcategoryIds.end(), categoryId);
        if (it != subcategoryIds.end()) {
            subcategoryIds.erase(it);
        }
    }
    
    void display() const {
        cout << "Category ID: " << categoryId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Parent ID: " << parentId << endl;
        cout << "Products: " << productIds.size() << endl;
        cout << "Subcategories: " << subcategoryIds.size() << endl;
    }
};

class Product {
private:
    int productId;
    string name;
    string description;
    double price;
    int quantity;
    ProductStatus status;
    int categoryId;
    string brand;
    string model;
    double weight;
    vector<string> images;
    vector<int> reviewIds;
    vector<string> tags;
    
public:
    Product(int productId = 0, const string& name = "", const string& description = "",
            double price = 0.0, int quantity = 0, ProductStatus status = IN_STOCK,
            int categoryId = 0, const string& brand = "", const string& model = "",
            double weight = 0.0)
        : productId(productId), name(name), description(description), price(price),
          quantity(quantity), status(status), categoryId(categoryId), brand(brand),
          model(model), weight(weight) {}
    
    int getProductId() const { return productId; }
    string getName() const { return name; }
    string getDescription() const { return description; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    ProductStatus getStatus() const { return status; }
    int getCategoryId() const { return categoryId; }
    string getBrand() const { return brand; }
    string getModel() const { return model; }
    double getWeight() const { return weight; }
    vector<string> getImages() const { return images; }
    vector<int> getReviewIds() const { return reviewIds; }
    vector<string> getTags() const { return tags; }
    
    void setProductId(int id) { productId = id; }
    void setName(const string& name) { this->name = name; }
    void setDescription(const string& description) { this->description = description; }
    void setPrice(double price) { this->price = price; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setStatus(ProductStatus status) { this->status = status; }
    void setCategoryId(int categoryId) { this->categoryId = categoryId; }
    void setBrand(const string& brand) { this->brand = brand; }
    void setModel(const string& model) { this->model = model; }
    void setWeight(double weight) { this->weight = weight; }
    
    void addImage(const string& imageUrl) {
        if (find(images.begin(), images.end(), imageUrl) == images.end()) {
            images.push_back(imageUrl);
        }
    }
    
    void removeImage(const string& imageUrl) {
        auto it = find(images.begin(), images.end(), imageUrl);
        if (it != images.end()) {
            images.erase(it);
        }
    }
    
    void addReview(int reviewId) {
        if (find(reviewIds.begin(), reviewIds.end(), reviewId) == reviewIds.end()) {
            reviewIds.push_back(reviewId);
        }
    }
    
    void removeReview(int reviewId) {
        auto it = find(reviewIds.begin(), reviewIds.end(), reviewId);
        if (it != reviewIds.end()) {
            reviewIds.erase(it);
        }
    }
    
    void addTag(const string& tag) {
        if (find(tags.begin(), tags.end(), tag) == tags.end()) {
            tags.push_back(tag);
        }
    }
    
    void removeTag(const string& tag) {
        auto it = find(tags.begin(), tags.end(), tag);
        if (it != tags.end()) {
            tags.erase(it);
        }
    }
    
    string getStatusString() const {
        switch (status) {
            case IN_STOCK: return "In Stock";
            case OUT_OF_STOCK: return "Out of Stock";
            case DISCONTINUED: return "Discontinued";
            case ON_BACKORDER: return "On Backorder";
            default: return "Unknown";
        }
    }
    
    bool isInStock() const {
        return status == IN_STOCK && quantity > 0;
    }
    
    double getDiscountedPrice(double discountPercentage) const {
        return price * (1.0 - discountPercentage / 100.0);
    }
    
    void display() const {
        cout << "Product ID: " << productId << endl;
        cout << "Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Price: $" << fixed << setprecision(2) << price << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Category ID: " << categoryId << endl;
        cout << "Brand: " << brand << endl;
        cout << "Model: " << model << endl;
        cout << "Weight: " << fixed << setprecision(2) << weight << " kg" << endl;
        cout << "Images: " << images.size() << endl;
        cout << "Reviews: " << reviewIds.size() << endl;
        cout << "Tags: ";
        for (size_t i = 0; i < tags.size(); i++) {
            if (i > 0) cout << ", ";
            cout << tags[i];
        }
        cout << endl;
    }
};

class Review {
private:
    int reviewId;
    int productId;
    int customerId;
    int rating;
    string title;
    string comment;
    date reviewDate;
    bool isVerifiedPurchase;
    vector<string> pros;
    vector<string> cons;
    
public:
    Review(int reviewId = 0, int productId = 0, int customerId = 0, int rating = 0,
          const string& title = "", const string& comment = "", date reviewDate = {0, 0, 0},
          bool isVerifiedPurchase = false)
        : reviewId(reviewId), productId(productId), customerId(customerId), rating(rating),
          title(title), comment(comment), reviewDate(reviewDate), isVerifiedPurchase(isVerifiedPurchase) {}
    
    int getReviewId() const { return reviewId; }
    int getProductId() const { return productId; }
    int getCustomerId() const { return customerId; }
    int getRating() const { return rating; }
    string getTitle() const { return title; }
    string getComment() const { return comment; }
    date getReviewDate() const { return reviewDate; }
    bool getIsVerifiedPurchase() const { return isVerifiedPurchase; }
    vector<string> getPros() const { return pros; }
    vector<string> getCons() const { return cons; }
    
    void setReviewId(int id) { reviewId = id; }
    void setProductId(int productId) { this->productId = productId; }
    void setCustomerId(int customerId) { this->customerId = customerId; }
    void setRating(int rating) { this->rating = rating; }
    void setTitle(const string& title) { this->title = title; }
    void setComment(const string& comment) { this->comment = comment; }
    void setReviewDate(date date) { reviewDate = date; }
    void setIsVerifiedPurchase(bool isVerified) { isVerifiedPurchase = isVerified; }
    
    void addPro(const string& pro) {
        if (find(pros.begin(), pros.end(), pro) == pros.end()) {
            pros.push_back(pro);
        }
    }
    
    void removePro(const string& pro) {
        auto it = find(pros.begin(), pros.end(), pro);
        if (it != pros.end()) {
            pros.erase(it);
        }
    }
    
    void addCon(const string& con) {
        if (find(cons.begin(), cons.end(), con) == cons.end()) {
            cons.push_back(con);
        }
    }
    
    void removeCon(const string& con) {
        auto it = find(cons.begin(), cons.end(), con);
        if (it != cons.end()) {
            cons.erase(it);
        }
    }
    
    bool isValidRating() const {
        return rating >= 1 && rating <= 5;
    }
    
    void display() const {
        cout << "Review ID: " << reviewId << endl;
        cout << "Product ID: " << productId << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Rating: ";
        for (int i = 1; i <= 5; i++) {
            if (i <= rating) {
                cout << "★";
            } else {
                cout << "☆";
            }
        }
        cout << " (" << rating << "/5)" << endl;
        cout << "Title: " << title << endl;
        cout << "Comment: " << comment << endl;
        cout << "Review Date: " << reviewDate.year << "-" 
             << reviewDate.month << "-" << reviewDate.day << endl;
        cout << "Verified Purchase: " << (isVerifiedPurchase ? "Yes" : "No") << endl;
        
        if (!pros.empty()) {
            cout << "Pros: ";
            for (size_t i = 0; i < pros.size(); i++) {
                if (i > 0) cout << ", ";
                cout << pros[i];
            }
            cout << endl;
        }
        
        if (!cons.empty()) {
            cout << "Cons: ";
            for (size_t i = 0; i < cons.size(); i++) {
                if (i > 0) cout << ", ";
                cout << cons[i];
            }
            cout << endl;
        }
    }
};

class CartItem {
private:
    int cartItemId;
    int productId;
    int quantity;
    double unitPrice;
    
public:
    CartItem(int cartItemId = 0, int productId = 0, int quantity = 0, double unitPrice = 0.0)
        : cartItemId(cartItemId), productId(productId), quantity(quantity), unitPrice(unitPrice) {}
    
    int getCartItemId() const { return cartItemId; }
    int getProductId() const { return productId; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    
    void setCartItemId(int id) { cartItemId = id; }
    void setProductId(int productId) { this->productId = productId; }
    void setQuantity(int quantity) { this->quantity = quantity; }
    void setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
    
    double getSubtotal() const {
        return unitPrice * quantity;
    }
    
    void display() const {
        cout << "Cart Item ID: " << cartItemId << endl;
        cout << "Product ID: " << productId << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Unit Price: $" << fixed << setprecision(2) << unitPrice << endl;
        cout << "Subtotal: $" << fixed << setprecision(2) << getSubtotal() << endl;
    }
};

class Cart {
private:
    int cartId;
    int customerId;
    vector<CartItem> items;
    date createdDate;
    date lastModifiedDate;
    bool isActive;
    
public:
    Cart(int cartId = 0, int customerId = 0, date createdDate = {0, 0, 0}, 
         date lastModifiedDate = {0, 0, 0}, bool isActive = true)
        : cartId(cartId), customerId(customerId), createdDate(createdDate), 
          lastModifiedDate(lastModifiedDate), isActive(isActive) {}
    
    int getCartId() const { return cartId; }
    int getCustomerId() const { return customerId; }
    vector<CartItem> getItems() const { return items; }
    date getCreatedDate() const { return createdDate; }
    date getLastModifiedDate() const { return lastModifiedDate; }
    bool getIsActive() const { return isActive; }
    
    void setCartId(int id) { cartId = id; }
    void setCustomerId(int customerId) { this->customerId = customerId; }
    void setCreatedDate(date date) { createdDate = date; }
    void setLastModifiedDate(date date) { lastModifiedDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    
    bool addItem(int productId, int quantity, double unitPrice) {
        if (!isActive || items.size() >= MAX_CART_ITEMS) {
            return false;
        }
        
        // Check if product already in cart
        for (auto& item : items) {
            if (item.getProductId() == productId) {
                item.setQuantity(item.getQuantity() + quantity);
                updateLastModified();
                return true;
            }
        }
        
        // Add new item
        CartItem newItem(getNextCartItemId(), productId, quantity, unitPrice);
        items.push_back(newItem);
        updateLastModified();
        
        return true;
    }
    
    bool updateItemQuantity(int productId, int newQuantity) {
        if (!isActive) {
            return false;
        }
        
        for (auto& item : items) {
            if (item.getProductId() == productId) {
                if (newQuantity <= 0) {
                    return removeItem(productId);
                } else {
                    item.setQuantity(newQuantity);
                    updateLastModified();
                    return true;
                }
            }
        }
        
        return false; // Item not found
    }
    
    bool removeItem(int productId) {
        if (!isActive) {
            return false;
        }
        
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->getProductId() == productId) {
                items.erase(it);
                updateLastModified();
                return true;
            }
        }
        
        return false; // Item not found
    }
    
    void clearCart() {
        if (isActive) {
            items.clear();
            updateLastModified();
        }
    }
    
    double getTotalPrice() const {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getSubtotal();
        }
        return total;
    }
    
    int getTotalQuantity() const {
        int total = 0;
        for (const auto& item : items) {
            total += item.getQuantity();
        }
        return total;
    }
    
    CartItem* getItem(int productId) {
        for (auto& item : items) {
            if (item.getProductId() == productId) {
                return &item;
            }
        }
        return nullptr;
    }
    
    void display() const {
        cout << "Cart ID: " << cartId << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Created Date: " << createdDate.year << "-" 
             << createdDate.month << "-" << createdDate.day << endl;
        cout << "Last Modified: " << lastModifiedDate.year << "-" 
             << lastModifiedDate.month << "-" << lastModifiedDate.day << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Items: " << items.size() << endl;
        cout << "Total Quantity: " << getTotalQuantity() << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << getTotalPrice() << endl;
        
        if (!items.empty()) {
            cout << endl << "Items:" << endl;
            for (const auto& item : items) {
                cout << "  Product ID: " << item.getProductId()
                     << ", Quantity: " << item.getQuantity()
                     << ", Unit Price: $" << fixed << setprecision(2) << item.getUnitPrice()
                     << ", Subtotal: $" << fixed << setprecision(2) << item.getSubtotal() << endl;
            }
        }
    }
    
private:
    int getNextCartItemId() const {
        int maxId = 0;
        for (const auto& item : items) {
            if (item.getCartItemId() > maxId) {
                maxId = item.getCartItemId();
            }
        }
        return maxId + 1;
    }
    
    void updateLastModified() {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        lastModifiedDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
    }
};

class Customer {
private:
    int customerId;
    string firstName;
    string lastName;
    string email;
    string password;
    string phone;
    string address;
    string city;
    string state;
    string zipCode;
    string country;
    date registrationDate;
    date lastLoginDate;
    bool isActive;
    vector<int> orderIds;
    int cartId;
    
public:
    Customer(int customerId = 0, const string& firstName = "", const string& lastName = "",
            const string& email = "", const string& password = "", const string& phone = "",
            const string& address = "", const string& city = "", const string& state = "",
            const string& zipCode = "", const string& country = "", 
            date registrationDate = {0, 0, 0}, date lastLoginDate = {0, 0, 0},
            bool isActive = true, int cartId = 0)
        : customerId(customerId), firstName(firstName), lastName(lastName), email(email),
          password(password), phone(phone), address(address), city(city), state(state),
          zipCode(zipCode), country(country), registrationDate(registrationDate),
          lastLoginDate(lastLoginDate), isActive(isActive), cartId(cartId) {}
    
    int getCustomerId() const { return customerId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return firstName + " " + lastName; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }
    string getCity() const { return city; }
    string getState() const { return state; }
    string getZipCode() const { return zipCode; }
    string getCountry() const { return country; }
    date getRegistrationDate() const { return registrationDate; }
    date getLastLoginDate() const { return lastLoginDate; }
    bool getIsActive() const { return isActive; }
    vector<int> getOrderIds() const { return orderIds; }
    int getCartId() const { return cartId; }
    
    void setCustomerId(int id) { customerId = id; }
    void setFirstName(const string& name) { firstName = name; }
    void setLastName(const string& name) { lastName = name; }
    void setEmail(const string& email) { this->email = email; }
    void setPassword(const string& password) { this->password = password; }
    void setPhone(const string& phone) { this->phone = phone; }
    void setAddress(const string& address) { this->address = address; }
    void setCity(const string& city) { this->city = city; }
    void setState(const string& state) { this->state = state; }
    void setZipCode(const string& zipCode) { this->zipCode = zipCode; }
    void setCountry(const string& country) { this->country = country; }
    void setRegistrationDate(date date) { registrationDate = date; }
    void setLastLoginDate(date date) { lastLoginDate = date; }
    void setIsActive(bool isActive) { this->isActive = isActive; }
    void setCartId(int cartId) { this->cartId = cartId; }
    
    void addOrder(int orderId) {
        if (find(orderIds.begin(), orderIds.end(), orderId) == orderIds.end()) {
            orderIds.push_back(orderId);
        }
    }
    
    void removeOrder(int orderId) {
        auto it = find(orderIds.begin(), orderIds.end(), orderId);
        if (it != orderIds.end()) {
            orderIds.erase(it);
        }
    }
    
    bool isValidEmail() const {
        // Simple email validation
        return email.find('@') != string::npos && email.find('.') != string::npos;
    }
    
    void updateLoginTime() {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        lastLoginDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
    }
    
    int getOrderCount() const {
        return orderIds.size();
    }
    
    void display() const {
        cout << "Customer ID: " << customerId << endl;
        cout << "Name: " << getFullName() << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address << endl;
        cout << "City: " << city << endl;
        cout << "State: " << state << endl;
        cout << "Zip Code: " << zipCode << endl;
        cout << "Country: " << country << endl;
        cout << "Registration Date: " << registrationDate.year << "-" 
             << registrationDate.month << "-" << registrationDate.day << endl;
        cout << "Last Login: " << lastLoginDate.year << "-" 
             << lastLoginDate.month << "-" << lastLoginDate.day << endl;
        cout << "Status: " << (isActive ? "Active" : "Inactive") << endl;
        cout << "Orders: " << orderIds.size() << endl;
        cout << "Cart ID: " << cartId << endl;
    }
};

class OrderItem {
private:
    int productId;
    string productName;
    int quantity;
    double unitPrice;
    double totalPrice;
    
public:
    OrderItem(int productId = 0, const string& productName = "", int quantity = 0, 
              double unitPrice = 0.0)
        : productId(productId), productName(productName), quantity(quantity), unitPrice(unitPrice) {
        totalPrice = unitPrice * quantity;
    }
    
    int getProductId() const { return productId; }
    string getProductName() const { return productName; }
    int getQuantity() const { return quantity; }
    double getUnitPrice() const { return unitPrice; }
    double getTotalPrice() const { return totalPrice; }
    
    void setProductId(int productId) { this->productId = productId; }
    void setProductName(const string& productName) { this->productName = productName; }
    void setQuantity(int quantity) { 
        this->quantity = quantity; 
        totalPrice = unitPrice * quantity;
    }
    void setUnitPrice(double unitPrice) { 
        this->unitPrice = unitPrice; 
        totalPrice = unitPrice * quantity;
    }
    
    void display() const {
        cout << "Product ID: " << productId << endl;
        cout << "Product Name: " << productName << endl;
        cout << "Quantity: " << quantity << endl;
        cout << "Unit Price: $" << fixed << setprecision(2) << unitPrice << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
    }
};

class Order {
private:
    int orderId;
    int customerId;
    vector<OrderItem> items;
    date orderDate;
    date shippingDate;
    date deliveryDate;
    OrderStatus status;
    PaymentMethod paymentMethod;
    double subtotal;
    double tax;
    double shipping;
    double total;
    string trackingNumber;
    string shippingAddress;
    string billingAddress;
    string notes;
    
public:
    Order(int orderId = 0, int customerId = 0, date orderDate = {0, 0, 0}, 
          date shippingDate = {0, 0, 0}, date deliveryDate = {0, 0, 0},
          OrderStatus status = PENDING, PaymentMethod paymentMethod = CREDIT_CARD,
          double subtotal = 0.0, double tax = 0.0, double shipping = 0.0,
          double total = 0.0, const string& trackingNumber = "",
          const string& shippingAddress = "", const string& billingAddress = "",
          const string& notes = "")
        : orderId(orderId), customerId(customerId), orderDate(orderDate), 
          shippingDate(shippingDate), deliveryDate(deliveryDate), status(status),
          paymentMethod(paymentMethod), subtotal(subtotal), tax(tax), shipping(shipping),
          total(total), trackingNumber(trackingNumber), shippingAddress(shippingAddress),
          billingAddress(billingAddress), notes(notes) {}
    
    int getOrderId() const { return orderId; }
    int getCustomerId() const { return customerId; }
    vector<OrderItem> getItems() const { return items; }
    date getOrderDate() const { return orderDate; }
    date getShippingDate() const { return shippingDate; }
    date getDeliveryDate() const { return deliveryDate; }
    OrderStatus getStatus() const { return status; }
    PaymentMethod getPaymentMethod() const { return paymentMethod; }
    double getSubtotal() const { return subtotal; }
    double getTax() const { return tax; }
    double getShipping() const { return shipping; }
    double getTotal() const { return total; }
    string getTrackingNumber() const { return trackingNumber; }
    string getShippingAddress() const { return shippingAddress; }
    string getBillingAddress() const { return billingAddress; }
    string getNotes() const { return notes; }
    
    void setOrderId(int id) { orderId = id; }
    void setCustomerId(int customerId) { this->customerId = customerId; }
    void setOrderDate(date date) { orderDate = date; }
    void setShippingDate(date date) { shippingDate = date; }
    void setDeliveryDate(date date) { deliveryDate = date; }
    void setStatus(OrderStatus status) { this->status = status; }
    void setPaymentMethod(PaymentMethod method) { paymentMethod = method; }
    void setSubtotal(double subtotal) { this->subtotal = subtotal; }
    void setTax(double tax) { this->tax = tax; }
    void setShipping(double shipping) { this->shipping = shipping; }
    void setTotal(double total) { this->total = total; }
    void setTrackingNumber(const string& number) { trackingNumber = number; }
    void setShippingAddress(const string& address) { shippingAddress = address; }
    void setBillingAddress(const string& address) { billingAddress = address; }
    void setNotes(const string& notes) { this->notes = notes; }
    
    void addItem(int productId, const string& productName, int quantity, double unitPrice) {
        items.push_back(OrderItem(productId, productName, quantity, unitPrice));
    }
    
    void calculateTotals() {
        subtotal = 0.0;
        for (const auto& item : items) {
            subtotal += item.getTotalPrice();
        }
        
        tax = subtotal * 0.08; // 8% tax
        shipping = subtotal > 50.0 ? 0.0 : 10.0; // Free shipping for orders over $50
        total = subtotal + tax + shipping;
    }
    
    string getStatusString() const {
        switch (status) {
            case PENDING: return "Pending";
            case PROCESSING: return "Processing";
            case SHIPPED: return "Shipped";
            case DELIVERED: return "Delivered";
            case CANCELLED: return "Cancelled";
            case REFUNDED: return "Refunded";
            default: return "Unknown";
        }
    }
    
    string getPaymentMethodString() const {
        switch (paymentMethod) {
            case CREDIT_CARD: return "Credit Card";
            case DEBIT_CARD: return "Debit Card";
            case PAYPAL: return "PayPal";
            case BANK_TRANSFER: return "Bank Transfer";
            case CASH_ON_DELIVERY: return "Cash on Delivery";
            default: return "Unknown";
        }
    }
    
    bool isDelivered() const {
        return status == DELIVERED;
    }
    
    bool isCancelled() const {
        return status == CANCELLED || status == REFUNDED;
    }
    
    int getTotalQuantity() const {
        int total = 0;
        for (const auto& item : items) {
            total += item.getQuantity();
        }
        return total;
    }
    
    void display() const {
        cout << "Order ID: " << orderId << endl;
        cout << "Customer ID: " << customerId << endl;
        cout << "Order Date: " << orderDate.year << "-" 
             << orderDate.month << "-" << orderDate.day << endl;
        cout << "Shipping Date: " << shippingDate.year << "-" 
             << shippingDate.month << "-" << shippingDate.day << endl;
        cout << "Delivery Date: " << deliveryDate.year << "-" 
             << deliveryDate.month << "-" << deliveryDate.day << endl;
        cout << "Status: " << getStatusString() << endl;
        cout << "Payment Method: " << getPaymentMethodString() << endl;
        cout << "Items: " << items.size() << endl;
        cout << "Total Quantity: " << getTotalQuantity() << endl;
        cout << "Subtotal: $" << fixed << setprecision(2) << subtotal << endl;
        cout << "Tax: $" << fixed << setprecision(2) << tax << endl;
        cout << "Shipping: $" << fixed << setprecision(2) << shipping << endl;
        cout << "Total: $" << fixed << setprecision(2) << total << endl;
        cout << "Tracking Number: " << trackingNumber << endl;
        cout << "Shipping Address: " << shippingAddress << endl;
        cout << "Billing Address: " << billingAddress << endl;
        cout << "Notes: " << notes << endl;
        
        if (!items.empty()) {
            cout << endl << "Items:" << endl;
            for (const auto& item : items) {
                cout << "  Product: " << item.getProductName()
                     << " (ID: " << item.getProductId() << ")"
                     << ", Quantity: " << item.getQuantity()
                     << ", Unit Price: $" << fixed << setprecision(2) << item.getUnitPrice()
                     << ", Total: $" << fixed << setprecision(2) << item.getTotalPrice() << endl;
            }
        }
    }
};

class ECommerceSystem {
private:
    vector<Product> products;
    vector<Customer> customers;
    vector<Order> orders;
    vector<Cart> carts;
    vector<Category> categories;
    vector<Review> reviews;
    
    int nextProductId;
    int nextCustomerId;
    int nextOrderId;
    int nextCartId;
    int nextCategoryId;
    int nextReviewId;
    
public:
    ECommerceSystem() {
        nextProductId = 1;
        nextCustomerId = 1;
        nextOrderId = 1;
        nextCartId = 1;
        nextCategoryId = 1;
        nextReviewId = 1;
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
    
    vector<Product*> getProductsByCategory(int categoryId) {
        vector<Product*> result;
        for (auto& product : products) {
            if (product.getCategoryId() == categoryId) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    vector<Product*> getInStockProducts() {
        vector<Product*> result;
        for (auto& product : products) {
            if (product.isInStock()) {
                result.push_back(&product);
            }
        }
        return result;
    }
    
    vector<Product*> getTopSellingProducts(int limit = 10) {
        vector<pair<int, Product*>> productSales;
        
        for (auto& product : products) {
            int salesCount = 0;
            
            for (const auto& order : orders) {
                for (const auto& item : order.getItems()) {
                    if (item.getProductId() == product.getProductId()) {
                        salesCount += item.getQuantity();
                    }
                }
            }
            
            productSales.push_back(make_pair(salesCount, &product));
        }
        
        sort(productSales.begin(), productSales.end(), 
             [](const pair<int, Product*>& a, const pair<int, Product*>& b) {
                 return a.first > b.first;
             });
        
        vector<Product*> result;
        for (int i = 0; i < min(limit, (int)productSales.size()); i++) {
            if (productSales[i].first > 0) { // Only include products with sales
                result.push_back(productSales[i].second);
            }
        }
        
        return result;
    }
    
    int getNextProductId() { return nextProductId++; }
    
    // Customer management
    Customer* findCustomer(int customerId) {
        for (auto& customer : customers) {
            if (customer.getCustomerId() == customerId) {
                return &customer;
            }
        }
        return nullptr;
    }
    
    Customer* findCustomerByEmail(const string& email) {
        for (auto& customer : customers) {
            if (customer.getEmail() == email) {
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
    
    vector<Customer*> getActiveCustomers() {
        vector<Customer*> result;
        for (auto& customer : customers) {
            if (customer.getIsActive()) {
                result.push_back(&customer);
            }
        }
        return result;
    }
    
    vector<Customer*> getTopCustomersByOrders(int limit = 10) {
        vector<pair<int, Customer*>> customerOrders;
        
        for (auto& customer : customers) {
            customerOrders.push_back(make_pair(customer.getOrderCount(), &customer));
        }
        
        sort(customerOrders.begin(), customerOrders.end(), 
             [](const pair<int, Customer*>& a, const pair<int, Customer*>& b) {
                 return a.first > b.first;
             });
        
        vector<Customer*> result;
        for (int i = 0; i < min(limit, (int)customerOrders.size()); i++) {
            if (customerOrders[i].first > 0) { // Only include customers with orders
                result.push_back(customerOrders[i].second);
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
    
    vector<Order*> getOrdersByStatus(OrderStatus status) {
        vector<Order*> result;
        for (auto& order : orders) {
            if (order.getStatus() == status) {
                result.push_back(&order);
            }
        }
        return result;
    }
    
    vector<Order*> getOrdersByDate(int year, int month, int day) {
        vector<Order*> result;
        for (auto& order : orders) {
            if (order.getOrderDate().year == year &&
                order.getOrderDate().month == month &&
                order.getOrderDate().day == day) {
                result.push_back(&order);
            }
        }
        return result;
    }
    
    vector<Order*> getRecentOrders(int days = 7) {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        vector<Order*> result;
        for (auto& order : orders) {
            date orderDate = order.getOrderDate();
            
            // Simple date comparison (not accurate for months with different days)
            int daysDiff = (today.year - orderDate.year) * 365;
            daysDiff += (today.month - orderDate.month) * 30;
            daysDiff += today.day - orderDate.day;
            
            if (daysDiff <= days) {
                result.push_back(&order);
            }
        }
        return result;
    }
    
    int getNextOrderId() { return nextOrderId++; }
    
    // Cart management
    Cart* findCart(int cartId) {
        for (auto& cart : carts) {
            if (cart.getCartId() == cartId) {
                return &cart;
            }
        }
        return nullptr;
    }
    
    Cart* findCartByCustomer(int customerId) {
        for (auto& cart : carts) {
            if (cart.getCustomerId() == customerId && cart.getIsActive()) {
                return &cart;
            }
        }
        return nullptr;
    }
    
    bool addCart(const Cart& cart) {
        carts.push_back(cart);
        return true;
    }
    
    int getNextCartId() { return nextCartId++; }
    
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
    
    vector<Category*> getRootCategories() {
        vector<Category*> result;
        for (auto& category : categories) {
            if (category.getParentId() == 0) {
                result.push_back(&category);
            }
        }
        return result;
    }
    
    vector<Category*> getSubcategories(int parentId) {
        vector<Category*> result;
        for (auto& category : categories) {
            if (category.getParentId() == parentId) {
                result.push_back(&category);
            }
        }
        return result;
    }
    
    int getNextCategoryId() { return nextCategoryId++; }
    
    // Review management
    Review* findReview(int reviewId) {
        for (auto& review : reviews) {
            if (review.getReviewId() == reviewId) {
                return &review;
            }
        }
        return nullptr;
    }
    
    bool addReview(const Review& review) {
        if (reviews.size() >= MAX_REVIEWS) {
            return false;
        }
        
        reviews.push_back(review);
        return true;
    }
    
    bool removeReview(int reviewId) {
        for (auto it = reviews.begin(); it != reviews.end(); ++it) {
            if (it->getReviewId() == reviewId) {
                reviews.erase(it);
                return true;
            }
        }
        return false;
    }
    
    vector<Review*> getAllReviews() {
        vector<Review*> result;
        for (auto& review : reviews) {
            result.push_back(&review);
        }
        return result;
    }
    
    vector<Review*> getReviewsByProduct(int productId) {
        vector<Review*> result;
        for (auto& review : reviews) {
            if (review.getProductId() == productId) {
                result.push_back(&review);
            }
        }
        return result;
    }
    
    vector<Review*> getReviewsByCustomer(int customerId) {
        vector<Review*> result;
        for (auto& review : reviews) {
            if (review.getCustomerId() == customerId) {
                result.push_back(&review);
            }
        }
        return result;
    }
    
    vector<Review*> getReviewsByRating(int minRating, int maxRating) {
        vector<Review*> result;
        for (auto& review : reviews) {
            if (review.getRating() >= minRating && review.getRating() <= maxRating) {
                result.push_back(&review);
            }
        }
        return result;
    }
    
    double getAverageRating(int productId) {
        vector<Review*> productReviews = getReviewsByProduct(productId);
        if (productReviews.empty()) {
            return 0.0;
        }
        
        double total = 0.0;
        for (auto& review : productReviews) {
            total += review->getRating();
        }
        
        return total / productReviews.size();
    }
    
    int getNextReviewId() { return nextReviewId++; }
    
    // Business operations
    Cart* getOrCreateCart(int customerId) {
        Cart* existingCart = findCartByCustomer(customerId);
        if (existingCart) {
            return existingCart;
        }
        
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Cart newCart(getNextCartId(), customerId, today, today, true);
        addCart(newCart);
        
        // Update customer's cart ID
        Customer* customer = findCustomer(customerId);
        if (customer) {
            customer->setCartId(newCart.getCartId());
        }
        
        return findCart(newCart.getCartId());
    }
    
    bool addToCart(int customerId, int productId, int quantity) {
        Product* product = findProduct(productId);
        if (!product || !product->isInStock() || quantity <= 0) {
            return false;
        }
        
        if (product->getQuantity() < quantity) {
            return false; // Not enough stock
        }
        
        Cart* cart = getOrCreateCart(customerId);
        if (!cart) {
            return false;
        }
        
        return cart->addItem(productId, quantity, product->getPrice());
    }
    
    bool updateCartItemQuantity(int customerId, int productId, int quantity) {
        Cart* cart = findCartByCustomer(customerId);
        if (!cart || quantity < 0) {
            return false;
        }
        
        Product* product = findProduct(productId);
        if (product && quantity > 0 && product->getQuantity() < quantity) {
            return false; // Not enough stock
        }
        
        return cart->updateItemQuantity(productId, quantity);
    }
    
    bool removeFromCart(int customerId, int productId) {
        Cart* cart = findCartByCustomer(customerId);
        if (!cart) {
            return false;
        }
        
        return cart->removeItem(productId);
    }
    
    bool clearCart(int customerId) {
        Cart* cart = findCartByCustomer(customerId);
        if (!cart) {
            return false;
        }
        
        cart->clearCart();
        return true;
    }
    
    bool checkout(int customerId, PaymentMethod paymentMethod, 
                 const string& shippingAddress, const string& billingAddress, 
                 const string& notes) {
        Cart* cart = findCartByCustomer(customerId);
        if (!cart || cart->getItems().empty()) {
            return false;
        }
        
        // Create order from cart
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Order newOrder(getNextOrderId(), customerId, today, today, {0, 0, 0}, 
                      PENDING, paymentMethod, 0.0, 0.0, 0.0, 0.0, "", 
                      shippingAddress, billingAddress, notes);
        
        // Add cart items to order
        for (const auto& cartItem : cart->getItems()) {
            Product* product = findProduct(cartItem.getProductId());
            if (product) {
                newOrder.addItem(product->getProductId(), product->getName(), 
                               cartItem.getQuantity(), cartItem.getUnitPrice());
                
                // Update product quantity
                product->setQuantity(product->getQuantity() - cartItem.getQuantity());
                
                // Update product status if needed
                if (product->getQuantity() == 0) {
                    product->setStatus(OUT_OF_STOCK);
                }
            }
        }
        
        // Calculate order totals
        newOrder.calculateTotals();
        
        // Add order
        if (!addOrder(newOrder)) {
            return false;
        }
        
        // Update customer's orders
        Customer* customer = findCustomer(customerId);
        if (customer) {
            customer->addOrder(newOrder.getOrderId());
        }
        
        // Clear cart
        cart->clearCart();
        
        return true;
    }
    
    bool updateOrderStatus(int orderId, OrderStatus status) {
        Order* order = findOrder(orderId);
        if (!order) {
            return false;
        }
        
        order->setStatus(status);
        
        // Update dates based on status
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        switch (status) {
            case PROCESSING:
                // Shipping date already set to today
                break;
            case SHIPPED:
                // Generate tracking number (simple example)
                order->setTrackingNumber("TRK" + to_string(orderId) + to_string(time(nullptr)));
                break;
            case DELIVERED:
                order->setDeliveryDate(today);
                break;
            case CANCELLED:
            case REFUNDED:
                // Return items to stock
                for (const auto& item : order->getItems()) {
                    Product* product = findProduct(item.getProductId());
                    if (product) {
                        product->setQuantity(product->getQuantity() + item.getQuantity());
                        
                        // Update product status if needed
                        if (product->getQuantity() > 0 && 
                            product->getStatus() == OUT_OF_STOCK) {
                            product->setStatus(IN_STOCK);
                        }
                    }
                }
                break;
            default:
                break;
        }
        
        return true;
    }
    
    bool addProductReview(int customerId, int productId, int rating, 
                         const string& title, const string& comment) {
        // Check if customer has purchased the product
        bool hasPurchased = false;
        vector<Order*> customerOrders = getOrdersByCustomer(customerId);
        
        for (Order* order : customerOrders) {
            for (const auto& item : order->getItems()) {
                if (item.getProductId() == productId && order->isDelivered()) {
                    hasPurchased = true;
                    break;
                }
            }
            if (hasPurchased) break;
        }
        
        if (!hasPurchased) {
            return false; // Customer hasn't purchased the product
        }
        
        // Check if customer has already reviewed the product
        vector<Review*> customerReviews = getReviewsByCustomer(customerId);
        for (Review* review : customerReviews) {
            if (review->getProductId() == productId) {
                return false; // Customer has already reviewed the product
            }
        }
        
        // Validate rating
        if (rating < 1 || rating > 5) {
            return false;
        }
        
        // Create review
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
        
        Review review(getNextReviewId(), productId, customerId, rating, title, comment, today, true);
        
        if (!addReview(review)) {
            return false;
        }
        
        // Update product's reviews
        Product* product = findProduct(productId);
        if (product) {
            product->addReview(review.getReviewId());
        }
        
        return true;
    }
    
    // Reports
    void generateSalesReport(int year, int month) {
        cout << "Sales Report for " << year << "-" << month << endl;
        cout << "=============================" << endl;
        
        vector<Order*> monthlyOrders;
        for (auto& order : orders) {
            if (order->getOrderDate().year == year &&
                order->getOrderDate().month == month) {
                monthlyOrders.push_back(&order);
            }
        }
        
        if (monthlyOrders.empty()) {
            cout << "No orders found for this month." << endl;
            return;
        }
        
        double totalRevenue = 0.0;
        int totalItems = 0;
        map<OrderStatus, int> statusCounts;
        map<PaymentMethod, int> paymentMethodCounts;
        map<int, double> categoryRevenue;
        
        for (Order* order : monthlyOrders) {
            if (!order->isCancelled()) {
                totalRevenue += order->getTotal();
                totalItems += order->getTotalQuantity();
            }
            
            statusCounts[order->getStatus()]++;
            paymentMethodCounts[order->getPaymentMethod()]++;
            
            // Calculate revenue by category
            for (const auto& item : order->getItems()) {
                Product* product = findProduct(item.getProductId());
                if (product) {
                    categoryRevenue[product->getCategoryId()] += item.getTotalPrice();
                }
            }
        }
        
        cout << "Total Orders: " << monthlyOrders.size() << endl;
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        cout << "Total Items Sold: " << totalItems << endl;
        cout << "Average Order Value: $" << fixed << setprecision(2) 
             << (monthlyOrders.size() > 0 ? totalRevenue / monthlyOrders.size() : 0.0) << endl;
        cout << endl;
        
        cout << "Order Status Distribution:" << endl;
        for (const auto& pair : statusCounts) {
            Order dummyOrder;
            cout << "  " << dummyOrder.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        cout << endl;
        
        cout << "Payment Method Distribution:" << endl;
        for (const auto& pair : paymentMethodCounts) {
            Order dummyOrder;
            cout << "  " << dummyOrder.getPaymentMethodStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        cout << endl;
        
        cout << "Revenue by Category:" << endl;
        for (const auto& pair : categoryRevenue) {
            Category* category = findCategory(pair.first);
            if (category) {
                cout << "  " << category->getName() 
                     << ": $" << fixed << setprecision(2) << pair.second << endl;
            }
        }
    }
    
    void generateProductReport() {
        cout << "Product Report" << endl;
        cout << "==============" << endl;
        
        cout << "Total Products: " << products.size() << endl;
        cout << "In Stock: " << getInStockProducts().size() << endl;
        
        map<ProductStatus, int> statusCounts;
        for (const auto& product : products) {
            statusCounts[product.getStatus()]++;
        }
        
        cout << "Product Status Distribution:" << endl;
        for (const auto& pair : statusCounts) {
            Product dummyProduct;
            cout << "  " << dummyProduct.getStatusStringStatic(pair.first) 
                 << ": " << pair.second << endl;
        }
        
        cout << endl << "Top 10 Selling Products:" << endl;
        vector<Product*> topProducts = getTopSellingProducts(10);
        for (int i = 0; i < topProducts.size(); i++) {
            Product* product = topProducts[i];
            int salesCount = 0;
            
            for (const auto& order : orders) {
                for (const auto& item : order.getItems()) {
                    if (item.getProductId() == product->getProductId()) {
                        salesCount += item.getQuantity();
                    }
                }
            }
            
            cout << (i+1) << ". " << product->getName()
                 << " (ID: " << product->getProductId() << ")"
                 << " - " << salesCount << " units sold"
                 << ", $" << fixed << setprecision(2) << (salesCount * product->getPrice())
                 << " revenue" << endl;
        }
        
        cout << endl << "Low Stock Products (5 or less):" << endl;
        bool lowStockFound = false;
        for (const auto& product : products) {
            if (product.isInStock() && product.getQuantity() <= 5) {
                cout << product->getName() << " (ID: " << product->getProductId()
                     << ") - " << product.getQuantity() << " units left" << endl;
                lowStockFound = true;
            }
        }
        
        if (!lowStockFound) {
            cout << "No low stock products found." << endl;
        }
    }
    
    void generateCustomerReport() {
        cout << "Customer Report" << endl;
        cout << "===============" << endl;
        
        cout << "Total Customers: " << customers.size() << endl;
        cout << "Active Customers: " << getActiveCustomers().size() << endl;
        cout << "Total Orders: " << orders.size() << endl;
        
        // Calculate total revenue
        double totalRevenue = 0.0;
        for (const auto& order : orders) {
            if (!order.isCancelled()) {
                totalRevenue += order.getTotal();
            }
        }
        
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        cout << "Average Customer Value: $" << fixed << setprecision(2) 
             << (customers.size() > 0 ? totalRevenue / customers.size() : 0.0) << endl;
        cout << endl;
        
        cout << "Top 10 Customers by Order Count:" << endl;
        vector<Customer*> topCustomers = getTopCustomersByOrders(10);
        for (int i = 0; i < topCustomers.size(); i++) {
            Customer* customer = topCustomers[i];
            cout << (i+1) << ". " << customer->getFullName()
                 << " (ID: " << customer->getCustomerId() << ")"
                 << " - " << customer->getOrderCount() << " orders" << endl;
        }
        
        cout << endl << "Customers with No Orders:" << endl;
        bool noOrdersFound = false;
        for (const auto& customer : customers) {
            if (customer->getOrderCount() == 0) {
                cout << customer->getFullName() << " (ID: " << customer->getCustomerId()
                     << ") - " << customer->getEmail() << endl;
                noOrdersFound = true;
            }
        }
        
        if (!noOrdersFound) {
            cout << "No customers without orders." << endl;
        }
    }
    
    void generateReviewReport() {
        cout << "Review Report" << endl;
        cout << "=============" << endl;
        
        cout << "Total Reviews: " << reviews.size() << endl;
        
        if (reviews.empty()) {
            cout << "No reviews found." << endl;
            return;
        }
        
        // Calculate average rating
        double totalRating = 0.0;
        for (const auto& review : reviews) {
            totalRating += review.getRating();
        }
        
        double averageRating = totalRating / reviews.size();
        cout << "Average Rating: " << fixed << setprecision(2) << averageRating << "/5" << endl;
        cout << endl;
        
        // Rating distribution
        map<int, int> ratingCounts;
        for (const auto& review : reviews) {
            ratingCounts[review.getRating()]++;
        }
        
        cout << "Rating Distribution:" << endl;
        for (int i = 5; i >= 1; i--) {
            cout << "  " << i << " stars: " << ratingCounts[i] 
                 << " (" << (reviews.size() > 0 ? ratingCounts[i] * 100 / reviews.size() : 0) << "%)" << endl;
        }
        cout << endl;
        
        // Most reviewed products
        map<int, int> productReviewCounts;
        for (const auto& review : reviews) {
            productReviewCounts[review.getProductId()]++;
        }
        
        cout << "Most Reviewed Products:" << endl;
        vector<pair<int, int>> sortedProductReviews;
        for (const auto& pair : productReviewCounts) {
            sortedProductReviews.push_back(pair);
        }
        
        sort(sortedProductReviews.begin(), sortedProductReviews.end(), 
             [](const pair<int, int>& a, const pair<int, int>& b) {
                 return a.second > b.second;
             });
        
        int maxProducts = min(5, (int)sortedProductReviews.size());
        for (int i = 0; i < maxProducts; i++) {
            int productId = sortedProductReviews[i].first;
            int reviewCount = sortedProductReviews[i].second;
            
            Product* product = findProduct(productId);
            if (product) {
                double avgRating = getAverageRating(productId);
                cout << "  " << product->getName() << " (ID: " << productId << ")"
                     << " - " << reviewCount << " reviews, "
                     << "avg rating: " << fixed << setprecision(2) << avgRating << "/5" << endl;
            }
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
        outFile << nextProductId << "|" << nextCustomerId << "|" << nextOrderId << "|" 
                << nextCartId << "|" << nextCategoryId << "|" << nextReviewId << endl;
        
        // Save products
        outFile << "PRODUCTS" << endl;
        for (const auto& product : products) {
            outFile << product.getProductId() << "|" 
                    << product.getName() << "|" 
                    << product.getDescription() << "|" 
                    << fixed << setprecision(2) << product.getPrice() << "|" 
                    << product.getQuantity() << "|" 
                    << static_cast<int>(product.getStatus()) << "|" 
                    << product.getCategoryId() << "|" 
                    << product.getBrand() << "|" 
                    << product.getModel() << "|" 
                    << fixed << setprecision(2) << product.getWeight() << "|";
            
            // Save images
            vector<string> images = product.getImages();
            outFile << images.size();
            for (const string& image : images) {
                outFile << "," << image;
            }
            outFile << "|";
            
            // Save review IDs
            vector<int> reviewIds = product.getReviewIds();
            outFile << reviewIds.size();
            for (int reviewId : reviewIds) {
                outFile << "," << reviewId;
            }
            outFile << "|";
            
            // Save tags
            vector<string> tags = product.getTags();
            outFile << tags.size();
            for (const string& tag : tags) {
                outFile << "," << tag;
            }
            outFile << endl;
        }
        
        // Save customers
        outFile << "CUSTOMERS" << endl;
        for (const auto& customer : customers) {
            outFile << customer.getCustomerId() << "|" 
                    << customer.getFirstName() << "|" 
                    << customer.getLastName() << "|" 
                    << customer.getEmail() << "|" 
                    << customer.getPassword() << "|" 
                    << customer.getPhone() << "|" 
                    << customer.getAddress() << "|" 
                    << customer.getCity() << "|" 
                    << customer.getState() << "|" 
                    << customer.getZipCode() << "|" 
                    << customer.getCountry() << "|" 
                    << customer.getRegistrationDate().year << "|" 
                    << customer.getRegistrationDate().month << "|" 
                    << customer.getRegistrationDate().day << "|" 
                    << customer.getLastLoginDate().year << "|" 
                    << customer.getLastLoginDate().month << "|" 
                    << customer.getLastLoginDate().day << "|" 
                    << (customer.getIsActive() ? "1" : "0") << "|" 
                    << customer.getCartId() << "|";
            
            // Save order IDs
            vector<int> orderIds = customer.getOrderIds();
            outFile << orderIds.size();
            for (int orderId : orderIds) {
                outFile << "," << orderId;
            }
            outFile << endl;
        }
        
        // Save orders
        outFile << "ORDERS" << endl;
        for (const auto& order : orders) {
            outFile << order.getOrderId() << "|" 
                    << order.getCustomerId() << "|" 
                    << order.getOrderDate().year << "|" 
                    << order.getOrderDate().month << "|" 
                    << order.getOrderDate().day << "|" 
                    << order.getShippingDate().year << "|" 
                    << order.getShippingDate().month << "|" 
                    << order.getShippingDate().day << "|" 
                    << order.getDeliveryDate().year << "|" 
                    << order.getDeliveryDate().month << "|" 
                    << order.getDeliveryDate().day << "|" 
                    << static_cast<int>(order.getStatus()) << "|" 
                    << static_cast<int>(order.getPaymentMethod()) << "|" 
                    << fixed << setprecision(2) << order.getSubtotal() << "|" 
                    << fixed << setprecision(2) << order.getTax() << "|" 
                    << fixed << setprecision(2) << order.getShipping() << "|" 
                    << fixed << setprecision(2) << order.getTotal() << "|" 
                    << order.getTrackingNumber() << "|" 
                    << order.getShippingAddress() << "|" 
                    << order.getBillingAddress() << "|" 
                    << order.getNotes() << "|";
            
            // Save items
            vector<OrderItem> items = order.getItems();
            outFile << items.size();
            for (const OrderItem& item : items) {
                outFile << "|" << item.getProductId()
                        << "," << item.getProductName()
                        << "," << item.getQuantity()
                        << "," << fixed << setprecision(2) << item.getUnitPrice();
            }
            outFile << endl;
        }
        
        // Save carts
        outFile << "CARTS" << endl;
        for (const auto& cart : carts) {
            outFile << cart.getCartId() << "|" 
                    << cart.getCustomerId() << "|" 
                    << cart.getCreatedDate().year << "|" 
                    << cart.getCreatedDate().month << "|" 
                    << cart.getCreatedDate().day << "|" 
                    << cart.getLastModifiedDate().year << "|" 
                    << cart.getLastModifiedDate().month << "|" 
                    << cart.getLastModifiedDate().day << "|" 
                    << (cart.getIsActive() ? "1" : "0") << "|";
            
            // Save items
            vector<CartItem> items = cart.getItems();
            outFile << items.size();
            for (const CartItem& item : items) {
                outFile << "|" << item.getCartItemId()
                        << "," << item.getProductId()
                        << "," << item.getQuantity()
                        << "," << fixed << setprecision(2) << item.getUnitPrice();
            }
            outFile << endl;
        }
        
        // Save categories
        outFile << "CATEGORIES" << endl;
        for (const auto& category : categories) {
            outFile << category.getCategoryId() << "|" 
                    << category.getName() << "|" 
                    << category.getDescription() << "|" 
                    << category.getParentId() << "|";
            
            // Save product IDs
            vector<int> productIds = category.getProductIds();
            outFile << productIds.size();
            for (int productId : productIds) {
                outFile << "," << productId;
            }
            outFile << "|";
            
            // Save subcategory IDs
            vector<int> subcategoryIds = category.getSubcategoryIds();
            outFile << subcategoryIds.size();
            for (int categoryId : subcategoryIds) {
                outFile << "," << categoryId;
            }
            outFile << endl;
        }
        
        // Save reviews
        outFile << "REVIEWS" << endl;
        for (const auto& review : reviews) {
            outFile << review.getReviewId() << "|" 
                    << review.getProductId() << "|" 
                    << review.getCustomerId() << "|" 
                    << review.getRating() << "|" 
                    << review.getTitle() << "|" 
                    << review.getComment() << "|" 
                    << review.getReviewDate().year << "|" 
                    << review.getReviewDate().month << "|" 
                    << review.getReviewDate().day << "|" 
                    << (review.getIsVerifiedPurchase() ? "1" : "0") << "|";
            
            // Save pros
            vector<string> pros = review.getPros();
            outFile << pros.size();
            for (const string& pro : pros) {
                outFile << "," << pro;
            }
            outFile << "|";
            
            // Save cons
            vector<string> cons = review.getCons();
            outFile << cons.size();
            for (const string& con : cons) {
                outFile << "," << con;
            }
            outFile << endl;
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
            } else if (line == "CUSTOMERS") {
                section = "CUSTOMERS";
                continue;
            } else if (line == "ORDERS") {
                section = "ORDERS";
                continue;
            } else if (line == "CARTS") {
                section = "CARTS";
                continue;
            } else if (line == "CATEGORIES") {
                section = "CATEGORIES";
                continue;
            } else if (line == "REVIEWS") {
                section = "REVIEWS";
                continue;
            }
            
            if (line.empty()) continue;
            
            stringstream ss(line);
            string token;
            vector<string> tokens;
            
            while (getline(ss, token, '|')) {
                tokens.push_back(token);
            }
            
            if (section == "NEXT_IDS" && tokens.size() >= 6) {
                nextProductId = stoi(tokens[0]);
                nextCustomerId = stoi(tokens[1]);
                nextOrderId = stoi(tokens[2]);
                nextCartId = stoi(tokens[3]);
                nextCategoryId = stoi(tokens[4]);
                nextReviewId = stoi(tokens[5]);
            } else if (section == "PRODUCTS" && tokens.size() >= 11) {
                int productId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                double price = stod(tokens[3]);
                int quantity = stoi(tokens[4]);
                ProductStatus status = static_cast<ProductStatus>(stoi(tokens[5]));
                int categoryId = stoi(tokens[6]);
                string brand = tokens[7];
                string model = tokens[8];
                double weight = stod(tokens[9]);
                
                Product product(productId, name, description, price, quantity, status,
                               categoryId, brand, model, weight);
                
                // Parse images
                int imageCount = stoi(tokens[10]);
                if (imageCount > 0 && tokens.size() > 11) {
                    stringstream imageSs(tokens[11]);
                    string imageToken;
                    while (getline(imageSs, imageToken, ',')) {
                        product.addImage(imageToken);
                    }
                }
                
                // Parse review IDs
                if (tokens.size() > 12) {
                    int reviewCount = stoi(tokens[12]);
                    if (reviewCount > 0) {
                        stringstream reviewSs(tokens[13]);
                        string reviewToken;
                        while (getline(reviewSs, reviewToken, ',')) {
                            product.addReview(stoi(reviewToken));
                        }
                    }
                }
                
                // Parse tags
                if (tokens.size() > 14) {
                    int tagCount = stoi(tokens[14]);
                    if (tagCount > 0) {
                        stringstream tagSs(tokens[15]);
                        string tagToken;
                        while (getline(tagSs, tagToken, ',')) {
                            product.addTag(tagToken);
                        }
                    }
                }
                
                products.push_back(product);
            } else if (section == "CUSTOMERS" && tokens.size() >= 19) {
                int customerId = stoi(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string email = tokens[3];
                string password = tokens[4];
                string phone = tokens[5];
                string address = tokens[6];
                string city = tokens[7];
                string state = tokens[8];
                string zipCode = tokens[9];
                string country = tokens[10];
                
                date registrationDate;
                registrationDate.year = stoi(tokens[11]);
                registrationDate.month = stoi(tokens[12]);
                registrationDate.day = stoi(tokens[13]);
                
                date lastLoginDate;
                lastLoginDate.year = stoi(tokens[14]);
                lastLoginDate.month = stoi(tokens[15]);
                lastLoginDate.day = stoi(tokens[16]);
                
                bool isActive = (tokens[17] == "1");
                int cartId = stoi(tokens[18]);
                
                Customer customer(customerId, firstName, lastName, email, password, phone,
                                address, city, state, zipCode, country, registrationDate,
                                lastLoginDate, isActive, cartId);
                
                // Parse order IDs
                if (tokens.size() > 19) {
                    int orderCount = stoi(tokens[19]);
                    if (orderCount > 0) {
                        stringstream orderSs(tokens[20]);
                        string orderToken;
                        while (getline(orderSs, orderToken, ',')) {
                            customer.addOrder(stoi(orderToken));
                        }
                    }
                }
                
                customers.push_back(customer);
            } else if (section == "ORDERS" && tokens.size() >= 22) {
                int orderId = stoi(tokens[0]);
                int customerId = stoi(tokens[1]);
                
                date orderDate;
                orderDate.year = stoi(tokens[2]);
                orderDate.month = stoi(tokens[3]);
                orderDate.day = stoi(tokens[4]);
                
                date shippingDate;
                shippingDate.year = stoi(tokens[5]);
                shippingDate.month = stoi(tokens[6]);
                shippingDate.day = stoi(tokens[7]);
                
                date deliveryDate;
                deliveryDate.year = stoi(tokens[8]);
                deliveryDate.month = stoi(tokens[9]);
                deliveryDate.day = stoi(tokens[10]);
                
                OrderStatus status = static_cast<OrderStatus>(stoi(tokens[11]));
                PaymentMethod paymentMethod = static_cast<PaymentMethod>(stoi(tokens[12]));
                double subtotal = stod(tokens[13]);
                double tax = stod(tokens[14]);
                double shipping = stod(tokens[15]);
                double total = stod(tokens[16]);
                string trackingNumber = tokens[17];
                string shippingAddress = tokens[18];
                string billingAddress = tokens[19];
                string notes = tokens[20];
                
                Order order(orderId, customerId, orderDate, shippingDate, deliveryDate,
                            status, paymentMethod, subtotal, tax, shipping, total,
                            trackingNumber, shippingAddress, billingAddress, notes);
                
                // Parse items
                if (tokens.size() > 21) {
                    int itemCount = stoi(tokens[21]);
                    for (int i = 0; i < itemCount; i++) {
                        if (tokens.size() > 22 + i) {
                            vector<string> itemTokens;
                            stringstream itemSs(tokens[22 + i]);
                            string itemToken;
                            while (getline(itemSs, itemToken, ',')) {
                                itemTokens.push_back(itemToken);
                            }
                            
                            if (itemTokens.size() >= 4) {
                                int productId = stoi(itemTokens[0]);
                                string productName = itemTokens[1];
                                int quantity = stoi(itemTokens[2]);
                                double unitPrice = stod(itemTokens[3]);
                                
                                order.addItem(productId, productName, quantity, unitPrice);
                            }
                        }
                    }
                }
                
                orders.push_back(order);
            } else if (section == "CARTS" && tokens.size() >= 10) {
                int cartId = stoi(tokens[0]);
                int customerId = stoi(tokens[1]);
                
                date createdDate;
                createdDate.year = stoi(tokens[2]);
                createdDate.month = stoi(tokens[3]);
                createdDate.day = stoi(tokens[4]);
                
                date lastModifiedDate;
                lastModifiedDate.year = stoi(tokens[5]);
                lastModifiedDate.month = stoi(tokens[6]);
                lastModifiedDate.day = stoi(tokens[7]);
                
                bool isActive = (tokens[8] == "1");
                
                Cart cart(cartId, customerId, createdDate, lastModifiedDate, isActive);
                
                // Parse items
                if (tokens.size() > 9) {
                    int itemCount = stoi(tokens[9]);
                    for (int i = 0; i < itemCount; i++) {
                        if (tokens.size() > 10 + i) {
                            vector<string> itemTokens;
                            stringstream itemSs(tokens[10 + i]);
                            string itemToken;
                            while (getline(itemSs, itemToken, ',')) {
                                itemTokens.push_back(itemToken);
                            }
                            
                            if (itemTokens.size() >= 4) {
                                int cartItemId = stoi(itemTokens[0]);
                                int productId = stoi(itemTokens[1]);
                                int quantity = stoi(itemTokens[2]);
                                double unitPrice = stod(itemTokens[3]);
                                
                                CartItem item(cartItemId, productId, quantity, unitPrice);
                                // Add item to cart (would need a method in Cart class)
                                // For now, we'll skip adding items to cart
                            }
                        }
                    }
                }
                
                carts.push_back(cart);
            } else if (section == "CATEGORIES" && tokens.size() >= 6) {
                int categoryId = stoi(tokens[0]);
                string name = tokens[1];
                string description = tokens[2];
                int parentId = stoi(tokens[3]);
                
                Category category(categoryId, name, description, parentId);
                
                // Parse product IDs
                if (tokens.size() > 4) {
                    int productCount = stoi(tokens[4]);
                    if (productCount > 0) {
                        stringstream productSs(tokens[5]);
                        string productToken;
                        while (getline(productSs, productToken, ',')) {
                            category.addProduct(stoi(productToken));
                        }
                    }
                }
                
                // Parse subcategory IDs
                if (tokens.size() > 6) {
                    int subcategoryCount = stoi(tokens[6]);
                    if (subcategoryCount > 0) {
                        stringstream subcategorySs(tokens[7]);
                        string subcategoryToken;
                        while (getline(subcategorySs, subcategoryToken, ',')) {
                            category.addSubcategory(stoi(subcategoryToken));
                        }
                    }
                }
                
                categories.push_back(category);
            } else if (section == "REVIEWS" && tokens.size() >= 10) {
                int reviewId = stoi(tokens[0]);
                int productId = stoi(tokens[1]);
                int customerId = stoi(tokens[2]);
                int rating = stoi(tokens[3]);
                string title = tokens[4];
                string comment = tokens[5];
                
                date reviewDate;
                reviewDate.year = stoi(tokens[6]);
                reviewDate.month = stoi(tokens[7]);
                reviewDate.day = stoi(tokens[8]);
                
                bool isVerifiedPurchase = (tokens[9] == "1");
                
                Review review(reviewId, productId, customerId, rating, title, comment,
                               reviewDate, isVerifiedPurchase);
                
                // Parse pros
                if (tokens.size() > 10) {
                    int proCount = stoi(tokens[10]);
                    if (proCount > 0) {
                        stringstream proSs(tokens[11]);
                        string proToken;
                        while (getline(proSs, proToken, ',')) {
                            review.addPro(proToken);
                        }
                    }
                }
                
                // Parse cons
                if (tokens.size() > 12) {
                    int conCount = stoi(tokens[12]);
                    if (conCount > 0) {
                        stringstream conSs(tokens[13]);
                        string conToken;
                        while (getline(conSs, conToken, ',')) {
                            review.addCon(conToken);
                        }
                    }
                }
                
                reviews.push_back(review);
            }
        }
        
        inFile.close();
        return true;
    }
    
    // Sample data generation
    void generateSampleData() {
        // Clear existing data
        products.clear();
        customers.clear();
        orders.clear();
        carts.clear();
        categories.clear();
        reviews.clear();
        
        nextProductId = 1;
        nextCustomerId = 1;
        nextOrderId = 1;
        nextCartId = 1;
        nextCategoryId = 1;
        nextReviewId = 1;
        
        // Create sample categories
        vector<Category> sampleCategories = {
            Category(getNextCategoryId(), "Electronics", "Electronic devices and accessories", 0),
            Category(getNextCategoryId(), "Computers", "Laptops, desktops, and accessories", 1),
            Category(getNextCategoryId(), "Mobile Phones", "Smartphones and accessories", 1),
            Category(getNextCategoryId(), "Home & Kitchen", "Home appliances and kitchenware", 0),
            Category(getNextCategoryId(), "Clothing", "Fashion and apparel", 0),
            Category(getNextCategoryId(), "Men's Clothing", "Men's fashion", 5),
            Category(getNextCategoryId(), "Women's Clothing", "Women's fashion", 5)
        };
        
        for (const auto& category : sampleCategories) {
            addCategory(category);
            
            // Add subcategories to parent categories
            if (category.getParentId() > 0) {
                Category* parent = findCategory(category.getParentId());
                if (parent) {
                    parent->addSubcategory(category.getCategoryId());
                }
            }
        }
        
        // Create sample products
        vector<Product> sampleProducts = {
            Product(getNextProductId(), "Laptop Pro 15", "High-performance laptop with 15-inch display", 
                   1299.99, 50, IN_STOCK, 2, "TechBrand", "LP15", 2.3),
            Product(getNextProductId(), "Smartphone X", "Latest flagship smartphone with advanced features", 
                   899.99, 75, IN_STOCK, 3, "PhoneCo", "PX2023", 0.2),
            Product(getNextProductId(), "Bluetooth Headphones", "Wireless noise-canceling headphones", 
                   199.99, 100, IN_STOCK, 1, "AudioTech", "BTH100", 0.3),
            Product(getNextProductId(), "Coffee Maker Deluxe", "Programmable coffee maker with grinder", 
                   149.99, 30, IN_STOCK, 4, "KitchenPro", "CMD200", 4.5),
            Product(getNextProductId(), "Men's Casual Shirt", "Comfortable cotton shirt for everyday wear", 
                   39.99, 60, IN_STOCK, 6, "StyleWear", "MCS001", 0.2),
            Product(getNextProductId(), "Women's Summer Dress", "Light and airy summer dress", 
                   59.99, 40, IN_STOCK, 7, "FashionHub", "WSD015", 0.15),
            Product(getNextProductId(), "Gaming Laptop Elite", "High-end gaming laptop with RGB keyboard", 
                   1799.99, 25, IN_STOCK, 2, "GameTech", "GLE300", 2.8),
            Product(getNextProductId(), "Tablet Pro", "10-inch tablet with stylus support", 
                   599.99, 45, IN_STOCK, 1, "TechBrand", "TP10", 0.5)
        };
        
        for (auto& product : sampleProducts) {
            addProduct(product);
            
            // Add product to category
            Category* category = findCategory(product.getCategoryId());
            if (category) {
                category->addProduct(product.getProductId());
            }
        }
        
        // Create sample customers
        vector<Customer> sampleCustomers = {
            Customer(getNextCustomerId(), "John", "Doe", "john.doe@email.com", "password123", 
                    "555-1234", "123 Main St", "Anytown", "CA", "12345", "USA", 
                    {2023, 1, 15}, {2023, 11, 7}, true),
            Customer(getNextCustomerId(), "Jane", "Smith", "jane.smith@email.com", "password456", 
                    "555-5678", "456 Oak Ave", "Somecity", "NY", "67890", "USA", 
                    {2023, 2, 20}, {2023, 11, 5}, true),
            Customer(getNextCustomerId(), "Robert", "Johnson", "robert.johnson@email.com", "password789", 
                    "555-9012", "789 Pine Rd", "Otherplace", "TX", "24680", "USA", 
                    {2023, 3, 10}, {2023, 11, 1}, true),
            Customer(getNextCustomerId(), "Emily", "Brown", "emily.brown@email.com", "password012", 
                    "555-3456", "321 Elm St", "Anycity", "FL", "13579", "USA", 
                    {2023, 4, 5}, {2023, 10, 28}, true),
            Customer(getNextCustomerId(), "Michael", "Davis", "michael.davis@email.com", "password345", 
                    "555-7890", "654 Maple Dr", "Townsville", "WA", "97531", "USA", 
                    {2023, 5, 12}, {2023, 11, 2}, true)
        };
        
        for (auto& customer : sampleCustomers) {
            addCustomer(customer);
        }
        
        // Create sample carts
        for (auto& customer : sampleCustomers) {
            time_t now = time(0);
            tm* timeinfo = localtime(&now);
            date today = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
            
            Cart cart(getNextCartId(), customer.getCustomerId(), today, today, true);
            addCart(cart);
            
            customer.setCartId(cart.getCartId());
        }
        
        // Create sample orders
        vector<Order> sampleOrders = {
            Order(getNextOrderId(), 1, {2023, 10, 15}, {2023, 10, 16}, {2023, 10, 20}, 
                  DELIVERED, CREDIT_CARD, 1299.99, 104.0, 0.0, 1403.99, 
                  "TRK1102523456", "123 Main St, Anytown, CA 12345", 
                  "123 Main St, Anytown, CA 12345", "Please leave at front door"),
            Order(getNextOrderId(), 2, {2023, 10, 20}, {2023, 10, 21}, {2023, 10, 25}, 
                  DELIVERED, PAYPAL, 899.99, 72.0, 0.0, 971.99, 
                  "TRK2102523457", "456 Oak Ave, Somecity, NY 67890", 
                  "456 Oak Ave, Somecity, NY 67890", ""),
            Order(getNextOrderId(), 3, {2023, 11, 1}, {2023, 11, 2}, {0, 0, 0}, 
                  PROCESSING, DEBIT_CARD, 239.98, 19.2, 10.0, 269.18, 
                  "", "789 Pine Rd, Otherplace, TX 24680", 
                  "789 Pine Rd, Otherplace, TX 24680", "Call before delivery"),
            Order(getNextOrderId(), 4, {2023, 11, 5}, {0, 0, 0}, {0, 0, 0}, 
                  PENDING, CASH_ON_DELIVERY, 199.99, 16.0, 10.0, 225.99, 
                  "", "321 Elm St, Anycity, FL 13579", 
                  "321 Elm St, Anycity, FL 13579", "")
        };
        
        for (auto& order : sampleOrders) {
            // Add items to orders
            if (order.getOrderId() == 1) {
                order.addItem(1, "Laptop Pro 15", 1, 1299.99);
            } else if (order.getOrderId() == 2) {
                order.addItem(2, "Smartphone X", 1, 899.99);
            } else if (order.getOrderId() == 3) {
                order.addItem(3, "Bluetooth Headphones", 1, 199.99);
                order.addItem(5, "Men's Casual Shirt", 1, 39.99);
            } else if (order.getOrderId() == 4) {
                order.addItem(3, "Bluetooth Headphones", 1, 199.99);
            }
            
            // Calculate totals
            order.calculateTotals();
            
            addOrder(order);
            
            // Add order to customer
            Customer* customer = findCustomer(order.getCustomerId());
            if (customer) {
                customer->addOrder(order.getOrderId());
            }
        }
        
        // Create sample reviews
        vector<Review> sampleReviews = {
            Review(getNextReviewId(), 1, 1, 5, "Excellent laptop!", "Great performance and beautiful display.", 
                   {2023, 10, 25}, true),
            Review(getNextReviewId(), 2, 2, 4, "Good phone", "Solid phone but battery could be better.", 
                   {2023, 10, 30}, true),
            Review(getNextReviewId(), 3, 3, 5, "Great headphones", "Amazing sound quality and noise cancellation.", 
                   {2023, 11, 3}, true),
            Review(getNextReviewId(), 1, 4, 5, "Worth every penny", "Perfect for work and gaming.", 
                   {2023, 11, 5}, true)
        };
        
        for (auto& review : sampleReviews) {
            addReview(review);
            
            // Add review to product
            Product* product = findProduct(review.getProductId());
            if (product) {
                product->addReview(review.getReviewId());
            }
        }
        
        cout << "Sample data generated successfully:" << endl;
        cout << "Categories: " << categories.size() << endl;
        cout << "Products: " << products.size() << endl;
        cout << "Customers: " << customers.size() << endl;
        cout << "Carts: " << carts.size() << endl;
        cout << "Orders: " << orders.size() << endl;
        cout << "Reviews: " << reviews.size() << endl;
    }
};

// Function prototypes
void displayMenu();
void handleMenuChoice(ECommerceSystem& system, int choice);
void productMenu(ECommerceSystem& system);
void customerMenu(ECommerceSystem& system);
void orderMenu(ECommerceSystem& system);
void cartMenu(ECommerceSystem& system);
void categoryMenu(ECommerceSystem& system);
void reviewMenu(ECommerceSystem& system);
void reportMenu(ECommerceSystem& system);
void fileMenu(ECommerceSystem& system);

int main() {
    ECommerceSystem system;
    
    cout << "E-Commerce Management System" << endl;
    cout << "Initializing system..." << endl;
    
    // Try to load data from file, or generate sample data if file doesn't exist
    if (!system.loadFromFile("ecommerce_data.txt")) {
        cout << "No existing data found. Generating sample data..." << endl;
        system.generateSampleData();
        system.saveToFile("ecommerce_data.txt");
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
    system.saveToFile("ecommerce_data.txt");
    cout << "Data saved. Exiting program." << endl;
    
    return 0;
}

void displayMenu() {
    cout << "E-Commerce Management System" << endl;
    cout << "1. Product Management" << endl;
    cout << "2. Customer Management" << endl;
    cout << "3. Order Management" << endl;
    cout << "4. Cart Management" << endl;
    cout << "5. Category Management" << endl;
    cout << "6. Review Management" << endl;
    cout << "7. Reports" << endl;
    cout << "8. File Operations" << endl;
    cout << "0. Exit" << endl;
}

void handleMenuChoice(ECommerceSystem& system, int choice) {
    switch (choice) {
        case 1:
            productMenu(system);
            break;
        case 2:
            customerMenu(system);
            break;
        case 3:
            orderMenu(system);
            break;
        case 4:
            cartMenu(system);
            break;
        case 5:
            categoryMenu(system);
            break;
        case 6:
            reviewMenu(system);
            break;
        case 7:
            reportMenu(system);
            break;
        case 8:
            fileMenu(system);
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
    }
}

void productMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Product Management" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Remove Product" << endl;
        cout << "3. List All Products" << endl;
        cout << "4. Search Products by Name" << endl;
        cout << "5. List Products by Category" << endl;
        cout << "6. List In-Stock Products" << endl;
        cout << "7. List Top Selling Products" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string name, description, brand, model;
                double price, weight;
                int quantity, categoryId;
                
                cout << "Enter Product Name: ";
                getline(cin, name);
                
                cout << "Enter Description: ";
                getline(cin, description);
                
                cout << "Enter Price: ";
                cin >> price;
                
                cout << "Enter Quantity: ";
                cin >> quantity;
                
                cout << "Enter Category ID: ";
                cin >> categoryId;
                
                cout << "Enter Brand: ";
                getline(cin, brand);
                
                cout << "Enter Model: ";
                getline(cin, model);
                
                cout << "Enter Weight (kg): ";
                cin >> weight;
                
                ProductStatus status = (quantity > 0) ? IN_STOCK : OUT_OF_STOCK;
                
                Product product(system.getNextProductId(), name, description, price, quantity, 
                               status, categoryId, brand, model, weight);
                
                if (system.addProduct(product)) {
                    cout << "Product added successfully." << endl;
                    
                    // Add product to category
                    Category* category = system.findCategory(categoryId);
                    if (category) {
                        category->addProduct(product.getProductId());
                    }
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
                vector<Product*> products = system.getAllProducts();
                if (products.empty()) {
                    cout << "No products found." << endl;
                } else {
                    cout << "Products (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice()
                             << ", Quantity: " << product->getQuantity()
                             << ", Status: " << product->getStatusString() << endl;
                    }
                }
                break;
            }
            case 4: {
                string name;
                cout << "Enter product name to search: ";
                getline(cin, name);
                
                vector<Product*> products = system.searchProductsByName(name);
                if (products.empty()) {
                    cout << "No products found matching name." << endl;
                } else {
                    cout << "Products matching '" << name << "' (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice() << endl;
                    }
                }
                break;
            }
            case 5: {
                int categoryId;
                cout << "Enter Category ID: ";
                cin >> categoryId;
                
                vector<Product*> products = system.getProductsByCategory(categoryId);
                if (products.empty()) {
                    cout << "No products found for this category." << endl;
                } else {
                    cout << "Products in Category " << categoryId << " (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Price: $" << fixed << setprecision(2) << product->getPrice() << endl;
                    }
                }
                break;
            }
            case 6: {
                vector<Product*> products = system.getInStockProducts();
                if (products.empty()) {
                    cout << "No in-stock products found." << endl;
                } else {
                    cout << "In-Stock Products (" << products.size() << "):" << endl;
                    for (Product* product : products) {
                        cout << "ID: " << product->getProductId()
                             << ", Name: " << product->getName()
                             << ", Quantity: " << product->getQuantity() << endl;
                    }
                }
                break;
            }
            case 7: {
                int limit;
                cout << "Enter number of top products to display: ";
                cin >> limit;
                
                vector<Product*> products = system.getTopSellingProducts(limit);
                if (products.empty()) {
                    cout << "No top selling products found." << endl;
                } else {
                    cout << "Top " << products.size() << " Selling Products:" << endl;
                    for (int i = 0; i < products.size(); i++) {
                        Product* product = products[i];
                        int salesCount = 0;
                        
                        vector<Order*> allOrders = system.getAllOrders();
                        for (Order* order : allOrders) {
                            for (const OrderItem& item : order->getItems()) {
                                if (item.getProductId() == product->getProductId()) {
                                    salesCount += item.getQuantity();
                                }
                            }
                        }
                        
                        cout << (i+1) << ". " << product->getName()
                             << " (ID: " << product->getProductId() << ")"
                             << " - " << salesCount << " units sold" << endl;
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

void customerMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Customer Management" << endl;
        cout << "1. Add Customer" << endl;
        cout << "2. Remove Customer" << endl;
        cout << "3. List All Customers" << endl;
        cout << "4. Search Customers by Name" << endl;
        cout << "5. List Active Customers" << endl;
        cout << "6. List Top Customers by Orders" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                string firstName, lastName, email, password, phone, address, city, state, zipCode, country;
                
                cout << "Enter First Name: ";
                getline(cin, firstName);
                
                cout << "Enter Last Name: ";
                getline(cin, lastName);
                
                cout << "Enter Email: ";
                getline(cin, email);
                
                cout << "Enter Password: ";
                getline(cin, password);
                
                cout << "Enter Phone: ";
                getline(cin, phone);
                
                cout << "Enter Address: ";
                getline(cin, address);
                
                cout << "Enter City: ";
                getline(cin, city);
                
                cout << "Enter State: ";
                getline(cin, state);
                
                cout << "Enter Zip Code: ";
                getline(cin, zipCode);
                
                cout << "Enter Country: ";
                getline(cin, country);
                
                // Get current date for registration date
                time_t now = time(0);
                tm* timeinfo = localtime(&now);
                date registrationDate = {timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday};
                
                Customer customer(system.getNextCustomerId(), firstName, lastName, email, password, 
                                phone, address, city, state, zipCode, country, registrationDate);
                
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
                             << ", Email: " << customer->getEmail()
                             << ", Orders: " << customer->getOrderCount() << endl;
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
                    cout << "No customers found matching name." << endl;
                } else {
                    cout << "Customers matching '" << name << "' (" << customers.size() << "):" << endl;
                    for (Customer* customer : customers) {
                        cout << "ID: " << customer->getCustomerId()
                             << ", Name: " << customer->getFullName()
                             << ", Email: " << customer->getEmail() << endl;
                    }
                }
                break;
            }
            case 5: {
                vector<Customer*> customers = system.getActiveCustomers();
                if (customers.empty()) {
                    cout << "No active customers found." << endl;
                } else {
                    cout << "Active Customers (" << customers.size() << "):" << endl;
                    for (Customer* customer : customers) {
                        cout << "ID: " << customer->getCustomerId()
                             << ", Name: " << customer->getFullName()
                             << ", Orders: " << customer->getOrderCount() << endl;
                    }
                }
                break;
            }
            case 6: {
                int limit;
                cout << "Enter number of top customers to display: ";
                cin >> limit;
                
                vector<Customer*> customers = system.getTopCustomersByOrders(limit);
                if (customers.empty()) {
                    cout << "No top customers found." << endl;
                } else {
                    cout << "Top " << customers.size() << " Customers by Order Count:" << endl;
                    for (int i = 0; i < customers.size(); i++) {
                        Customer* customer = customers[i];
                        cout << (i+1) << ". " << customer->getFullName()
                             << " (ID: " << customer->getCustomerId() << ")"
                             << " - " << customer->getOrderCount() << " orders" << endl;
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

void orderMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Order Management" << endl;
        cout << "1. List All Orders" << endl;
        cout << "2. List Orders by Customer" << endl;
        cout << "3. List Orders by Status" << endl;
        cout << "4. List Orders by Date" << endl;
        cout << "5. List Recent Orders" << endl;
        cout << "6. Update Order Status" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                vector<Order*> orders = system.getAllOrders();
                if (orders.empty()) {
                    cout << "No orders found." << endl;
                } else {
                    cout << "Orders (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Date: " << order->getOrderDate().year 
                             << "-" << order->getOrderDate().month 
                             << "-" << order->getOrderDate().day
                             << ", Status: " << order->getStatusString()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotal() << endl;
                    }
                }
                break;
            }
            case 2: {
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
                             << ", Date: " << order->getOrderDate().year 
                             << "-" << order->getOrderDate().month 
                             << "-" << order->getOrderDate().day
                             << ", Status: " << order->getStatusString()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotal() << endl;
                    }
                }
                break;
            }
            case 3: {
                int statusChoice;
                cout << "Enter Status:" << endl;
                cout << "1. Pending" << endl;
                cout << "2. Processing" << endl;
                cout << "3. Shipped" << endl;
                cout << "4. Delivered" << endl;
                cout << "5. Cancelled" << endl;
                cout << "6. Refunded" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                OrderStatus status = static_cast<OrderStatus>(statusChoice);
                
                vector<Order*> orders = system.getOrdersByStatus(status);
                if (orders.empty()) {
                    Order dummyOrder;
                    cout << "No " << dummyOrder.getStatusStringStatic(status) << " orders found." << endl;
                } else {
                    Order dummyOrder;
                    cout << dummyOrder.getStatusStringStatic(status) << " Orders (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Date: " << order->getOrderDate().year 
                             << "-" << order->getOrderDate().month 
                             << "-" << order->getOrderDate().day << endl;
                    }
                }
                break;
            }
            case 4: {
                int year, month, day;
                cout << "Enter Date (YYYY MM DD): ";
                cin >> year >> month >> day;
                
                vector<Order*> orders = system.getOrdersByDate(year, month, day);
                if (orders.empty()) {
                    cout << "No orders found for this date." << endl;
                } else {
                    cout << "Orders for " << year << "-" << month << "-" << day << " (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Status: " << order->getStatusString()
                             << ", Total: $" << fixed << setprecision(2) << order->getTotal() << endl;
                    }
                }
                break;
            }
            case 5: {
                int days;
                cout << "Enter number of days: ";
                cin >> days;
                
                vector<Order*> orders = system.getRecentOrders(days);
                if (orders.empty()) {
                    cout << "No recent orders found." << endl;
                } else {
                    cout << "Orders from last " << days << " days (" << orders.size() << "):" << endl;
                    for (Order* order : orders) {
                        cout << "ID: " << order->getOrderId()
                             << ", Customer ID: " << order->getCustomerId()
                             << ", Date: " << order->getOrderDate().year 
                             << "-" << order->getOrderDate().month 
                             << "-" << order->getOrderDate().day
                             << ", Status: " << order->getStatusString() << endl;
                    }
                }
                break;
            }
            case 6: {
                int orderId, statusChoice;
                cout << "Enter Order ID: ";
                cin >> orderId;
                
                cout << "Enter New Status:" << endl;
                cout << "1. Pending" << endl;
                cout << "2. Processing" << endl;
                cout << "3. Shipped" << endl;
                cout << "4. Delivered" << endl;
                cout << "5. Cancelled" << endl;
                cout << "6. Refunded" << endl;
                cout << "Enter choice: ";
                cin >> statusChoice;
                
                OrderStatus status = static_cast<OrderStatus>(statusChoice);
                
                if (system.updateOrderStatus(orderId, status)) {
                    cout << "Order status updated successfully." << endl;
                } else {
                    cout << "Failed to update order status. Order ID may not exist." << endl;
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

void cartMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Cart Management" << endl;
        cout << "1. Add Item to Cart" << endl;
        cout << "2. Update Cart Item Quantity" << endl;
        cout << "3. Remove Item from Cart" << endl;
        cout << "4. Clear Cart" << endl;
        cout << "5. View Cart" << endl;
        cout << "6. Checkout" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int customerId, productId, quantity;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                cout << "Enter Product ID: ";
                cin >> productId;
                
                cout << "Enter Quantity: ";
                cin >> quantity;
                
                if (system.addToCart(customerId, productId, quantity)) {
                    cout << "Item added to cart successfully." << endl;
                } else {
                    cout << "Failed to add item to cart. Product may not exist or out of stock." << endl;
                }
                break;
            }
            case 2: {
                int customerId, productId, quantity;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                cout << "Enter Product ID: ";
                cin >> productId;
                
                cout << "Enter New Quantity: ";
                cin >> quantity;
                
                if (system.updateCartItemQuantity(customerId, productId, quantity)) {
                    cout << "Cart item quantity updated successfully." << endl;
                } else {
                    cout << "Failed to update cart item quantity." << endl;
                }
                break;
            }
            case 3: {
                int customerId, productId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                cout << "Enter Product ID to remove: ";
                cin >> productId;
                
                if (system.removeFromCart(customerId, productId)) {
                    cout << "Item removed from cart successfully." << endl;
                } else {
                    cout << "Failed to remove item from cart." << endl;
                }
                break;
            }
            case 4: {
                int customerId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                if (system.clearCart(customerId)) {
                    cout << "Cart cleared successfully." << endl;
                } else {
                    cout << "Failed to clear cart." << endl;
                }
                break;
            }
            case 5: {
                int customerId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                Cart* cart = system.findCartByCustomer(customerId);
                if (cart) {
                    cart->display();
                } else {
                    cout << "Cart not found for this customer." << endl;
                }
                break;
            }
            case 6: {
                int customerId, paymentMethod;
                string shippingAddress, billingAddress, notes;
                
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cin.ignore();
                
                cout << "Enter Shipping Address: ";
                getline(cin, shippingAddress);
                
                cout << "Enter Billing Address: ";
                getline(cin, billingAddress);
                
                cout << "Enter Notes (optional): ";
                getline(cin, notes);
                
                cout << "Enter Payment Method:" << endl;
                cout << "1. Credit Card" << endl;
                cout << "2. Debit Card" << endl;
                cout << "3. PayPal" << endl;
                cout << "4. Bank Transfer" << endl;
                cout << "5. Cash on Delivery" << endl;
                cout << "Enter choice: ";
                cin >> paymentMethod;
                
                PaymentMethod method = static_cast<PaymentMethod>(paymentMethod);
                
                if (system.checkout(customerId, method, shippingAddress, billingAddress, notes)) {
                    cout << "Checkout successful." << endl;
                } else {
                    cout << "Failed to checkout. Cart may be empty." << endl;
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

void categoryMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Category Management" << endl;
        cout << "1. Add Category" << endl;
        cout << "2. Remove Category" << endl;
        cout << "3. List All Categories" << endl;
        cout << "4. List Root Categories" << endl;
        cout << "5. List Subcategories" << endl;
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
                
                cout << "Enter Description: ";
                getline(cin, description);
                
                cout << "Enter Parent Category ID (0 for root): ";
                cin >> parentId;
                
                Category category(system.getNextCategoryId(), name, description, parentId);
                
                if (system.addCategory(category)) {
                    cout << "Category added successfully." << endl;
                    
                    // Add as subcategory to parent if not root
                    if (parentId > 0) {
                        Category* parent = system.findCategory(parentId);
                        if (parent) {
                            parent->addSubcategory(category.getCategoryId());
                        }
                    }
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
                             << ", Parent ID: " << category->getParentId()
                             << ", Products: " << category->getProductIds().size() << endl;
                    }
                }
                break;
            }
            case 4: {
                vector<Category*> categories = system.getRootCategories();
                if (categories.empty()) {
                    cout << "No root categories found." << endl;
                } else {
                    cout << "Root Categories (" << categories.size() << "):" << endl;
                    for (Category* category : categories) {
                        cout << "ID: " << category->getCategoryId()
                             << ", Name: " << category->getName()
                             << ", Subcategories: " << category->getSubcategoryIds().size() << endl;
                    }
                }
                break;
            }
            case 5: {
                int parentId;
                cout << "Enter Parent Category ID: ";
                cin >> parentId;
                
                vector<Category*> categories = system.getSubcategories(parentId);
                if (categories.empty()) {
                    cout << "No subcategories found for this parent." << endl;
                } else {
                    cout << "Subcategories of Category " << parentId << " (" << categories.size() << "):" << endl;
                    for (Category* category : categories) {
                        cout << "ID: " << category->getCategoryId()
                             << ", Name: " << category->getName()
                             << ", Products: " << category->getProductIds().size() << endl;
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

void reviewMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Review Management" << endl;
        cout << "1. Add Product Review" << endl;
        cout << "2. List All Reviews" << endl;
        cout << "3. List Reviews by Product" << endl;
        cout << "4. List Reviews by Customer" << endl;
        cout << "5. List Reviews by Rating" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int customerId, productId, rating;
                string title, comment;
                
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                cout << "Enter Product ID: ";
                cin >> productId;
                
                cout << "Enter Rating (1-5): ";
                cin >> rating;
                cin.ignore();
                
                cout << "Enter Review Title: ";
                getline(cin, title);
                
                cout << "Enter Review Comment: ";
                getline(cin, comment);
                
                if (system.addProductReview(customerId, productId, rating, title, comment)) {
                    cout << "Review added successfully." << endl;
                } else {
                    cout << "Failed to add review. Customer may not have purchased the product or has already reviewed it." << endl;
                }
                break;
            }
            case 2: {
                vector<Review*> reviews = system.getAllReviews();
                if (reviews.empty()) {
                    cout << "No reviews found." << endl;
                } else {
                    cout << "Reviews (" << reviews.size() << "):" << endl;
                    for (Review* review : reviews) {
                        cout << "ID: " << review->getReviewId()
                             << ", Product ID: " << review->getProductId()
                             << ", Customer ID: " << review->getCustomerId()
                             << ", Rating: " << review->getRating() << "/5"
                             << ", Title: " << review->getTitle() << endl;
                    }
                }
                break;
            }
            case 3: {
                int productId;
                cout << "Enter Product ID: ";
                cin >> productId;
                
                vector<Review*> reviews = system.getReviewsByProduct(productId);
                if (reviews.empty()) {
                    cout << "No reviews found for this product." << endl;
                } else {
                    cout << "Reviews for Product " << productId << " (" << reviews.size() << "):" << endl;
                    
                    double avgRating = system.getAverageRating(productId);
                    cout << "Average Rating: " << fixed << setprecision(2) << avgRating << "/5" << endl;
                    
                    for (Review* review : reviews) {
                        cout << "ID: " << review->getReviewId()
                             << ", Customer ID: " << review->getCustomerId()
                             << ", Rating: " << review->getRating() << "/5"
                             << ", Title: " << review->getTitle() << endl;
                    }
                }
                break;
            }
            case 4: {
                int customerId;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                
                vector<Review*> reviews = system.getReviewsByCustomer(customerId);
                if (reviews.empty()) {
                    cout << "No reviews found for this customer." << endl;
                } else {
                    cout << "Reviews by Customer " << customerId << " (" << reviews.size() << "):" << endl;
                    for (Review* review : reviews) {
                        cout << "ID: " << review->getReviewId()
                             << ", Product ID: " << review->getProductId()
                             << ", Rating: " << review->getRating() << "/5"
                             << ", Title: " << review->getTitle() << endl;
                    }
                }
                break;
            }
            case 5: {
                int minRating, maxRating;
                cout << "Enter Minimum Rating (1-5): ";
                cin >> minRating;
                
                cout << "Enter Maximum Rating (1-5): ";
                cin >> maxRating;
                
                vector<Review*> reviews = system.getReviewsByRating(minRating, maxRating);
                if (reviews.empty()) {
                    cout << "No reviews found in this rating range." << endl;
                } else {
                    cout << "Reviews with Rating " << minRating << "-" << maxRating << " (" << reviews.size() << "):" << endl;
                    for (Review* review : reviews) {
                        cout << "ID: " << review->getReviewId()
                             << ", Product ID: " << review->getProductId()
                             << ", Customer ID: " << review->getCustomerId()
                             << ", Title: " << review->getTitle() << endl;
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

void reportMenu(ECommerceSystem& system) {
    int choice;
    do {
        cout << "Reports" << endl;
        cout << "1. Generate Sales Report" << endl;
        cout << "2. Generate Product Report" << endl;
        cout << "3. Generate Customer Report" << endl;
        cout << "4. Generate Review Report" << endl;
        cout << "0. Back to Main Menu" << endl;
        
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1: {
                int year, month;
                cout << "Enter Year: ";
                cin >> year;
                
                cout << "Enter Month: ";
                cin >> month;
                
                system.generateSalesReport(year, month);
                break;
            }
            case 2: {
                system.generateProductReport();
                break;
            }
            case 3: {
                system.generateCustomerReport();
                break;
            }
            case 4: {
                system.generateReviewReport();
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

void fileMenu(ECommerceSystem& system) {
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
                    filename = "ecommerce_data.txt";
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
                    filename = "ecommerce_data.txt";
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
                cout << "Sample data generated." << endl;
                
                // Save the sample data
                if (system.saveToFile("ecommerce_data.txt")) {
                    cout << "Sample data saved." << endl;
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
}
<｜fim▁hole｜>