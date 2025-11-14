/**
 * E-commerce Platform System
 * 
 * This class implements a comprehensive e-commerce platform
 * that can handle products, customers, orders, payments, and inventory management.
 * 
 * Features:
 * - Product catalog management with categories and variants
 * - Customer registration and management
 * - Shopping cart functionality
 * - Order processing and tracking
 * - Payment processing simulation
 * - Inventory management
 * - Shipping and handling
 * - Reviews and ratings
 * - Promotions and discounts
 */

import java.util.*;
import java.text.*;
import java.io.*;
import java.time.LocalDate;
import java.time.temporal.ChronoUnit;

/**
 * Product class represents a product in the e-commerce platform
 */
class Product {
    private String productId;
    private String name;
    private String description;
    private String category;
    private double price;
    private double salePrice;
    private String currency;
    private boolean onSale;
    private List<String> tags;
    private String brand;
    private String model;
    private int stockQuantity;
    private int minStockLevel;
    private double weight;
    private String dimensions;
    private String sku;
    private List<String> images;
    private Map<String, ProductVariant> variants;
    private List<ProductReview> reviews;
    private double averageRating;
    private int reviewCount;
    private String status;  // ACTIVE, INACTIVE, DISCONTINUED
    private Date createdDate;
    private Date updatedDate;
    private boolean taxable;
    private boolean shippable;
    private double shippingCost;
    
    /**
     * Constructor for Product
     * @param productId Product ID
     * @param name Product name
     * @param description Product description
     * @param category Product category
     * @param price Regular price
     * @param currency Currency code
     * @param brand Product brand
     * @param model Product model
     * @param stockQuantity Stock quantity
     * @param minStockLevel Minimum stock level
     * @param weight Product weight
     * @param dimensions Product dimensions
     * @param sku Product SKU
     */
    public Product(String productId, String name, String description, String category,
                  double price, String currency, String brand, String model,
                  int stockQuantity, int minStockLevel, double weight, String dimensions, String sku) {
        this.productId = productId;
        this.name = name;
        this.description = description;
        this.category = category;
        this.price = price;
        this.salePrice = price;  // Default sale price equals regular price
        this.currency = currency;
        this.onSale = false;
        this.tags = new ArrayList<>();
        this.brand = brand;
        this.model = model;
        this.stockQuantity = stockQuantity;
        this.minStockLevel = minStockLevel;
        this.weight = weight;
        this.dimensions = dimensions;
        this.sku = sku;
        this.images = new ArrayList<>();
        this.variants = new HashMap<>();
        this.reviews = new ArrayList<>();
        this.averageRating = 0.0;
        this.reviewCount = 0;
        this.status = "ACTIVE";
        this.createdDate = new Date();
        this.updatedDate = new Date();
        this.taxable = true;
        this.shippable = true;
        this.shippingCost = 0.0;
    }
    
    /**
     * Get product ID
     * @return Product ID
     */
    public String getProductId() {
        return productId;
    }
    
    /**
     * Get product name
     * @return Product name
     */
    public String getName() {
        return name;
    }
    
    /**
     * Get product description
     * @return Product description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get product category
     * @return Product category
     */
    public String getCategory() {
        return category;
    }
    
    /**
     * Get regular price
     * @return Regular price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get sale price
     * @return Sale price
     */
    public double getSalePrice() {
        return salePrice;
    }
    
    /**
     * Get current price (sale price if on sale, otherwise regular price)
     * @return Current price
     */
    public double getCurrentPrice() {
        return onSale ? salePrice : price;
    }
    
    /**
     * Get currency
     * @return Currency code
     */
    public String getCurrency() {
        return currency;
    }
    
    /**
     * Check if product is on sale
     * @return True if on sale, false otherwise
     */
    public boolean isOnSale() {
        return onSale;
    }
    
    /**
     * Get tags
     * @return List of product tags
     */
    public List<String> getTags() {
        return tags;
    }
    
    /**
     * Get brand
     * @return Product brand
     */
    public String getBrand() {
        return brand;
    }
    
    /**
     * Get model
     * @return Product model
     */
    public String getModel() {
        return model;
    }
    
    /**
     * Get stock quantity
     * @return Stock quantity
     */
    public int getStockQuantity() {
        return stockQuantity;
    }
    
    /**
     * Get minimum stock level
     * @return Minimum stock level
     */
    public int getMinStockLevel() {
        return minStockLevel;
    }
    
    /**
     * Get weight
     * @return Product weight
     */
    public double getWeight() {
        return weight;
    }
    
    /**
     * Get dimensions
     * @return Product dimensions
     */
    public String getDimensions() {
        return dimensions;
    }
    
    /**
     * Get SKU
     * @return Product SKU
     */
    public String getSku() {
        return sku;
    }
    
    /**
     * Get images
     * @return List of product images
     */
    public List<String> getImages() {
        return images;
    }
    
    /**
     * Get variants
     * @return Map of product variants
     */
    public Map<String, ProductVariant> getVariants() {
        return variants;
    }
    
    /**
     * Get reviews
     * @return List of product reviews
     */
    public List<ProductReview> getReviews() {
        return reviews;
    }
    
    /**
     * Get average rating
     * @return Average rating
     */
    public double getAverageRating() {
        return averageRating;
    }
    
    /**
     * Get review count
     * @return Number of reviews
     */
    public int getReviewCount() {
        return reviewCount;
    }
    
    /**
     * Get status
     * @return Product status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get created date
     * @return Product creation date
     */
    public Date getCreatedDate() {
        return createdDate;
    }
    
    /**
     * Get updated date
     * @return Product last update date
     */
    public Date getUpdatedDate() {
        return updatedDate;
    }
    
    /**
     * Check if product is taxable
     * @return True if taxable, false otherwise
     */
    public boolean isTaxable() {
        return taxable;
    }
    
    /**
     * Check if product is shippable
     * @return True if shippable, false otherwise
     */
    public boolean isShippable() {
        return shippable;
    }
    
    /**
     * Get shipping cost
     * @return Shipping cost
     */
    public double getShippingCost() {
        return shippingCost;
    }
    
    /**
     * Update product name
     * @param name New product name
     */
    public void updateName(String name) {
        this.name = name;
        this.updatedDate = new Date();
    }
    
    /**
     * Update product description
     * @param description New product description
     */
    public void updateDescription(String description) {
        this.description = description;
        this.updatedDate = new Date();
    }
    
    /**
     * Update product price
     * @param price New regular price
     */
    public void updatePrice(double price) {
        this.price = price;
        if (!onSale) {
            this.salePrice = price;
        }
        this.updatedDate = new Date();
    }
    
    /**
     * Put product on sale
     * @param salePrice Sale price
     */
    public void putOnSale(double salePrice) {
        this.salePrice = salePrice;
        this.onSale = true;
        this.updatedDate = new Date();
    }
    
    /**
     * Remove product from sale
     */
    public void removeFromSale() {
        this.onSale = false;
        this.salePrice = price;
        this.updatedDate = new Date();
    }
    
    /**
     * Update stock quantity
     * @param quantity New stock quantity
     */
    public void updateStockQuantity(int quantity) {
        this.stockQuantity = quantity;
        this.updatedDate = new Date();
    }
    
    /**
     * Add stock
     * @param quantity Quantity to add
     */
    public void addStock(int quantity) {
        this.stockQuantity += quantity;
        this.updatedDate = new Date();
    }
    
    /**
     * Reduce stock
     * @param quantity Quantity to reduce
     * @return True if successful, false if insufficient stock
     */
    public boolean reduceStock(int quantity) {
        if (this.stockQuantity >= quantity) {
            this.stockQuantity -= quantity;
            this.updatedDate = new Date();
            return true;
        }
        return false;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
        this.updatedDate = new Date();
    }
    
    /**
     * Add tag
     * @param tag Tag to add
     */
    public void addTag(String tag) {
        if (!tags.contains(tag)) {
            tags.add(tag);
            this.updatedDate = new Date();
        }
    }
    
    /**
     * Remove tag
     * @param tag Tag to remove
     */
    public void removeTag(String tag) {
        if (tags.remove(tag)) {
            this.updatedDate = new Date();
        }
    }
    
    /**
     * Add image
     * @param imageUrl Image URL to add
     */
    public void addImage(String imageUrl) {
        if (!images.contains(imageUrl)) {
            images.add(imageUrl);
            this.updatedDate = new Date();
        }
    }
    
    /**
     * Remove image
     * @param imageUrl Image URL to remove
     */
    public void removeImage(String imageUrl) {
        if (images.remove(imageUrl)) {
            this.updatedDate = new Date();
        }
    }
    
    /**
     * Add variant
     * @param variantId Variant ID
     * @param variant Product variant
     */
    public void addVariant(String variantId, ProductVariant variant) {
        variants.put(variantId, variant);
        this.updatedDate = new Date();
    }
    
    /**
     * Remove variant
     * @param variantId Variant ID to remove
     */
    public void removeVariant(String variantId) {
        if (variants.remove(variantId) != null) {
            this.updatedDate = new Date();
        }
    }
    
    /**
     * Add review
     * @param review Product review
     */
    public void addReview(ProductReview review) {
        reviews.add(review);
        updateAverageRating();
        this.updatedDate = new Date();
    }
    
    /**
     * Update average rating
     */
    private void updateAverageRating() {
        if (reviews.isEmpty()) {
            averageRating = 0.0;
            reviewCount = 0;
            return;
        }
        
        double totalRating = 0.0;
        for (ProductReview review : reviews) {
            totalRating += review.getRating();
        }
        
        averageRating = totalRating / reviews.size();
        reviewCount = reviews.size();
    }
    
    /**
     * Check if stock is low
     * @return True if stock is below minimum level
     */
    public boolean isLowStock() {
        return stockQuantity <= minStockLevel;
    }
    
    /**
     * Check if product is in stock
     * @return True if product is in stock
     */
    public boolean isInStock() {
        return stockQuantity > 0;
    }
    
    /**
     * Get product summary
     * @return Product summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Product ID: ").append(productId).append("\n");
        sb.append("Name: ").append(name).append("\n");
        sb.append("Description: ").append(description).append("\n");
        sb.append("Category: ").append(category).append("\n");
        sb.append("Brand: ").append(brand).append("\n");
        sb.append("Model: ").append(model).append("\n");
        sb.append("SKU: ").append(sku).append("\n");
        
        if (onSale) {
            sb.append("Price: ").append(currency).append(String.format("%.2f", salePrice))
              .append(" (was ").append(currency).append(String.format("%.2f", price)).append(")\n");
        } else {
            sb.append("Price: ").append(currency).append(String.format("%.2f", price)).append("\n");
        }
        
        sb.append("Stock Quantity: ").append(stockQuantity).append("\n");
        sb.append("Min Stock Level: ").append(minStockLevel).append("\n");
        sb.append("Weight: ").append(weight).append(" kg\n");
        sb.append("Dimensions: ").append(dimensions).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Created Date: ").append(sdf.format(createdDate)).append("\n");
        sb.append("Updated Date: ").append(sdf.format(updatedDate)).append("\n");
        sb.append("Average Rating: ").append(String.format("%.1f", averageRating)).append("/5.0 (")
          .append(reviewCount).append(" reviews)\n");
        
        if (!tags.isEmpty()) {
            sb.append("Tags: ").append(String.join(", ", tags)).append("\n");
        }
        
        if (!images.isEmpty()) {
            sb.append("Images: ").append(images.size()).append("\n");
        }
        
        if (!variants.isEmpty()) {
            sb.append("Variants: ").append(variants.size()).append("\n");
        }
        
        sb.append("Taxable: ").append(taxable ? "Yes" : "No").append("\n");
        sb.append("Shippable: ").append(shippable ? "Yes" : "No").append("\n");
        
        if (shippingCost > 0) {
            sb.append("Shipping Cost: ").append(currency).append(String.format("%.2f", shippingCost)).append("\n");
        }
        
        if (isLowStock()) {
            sb.append("WARNING: Low Stock!\n");
        }
        
        return sb.toString();
    }
}

/**
 * ProductVariant class represents a product variant
 */
class ProductVariant {
    private String variantId;
    private String name;
    private Map<String, String> attributes;
    private double price;
    private int stockQuantity;
    private String sku;
    private List<String> images;
    
    /**
     * Constructor for ProductVariant
     * @param variantId Variant ID
     * @param name Variant name
     * @param price Variant price
     * @param stockQuantity Variant stock quantity
     * @param sku Variant SKU
     */
    public ProductVariant(String variantId, String name, double price,
                        int stockQuantity, String sku) {
        this.variantId = variantId;
        this.name = name;
        this.attributes = new HashMap<>();
        this.price = price;
        this.stockQuantity = stockQuantity;
        this.sku = sku;
        this.images = new ArrayList<>();
    }
    
    /**
     * Get variant ID
     * @return Variant ID
     */
    public String getVariantId() {
        return variantId;
    }
    
    /**
     * Get variant name
     * @return Variant name
     */
    public String getName() {
        return name;
    }
    
    /**
     * Get attributes
     * @return Map of variant attributes
     */
    public Map<String, String> getAttributes() {
        return attributes;
    }
    
    /**
     * Get price
     * @return Variant price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get stock quantity
     * @return Stock quantity
     */
    public int getStockQuantity() {
        return stockQuantity;
    }
    
    /**
     * Get SKU
     * @return Variant SKU
     */
    public String getSku() {
        return sku;
    }
    
    /**
     * Get images
     * @return List of variant images
     */
    public List<String> getImages() {
        return images;
    }
    
    /**
     * Add attribute
     * @param key Attribute key
     * @param value Attribute value
     */
    public void addAttribute(String key, String value) {
        attributes.put(key, value);
    }
    
    /**
     * Remove attribute
     * @param key Attribute key to remove
     */
    public void removeAttribute(String key) {
        attributes.remove(key);
    }
    
    /**
     * Update price
     * @param price New price
     */
    public void updatePrice(double price) {
        this.price = price;
    }
    
    /**
     * Update stock quantity
     * @param quantity New stock quantity
     */
    public void updateStockQuantity(int quantity) {
        this.stockQuantity = quantity;
    }
    
    /**
     * Add stock
     * @param quantity Quantity to add
     */
    public void addStock(int quantity) {
        this.stockQuantity += quantity;
    }
    
    /**
     * Reduce stock
     * @param quantity Quantity to reduce
     * @return True if successful, false if insufficient stock
     */
    public boolean reduceStock(int quantity) {
        if (this.stockQuantity >= quantity) {
            this.stockQuantity -= quantity;
            return true;
        }
        return false;
    }
    
    /**
     * Add image
     * @param imageUrl Image URL to add
     */
    public void addImage(String imageUrl) {
        if (!images.contains(imageUrl)) {
            images.add(imageUrl);
        }
    }
    
    /**
     * Remove image
     * @param imageUrl Image URL to remove
     */
    public void removeImage(String imageUrl) {
        images.remove(imageUrl);
    }
    
    /**
     * Check if variant is in stock
     * @return True if variant is in stock
     */
    public boolean isInStock() {
        return stockQuantity > 0;
    }
    
    /**
     * Get variant summary
     * @return Variant summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Variant ID: ").append(variantId).append("\n");
        sb.append("Name: ").append(name).append("\n");
        
        for (Map.Entry<String, String> entry : attributes.entrySet()) {
            sb.append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("Price: ").append(String.format("%.2f", price)).append("\n");
        sb.append("Stock Quantity: ").append(stockQuantity).append("\n");
        sb.append("SKU: ").append(sku).append("\n");
        
        if (!images.isEmpty()) {
            sb.append("Images: ").append(images.size()).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * ProductReview class represents a product review
 */
class ProductReview {
    private String reviewId;
    private String productId;
    private String customerId;
    private String customerName;
    private int rating;
    private String title;
    private String content;
    private Date reviewDate;
    private boolean verified;
    private boolean approved;
    private int helpfulCount;
    private int notHelpfulCount;
    
    /**
     * Constructor for ProductReview
     * @param reviewId Review ID
     * @param productId Product ID
     * @param customerId Customer ID
     * @param customerName Customer name
     * @param rating Review rating (1-5)
     * @param title Review title
     * @param content Review content
     */
    public ProductReview(String reviewId, String productId, String customerId,
                        String customerName, int rating, String title, String content) {
        this.reviewId = reviewId;
        this.productId = productId;
        this.customerId = customerId;
        this.customerName = customerName;
        this.rating = rating;
        this.title = title;
        this.content = content;
        this.reviewDate = new Date();
        this.verified = false;
        this.approved = false;
        this.helpfulCount = 0;
        this.notHelpfulCount = 0;
    }
    
    /**
     * Get review ID
     * @return Review ID
     */
    public String getReviewId() {
        return reviewId;
    }
    
    /**
     * Get product ID
     * @return Product ID
     */
    public String getProductId() {
        return productId;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get customer name
     * @return Customer name
     */
    public String getCustomerName() {
        return customerName;
    }
    
    /**
     * Get rating
     * @return Review rating
     */
    public int getRating() {
        return rating;
    }
    
    /**
     * Get title
     * @return Review title
     */
    public String getTitle() {
        return title;
    }
    
    /**
     * Get content
     * @return Review content
     */
    public String getContent() {
        return content;
    }
    
    /**
     * Get review date
     * @return Review date
     */
    public Date getReviewDate() {
        return reviewDate;
    }
    
    /**
     * Check if review is verified
     * @return True if verified, false otherwise
     */
    public boolean isVerified() {
        return verified;
    }
    
    /**
     * Check if review is approved
     * @return True if approved, false otherwise
     */
    public boolean isApproved() {
        return approved;
    }
    
    /**
     * Get helpful count
     * @return Number of helpful votes
     */
    public int getHelpfulCount() {
        return helpfulCount;
    }
    
    /**
     * Get not helpful count
     * @return Number of not helpful votes
     */
    public int getNotHelpfulCount() {
        return notHelpfulCount;
    }
    
    /**
     * Verify review
     */
    public void verifyReview() {
        this.verified = true;
    }
    
    /**
     * Approve review
     */
    public void approveReview() {
        this.approved = true;
    }
    
    /**
     * Reject review
     */
    public void rejectReview() {
        this.approved = false;
    }
    
    /**
     * Mark as helpful
     */
    public void markAsHelpful() {
        this.helpfulCount++;
    }
    
    /**
     * Mark as not helpful
     */
    public void markAsNotHelpful() {
        this.notHelpfulCount++;
    }
    
    /**
     * Get review summary
     * @return Review summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Review ID: ").append(reviewId).append("\n");
        sb.append("Product ID: ").append(productId).append("\n");
        sb.append("Customer: ").append(customerName).append(" (").append(customerId).append(")\n");
        sb.append("Rating: ").append(rating).append("/5\n");
        sb.append("Title: ").append(title).append("\n");
        sb.append("Content: ").append(content).append("\n");
        sb.append("Date: ").append(sdf.format(reviewDate)).append("\n");
        sb.append("Verified: ").append(verified ? "Yes" : "No").append("\n");
        sb.append("Approved: ").append(approved ? "Yes" : "No").append("\n");
        sb.append("Helpful: ").append(helpfulCount).append(" | Not Helpful: ").append(notHelpfulCount).append("\n");
        
        return sb.toString();
    }
}

/**
 * Customer class represents a customer in the e-commerce platform
 */
class Customer {
    private String customerId;
    private String firstName;
    private String lastName;
    private String email;
    private String password;
    private String phone;
    private Date registrationDate;
    private Date lastLoginDate;
    private String status;  // ACTIVE, INACTIVE, SUSPENDED
    private String membershipTier;  // BRONZE, SILVER, GOLD, PLATINUM
    private double accountBalance;
    private List<Address> addresses;
    private Address defaultAddress;
    private List<PaymentMethod> paymentMethods;
    private PaymentMethod defaultPaymentMethod;
    private List<String> favoriteProducts;
    private List<String> recentlyViewed;
    private ShoppingCart shoppingCart;
    private List<Order> orderHistory;
    private List<WishlistItem> wishlist;
    private double totalSpent;
    private int orderCount;
    private Map<String, String> preferences;
    private List<Coupon> coupons;
    
    /**
     * Constructor for Customer
     * @param customerId Customer ID
     * @param firstName First name
     * @param lastName Last name
     * @param email Email address
     * @param password Password
     * @param phone Phone number
     */
    public Customer(String customerId, String firstName, String lastName,
                   String email, String password, String phone) {
        this.customerId = customerId;
        this.firstName = firstName;
        this.lastName = lastName;
        this.email = email;
        this.password = password;
        this.phone = phone;
        this.registrationDate = new Date();
        this.lastLoginDate = null;
        this.status = "ACTIVE";
        this.membershipTier = "BRONZE";
        this.accountBalance = 0.0;
        this.addresses = new ArrayList<>();
        this.defaultAddress = null;
        this.paymentMethods = new ArrayList<>();
        this.defaultPaymentMethod = null;
        this.favoriteProducts = new ArrayList<>();
        this.recentlyViewed = new ArrayList<>();
        this.shoppingCart = new ShoppingCart(customerId);
        this.orderHistory = new ArrayList<>();
        this.wishlist = new ArrayList<>();
        this.totalSpent = 0.0;
        this.orderCount = 0;
        this.preferences = new HashMap<>();
        this.coupons = new ArrayList<>();
        
        // Set default preferences
        preferences.put("emailNotifications", "true");
        preferences.put("smsNotifications", "false");
        preferences.put("promotionalEmails", "true");
        preferences.put("currency", "USD");
        preferences.put("language", "ENGLISH");
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get first name
     * @return First name
     */
    public String getFirstName() {
        return firstName;
    }
    
    /**
     * Get last name
     * @return Last name
     */
    public String getLastName() {
        return lastName;
    }
    
    /**
     * Get full name
     * @return Full name
     */
    public String getFullName() {
        return firstName + " " + lastName;
    }
    
    /**
     * Get email
     * @return Email address
     */
    public String getEmail() {
        return email;
    }
    
    /**
     * Get password
     * @return Password
     */
    public String getPassword() {
        return password;
    }
    
    /**
     * Get phone
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Get registration date
     * @return Registration date
     */
    public Date getRegistrationDate() {
        return registrationDate;
    }
    
    /**
     * Get last login date
     * @return Last login date
     */
    public Date getLastLoginDate() {
        return lastLoginDate;
    }
    
    /**
     * Get status
     * @return Customer status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get membership tier
     * @return Membership tier
     */
    public String getMembershipTier() {
        return membershipTier;
    }
    
    /**
     * Get account balance
     * @return Account balance
     */
    public double getAccountBalance() {
        return accountBalance;
    }
    
    /**
     * Get addresses
     * @return List of addresses
     */
    public List<Address> getAddresses() {
        return addresses;
    }
    
    /**
     * Get default address
     * @return Default address
     */
    public Address getDefaultAddress() {
        return defaultAddress;
    }
    
    /**
     * Get payment methods
     * @return List of payment methods
     */
    public List<PaymentMethod> getPaymentMethods() {
        return paymentMethods;
    }
    
    /**
     * Get default payment method
     * @return Default payment method
     */
    public PaymentMethod getDefaultPaymentMethod() {
        return defaultPaymentMethod;
    }
    
    /**
     * Get favorite products
     * @return List of favorite product IDs
     */
    public List<String> getFavoriteProducts() {
        return favoriteProducts;
    }
    
    /**
     * Get recently viewed
     * @return List of recently viewed product IDs
     */
    public List<String> getRecentlyViewed() {
        return recentlyViewed;
    }
    
    /**
     * Get shopping cart
     * @return Shopping cart
     */
    public ShoppingCart getShoppingCart() {
        return shoppingCart;
    }
    
    /**
     * Get order history
     * @return List of orders
     */
    public List<Order> getOrderHistory() {
        return orderHistory;
    }
    
    /**
     * Get wishlist
     * @return List of wishlist items
     */
    public List<WishlistItem> getWishlist() {
        return wishlist;
    }
    
    /**
     * Get total spent
     * @return Total amount spent
     */
    public double getTotalSpent() {
        return totalSpent;
    }
    
    /**
     * Get order count
     * @return Number of orders
     */
    public int getOrderCount() {
        return orderCount;
    }
    
    /**
     * Get preferences
     * @return Map of preferences
     */
    public Map<String, String> getPreferences() {
        return preferences;
    }
    
    /**
     * Get coupons
     * @return List of coupons
     */
    public List<Coupon> getCoupons() {
        return coupons;
    }
    
    /**
     * Update first name
     * @param firstName New first name
     */
    public void updateFirstName(String firstName) {
        this.firstName = firstName;
    }
    
    /**
     * Update last name
     * @param lastName New last name
     */
    public void updateLastName(String lastName) {
        this.lastName = lastName;
    }
    
    /**
     * Update email
     * @param email New email
     */
    public void updateEmail(String email) {
        this.email = email;
    }
    
    /**
     * Update password
     * @param password New password
     */
    public void updatePassword(String password) {
        this.password = password;
    }
    
    /**
     * Update phone
     * @param phone New phone
     */
    public void updatePhone(String phone) {
        this.phone = phone;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
    }
    
    /**
     * Update membership tier
     * @param tier New membership tier
     */
    public void updateMembershipTier(String tier) {
        this.membershipTier = tier;
    }
    
    /**
     * Update account balance
     * @param amount Amount to add (can be negative)
     */
    public void updateAccountBalance(double amount) {
        this.accountBalance += amount;
    }
    
    /**
     * Update last login date
     */
    public void updateLastLoginDate() {
        this.lastLoginDate = new Date();
    }
    
    /**
     * Add address
     * @param address Address to add
     */
    public void addAddress(Address address) {
        addresses.add(address);
        
        // If this is the first address, set as default
        if (defaultAddress == null) {
            defaultAddress = address;
        }
    }
    
    /**
     * Remove address
     * @param addressId Address ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeAddress(String addressId) {
        for (int i = 0; i < addresses.size(); i++) {
            if (addresses.get(i).getAddressId().equals(addressId)) {
                Address removed = addresses.remove(i);
                
                // If removed was default, set new default
                if (removed == defaultAddress && !addresses.isEmpty()) {
                    defaultAddress = addresses.get(0);
                } else if (removed == defaultAddress && addresses.isEmpty()) {
                    defaultAddress = null;
                }
                
                return true;
            }
        }
        return false;
    }
    
    /**
     * Set default address
     * @param addressId Address ID to set as default
     * @return True if set successfully, false otherwise
     */
    public boolean setDefaultAddress(String addressId) {
        for (Address address : addresses) {
            if (address.getAddressId().equals(addressId)) {
                defaultAddress = address;
                return true;
            }
        }
        return false;
    }
    
    /**
     * Add payment method
     * @param paymentMethod Payment method to add
     */
    public void addPaymentMethod(PaymentMethod paymentMethod) {
        paymentMethods.add(paymentMethod);
        
        // If this is the first payment method, set as default
        if (defaultPaymentMethod == null) {
            defaultPaymentMethod = paymentMethod;
        }
    }
    
    /**
     * Remove payment method
     * @param paymentMethodId Payment method ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removePaymentMethod(String paymentMethodId) {
        for (int i = 0; i < paymentMethods.size(); i++) {
            if (paymentMethods.get(i).getPaymentMethodId().equals(paymentMethodId)) {
                PaymentMethod removed = paymentMethods.remove(i);
                
                // If removed was default, set new default
                if (removed == defaultPaymentMethod && !paymentMethods.isEmpty()) {
                    defaultPaymentMethod = paymentMethods.get(0);
                } else if (removed == defaultPaymentMethod && paymentMethods.isEmpty()) {
                    defaultPaymentMethod = null;
                }
                
                return true;
            }
        }
        return false;
    }
    
    /**
     * Set default payment method
     * @param paymentMethodId Payment method ID to set as default
     * @return True if set successfully, false otherwise
     */
    public boolean setDefaultPaymentMethod(String paymentMethodId) {
        for (PaymentMethod paymentMethod : paymentMethods) {
            if (paymentMethod.getPaymentMethodId().equals(paymentMethodId)) {
                defaultPaymentMethod = paymentMethod;
                return true;
            }
        }
        return false;
    }
    
    /**
     * Add favorite product
     * @param productId Product ID to add
     */
    public void addFavoriteProduct(String productId) {
        if (!favoriteProducts.contains(productId)) {
            favoriteProducts.add(productId);
        }
    }
    
    /**
     * Remove favorite product
     * @param productId Product ID to remove
     */
    public void removeFavoriteProduct(String productId) {
        favoriteProducts.remove(productId);
    }
    
    /**
     * Add to recently viewed
     * @param productId Product ID to add
     */
    public void addToRecentlyViewed(String productId) {
        // Remove if already in list
        recentlyViewed.remove(productId);
        
        // Add to front
        recentlyViewed.add(0, productId);
        
        // Limit to 20 items
        if (recentlyViewed.size() > 20) {
            recentlyViewed.remove(recentlyViewed.size() - 1);
        }
    }
    
    /**
     * Add order to history
     * @param order Order to add
     */
    public void addOrder(Order order) {
        orderHistory.add(order);
        orderCount++;
        totalSpent += order.getTotalAmount();
        
        // Update membership tier based on total spent
        updateMembershipTierBasedOnSpending();
    }
    
    /**
     * Add item to wishlist
     * @param wishlistItem Wishlist item to add
     */
    public void addToWishlist(WishlistItem wishlistItem) {
        // Check if item already exists
        for (WishlistItem item : wishlist) {
            if (item.getProductId().equals(wishlistItem.getProductId())) {
                // Update existing item
                item.updateQuantity(wishlistItem.getQuantity());
                item.updateNote(wishlistItem.getNote());
                return;
            }
        }
        
        // Add new item
        wishlist.add(wishlistItem);
    }
    
    /**
     * Remove item from wishlist
     * @param productId Product ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeFromWishlist(String productId) {
        for (int i = 0; i < wishlist.size(); i++) {
            if (wishlist.get(i).getProductId().equals(productId)) {
                wishlist.remove(i);
                return true;
            }
        }
        return false;
    }
    
    /**
     * Add coupon
     * @param coupon Coupon to add
     */
    public void addCoupon(Coupon coupon) {
        coupons.add(coupon);
    }
    
    /**
     * Remove coupon
     * @param couponId Coupon ID to remove
     * @return True if removed successfully, false otherwise
     */
    public boolean removeCoupon(String couponId) {
        for (int i = 0; i < coupons.size(); i++) {
            if (coupons.get(i).getCouponId().equals(couponId)) {
                coupons.remove(i);
                return true;
            }
        }
        return false;
    }
    
    /**
     * Update preference
     * @param key Preference key
     * @param value Preference value
     */
    public void updatePreference(String key, String value) {
        preferences.put(key, value);
    }
    
    /**
     * Update membership tier based on total spending
     */
    private void updateMembershipTierBasedOnSpending() {
        if (totalSpent >= 1000.0) {
            membershipTier = "PLATINUM";
        } else if (totalSpent >= 500.0) {
            membershipTier = "GOLD";
        } else if (totalSpent >= 100.0) {
            membershipTier = "SILVER";
        } else {
            membershipTier = "BRONZE";
        }
    }
    
    /**
     * Get customer summary
     * @return Customer summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Name: ").append(getFullName()).append("\n");
        sb.append("Email: ").append(email).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Registration Date: ").append(sdf.format(registrationDate)).append("\n");
        
        if (lastLoginDate != null) {
            sb.append("Last Login Date: ").append(sdf.format(lastLoginDate)).append("\n");
        }
        
        sb.append("Status: ").append(status).append("\n");
        sb.append("Membership Tier: ").append(membershipTier).append("\n");
        sb.append("Account Balance: $").append(String.format("%.2f", accountBalance)).append("\n");
        sb.append("Total Spent: $").append(String.format("%.2f", totalSpent)).append("\n");
        sb.append("Order Count: ").append(orderCount).append("\n");
        sb.append("Addresses: ").append(addresses.size()).append("\n");
        sb.append("Payment Methods: ").append(paymentMethods.size()).append("\n");
        sb.append("Favorite Products: ").append(favoriteProducts.size()).append("\n");
        sb.append("Shopping Cart Items: ").append(shoppingCart.getItems().size()).append("\n");
        sb.append("Wishlist Items: ").append(wishlist.size()).append("\n");
        sb.append("Available Coupons: ").append(coupons.size()).append("\n");
        
        if (defaultAddress != null) {
            sb.append("Default Address: ").append(defaultAddress.getSummary()).append("\n");
        }
        
        if (defaultPaymentMethod != null) {
            sb.append("Default Payment Method: ").append(defaultPaymentMethod.getSummary()).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Address class represents a customer address
 */
class Address {
    private String addressId;
    private String type;  // SHIPPING, BILLING, BOTH
    private String recipientName;
    private String street;
    private String city;
    private String state;
    private String postalCode;
    private String country;
    private String phone;
    private boolean isDefault;
    
    /**
     * Constructor for Address
     * @param addressId Address ID
     * @param type Address type
     * @param recipientName Recipient name
     * @param street Street address
     * @param city City
     * @param state State
     * @param postalCode Postal code
     * @param country Country
     * @param phone Phone number
     */
    public Address(String addressId, String type, String recipientName,
                  String street, String city, String state, String postalCode,
                  String country, String phone) {
        this.addressId = addressId;
        this.type = type;
        this.recipientName = recipientName;
        this.street = street;
        this.city = city;
        this.state = state;
        this.postalCode = postalCode;
        this.country = country;
        this.phone = phone;
        this.isDefault = false;
    }
    
    /**
     * Get address ID
     * @return Address ID
     */
    public String getAddressId() {
        return addressId;
    }
    
    /**
     * Get address type
     * @return Address type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get recipient name
     * @return Recipient name
     */
    public String getRecipientName() {
        return recipientName;
    }
    
    /**
     * Get street
     * @return Street address
     */
    public String getStreet() {
        return street;
    }
    
    /**
     * Get city
     * @return City
     */
    public String getCity() {
        return city;
    }
    
    /**
     * Get state
     * @return State
     */
    public String getState() {
        return state;
    }
    
    /**
     * Get postal code
     * @return Postal code
     */
    public String getPostalCode() {
        return postalCode;
    }
    
    /**
     * Get country
     * @return Country
     */
    public String getCountry() {
        return country;
    }
    
    /**
     * Get phone
     * @return Phone number
     */
    public String getPhone() {
        return phone;
    }
    
    /**
     * Check if address is default
     * @return True if default, false otherwise
     */
    public boolean isDefault() {
        return isDefault;
    }
    
    /**
     * Set as default
     */
    public void setAsDefault() {
        this.isDefault = true;
    }
    
    /**
     * Unset as default
     */
    public void unsetAsDefault() {
        this.isDefault = false;
    }
    
    /**
     * Get full address
     * @return Full address string
     */
    public String getFullAddress() {
        return street + ", " + city + ", " + state + " " + postalCode + ", " + country;
    }
    
    /**
     * Get address summary
     * @return Address summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append(recipientName).append("\n");
        sb.append(street).append("\n");
        sb.append(city).append(", ").append(state).append(" ").append(postalCode).append("\n");
        sb.append(country).append("\n");
        sb.append("Phone: ").append(phone).append("\n");
        sb.append("Type: ").append(type).append("\n");
        
        if (isDefault) {
            sb.append("(Default)\n");
        }
        
        return sb.toString();
    }
}

/**
 * PaymentMethod class represents a payment method
 */
class PaymentMethod {
    private String paymentMethodId;
    private String type;  // CREDIT_CARD, DEBIT_CARD, PAYPAL, BANK_TRANSFER
    private String nickname;
    private String cardNumber;
    private String cardholderName;
    private String expiryDate;
    private String cvv;
    private String bankName;
    private String accountNumber;
    private String routingNumber;
    private String paypalEmail;
    private boolean isDefault;
    private Date addedDate;
    
    /**
     * Constructor for PaymentMethod (Credit/Debit Card)
     * @param paymentMethodId Payment method ID
     * @param type Payment method type
     * @param nickname Nickname
     * @param cardNumber Card number
     * @param cardholderName Cardholder name
     * @param expiryDate Expiry date
     * @param cvv CVV
     */
    public PaymentMethod(String paymentMethodId, String type, String nickname,
                       String cardNumber, String cardholderName, String expiryDate, String cvv) {
        this.paymentMethodId = paymentMethodId;
        this.type = type;
        this.nickname = nickname;
        this.cardNumber = cardNumber;
        this.cardholderName = cardholderName;
        this.expiryDate = expiryDate;
        this.cvv = cvv;
        this.bankName = "";
        this.accountNumber = "";
        this.routingNumber = "";
        this.paypalEmail = "";
        this.isDefault = false;
        this.addedDate = new Date();
    }
    
    /**
     * Constructor for PaymentMethod (Bank Transfer)
     * @param paymentMethodId Payment method ID
     * @param type Payment method type
     * @param nickname Nickname
     * @param bankName Bank name
     * @param accountNumber Account number
     * @param routingNumber Routing number
     */
    public PaymentMethod(String paymentMethodId, String type, String nickname,
                       String bankName, String accountNumber, String routingNumber) {
        this.paymentMethodId = paymentMethodId;
        this.type = type;
        this.nickname = nickname;
        this.cardNumber = "";
        this.cardholderName = "";
        this.expiryDate = "";
        this.cvv = "";
        this.bankName = bankName;
        this.accountNumber = accountNumber;
        this.routingNumber = routingNumber;
        this.paypalEmail = "";
        this.isDefault = false;
        this.addedDate = new Date();
    }
    
    /**
     * Constructor for PaymentMethod (PayPal)
     * @param paymentMethodId Payment method ID
     * @param type Payment method type
     * @param nickname Nickname
     * @param paypalEmail PayPal email
     */
    public PaymentMethod(String paymentMethodId, String type, String nickname, String paypalEmail) {
        this.paymentMethodId = paymentMethodId;
        this.type = type;
        this.nickname = nickname;
        this.cardNumber = "";
        this.cardholderName = "";
        this.expiryDate = "";
        this.cvv = "";
        this.bankName = "";
        this.accountNumber = "";
        this.routingNumber = "";
        this.paypalEmail = paypalEmail;
        this.isDefault = false;
        this.addedDate = new Date();
    }
    
    /**
     * Get payment method ID
     * @return Payment method ID
     */
    public String getPaymentMethodId() {
        return paymentMethodId;
    }
    
    /**
     * Get payment method type
     * @return Payment method type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get nickname
     * @return Payment method nickname
     */
    public String getNickname() {
        return nickname;
    }
    
    /**
     * Get card number
     * @return Card number (last 4 digits)
     */
    public String getCardNumber() {
        if (cardNumber.isEmpty()) {
            return "";
        }
        
        // Return only last 4 digits
        if (cardNumber.length() > 4) {
            return "****-****-****-" + cardNumber.substring(cardNumber.length() - 4);
        }
        
        return cardNumber;
    }
    
    /**
     * Get cardholder name
     * @return Cardholder name
     */
    public String getCardholderName() {
        return cardholderName;
    }
    
    /**
     * Get expiry date
     * @return Card expiry date
     */
    public String getExpiryDate() {
        return expiryDate;
    }
    
    /**
     * Get bank name
     * @return Bank name
     */
    public String getBankName() {
        return bankName;
    }
    
    /**
     * Get account number
     * @return Account number (last 4 digits)
     */
    public String getAccountNumber() {
        if (accountNumber.isEmpty()) {
            return "";
        }
        
        // Return only last 4 digits
        if (accountNumber.length() > 4) {
            return "****" + accountNumber.substring(accountNumber.length() - 4);
        }
        
        return accountNumber;
    }
    
    /**
     * Get routing number
     * @return Routing number
     */
    public String getRoutingNumber() {
        return routingNumber;
    }
    
    /**
     * Get PayPal email
     * @return PayPal email
     */
    public String getPaypalEmail() {
        return paypalEmail;
    }
    
    /**
     * Check if payment method is default
     * @return True if default, false otherwise
     */
    public boolean isDefault() {
        return isDefault;
    }
    
    /**
     * Get added date
     * @return Date when payment method was added
     */
    public Date getAddedDate() {
        return addedDate;
    }
    
    /**
     * Set as default
     */
    public void setAsDefault() {
        this.isDefault = true;
    }
    
    /**
     * Unset as default
     */
    public void unsetAsDefault() {
        this.isDefault = false;
    }
    
    /**
     * Update nickname
     * @param nickname New nickname
     */
    public void updateNickname(String nickname) {
        this.nickname = nickname;
    }
    
    /**
     * Get payment method summary
     * @return Payment method summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append(nickname).append(" (");
        
        if (type.equals("CREDIT_CARD") || type.equals("DEBIT_CARD")) {
            sb.append(type.replace("_", " "));
            sb.append(" ending in ").append(getCardNumber().substring(getCardNumber().length() - 4));
            sb.append(", expires ").append(expiryDate);
        } else if (type.equals("BANK_TRANSFER")) {
            sb.append("Bank Transfer");
            sb.append(", account ending in ").append(getAccountNumber().substring(getAccountNumber().length() - 4));
        } else if (type.equals("PAYPAL")) {
            sb.append("PayPal");
            sb.append(", ").append(paypalEmail);
        }
        
        sb.append(")");
        
        if (isDefault) {
            sb.append(" (Default)");
        }
        
        return sb.toString();
    }
}

/**
 * ShoppingCart class represents a customer's shopping cart
 */
class ShoppingCart {
    private String customerId;
    private List<ShoppingCartItem> items;
    private Date createdDate;
    private Date lastUpdatedDate;
    private String status;  // ACTIVE, ABANDONED, CHECKED_OUT
    private String couponCode;
    private double discountAmount;
    private boolean validCoupon;
    private String couponMessage;
    
    /**
     * Constructor for ShoppingCart
     * @param customerId Customer ID
     */
    public ShoppingCart(String customerId) {
        this.customerId = customerId;
        this.items = new ArrayList<>();
        this.createdDate = new Date();
        this.lastUpdatedDate = new Date();
        this.status = "ACTIVE";
        this.couponCode = "";
        this.discountAmount = 0.0;
        this.validCoupon = false;
        this.couponMessage = "";
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get items
     * @return List of shopping cart items
     */
    public List<ShoppingCartItem> getItems() {
        return items;
    }
    
    /**
     * Get created date
     * @return Cart creation date
     */
    public Date getCreatedDate() {
        return createdDate;
    }
    
    /**
     * Get last updated date
     * @return Last update date
     */
    public Date getLastUpdatedDate() {
        return lastUpdatedDate;
    }
    
    /**
     * Get status
     * @return Cart status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get coupon code
     * @return Applied coupon code
     */
    public String getCouponCode() {
        return couponCode;
    }
    
    /**
     * Get discount amount
     * @return Discount amount
     */
    public double getDiscountAmount() {
        return discountAmount;
    }
    
    /**
     * Check if coupon is valid
     * @return True if coupon is valid, false otherwise
     */
    public boolean isValidCoupon() {
        return validCoupon;
    }
    
    /**
     * Get coupon message
     * @return Coupon message
     */
    public String getCouponMessage() {
        return couponMessage;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
        this.lastUpdatedDate = new Date();
    }
    
    /**
     * Add item to cart
     * @param productId Product ID
     * @param productName Product name
     * @param price Item price
     * @param quantity Item quantity
     * @param variantId Product variant ID (optional)
     * @return True if added successfully, false otherwise
     */
    public boolean addItem(String productId, String productName, double price, int quantity, String variantId) {
        // Check if item already exists
        for (ShoppingCartItem item : items) {
            if (item.getProductId().equals(productId) && 
                (variantId == null || variantId.equals(item.getVariantId()))) {
                // Update existing item
                item.updateQuantity(item.getQuantity() + quantity);
                this.lastUpdatedDate = new Date();
                return true;
            }
        }
        
        // Add new item
        ShoppingCartItem newItem = new ShoppingCartItem(
            "SCI" + (items.size() + 1), productId, productName, price, quantity, variantId);
        items.add(newItem);
        this.lastUpdatedDate = new Date();
        return true;
    }
    
    /**
     * Add item to cart (without variant)
     * @param productId Product ID
     * @param productName Product name
     * @param price Item price
     * @param quantity Item quantity
     * @return True if added successfully, false otherwise
     */
    public boolean addItem(String productId, String productName, double price, int quantity) {
        return addItem(productId, productName, price, quantity, null);
    }
    
    /**
     * Update item quantity
     * @param itemId Item ID
     * @param quantity New quantity
     * @return True if updated successfully, false otherwise
     */
    public boolean updateItemQuantity(String itemId, int quantity) {
        if (quantity <= 0) {
            return removeItem(itemId);
        }
        
        for (ShoppingCartItem item : items) {
            if (item.getItemId().equals(itemId)) {
                item.updateQuantity(quantity);
                this.lastUpdatedDate = new Date();
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * Remove item from cart
     * @param itemId Item ID
     * @return True if removed successfully, false otherwise
     */
    public boolean removeItem(String itemId) {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getItemId().equals(itemId)) {
                items.remove(i);
                this.lastUpdatedDate = new Date();
                return true;
            }
        }
        return false;
    }
    
    /**
     * Clear cart
     */
    public void clearCart() {
        items.clear();
        this.lastUpdatedDate = new Date();
        this.couponCode = "";
        this.discountAmount = 0.0;
        this.validCoupon = false;
        this.couponMessage = "";
    }
    
    /**
     * Apply coupon
     * @param couponCode Coupon code
     * @param discountAmount Discount amount
     * @param validCoupon Whether coupon is valid
     * @param message Coupon message
     */
    public void applyCoupon(String couponCode, double discountAmount, boolean validCoupon, String message) {
        this.couponCode = couponCode;
        this.discountAmount = discountAmount;
        this.validCoupon = validCoupon;
        this.couponMessage = message;
        this.lastUpdatedDate = new Date();
    }
    
    /**
     * Remove coupon
     */
    public void removeCoupon() {
        this.couponCode = "";
        this.discountAmount = 0.0;
        this.validCoupon = false;
        this.couponMessage = "";
        this.lastUpdatedDate = new Date();
    }
    
    /**
     * Get subtotal
     * @return Cart subtotal
     */
    public double getSubtotal() {
        double subtotal = 0.0;
        
        for (ShoppingCartItem item : items) {
            subtotal += item.getTotalPrice();
        }
        
        return subtotal;
    }
    
    /**
     * Get total after discount
     * @return Total after discount
     */
    public double getTotal() {
        double total = getSubtotal() - discountAmount;
        return Math.max(0.0, total);  // Ensure total is not negative
    }
    
    /**
     * Get item count
     * @return Number of items in cart
     */
    public int getItemCount() {
        return items.size();
    }
    
    /**
     * Get total quantity
     * @return Total quantity of all items
     */
    public int getTotalQuantity() {
        int totalQuantity = 0;
        
        for (ShoppingCartItem item : items) {
            totalQuantity += item.getQuantity();
        }
        
        return totalQuantity;
    }
    
    /**
     * Check if cart is empty
     * @return True if cart is empty, false otherwise
     */
    public boolean isEmpty() {
        return items.isEmpty();
    }
    
    /**
     * Get shopping cart summary
     * @return Shopping cart summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        
        sb.append("Customer ID: ").append(customerId).append("\n");
        sb.append("Status: ").append(status).append("\n");
        sb.append("Created: ").append(sdf.format(createdDate)).append("\n");
        sb.append("Last Updated: ").append(sdf.format(lastUpdatedDate)).append("\n");
        sb.append("Items: ").append(items.size()).append("\n");
        sb.append("Total Quantity: ").append(getTotalQuantity()).append("\n");
        sb.append("Subtotal: $").append(String.format("%.2f", getSubtotal())).append("\n");
        
        if (validCoupon && !couponCode.isEmpty()) {
            sb.append("Coupon: ").append(couponCode).append(" (-$").append(String.format("%.2f", discountAmount)).append(")\n");
        }
        
        sb.append("Total: $").append(String.format("%.2f", getTotal())).append("\n");
        
        if (!items.isEmpty()) {
            sb.append("\nCart Items:\n");
            for (ShoppingCartItem item : items) {
                sb.append("- ").append(item.getProductName())
                  .append(" (Qty: ").append(item.getQuantity())
                  .append(", $").append(String.format("%.2f", item.getTotalPrice())).append(")\n");
            }
        }
        
        return sb.toString();
    }
}

/**
 * ShoppingCartItem class represents an item in the shopping cart
 */
class ShoppingCartItem {
    private String itemId;
    private String productId;
    private String productName;
    private double price;
    private int quantity;
    private String variantId;
    private String variantName;
    
    /**
     * Constructor for ShoppingCartItem
     * @param itemId Item ID
     * @param productId Product ID
     * @param productName Product name
     * @param price Item price
     * @param quantity Item quantity
     * @param variantId Product variant ID (optional)
     */
    public ShoppingCartItem(String itemId, String productId, String productName,
                          double price, int quantity, String variantId) {
        this.itemId = itemId;
        this.productId = productId;
        this.productName = productName;
        this.price = price;
        this.quantity = quantity;
        this.variantId = variantId;
        this.variantName = "";
    }
    
    /**
     * Get item ID
     * @return Item ID
     */
    public String getItemId() {
        return itemId;
    }
    
    /**
     * Get product ID
     * @return Product ID
     */
    public String getProductId() {
        return productId;
    }
    
    /**
     * Get product name
     * @return Product name
     */
    public String getProductName() {
        return productName;
    }
    
    /**
     * Get price
     * @return Item price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get quantity
     * @return Item quantity
     */
    public int getQuantity() {
        return quantity;
    }
    
    /**
     * Get variant ID
     * @return Product variant ID
     */
    public String getVariantId() {
        return variantId;
    }
    
    /**
     * Get variant name
     * @return Product variant name
     */
    public String getVariantName() {
        return variantName;
    }
    
    /**
     * Get total price
     * @return Total price (price × quantity)
     */
    public double getTotalPrice() {
        return price * quantity;
    }
    
    /**
     * Update quantity
     * @param quantity New quantity
     */
    public void updateQuantity(int quantity) {
        this.quantity = quantity;
    }
    
    /**
     * Update variant name
     * @param variantName New variant name
     */
    public void updateVariantName(String variantName) {
        this.variantName = variantName;
    }
    
    /**
     * Get shopping cart item summary
     * @return Item summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Item ID: ").append(itemId).append("\n");
        sb.append("Product ID: ").append(productId).append("\n");
        sb.append("Product Name: ").append(productName).append("\n");
        sb.append("Price: $").append(String.format("%.2f", price)).append("\n");
        sb.append("Quantity: ").append(quantity).append("\n");
        sb.append("Total: $").append(String.format("%.2f", getTotalPrice())).append("\n");
        
        if (variantId != null && !variantId.isEmpty()) {
            sb.append("Variant ID: ").append(variantId).append("\n");
            
            if (!variantName.isEmpty()) {
                sb.append("Variant Name: ").append(variantName).append("\n");
            }
        }
        
        return sb.toString();
    }
}

/**
 * WishlistItem class represents an item in a customer's wishlist
 */
class WishlistItem {
    private String wishlistItemId;
    private String productId;
    private String productName;
    private double price;
    private int quantity;
    private String note;
    private Date addedDate;
    
    /**
     * Constructor for WishlistItem
     * @param wishlistItemId Wishlist item ID
     * @param productId Product ID
     * @param productName Product name
     * @param price Product price
     * @param quantity Desired quantity
     * @param note Note
     */
    public WishlistItem(String wishlistItemId, String productId, String productName,
                       double price, int quantity, String note) {
        this.wishlistItemId = wishlistItemId;
        this.productId = productId;
        this.productName = productName;
        this.price = price;
        this.quantity = quantity;
        this.note = note;
        this.addedDate = new Date();
    }
    
    /**
     * Get wishlist item ID
     * @return Wishlist item ID
     */
    public String getWishlistItemId() {
        return wishlistItemId;
    }
    
    /**
     * Get product ID
     * @return Product ID
     */
    public String getProductId() {
        return productId;
    }
    
    /**
     * Get product name
     * @return Product name
     */
    public String getProductName() {
        return productName;
    }
    
    /**
     * Get price
     * @return Product price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get quantity
     * @return Desired quantity
     */
    public int getQuantity() {
        return quantity;
    }
    
    /**
     * Get note
     * @return Item note
     */
    public String getNote() {
        return note;
    }
    
    /**
     * Get added date
     * @return Date when item was added
     */
    public Date getAddedDate() {
        return addedDate;
    }
    
    /**
     * Update quantity
     * @param quantity New quantity
     */
    public void updateQuantity(int quantity) {
        this.quantity = quantity;
    }
    
    /**
     * Update note
     * @param note New note
     */
    public void updateNote(String note) {
        this.note = note;
    }
    
    /**
     * Update price
     * @param price New price
     */
    public void updatePrice(double price) {
        this.price = price;
    }
    
    /**
     * Get total price
     * @return Total price (price × quantity)
     */
    public double getTotalPrice() {
        return price * quantity;
    }
    
    /**
     * Get wishlist item summary
     * @return Item summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Wishlist Item ID: ").append(wishlistItemId).append("\n");
        sb.append("Product ID: ").append(productId).append("\n");
        sb.append("Product Name: ").append(productName).append("\n");
        sb.append("Price: $").append(String.format("%.2f", price)).append("\n");
        sb.append("Quantity: ").append(quantity).append("\n");
        sb.append("Total: $").append(String.format("%.2f", getTotalPrice())).append("\n");
        sb.append("Added Date: ").append(sdf.format(addedDate)).append("\n");
        
        if (!note.trim().isEmpty()) {
            sb.append("Note: ").append(note).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Coupon class represents a discount coupon
 */
class Coupon {
    private String couponId;
    private String code;
    private String description;
    private String type;  // PERCENTAGE, FIXED_AMOUNT
    private double value;
    private double minOrderAmount;
    private Date startDate;
    private Date endDate;
    private int usageLimit;
    private int usedCount;
    private boolean isActive;
    private List<String> applicableCategories;
    private List<String> applicableProducts;
    private List<String> excludedCategories;
    private List<String> excludedProducts;
    
    /**
     * Constructor for Coupon
     * @param couponId Coupon ID
     * @param code Coupon code
     * @param description Coupon description
     * @param type Discount type
     * @param value Discount value
     * @param minOrderAmount Minimum order amount
     * @param startDate Start date
     * @param endDate End date
     * @param usageLimit Usage limit
     */
    public Coupon(String couponId, String code, String description, String type,
                 double value, double minOrderAmount, Date startDate, Date endDate, int usageLimit) {
        this.couponId = couponId;
        this.code = code;
        this.description = description;
        this.type = type;
        this.value = value;
        this.minOrderAmount = minOrderAmount;
        this.startDate = startDate;
        this.endDate = endDate;
        this.usageLimit = usageLimit;
        this.usedCount = 0;
        this.isActive = true;
        this.applicableCategories = new ArrayList<>();
        this.applicableProducts = new ArrayList<>();
        this.excludedCategories = new ArrayList<>();
        this.excludedProducts = new ArrayList<>();
    }
    
    /**
     * Get coupon ID
     * @return Coupon ID
     */
    public String getCouponId() {
        return couponId;
    }
    
    /**
     * Get coupon code
     * @return Coupon code
     */
    public String getCode() {
        return code;
    }
    
    /**
     * Get description
     * @return Coupon description
     */
    public String getDescription() {
        return description;
    }
    
    /**
     * Get discount type
     * @return Discount type
     */
    public String getType() {
        return type;
    }
    
    /**
     * Get discount value
     * @return Discount value
     */
    public double getValue() {
        return value;
    }
    
    /**
     * Get minimum order amount
     * @return Minimum order amount
     */
    public double getMinOrderAmount() {
        return minOrderAmount;
    }
    
    /**
     * Get start date
     * @return Start date
     */
    public Date getStartDate() {
        return startDate;
    }
    
    /**
     * Get end date
     * @return End date
     */
    public Date getEndDate() {
        return endDate;
    }
    
    /**
     * Get usage limit
     * @return Usage limit
     */
    public int getUsageLimit() {
        return usageLimit;
    }
    
    /**
     * Get used count
     * @return Number of times used
     */
    public int getUsedCount() {
        return usedCount;
    }
    
    /**
     * Check if coupon is active
     * @return True if active, false otherwise
     */
    public boolean isActive() {
        return isActive;
    }
    
    /**
     * Get applicable categories
     * @return List of applicable categories
     */
    public List<String> getApplicableCategories() {
        return applicableCategories;
    }
    
    /**
     * Get applicable products
     * @return List of applicable products
     */
    public List<String> getApplicableProducts() {
        return applicableProducts;
    }
    
    /**
     * Get excluded categories
     * @return List of excluded categories
     */
    public List<String> getExcludedCategories() {
        return excludedCategories;
    }
    
    /**
     * Get excluded products
     * @return List of excluded products
     */
    public List<String> getExcludedProducts() {
        return excludedProducts;
    }
    
    /**
     * Activate coupon
     */
    public void activate() {
        this.isActive = true;
    }
    
    /**
     * Deactivate coupon
     */
    public void deactivate() {
        this.isActive = false;
    }
    
    /**
     * Add applicable category
     * @param category Category to add
     */
    public void addApplicableCategory(String category) {
        if (!applicableCategories.contains(category)) {
            applicableCategories.add(category);
        }
    }
    
    /**
     * Add applicable product
     * @param productId Product ID to add
     */
    public void addApplicableProduct(String productId) {
        if (!applicableProducts.contains(productId)) {
            applicableProducts.add(productId);
        }
    }
    
    /**
     * Add excluded category
     * @param category Category to add
     */
    public void addExcludedCategory(String category) {
        if (!excludedCategories.contains(category)) {
            excludedCategories.add(category);
        }
    }
    
    /**
     * Add excluded product
     * @param productId Product ID to add
     */
    public void addExcludedProduct(String productId) {
        if (!excludedProducts.contains(productId)) {
            excludedProducts.add(productId);
        }
    }
    
    /**
     * Increment used count
     */
    public void incrementUsedCount() {
        this.usedCount++;
    }
    
    /**
     * Calculate discount amount for a given cart total
     * @param cartTotal Cart total
     * @return Discount amount
     */
    public double calculateDiscount(double cartTotal) {
        if (type.equals("PERCENTAGE")) {
            return cartTotal * (value / 100.0);
        } else if (type.equals("FIXED_AMOUNT")) {
            return Math.min(value, cartTotal);  // Don't exceed cart total
        }
        
        return 0.0;
    }
    
    /**
     * Check if coupon is currently valid
     * @param currentDate Current date
     * @return True if valid, false otherwise
     */
    public boolean isValid(Date currentDate) {
        if (!isActive) {
            return false;
        }
        
        if (usageLimit > 0 && usedCount >= usageLimit) {
            return false;
        }
        
        if (currentDate.before(startDate) || currentDate.after(endDate)) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Check if coupon is valid for a given cart
     * @param cartTotal Cart total
     * @param currentDate Current date
     * @return True if valid, false otherwise
     */
    public boolean isValidForCart(double cartTotal, Date currentDate) {
        if (!isValid(currentDate)) {
            return false;
        }
        
        if (cartTotal < minOrderAmount) {
            return false;
        }
        
        return true;
    }
    
    /**
     * Get coupon summary
     * @return Coupon summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Coupon ID: ").append(couponId).append("\n");
        sb.append("Code: ").append(code).append("\n");
        sb.append("Description: ").append(description).append("\n");
        
        if (type.equals("PERCENTAGE")) {
            sb.append("Discount: ").append(value).append("%\n");
        } else if (type.equals("FIXED_AMOUNT")) {
            sb.append("Discount: $").append(String.format("%.2f", value)).append("\n");
        }
        
        sb.append("Min Order Amount: $").append(String.format("%.2f", minOrderAmount)).append("\n");
        sb.append("Start Date: ").append(sdf.format(startDate)).append("\n");
        sb.append("End Date: ").append(sdf.format(endDate)).append("\n");
        
        if (usageLimit > 0) {
            sb.append("Usage: ").append(usedCount).append("/").append(usageLimit).append("\n");
        } else {
            sb.append("Usage: ").append(usedCount).append(" (Unlimited)\n");
        }
        
        sb.append("Status: ").append(isActive ? "Active" : "Inactive").append("\n");
        
        if (!applicableCategories.isEmpty()) {
            sb.append("Applicable Categories: ").append(String.join(", ", applicableCategories)).append("\n");
        }
        
        if (!applicableProducts.isEmpty()) {
            sb.append("Applicable Products: ").append(applicableProducts.size()).append("\n");
        }
        
        if (!excludedCategories.isEmpty()) {
            sb.append("Excluded Categories: ").append(String.join(", ", excludedCategories)).append("\n");
        }
        
        if (!excludedProducts.isEmpty()) {
            sb.append("Excluded Products: ").append(excludedProducts.size()).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * Order class represents a customer order
 */
class Order {
    private String orderId;
    private String customerId;
    private String customerName;
    private String customerEmail;
    private Date orderDate;
    private Date shippedDate;
    private Date deliveredDate;
    private String status;  // PENDING, CONFIRMED, PROCESSING, SHIPPED, DELIVERED, CANCELLED, REFUNDED
    private List<OrderItem> items;
    private String shippingAddressId;
    private String shippingAddress;
    private double shippingCost;
    private String paymentMethodId;
    private String paymentMethod;
    private double subtotal;
    private double taxAmount;
    private double shippingAmount;
    private double discountAmount;
    private double totalAmount;
    private String couponCode;
    private String trackingNumber;
    private String carrier;
    private String notes;
    
    /**
     * Constructor for Order
     * @param orderId Order ID
     * @param customerId Customer ID
     * @param customerName Customer name
     * @param customerEmail Customer email
     * @param orderDate Order date
     * @param shippingAddressId Shipping address ID
     * @param shippingAddress Shipping address
     * @param shippingCost Shipping cost
     * @param paymentMethodId Payment method ID
     * @param paymentMethod Payment method
     */
    public Order(String orderId, String customerId, String customerName, String customerEmail,
                Date orderDate, String shippingAddressId, String shippingAddress,
                double shippingCost, String paymentMethodId, String paymentMethod) {
        this.orderId = orderId;
        this.customerId = customerId;
        this.customerName = customerName;
        this.customerEmail = customerEmail;
        this.orderDate = orderDate;
        this.shippedDate = null;
        this.deliveredDate = null;
        this.status = "PENDING";
        this.items = new ArrayList<>();
        this.shippingAddressId = shippingAddressId;
        this.shippingAddress = shippingAddress;
        this.shippingCost = shippingCost;
        this.paymentMethodId = paymentMethodId;
        this.paymentMethod = paymentMethod;
        this.subtotal = 0.0;
        this.taxAmount = 0.0;
        this.shippingAmount = shippingCost;
        this.discountAmount = 0.0;
        this.totalAmount = 0.0;
        this.couponCode = "";
        this.trackingNumber = "";
        this.carrier = "";
        this.notes = "";
    }
    
    /**
     * Get order ID
     * @return Order ID
     */
    public String getOrderId() {
        return orderId;
    }
    
    /**
     * Get customer ID
     * @return Customer ID
     */
    public String getCustomerId() {
        return customerId;
    }
    
    /**
     * Get customer name
     * @return Customer name
     */
    public String getCustomerName() {
        return customerName;
    }
    
    /**
     * Get customer email
     * @return Customer email
     */
    public String getCustomerEmail() {
        return customerEmail;
    }
    
    /**
     * Get order date
     * @return Order date
     */
    public Date getOrderDate() {
        return orderDate;
    }
    
    /**
     * Get shipped date
     * @return Shipped date
     */
    public Date getShippedDate() {
        return shippedDate;
    }
    
    /**
     * Get delivered date
     * @return Delivered date
     */
    public Date getDeliveredDate() {
        return deliveredDate;
    }
    
    /**
     * Get status
     * @return Order status
     */
    public String getStatus() {
        return status;
    }
    
    /**
     * Get items
     * @return List of order items
     */
    public List<OrderItem> getItems() {
        return items;
    }
    
    /**
     * Get shipping address ID
     * @return Shipping address ID
     */
    public String getShippingAddressId() {
        return shippingAddressId;
    }
    
    /**
     * Get shipping address
     * @return Shipping address
     */
    public String getShippingAddress() {
        return shippingAddress;
    }
    
    /**
     * Get shipping cost
     * @return Shipping cost
     */
    public double getShippingCost() {
        return shippingCost;
    }
    
    /**
     * Get payment method ID
     * @return Payment method ID
     */
    public String getPaymentMethodId() {
        return paymentMethodId;
    }
    
    /**
     * Get payment method
     * @return Payment method
     */
    public String getPaymentMethod() {
        return paymentMethod;
    }
    
    /**
     * Get subtotal
     * @return Order subtotal
     */
    public double getSubtotal() {
        return subtotal;
    }
    
    /**
     * Get tax amount
     * @return Tax amount
     */
    public double getTaxAmount() {
        return taxAmount;
    }
    
    /**
     * Get shipping amount
     * @return Shipping amount
     */
    public double getShippingAmount() {
        return shippingAmount;
    }
    
    /**
     * Get discount amount
     * @return Discount amount
     */
    public double getDiscountAmount() {
        return discountAmount;
    }
    
    /**
     * Get total amount
     * @return Total order amount
     */
    public double getTotalAmount() {
        return totalAmount;
    }
    
    /**
     * Get coupon code
     * @return Applied coupon code
     */
    public String getCouponCode() {
        return couponCode;
    }
    
    /**
     * Get tracking number
     * @return Tracking number
     */
    public String getTrackingNumber() {
        return trackingNumber;
    }
    
    /**
     * Get carrier
     * @return Shipping carrier
     */
    public String getCarrier() {
        return carrier;
    }
    
    /**
     * Get notes
     * @return Order notes
     */
    public String getNotes() {
        return notes;
    }
    
    /**
     * Update status
     * @param status New status
     */
    public void updateStatus(String status) {
        this.status = status;
        
        // Update relevant dates based on status
        Date now = new Date();
        if (status.equals("SHIPPED") && shippedDate == null) {
            this.shippedDate = now;
        } else if (status.equals("DELIVERED") && deliveredDate == null) {
            this.deliveredDate = now;
        }
    }
    
    /**
     * Add item to order
     * @param itemId Item ID
     * @param productId Product ID
     * @param productName Product name
     * @param price Item price
     * @param quantity Item quantity
     * @param variantId Product variant ID (optional)
     * @param variantName Product variant name (optional)
     */
    public void addItem(String itemId, String productId, String productName, double price,
                      int quantity, String variantId, String variantName) {
        OrderItem item = new OrderItem(itemId, productId, productName, price, quantity, variantId, variantName);
        items.add(item);
        recalculateTotals();
    }
    
    /**
     * Remove item from order
     * @param itemId Item ID
     * @return True if removed successfully, false otherwise
     */
    public boolean removeItem(String itemId) {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getItemId().equals(itemId)) {
                items.remove(i);
                recalculateTotals();
                return true;
            }
        }
        return false;
    }
    
    /**
     * Set shipping tracking information
     * @param trackingNumber Tracking number
     * @param carrier Shipping carrier
     */
    public void setShippingTracking(String trackingNumber, String carrier) {
        this.trackingNumber = trackingNumber;
        this.carrier = carrier;
    }
    
    /**
     * Apply coupon
     * @param couponCode Coupon code
     * @param discountAmount Discount amount
     */
    public void applyCoupon(String couponCode, double discountAmount) {
        this.couponCode = couponCode;
        this.discountAmount = discountAmount;
        recalculateTotals();
    }
    
    /**
     * Add note
     * @param note Order note
     */
    public void addNote(String note) {
        if (notes.isEmpty()) {
            notes = note;
        } else {
            notes += "; " + note;
        }
    }
    
    /**
     * Recalculate order totals
     */
    private void recalculateTotals() {
        // Calculate subtotal
        subtotal = 0.0;
        for (OrderItem item : items) {
            subtotal += item.getTotalPrice();
        }
        
        // Calculate tax (simplified: 8% for taxable items)
        taxAmount = subtotal * 0.08;
        
        // Calculate total
        totalAmount = subtotal + taxAmount + shippingAmount - discountAmount;
        
        // Ensure total is not negative
        if (totalAmount < 0) {
            totalAmount = 0.0;
        }
    }
    
    /**
     * Get order summary
     * @return Order summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("Order ID: ").append(orderId).append("\n");
        sb.append("Customer: ").append(customerName).append(" (").append(customerId).append(")\n");
        sb.append("Email: ").append(customerEmail).append("\n");
        sb.append("Order Date: ").append(sdf.format(orderDate)).append("\n");
        sb.append("Status: ").append(status).append("\n");
        
        if (shippedDate != null) {
            sb.append("Shipped Date: ").append(sdf.format(shippedDate)).append("\n");
        }
        
        if (deliveredDate != null) {
            sb.append("Delivered Date: ").append(sdf.format(deliveredDate)).append("\n");
        }
        
        sb.append("\nItems:\n");
        for (OrderItem item : items) {
            sb.append("- ").append(item.getProductName());
            
            if (item.getVariantId() != null && !item.getVariantId().isEmpty()) {
                sb.append(" (").append(item.getVariantName()).append(")");
            }
            
            sb.append(" x ").append(item.getQuantity())
              .append(" = $").append(String.format("%.2f", item.getTotalPrice())).append("\n");
        }
        
        sb.append("\nShipping Address: ").append(shippingAddress).append("\n");
        sb.append("Payment Method: ").append(paymentMethod).append("\n");
        sb.append("Subtotal: $").append(String.format("%.2f", subtotal)).append("\n");
        sb.append("Tax: $").append(String.format("%.2f", taxAmount)).append("\n");
        sb.append("Shipping: $").append(String.format("%.2f", shippingAmount)).append("\n");
        
        if (!couponCode.isEmpty()) {
            sb.append("Coupon: ").append(couponCode).append(" (-$").append(String.format("%.2f", discountAmount)).append(")\n");
        }
        
        sb.append("Total: $").append(String.format("%.2f", totalAmount)).append("\n");
        
        if (!trackingNumber.isEmpty()) {
            sb.append("Tracking: ").append(trackingNumber).append(" (").append(carrier).append(")\n");
        }
        
        if (!notes.trim().isEmpty()) {
            sb.append("Notes: ").append(notes).append("\n");
        }
        
        return sb.toString();
    }
}

/**
 * OrderItem class represents an item in an order
 */
class OrderItem {
    private String itemId;
    private String productId;
    private String productName;
    private double price;
    private int quantity;
    private String variantId;
    private String variantName;
    
    /**
     * Constructor for OrderItem
     * @param itemId Item ID
     * @param productId Product ID
     * @param productName Product name
     * @param price Item price
     * @param quantity Item quantity
     * @param variantId Product variant ID (optional)
     * @param variantName Product variant name (optional)
     */
    public OrderItem(String itemId, String productId, String productName, double price,
                    int quantity, String variantId, String variantName) {
        this.itemId = itemId;
        this.productId = productId;
        this.productName = productName;
        this.price = price;
        this.quantity = quantity;
        this.variantId = variantId;
        this.variantName = variantName;
    }
    
    /**
     * Get item ID
     * @return Item ID
     */
    public String getItemId() {
        return itemId;
    }
    
    /**
     * Get product ID
     * @return Product ID
     */
    public String getProductId() {
        return productId;
    }
    
    /**
     * Get product name
     * @return Product name
     */
    public String getProductName() {
        return productName;
    }
    
    /**
     * Get price
     * @return Item price
     */
    public double getPrice() {
        return price;
    }
    
    /**
     * Get quantity
     * @return Item quantity
     */
    public int getQuantity() {
        return quantity;
    }
    
    /**
     * Get variant ID
     * @return Product variant ID
     */
    public String getVariantId() {
        return variantId;
    }
    
    /**
     * Get variant name
     * @return Product variant name
     */
    public String getVariantName() {
        return variantName;
    }
    
    /**
     * Get total price
     * @return Total price (price × quantity)
     */
    public double getTotalPrice() {
        return price * quantity;
    }
    
    /**
     * Get order item summary
     * @return Item summary string
     */
    public String getSummary() {
        StringBuilder sb = new StringBuilder();
        
        sb.append("Item ID: ").append(itemId).append("\n");
        sb.append("Product ID: ").append(productId).append("\n");
        sb.append("Product Name: ").append(productName).append("\n");
        sb.append("Price: $").append(String.format("%.2f", price)).append("\n");
        sb.append("Quantity: ").append(quantity).append("\n");
        sb.append("Total: $").append(String.format("%.2f", getTotalPrice())).append("\n");
        
        if (variantId != null && !variantId.isEmpty()) {
            sb.append("Variant ID: ").append(variantId).append("\n");
            
            if (!variantName.isEmpty()) {
                sb.append("Variant Name: ").append(variantName).append("\n");
            }
        }
        
        return sb.toString();
    }
}

/**
 * ECommercePlatform class manages the entire e-commerce platform
 */
public class ECommercePlatform {
    private String platformName;
    private String version;
    private String company;
    private List<Product> products;
    private Map<String, Product> productMap;
    private List<Customer> customers;
    private Map<String, Customer> customerMap;
    private List<Order> orders;
    private Map<String, Order> orderMap;
    private List<Coupon> coupons;
    private Map<String, Coupon> couponMap;
    private List<String> categories;
    private Map<String, Double> taxRates;
    private Map<String, Double> shippingRates;
    
    /**
     * Constructor for ECommercePlatform
     * @param platformName Platform name
     * @param version Platform version
     * @param company Company name
     */
    public ECommercePlatform(String platformName, String version, String company) {
        this.platformName = platformName;
        this.version = version;
        this.company = company;
        this.products = new ArrayList<>();
        this.productMap = new HashMap<>();
        this.customers = new ArrayList<>();
        this.customerMap = new HashMap<>();
        this.orders = new ArrayList<>();
        this.orderMap = new HashMap<>();
        this.coupons = new ArrayList<>();
        this.couponMap = new HashMap<>();
        this.categories = new ArrayList<>();
        this.taxRates = new HashMap<>();
        this.shippingRates = new HashMap<>();
        
        // Initialize categories
        categories.add("Electronics");
        categories.add("Clothing");
        categories.add("Home & Garden");
        categories.add("Sports & Outdoors");
        categories.add("Books");
        categories.add("Toys & Games");
        categories.add("Health & Beauty");
        categories.add("Food & Beverages");
        categories.add("Automotive");
        categories.add("Jewelry");
        categories.add("Furniture");
        
        // Initialize tax rates
        taxRates.put("DEFAULT", 0.08);
        taxRates.put("ELECTRONICS", 0.09);
        taxRates.put("CLOTHING", 0.07);
        taxRates.put("BOOKS", 0.05);
        taxRates.put("FOOD", 0.02);
        
        // Initialize shipping rates
        shippingRates.put("STANDARD", 5.99);
        shippingRates.put("EXPRESS", 12.99);
        shippingRates.put("OVERNIGHT", 24.99);
        shippingRates.put("FREE_SHIPPING_THRESHOLD", 50.0);
    }
    
    /**
     * Get platform name
     * @return Platform name
     */
    public String getPlatformName() {
        return platformName;
    }
    
    /**
     * Get version
     * @return Platform version
     */
    public String getVersion() {
        return version;
    }
    
    /**
     * Get company
     * @return Company name
     */
    public String getCompany() {
        return company;
    }
    
    /**
     * Get products
     * @return List of products
     */
    public List<Product> getProducts() {
        return products;
    }
    
    /**
     * Get customers
     * @return List of customers
     */
    public List<Customer> getCustomers() {
        return customers;
    }
    
    /**
     * Get orders
     * @return List of orders
     */
    public List<Order> getOrders() {
        return orders;
    }
    
    /**
     * Get coupons
     * @return List of coupons
     */
    public List<Coupon> getCoupons() {
        return coupons;
    }
    
    /**
     * Get categories
     * @return List of product categories
     */
    public List<String> getCategories() {
        return categories;
    }
    
    /**
     * Get tax rates
     * @return Map of tax rates
     */
    public Map<String, Double> getTaxRates() {
        return taxRates;
    }
    
    /**
     * Get shipping rates
     * @return Map of shipping rates
     */
    public Map<String, Double> getShippingRates() {
        return shippingRates;
    }
    
    /**
     * Add a product
     * @param product Product to add
     * @return True if added successfully, false otherwise
     */
    public boolean addProduct(Product product) {
        if (productMap.containsKey(product.getProductId())) {
            return false;
        }
        
        products.add(product);
        productMap.put(product.getProductId(), product);
        
        // Add category if not exists
        String category = product.getCategory();
        if (!categories.contains(category)) {
            categories.add(category);
        }
        
        return true;
    }
    
    /**
     * Add a customer
     * @param customer Customer to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCustomer(Customer customer) {
        if (customerMap.containsKey(customer.getCustomerId())) {
            return false;
        }
        
        customers.add(customer);
        customerMap.put(customer.getCustomerId(), customer);
        return true;
    }
    
    /**
     * Add an order
     * @param order Order to add
     * @return True if added successfully, false otherwise
     */
    public boolean addOrder(Order order) {
        if (orderMap.containsKey(order.getOrderId())) {
            return false;
        }
        
        orders.add(order);
        orderMap.put(order.getOrderId(), order);
        return true;
    }
    
    /**
     * Add a coupon
     * @param coupon Coupon to add
     * @return True if added successfully, false otherwise
     */
    public boolean addCoupon(Coupon coupon) {
        if (couponMap.containsKey(coupon.getCouponId())) {
            return false;
        }
        
        coupons.add(coupon);
        couponMap.put(coupon.getCouponId(), coupon);
        return true;
    }
    
    /**
     * Get product by ID
     * @param productId Product ID
     * @return Product if found, null otherwise
     */
    public Product getProductById(String productId) {
        return productMap.get(productId);
    }
    
    /**
     * Get customer by ID
     * @param customerId Customer ID
     * @return Customer if found, null otherwise
     */
    public Customer getCustomerById(String customerId) {
        return customerMap.get(customerId);
    }
    
    /**
     * Get order by ID
     * @param orderId Order ID
     * @return Order if found, null otherwise
     */
    public Order getOrderById(String orderId) {
        return orderMap.get(orderId);
    }
    
    /**
     * Get coupon by code
     * @param couponCode Coupon code
     * @return Coupon if found, null otherwise
     */
    public Coupon getCouponByCode(String couponCode) {
        for (Coupon coupon : coupons) {
            if (coupon.getCode().equalsIgnoreCase(couponCode)) {
                return coupon;
            }
        }
        return null;
    }
    
    /**
     * Get products by category
     * @param category Product category
     * @return List of products in category
     */
    public List<Product> getProductsByCategory(String category) {
        List<Product> result = new ArrayList<>();
        
        for (Product product : products) {
            if (product.getCategory().equalsIgnoreCase(category)) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * Search products by keyword
     * @param keyword Search keyword
     * @return List of matching products
     */
    public List<Product> searchProducts(String keyword) {
        List<Product> result = new ArrayList<>();
        String lowercaseKeyword = keyword.toLowerCase();
        
        for (Product product : products) {
            boolean matches = false;
            
            // Check name
            if (product.getName().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check description
            if (product.getDescription().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check brand
            if (product.getBrand().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check SKU
            if (product.getSku().toLowerCase().contains(lowercaseKeyword)) {
                matches = true;
            }
            
            // Check tags
            for (String tag : product.getTags()) {
                if (tag.toLowerCase().contains(lowercaseKeyword)) {
                    matches = true;
                    break;
                }
            }
            
            if (matches) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * Get available products (in stock)
     * @return List of available products
     */
    public List<Product> getAvailableProducts() {
        List<Product> result = new ArrayList<>();
        
        for (Product product : products) {
            if (product.isInStock() && product.getStatus().equals("ACTIVE")) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * Get low stock products
     * @return List of low stock products
     */
    public List<Product> getLowStockProducts() {
        List<Product> result = new ArrayList<>();
        
        for (Product product : products) {
            if (product.isLowStock()) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * Get active customers
     * @return List of active customers
     */
    public List<Customer> getActiveCustomers() {
        List<Customer> result = new ArrayList<>();
        
        for (Customer customer : customers) {
            if (customer.getStatus().equals("ACTIVE")) {
                result.add(customer);
            }
        }
        
        return result;
    }
    
    /**
     * Get orders by status
     * @param status Order status
     * @return List of orders with specified status
     */
    public List<Order> getOrdersByStatus(String status) {
        List<Order> result = new ArrayList<>();
        
        for (Order order : orders) {
            if (order.getStatus().equals(status)) {
                result.add(order);
            }
        }
        
        return result;
    }
    
    /**
     * Get active coupons
     * @return List of active coupons
     */
    public List<Coupon> getActiveCoupons() {
        List<Coupon> result = new ArrayList<>();
        Date currentDate = new Date();
        
        for (Coupon coupon : coupons) {
            if (coupon.isValid(currentDate)) {
                result.add(coupon);
            }
        }
        
        return result;
    }
    
    /**
     * Calculate shipping cost
     * @param shippingType Shipping type
     * @param cartTotal Cart total
     * @return Shipping cost
     */
    public double calculateShippingCost(String shippingType, double cartTotal) {
        // Check if order qualifies for free shipping
        Double freeShippingThreshold = shippingRates.get("FREE_SHIPPING_THRESHOLD");
        if (freeShippingThreshold != null && cartTotal >= freeShippingThreshold) {
            return 0.0;
        }
        
        // Get shipping cost for type
        Double shippingCost = shippingRates.get(shippingType.toUpperCase());
        if (shippingCost != null) {
            return shippingCost;
        }
        
        // Default to standard shipping
        Double standardCost = shippingRates.get("STANDARD");
        return standardCost != null ? standardCost : 5.99;
    }
    
    /**
     * Calculate tax amount
     * @param subtotal Order subtotal
     * @param category Product category
     * @return Tax amount
     */
    public double calculateTaxAmount(double subtotal, String category) {
        // Get tax rate for category
        Double taxRate = taxRates.get(category.toUpperCase());
        if (taxRate == null) {
            // Default tax rate
            taxRate = taxRates.get("DEFAULT");
        }
        
        if (taxRate != null) {
            return subtotal * taxRate;
        }
        
        return 0.0;
    }
    
    /**
     * Process order checkout
     * @param customerId Customer ID
     * @param shippingAddressId Shipping address ID
     * @param paymentMethodId Payment method ID
     * @param shippingType Shipping type
     * @param couponCode Coupon code (optional)
     * @return Order ID if successful, null otherwise
     */
    public String processCheckout(String customerId, String shippingAddressId,
                                String paymentMethodId, String shippingType, String couponCode) {
        // Get customer
        Customer customer = getCustomerById(customerId);
        if (customer == null || !customer.getStatus().equals("ACTIVE")) {
            return null;
        }
        
        // Check if shopping cart is empty
        ShoppingCart cart = customer.getShoppingCart();
        if (cart.isEmpty()) {
            return null;
        }
        
        // Get shipping address
        String shippingAddress = "";
        for (Address address : customer.getAddresses()) {
            if (address.getAddressId().equals(shippingAddressId)) {
                shippingAddress = address.getFullAddress();
                break;
            }
        }
        
        if (shippingAddress.isEmpty()) {
            return null;
        }
        
        // Get payment method
        String paymentMethod = "";
        for (PaymentMethod method : customer.getPaymentMethods()) {
            if (method.getPaymentMethodId().equals(paymentMethodId)) {
                paymentMethod = method.getType().replace("_", " ");
                break;
            }
        }
        
        if (paymentMethod.isEmpty()) {
            return null;
        }
        
        // Calculate shipping cost
        double shippingCost = calculateShippingCost(shippingType, cart.getSubtotal());
        
        // Create order
        String orderId = "ORD" + (orders.size() + 1);
        Order order = new Order(
            orderId, customerId, customer.getFullName(), customer.getEmail(),
            new Date(), shippingAddressId, shippingAddress, shippingCost,
            paymentMethodId, paymentMethod
        );
        
        // Add items to order
        for (ShoppingCartItem item : cart.getItems()) {
            // Get product
            Product product = getProductById(item.getProductId());
            if (product == null || !product.isInStock()) {
                continue;  // Skip unavailable products
            }
            
            // Add item to order
            order.addItem(
                item.getItemId(),
                item.getProductId(),
                item.getProductName(),
                item.getPrice(),
                item.getQuantity(),
                item.getVariantId(),
                item.getVariantName()
            );
            
            // Reduce product stock
            product.reduceStock(item.getQuantity());
        }
        
        // Apply coupon if valid
        if (couponCode != null && !couponCode.trim().isEmpty()) {
            Coupon coupon = getCouponByCode(couponCode);
            if (coupon != null && coupon.isValidForCart(order.getSubtotal(), new Date())) {
                order.applyCoupon(couponCode, coupon.calculateDiscount(order.getSubtotal()));
                coupon.incrementUsedCount();
            }
        }
        
        // Confirm order
        order.updateStatus("CONFIRMED");
        
        // Add order to system
        addOrder(order);
        
        // Add order to customer history
        customer.addOrder(order);
        
        // Clear shopping cart
        cart.updateStatus("CHECKED_OUT");
        cart = new ShoppingCart(customerId);
        customer.shoppingCart = cart;
        
        return orderId;
    }
    
    /**
     * Generate platform report
     * @return Platform report string
     */
    public String generatePlatformReport() {
        StringBuilder sb = new StringBuilder();
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        sb.append("=====================================\n");
        sb.append("      E-COMMERCE PLATFORM REPORT\n");
        sb.append("=====================================\n");
        sb.append("Platform: ").append(platformName).append("\n");
        sb.append("Version: ").append(version).append("\n");
        sb.append("Company: ").append(company).append("\n");
        sb.append("Report Date: ").append(sdf.format(new Date())).append("\n\n");
        
        // Product statistics
        sb.append("PRODUCT STATISTICS\n");
        sb.append("-------------------\n");
        sb.append("Total Products: ").append(products.size()).append("\n");
        
        Map<String, Integer> productsByCategory = new HashMap<>();
        int totalStock = 0;
        int lowStockCount = 0;
        int outOfStockCount = 0;
        
        for (Product product : products) {
            String category = product.getCategory();
            productsByCategory.put(category, productsByCategory.getOrDefault(category, 0) + 1);
            
            totalStock += product.getStockQuantity();
            
            if (product.isLowStock()) {
                lowStockCount++;
            }
            
            if (!product.isInStock()) {
                outOfStockCount++;
            }
        }
        
        sb.append("Total Stock: ").append(totalStock).append("\n");
        sb.append("Low Stock Products: ").append(lowStockCount).append("\n");
        sb.append("Out of Stock Products: ").append(outOfStockCount).append("\n");
        
        sb.append("\nProducts by Category:\n");
        for (Map.Entry<String, Integer> entry : productsByCategory.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Customer statistics
        sb.append("\nCUSTOMER STATISTICS\n");
        sb.append("--------------------\n");
        sb.append("Total Customers: ").append(customers.size()).append("\n");
        
        Map<String, Integer> customersByStatus = new HashMap<>();
        Map<String, Integer> customersByTier = new HashMap<>();
        double totalSpent = 0.0;
        
        for (Customer customer : customers) {
            String status = customer.getStatus();
            customersByStatus.put(status, customersByStatus.getOrDefault(status, 0) + 1);
            
            String tier = customer.getMembershipTier();
            customersByTier.put(tier, customersByTier.getOrDefault(tier, 0) + 1);
            
            totalSpent += customer.getTotalSpent();
        }
        
        sb.append("Total Spent: $").append(String.format("%.2f", totalSpent)).append("\n");
        sb.append("Average Spent per Customer: $").append(String.format("%.2f", totalSpent / customers.size())).append("\n");
        
        sb.append("\nCustomers by Status:\n");
        for (Map.Entry<String, Integer> entry : customersByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        sb.append("\nCustomers by Membership Tier:\n");
        for (Map.Entry<String, Integer> entry : customersByTier.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Order statistics
        sb.append("\nORDER STATISTICS\n");
        sb.append("-----------------\n");
        sb.append("Total Orders: ").append(orders.size()).append("\n");
        
        Map<String, Integer> ordersByStatus = new HashMap<>();
        double totalRevenue = 0.0;
        
        for (Order order : orders) {
            String status = order.getStatus();
            ordersByStatus.put(status, ordersByStatus.getOrDefault(status, 0) + 1);
            
            totalRevenue += order.getTotalAmount();
        }
        
        sb.append("Total Revenue: $").append(String.format("%.2f", totalRevenue)).append("\n");
        sb.append("Average Order Value: $").append(String.format("%.2f", totalRevenue / orders.size())).append("\n");
        
        sb.append("\nOrders by Status:\n");
        for (Map.Entry<String, Integer> entry : ordersByStatus.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Coupon statistics
        sb.append("\nCOUPON STATISTICS\n");
        sb.append("------------------\n");
        sb.append("Total Coupons: ").append(coupons.size()).append("\n");
        
        Map<String, Integer> couponsByType = new HashMap<>();
        int activeCoupons = 0;
        
        for (Coupon coupon : coupons) {
            String type = coupon.getType();
            couponsByType.put(type, couponsByType.getOrDefault(type, 0) + 1);
            
            if (coupon.isValid(new Date())) {
                activeCoupons++;
            }
        }
        
        sb.append("Active Coupons: ").append(activeCoupons).append("\n");
        
        sb.append("\nCoupons by Type:\n");
        for (Map.Entry<String, Integer> entry : couponsByType.entrySet()) {
            sb.append("  ").append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }
        
        // Top selling products
        sb.append("\nTOP 5 BEST-SELLING PRODUCTS\n");
        sb.append("---------------------------\n");
        
        Map<String, Integer> productSales = new HashMap<>();
        for (Order order : orders) {
            for (OrderItem item : order.getItems()) {
                String productId = item.getProductId();
                productSales.put(productId, productSales.getOrDefault(productId, 0) + item.getQuantity());
            }
        }
        
        List<Map.Entry<String, Integer>> sortedProductSales = new ArrayList<>(productSales.entrySet());
        sortedProductSales.sort((a, b) -> b.getValue().compareTo(a.getValue()));
        
        for (int i = 0; i < Math.min(5, sortedProductSales.size()); i++) {
            Map.Entry<String, Integer> entry = sortedProductSales.get(i);
            String productId = entry.getKey();
            int quantity = entry.getValue();
            Product product = getProductById(productId);
            
            sb.append("  ").append(i + 1).append(". ").append(product != null ? product.getName() : "Unknown")
              .append(" (").append(quantity).append(" sold)\n");
        }
        
        // Top customers
        sb.append("\nTOP 5 CUSTOMERS BY SPENDING\n");
        sb.append("----------------------------\n");
        
        List<Customer> sortedCustomers = new ArrayList<>(customers);
        sortedCustomers.sort((a, b) -> Double.compare(b.getTotalSpent(), a.getTotalSpent()));
        
        for (int i = 0; i < Math.min(5, sortedCustomers.size()); i++) {
            Customer customer = sortedCustomers.get(i);
            sb.append("  ").append(i + 1).append(". ").append(customer.getFullName())
              .append(" ($").append(String.format("%.2f", customer.getTotalSpent())).append(")\n");
        }
        
        return sb.toString();
    }
    
    /**
     * Save platform data to file
     * @param filename File name
     * @return True if saved successfully, false otherwise
     */
    public boolean saveToFile(String filename) {
        try {
            PrintWriter writer = new PrintWriter(new FileWriter(filename));
            
            writer.println("E-COMMERCE PLATFORM DATA EXPORT");
            writer.println("==================================");
            writer.println("Platform: " + platformName);
            writer.println("Version: " + version);
            writer.println("Company: " + company);
            writer.println("Export Date: " + new Date());
            writer.println();
            
            // Export products
            writer.println("PRODUCTS");
            writer.println("--------");
            for (Product product : products) {
                writer.println(product.getSummary());
                writer.println();
            }
            
            // Export customers
            writer.println("CUSTOMERS");
            writer.println("---------");
            for (Customer customer : customers) {
                writer.println(customer.getSummary());
                writer.println();
            }
            
            // Export orders
            writer.println("ORDERS");
            writer.println("------");
            for (Order order : orders) {
                writer.println(order.getSummary());
                writer.println();
            }
            
            // Export coupons
            writer.println("COUPONS");
            writer.println("-------");
            for (Coupon coupon : coupons) {
                writer.println(coupon.getSummary());
                writer.println();
            }
            
            writer.close();
            return true;
        } catch (IOException e) {
            System.err.println("Error saving to file: " + e.getMessage());
            return false;
        }
    }
    
    /**
     * Main method for demonstration
     */
    public static void main(String[] args) {
        // Create a new e-commerce platform
        ECommercePlatform platform = new ECommercePlatform("ShopMax Pro", "3.2", "E-Commerce Solutions Inc");
        
        // Create products
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
        
        Product product1 = new Product("P001", "Wireless Headphones", "Premium wireless headphones with noise cancellation and 30-hour battery life.",
                                   "Electronics", 199.99, "USD", "AudioTech", "WH-1000XM4", 50, 10,
                                   0.25, "7.2 x 3.0 x 6.8 inches", "ATH-WH1000XM4");
        product1.addTag("Wireless");
        product1.addTag("Noise-Cancelling");
        product1.addTag("Premium");
        product1.putOnSale(149.99);
        
        Product product2 = new Product("P002", "Smart Watch", "Advanced fitness tracker with heart rate monitor and GPS.",
                                   "Electronics", 299.99, "USD", "FitTech", "FT-SW500", 30, 5,
                                   0.05, "1.6 x 1.6 x 0.4 inches", "FT-SW500");
        product2.addTag("Fitness");
        product2.addTag("GPS");
        product2.addTag("Waterproof");
        
        Product product3 = new Product("P003", "Organic Cotton T-Shirt", "Comfortable and sustainable t-shirt made from 100% organic cotton.",
                                   "Clothing", 24.99, "USD", "EcoWear", "EW-OTS001", 100, 20,
                                   0.2, "Size M dimensions", "EW-OTS001-M");
        product3.addTag("Organic");
        product3.addTag("Sustainable");
        product3.addTag("Cotton");
        
        Product product4 = new Product("P004", "JavaScript: The Good Parts", "Comprehensive guide to JavaScript programming.",
                                   "Books", 39.99, "USD", "TechPress", "JS-GP001", 25, 5,
                                   0.8, "8.5 x 1.2 x 10.2 inches", "978-0596517748");
        product4.addTag("Programming");
        product4.addTag("JavaScript");
        product4.addTag("Technology");
        
        Product product5 = new Product("P005", "Running Shoes", "Lightweight running shoes with advanced cushioning technology.",
                                   "Sports & Outdoors", 89.99, "USD", "RunFast", "RF-RS100", 40, 10,
                                   0.6, "Size 10 dimensions", "RF-RS100-10");
        product5.addTag("Running");
        product5.addTag("Athletic");
        product5.addTag("Lightweight");
        
        // Add products to platform
        platform.addProduct(product1);
        platform.addProduct(product2);
        platform.addProduct(product3);
        platform.addProduct(product4);
        platform.addProduct(product5);
        
        // Create customers
        Customer customer1 = new Customer("C001", "John", "Doe", "john.doe@email.com", "password123", "(555) 123-4567");
        customer1.addAddress(new Address("A001", "SHIPPING", "John Doe", "123 Main St", "New York", "NY", "10001", "USA", "(555) 123-4567"));
        customer1.addPaymentMethod(new PaymentMethod("PM001", "CREDIT_CARD", "Personal Visa", "4532-1234-5678-9012", "John Doe", "12/25", "123"));
        customer1.getShoppingCart().addItem("P001", "Wireless Headphones", 149.99, 1);
        
        Customer customer2 = new Customer("C002", "Jane", "Smith", "jane.smith@email.com", "password456", "(555) 987-6543");
        customer2.addAddress(new Address("A002", "SHIPPING", "Jane Smith", "456 Oak Ave", "Los Angeles", "CA", "90001", "USA", "(555) 987-6543"));
        customer2.addPaymentMethod(new PaymentMethod("PM002", "PAYPAL", "Personal PayPal", "jane.smith@paypal.com"));
        customer2.getShoppingCart().addItem("P002", "Smart Watch", 299.99, 1);
        customer2.getShoppingCart().addItem("P003", "Organic Cotton T-Shirt", 24.99, 2);
        
        Customer customer3 = new Customer("C003", "Michael", "Johnson", "michael.johnson@email.com", "password789", "(555) 456-7890");
        customer3.addAddress(new Address("A003", "SHIPPING", "Michael Johnson", "789 Pine Rd", "Chicago", "IL", "60607", "USA", "(555) 456-7890"));
        customer3.addPaymentMethod(new PaymentMethod("PM003", "DEBIT_CARD", "Personal Mastercard", "5432-9876-5432-1098", "Michael Johnson", "09/24", "456"));
        customer3.getShoppingCart().addItem("P004", "JavaScript: The Good Parts", 39.99, 1);
        customer3.getShoppingCart().addItem("P005", "Running Shoes", 89.99, 1);
        
        // Add customers to platform
        platform.addCustomer(customer1);
        platform.addCustomer(customer2);
        platform.addCustomer(customer3);
        
        // Create coupons
        Coupon coupon1 = new Coupon("CP001", "SAVE20", "Save 20% on your order", "PERCENTAGE", 20.0, 50.0,
                                  sdf.parse("2023-01-01"), sdf.parse("2023-12-31"), 1000);
        coupon1.addApplicableCategory("Electronics");
        
        Coupon coupon2 = new Coupon("CP002", "FLAT10", "$10 off your order", "FIXED_AMOUNT", 10.0, 25.0,
                                  sdf.parse("2023-06-01"), sdf.parse("2023-09-30"), 500);
        
        Coupon coupon3 = new Coupon("CP003", "FREESHIP", "Free shipping on orders over $50", "FIXED_AMOUNT", 5.99, 50.0,
                                  sdf.parse("2023-01-01"), sdf.parse("2023-12-31"), 0);
        
        // Add coupons to platform
        platform.addCoupon(coupon1);
        platform.addCoupon(coupon2);
        platform.addCoupon(coupon3);
        
        // Process some orders
        String orderId1 = platform.processCheckout("C001", "A001", "PM001", "STANDARD", null);
        String orderId2 = platform.processCheckout("C002", "A002", "PM002", "EXPRESS", "SAVE20");
        String orderId3 = platform.processCheckout("C003", "A003", "PM003", "STANDARD", "FLAT10");
        
        // Print platform information
        System.out.println("E-commerce Platform");
        System.out.println("===================");
        System.out.println("Platform: " + platform.getPlatformName());
        System.out.println("Version: " + platform.getVersion());
        System.out.println("Company: " + platform.getCompany());
        System.out.println("Total Products: " + platform.getProducts().size());
        System.out.println("Total Customers: " + platform.getCustomers().size());
        System.out.println("Total Orders: " + platform.getOrders().size());
        System.out.println("Total Coupons: " + platform.getCoupons().size());
        System.out.println();
        
        // Print product summaries
        System.out.println("Product Summaries");
        System.out.println("==================");
        for (Product product : platform.getProducts()) {
            System.out.println(product.getSummary());
            System.out.println();
        }
        
        // Print customer summaries
        System.out.println("Customer Summaries");
        System.out.println("===================");
        for (Customer customer : platform.getCustomers()) {
            System.out.println(customer.getSummary());
            System.out.println();
        }
        
        // Print order summaries
        System.out.println("Order Summaries");
        System.out.println("================");
        for (Order order : platform.getOrders()) {
            System.out.println(order.getSummary());
            System.out.println();
        }
        
        // Print coupon summaries
        System.out.println("Coupon Summaries");
        System.out.println("=================");
        for (Coupon coupon : platform.getCoupons()) {
            System.out.println(coupon.getSummary());
            System.out.println();
        }
        
        // Generate and print platform report
        String report = platform.generatePlatformReport();
        System.out.println(report);
        
        // Save data to file
        platform.saveToFile("ecommerce_platform_data_export.txt");
        
        System.out.println("\nE-commerce Platform demonstration completed successfully!");
    }
}
<｜fim▁hole｜>