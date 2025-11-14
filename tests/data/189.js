/**
 * E-commerce Shopping Cart and Inventory System
 * 
 * This module implements a comprehensive e-commerce system with shopping cart
 * functionality, inventory management, and order processing. It demonstrates
 * complex business logic for online retail applications.
 * 
 * Features:
 * - Product catalog management
 * - Shopping cart functionality
 * - Inventory tracking
 * - Order processing
 * - Customer management
 * - Payment processing
 * - Discount and promotion system
 */

// Product class represents an item in inventory
class Product {
    constructor(id, name, description, price, category, stock = 0) {
        this.id = id;
        this.name = name;
        this.description = description;
        this.price = price;
        this.category = category;
        this.stock = stock;
        this.images = [];
        this.specifications = new Map();
        this.reviews = [];
        this.rating = 0;
        this.tags = new Set();
        this.isAvailable = stock > 0;
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.weight = 0;
        this.dimensions = { length: 0, width: 0, height: 0 };
        this.isDigital = false;
        this.downloadUrl = null;
        this.taxRate = 0.1;
        this.shippingRequired = true;
    }
    
    /**
     * Add image to product
     * @param {string} url - Image URL
     * @param {string} altText - Alt text for image
     * @param {boolean} isPrimary - Whether this is the primary image
     */
    addImage(url, altText = '', isPrimary = false) {
        const image = {
            id: this.generateId(),
            url: url,
            altText: altText,
            isPrimary: isPrimary || this.images.length === 0,
            addedAt: new Date()
        };
        
        // If this is primary, unmark others
        if (image.isPrimary) {
            this.images.forEach(img => img.isPrimary = false);
        }
        
        this.images.push(image);
        this.updatedAt = new Date();
        
        return image.id;
    }
    
    /**
     * Remove image from product
     * @param {string} imageId - Image ID to remove
     */
    removeImage(imageId) {
        const index = this.images.findIndex(img => img.id === imageId);
        if (index !== -1) {
            const removed = this.images.splice(index, 1)[0];
            
            // If removed image was primary, set first remaining as primary
            if (removed.isPrimary && this.images.length > 0) {
                this.images[0].isPrimary = true;
            }
            
            this.updatedAt = new Date();
            return true;
        }
        
        return false;
    }
    
    /**
     * Get primary image
     */
    getPrimaryImage() {
        return this.images.find(img => img.isPrimary) || null;
    }
    
    /**
     * Add specification
     * @param {string} name - Specification name
     * @param {string} value - Specification value
     */
    addSpecification(name, value) {
        this.specifications.set(name, value);
        this.updatedAt = new Date();
    }
    
    /**
     * Remove specification
     * @param {string} name - Specification name to remove
     */
    removeSpecification(name) {
        const removed = this.specifications.delete(name);
        if (removed) {
            this.updatedAt = new Date();
        }
        return removed;
    }
    
    /**
     * Add tag
     * @param {string} tag - Tag to add
     */
    addTag(tag) {
        this.tags.add(tag.toLowerCase());
        this.updatedAt = new Date();
    }
    
    /**
     * Remove tag
     * @param {string} tag - Tag to remove
     */
    removeTag(tag) {
        const removed = this.tags.delete(tag.toLowerCase());
        if (removed) {
            this.updatedAt = new Date();
        }
        return removed;
    }
    
    /**
     * Add review
     * @param {string} userId - User ID
     * @param {number} rating - Rating (1-5)
     * @param {string} comment - Review comment
     */
    addReview(userId, rating, comment) {
        if (rating < 1 || rating > 5) {
            throw new Error('Rating must be between 1 and 5');
        }
        
        const review = {
            id: this.generateId(),
            userId: userId,
            rating: rating,
            comment: comment,
            createdAt: new Date(),
            isVerified: false
        };
        
        this.reviews.push(review);
        this.updateRating();
        this.updatedAt = new Date();
        
        return review.id;
    }
    
    /**
     * Update overall rating based on reviews
     */
    updateRating() {
        if (this.reviews.length === 0) {
            this.rating = 0;
            return;
        }
        
        const totalRating = this.reviews.reduce((sum, review) => sum + review.rating, 0);
        this.rating = Math.round((totalRating / this.reviews.length) * 10) / 10;
    }
    
    /**
     * Update stock
     * @param {number} quantity - New stock quantity
     */
    updateStock(quantity) {
        this.stock = Math.max(0, quantity);
        this.isAvailable = this.stock > 0;
        this.updatedAt = new Date();
    }
    
    /**
     * Check if product is in stock
     */
    isInStock() {
        return this.stock > 0;
    }
    
    /**
     * Calculate price with tax
     */
    getPriceWithTax() {
        return this.price * (1 + this.taxRate);
    }
    
    /**
     * Generate unique ID
     */
    generateId() {
        return Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Get product summary
     */
    getSummary() {
        return {
            id: this.id,
            name: this.name,
            description: this.description,
            price: this.price,
            priceWithTax: this.getPriceWithTax(),
            category: this.category,
            stock: this.stock,
            isAvailable: this.isAvailable,
            rating: this.rating,
            reviewCount: this.reviews.length,
            tags: Array.from(this.tags),
            createdAt: this.createdAt,
            updatedAt: this.updatedAt,
            primaryImage: this.getPrimaryImage(),
            weight: this.weight,
            dimensions: this.dimensions,
            isDigital: this.isDigital,
            shippingRequired: this.shippingRequired
        };
    }
    
    /**
     * Get detailed product information
     */
    getDetails() {
        return {
            ...this.getSummary(),
            specifications: Object.fromEntries(this.specifications),
            images: this.images,
            reviews: this.reviews
        };
    }
}

// ShoppingCart class represents a user's shopping cart
class ShoppingCart {
    constructor(userId) {
        this.id = this.generateId();
        this.userId = userId;
        this.items = new Map(); // productId -> cart item
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.couponCode = null;
        this.discount = 0;
        this.subtotal = 0;
        this.tax = 0;
        this.shipping = 0;
        this.total = 0;
        this.currency = 'USD';
        this.expiredAt = null;
    }
    
    /**
     * Add item to cart
     * @param {Product} product - Product to add
     * @param {number} quantity - Quantity to add
     * @param {Object} options - Product options (e.g., size, color)
     */
    addItem(product, quantity = 1, options = {}) {
        if (!product.isAvailable) {
            throw new Error(`Product "${product.name}" is not available`);
        }
        
        if (product.stock < quantity) {
            throw new Error(`Only ${product.stock} units of "${product.name}" are available`);
        }
        
        const itemKey = this.getItemKey(product.id, options);
        let item = this.items.get(itemKey);
        
        if (item) {
            // Update existing item
            const newQuantity = item.quantity + quantity;
            if (product.stock < newQuantity) {
                throw new Error(`Only ${product.stock} units of "${product.name}" are available`);
            }
            
            item.quantity = newQuantity;
            item.subtotal = item.price * newQuantity;
        } else {
            // Add new item
            item = {
                id: this.generateId(),
                productId: product.id,
                product: product.getSummary(),
                quantity: quantity,
                price: product.price,
                options: options,
                addedAt: new Date(),
                subtotal: product.price * quantity
            };
            
            this.items.set(itemKey, item);
        }
        
        this.updatedAt = new Date();
        this.calculateTotals();
        
        return item.id;
    }
    
    /**
     * Update item quantity
     * @param {string} itemId - Cart item ID
     * @param {number} quantity - New quantity
     */
    updateItemQuantity(itemId, quantity) {
        let item = null;
        
        for (const [key, cartItem] of this.items) {
            if (cartItem.id === itemId) {
                item = cartItem;
                break;
            }
        }
        
        if (!item) {
            throw new Error(`Item with ID "${itemId}" not found in cart`);
        }
        
        if (quantity <= 0) {
            return this.removeItem(itemId);
        }
        
        // Check stock availability
        if (item.product.stock < quantity) {
            throw new Error(`Only ${item.product.stock} units of "${item.product.name}" are available`);
        }
        
        item.quantity = quantity;
        item.subtotal = item.price * quantity;
        
        this.updatedAt = new Date();
        this.calculateTotals();
        
        return true;
    }
    
    /**
     * Remove item from cart
     * @param {string} itemId - Cart item ID
     */
    removeItem(itemId) {
        let key = null;
        
        for (const [itemKey, item] of this.items) {
            if (item.id === itemId) {
                key = itemKey;
                break;
            }
        }
        
        if (key) {
            this.items.delete(key);
            this.updatedAt = new Date();
            this.calculateTotals();
            return true;
        }
        
        return false;
    }
    
    /**
     * Clear cart
     */
    clear() {
        this.items.clear();
        this.couponCode = null;
        this.discount = 0;
        this.updatedAt = new Date();
        this.calculateTotals();
    }
    
    /**
     * Apply coupon code
     * @param {string} code - Coupon code
     * @param {number} discount - Discount amount or percentage
     * @param {boolean} isPercentage - Whether discount is percentage
     */
    applyCoupon(code, discount, isPercentage = false) {
        this.couponCode = code;
        
        if (isPercentage) {
            this.discount = this.subtotal * (discount / 100);
        } else {
            this.discount = Math.min(discount, this.subtotal);
        }
        
        this.updatedAt = new Date();
        this.calculateTotals();
    }
    
    /**
     * Remove coupon
     */
    removeCoupon() {
        this.couponCode = null;
        this.discount = 0;
        this.updatedAt = new Date();
        this.calculateTotals();
    }
    
    /**
     * Calculate cart totals
     */
    calculateTotals() {
        // Calculate subtotal
        this.subtotal = 0;
        for (const item of this.items.values()) {
            this.subtotal += item.subtotal;
        }
        
        // Calculate tax
        this.tax = 0;
        for (const item of this.items.values()) {
            this.tax += item.price * item.quantity * item.product.taxRate;
        }
        
        // Calculate shipping
        this.shipping = this.calculateShipping();
        
        // Calculate total
        this.total = Math.max(0, this.subtotal + this.tax + this.shipping - this.discount);
    }
    
    /**
     * Calculate shipping cost
     */
    calculateShipping() {
        if (this.items.size === 0) {
            return 0;
        }
        
        let totalWeight = 0;
        let shippingRequired = false;
        
        for (const item of this.items.values()) {
            totalWeight += (item.product.weight || 0) * item.quantity;
            if (item.product.shippingRequired) {
                shippingRequired = true;
            }
        }
        
        // No shipping for digital only orders
        if (!shippingRequired) {
            return 0;
        }
        
        // Simple shipping calculation
        if (this.subtotal >= 50) {
            return 0; // Free shipping for orders over $50
        }
        
        return Math.max(5, totalWeight * 0.5); // Minimum $5 shipping
    }
    
    /**
     * Get item key for cart items map
     * @param {string} productId - Product ID
     * @param {Object} options - Product options
     */
    getItemKey(productId, options) {
        const optionsStr = JSON.stringify(options);
        return `${productId}:${optionsStr}`;
    }
    
    /**
     * Generate unique ID
     */
    generateId() {
        return Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Get cart items as array
     */
    getItems() {
        return Array.from(this.items.values());
    }
    
    /**
     * Get item count
     */
    getItemCount() {
        let count = 0;
        for (const item of this.items.values()) {
            count += item.quantity;
        }
        return count;
    }
    
    /**
     * Get cart summary
     */
    getSummary() {
        return {
            id: this.id,
            userId: this.userId,
            itemCount: this.getItemCount(),
            items: this.getItems(),
            subtotal: this.subtotal,
            tax: this.tax,
            shipping: this.shipping,
            discount: this.discount,
            total: this.total,
            currency: this.currency,
            couponCode: this.couponCode,
            createdAt: this.createdAt,
            updatedAt: this.updatedAt
        };
    }
}

// Order class represents a customer order
class Order {
    constructor(id, userId, cart, shippingAddress, billingAddress) {
        this.id = id;
        this.userId = userId;
        this.items = cart.getItems();
        this.subtotal = cart.subtotal;
        this.tax = cart.tax;
        this.shipping = cart.shipping;
        this.discount = cart.discount;
        this.total = cart.total;
        this.currency = cart.currency;
        this.couponCode = cart.couponCode;
        this.status = 'pending'; // pending, processing, shipped, delivered, cancelled, returned
        this.shippingAddress = shippingAddress;
        this.billingAddress = billingAddress;
        this.paymentMethod = null;
        this.paymentStatus = 'pending'; // pending, paid, failed, refunded
        this.createdAt = new Date();
        this.updatedAt = new Date();
        this.shippedAt = null;
        this.deliveredAt = null;
        this.trackingNumber = null;
        this.notes = '';
        this.orderNumber = this.generateOrderNumber();
        this.estimatedDelivery = this.calculateEstimatedDelivery();
    }
    
    /**
     * Update order status
     * @param {string} status - New status
     * @param {string} notes - Optional notes
     */
    updateStatus(status, notes = '') {
        const validStatuses = ['pending', 'processing', 'shipped', 'delivered', 'cancelled', 'returned'];
        
        if (!validStatuses.includes(status)) {
            throw new Error(`Invalid status: ${status}`);
        }
        
        this.status = status;
        this.updatedAt = new Date();
        
        if (notes) {
            this.notes += `${new Date().toISOString()}: ${notes}\n`;
        }
        
        // Update timestamps based on status
        if (status === 'shipped' && !this.shippedAt) {
            this.shippedAt = new Date();
        } else if (status === 'delivered' && !this.deliveredAt) {
            this.deliveredAt = new Date();
        }
    }
    
    /**
     * Set payment method
     * @param {string} method - Payment method
     * @param {string} status - Payment status
     */
    setPayment(method, status = 'pending') {
        this.paymentMethod = method;
        this.paymentStatus = status;
        this.updatedAt = new Date();
    }
    
    /**
     * Set tracking number
     * @param {string} trackingNumber - Tracking number
     */
    setTrackingNumber(trackingNumber) {
        this.trackingNumber = trackingNumber;
        this.updatedAt = new Date();
    }
    
    /**
     * Calculate estimated delivery date
     */
    calculateEstimatedDelivery() {
        const deliveryDate = new Date();
        
        // Add 3-7 business days for delivery
        let daysToAdd = 3 + Math.floor(Math.random() * 5);
        let businessDaysAdded = 0;
        
        while (businessDaysAdded < daysToAdd) {
            deliveryDate.setDate(deliveryDate.getDate() + 1);
            
            // Monday=1, Friday=5
            if (deliveryDate.getDay() >= 1 && deliveryDate.getDay() <= 5) {
                businessDaysAdded++;
            }
        }
        
        return deliveryDate;
    }
    
    /**
     * Generate order number
     */
    generateOrderNumber() {
        const date = new Date();
        const year = date.getFullYear();
        const month = (date.getMonth() + 1).toString().padStart(2, '0');
        const day = date.getDate().toString().padStart(2, '0');
        const random = Math.floor(Math.random() * 10000).toString().padStart(4, '0');
        
        return `${year}${month}${day}-${random}`;
    }
    
    /**
     * Cancel order
     * @param {string} reason - Cancellation reason
     */
    cancel(reason = '') {
        if (this.status === 'shipped' || this.status === 'delivered') {
            throw new Error('Cannot cancel order that has been shipped or delivered');
        }
        
        this.updateStatus('cancelled', reason);
        
        // Process refund if payment was made
        if (this.paymentStatus === 'paid') {
            this.processRefund();
        }
    }
    
    /**
     * Process refund
     */
    processRefund() {
        // In a real implementation, this would interact with payment processor
        this.paymentStatus = 'refunded';
        this.updatedAt = new Date();
    }
    
    /**
     * Get order summary
     */
    getSummary() {
        return {
            id: this.id,
            orderNumber: this.orderNumber,
            userId: this.userId,
            status: this.status,
            paymentStatus: this.paymentStatus,
            itemCount: this.items.length,
            subtotal: this.subtotal,
            tax: this.tax,
            shipping: this.shipping,
            discount: this.discount,
            total: this.total,
            currency: this.currency,
            createdAt: this.createdAt,
            updatedAt: this.updatedAt,
            shippedAt: this.shippedAt,
            deliveredAt: this.deliveredAt,
            estimatedDelivery: this.estimatedDelivery,
            trackingNumber: this.trackingNumber
        };
    }
    
    /**
     * Get detailed order information
     */
    getDetails() {
        return {
            ...this.getSummary(),
            items: this.items,
            shippingAddress: this.shippingAddress,
            billingAddress: this.billingAddress,
            paymentMethod: this.paymentMethod,
            notes: this.notes
        };
    }
}

// Customer class represents a customer
class Customer {
    constructor(id, email, firstName, lastName) {
        this.id = id;
        this.email = email;
        this.firstName = firstName;
        this.lastName = lastName;
        this.phone = null;
        this.dateOfBirth = null;
        this.addresses = [];
        this.paymentMethods = [];
        this.orders = [];
        this.wishlist = new Set();
        this.preferences = {
            currency: 'USD',
            language: 'en',
            notifications: true,
            newsletter: false
        };
        this.loyaltyPoints = 0;
        this.tier = 'bronze'; // bronze, silver, gold, platinum
        this.createdAt = new Date();
        this.lastLogin = null;
        this.isVerified = false;
    }
    
    /**
     * Get full name
     */
    getFullName() {
        return `${this.firstName} ${this.lastName}`;
    }
    
    /**
     * Add address
     * @param {Object} address - Address object
     * @param {boolean} isDefault - Whether this is the default address
     */
    addAddress(address, isDefault = false) {
        const newAddress = {
            id: this.generateId(),
            ...address,
            isDefault: isDefault || this.addresses.length === 0,
            addedAt: new Date()
        };
        
        // If this is default, unmark others
        if (newAddress.isDefault) {
            this.addresses.forEach(addr => addr.isDefault = false);
        }
        
        this.addresses.push(newAddress);
        return newAddress.id;
    }
    
    /**
     * Remove address
     * @param {string} addressId - Address ID to remove
     */
    removeAddress(addressId) {
        const index = this.addresses.findIndex(addr => addr.id === addressId);
        if (index !== -1) {
            const removed = this.addresses.splice(index, 1)[0];
            
            // If removed address was default, set first remaining as default
            if (removed.isDefault && this.addresses.length > 0) {
                this.addresses[0].isDefault = true;
            }
            
            return true;
        }
        
        return false;
    }
    
    /**
     * Get default address
     */
    getDefaultAddress() {
        return this.addresses.find(addr => addr.isDefault) || null;
    }
    
    /**
     * Add payment method
     * @param {Object} paymentMethod - Payment method object
     * @param {boolean} isDefault - Whether this is the default payment method
     */
    addPaymentMethod(paymentMethod, isDefault = false) {
        const newMethod = {
            id: this.generateId(),
            ...paymentMethod,
            isDefault: isDefault || this.paymentMethods.length === 0,
            addedAt: new Date()
        };
        
        // If this is default, unmark others
        if (newMethod.isDefault) {
            this.paymentMethods.forEach(method => method.isDefault = false);
        }
        
        this.paymentMethods.push(newMethod);
        return newMethod.id;
    }
    
    /**
     * Remove payment method
     * @param {string} methodId - Payment method ID to remove
     */
    removePaymentMethod(methodId) {
        const index = this.paymentMethods.findIndex(method => method.id === methodId);
        if (index !== -1) {
            const removed = this.paymentMethods.splice(index, 1)[0];
            
            // If removed method was default, set first remaining as default
            if (removed.isDefault && this.paymentMethods.length > 0) {
                this.paymentMethods[0].isDefault = true;
            }
            
            return true;
        }
        
        return false;
    }
    
    /**
     * Get default payment method
     */
    getDefaultPaymentMethod() {
        return this.paymentMethods.find(method => method.isDefault) || null;
    }
    
    /**
     * Add order
     * @param {Order} order - Order to add
     */
    addOrder(order) {
        this.orders.push(order.getSummary());
        
        // Update loyalty points
        this.addLoyaltyPoints(Math.floor(order.total / 10));
    }
    
    /**
     * Add product to wishlist
     * @param {string} productId - Product ID to add
     */
    addToWishlist(productId) {
        this.wishlist.add(productId);
    }
    
    /**
     * Remove product from wishlist
     * @param {string} productId - Product ID to remove
     */
    removeFromWishlist(productId) {
        return this.wishlist.delete(productId);
    }
    
    /**
     * Check if product is in wishlist
     * @param {string} productId - Product ID to check
     */
    isInWishlist(productId) {
        return this.wishlist.has(productId);
    }
    
    /**
     * Add loyalty points
     * @param {number} points - Points to add
     */
    addLoyaltyPoints(points) {
        this.loyaltyPoints += points;
        this.updateTier();
    }
    
    /**
     * Redeem loyalty points
     * @param {number} points - Points to redeem
     */
    redeemLoyaltyPoints(points) {
        if (points > this.loyaltyPoints) {
            throw new Error('Insufficient loyalty points');
        }
        
        this.loyaltyPoints -= points;
        return points;
    }
    
    /**
     * Update loyalty tier based on points
     */
    updateTier() {
        if (this.loyaltyPoints >= 1000) {
            this.tier = 'platinum';
        } else if (this.loyaltyPoints >= 500) {
            this.tier = 'gold';
        } else if (this.loyaltyPoints >= 200) {
            this.tier = 'silver';
        } else {
            this.tier = 'bronze';
        }
    }
    
    /**
     * Update last login
     */
    updateLastLogin() {
        this.lastLogin = new Date();
    }
    
    /**
     * Generate unique ID
     */
    generateId() {
        return Date.now().toString(36) + Math.random().toString(36).substring(2);
    }
    
    /**
     * Get customer summary
     */
    getSummary() {
        return {
            id: this.id,
            email: this.email,
            name: this.getFullName(),
            loyaltyPoints: this.loyaltyPoints,
            tier: this.tier,
            orderCount: this.orders.length,
            wishlistCount: this.wishlist.size,
            createdAt: this.createdAt,
            lastLogin: this.lastLogin,
            isVerified: this.isVerified
        };
    }
}

// Main function to demonstrate e-commerce system
function main() {
    console.log('Initializing E-commerce System...\n');
    
    // Create products
    const laptop = new Product(
        'laptop1',
        'Premium Laptop Pro',
        'High-performance laptop with latest processor and graphics',
        1299.99,
        'Electronics',
        15
    );
    
    laptop.addImage('https://example.com/laptop1.jpg', 'Premium Laptop Pro', true);
    laptop.addImage('https://example.com/laptop1-2.jpg', 'Laptop side view');
    laptop.addSpecification('CPU', 'Intel Core i7-12700H');
    laptop.addSpecification('RAM', '16GB DDR4');
    laptop.addSpecification('Storage', '512GB SSD');
    laptop.addSpecification('Display', '15.6" Full HD');
    laptop.addTag('laptop');
    laptop.addTag('computer');
    laptop.addTag('portable');
    laptop.weight = 2.1;
    laptop.dimensions = { length: 35.5, width: 23.8, height: 1.8 };
    
    const headphones = new Product(
        'headphones1',
        'Wireless Noise-Canceling Headphones',
        'Premium wireless headphones with active noise cancellation',
        249.99,
        'Electronics',
        30
    );
    
    headphones.addImage('https://example.com/headphones1.jpg', 'Wireless Headphones', true);
    headphones.addSpecification('Type', 'Over-ear');
    headphones.addSpecification('Connectivity', 'Bluetooth 5.0');
    headphones.addSpecification('Battery Life', '30 hours');
    headphones.addSpecification('Noise Cancellation', 'Active');
    headphones.addTag('headphones');
    headphones.addTag('wireless');
    headphones.addTag('audio');
    headphones.weight = 0.25;
    headphones.dimensions = { length: 18, width: 16, height: 8 };
    
    const ebook = new Product(
        'ebook1',
        'JavaScript: The Definitive Guide',
        'Comprehensive guide to JavaScript programming',
        39.99,
        'Books',
        100
    );
    
    ebook.addImage('https://example.com/ebook1.jpg', 'JavaScript eBook Cover', true);
    ebook.addSpecification('Author', 'David Flanagan');
    ebook.addSpecification('Pages', '704');
    ebook.addSpecification('Format', 'PDF, ePub');
    ebook.addSpecification('Language', 'English');
    ebook.addTag('javascript');
    ebook.addTag('programming');
    ebook.addTag('web');
    ebook.isDigital = true;
    ebook.downloadUrl = 'https://example.com/download/ebook1.pdf';
    ebook.shippingRequired = false;
    
    console.log('Created products:');
    console.log(`- ${laptop.name}: $${laptop.price} (${laptop.stock} in stock)`);
    console.log(`- ${headphones.name}: $${headphones.price} (${headphones.stock} in stock)`);
    console.log(`- ${ebook.name}: $${ebook.price} (${ebook.stock} in stock)`);
    
    // Create customer
    const customer = new Customer('customer1', 'john.doe@example.com', 'John', 'Doe');
    
    customer.addAddress({
        street: '123 Main St',
        city: 'New York',
        state: 'NY',
        zipCode: '10001',
        country: 'USA'
    }, true);
    
    customer.addPaymentMethod({
        type: 'credit-card',
        cardType: 'Visa',
        lastFour: '1234',
        expiryMonth: 12,
        expiryYear: 2025
    }, true);
    
    customer.addToWishlist(laptop.id);
    customer.addToWishlist(ebook.id);
    customer.addLoyaltyPoints(50);
    
    console.log('\nCreated customer:');
    console.log(`- Name: ${customer.getFullName()}`);
    console.log(`- Email: ${customer.email}`);
    console.log(`- Loyalty Points: ${customer.loyaltyPoints}`);
    console.log(`- Tier: ${customer.tier}`);
    console.log(`- Wishlist: ${customer.wishlist.size} items`);
    
    // Create shopping cart
    const cart = new ShoppingCart(customer.id);
    
    // Add items to cart
    cart.addItem(laptop, 1, { color: 'Silver', warranty: '2-year' });
    cart.addItem(headphones, 2, { color: 'Black' });
    cart.addItem(ebook, 1);
    
    console.log('\nShopping cart created:');
    console.log(`- Items: ${cart.getItemCount()}`);
    console.log(`- Subtotal: $${cart.subtotal.toFixed(2)}`);
    console.log(`- Tax: $${cart.tax.toFixed(2)}`);
    console.log(`- Shipping: $${cart.shipping.toFixed(2)}`);
    console.log(`- Total: $${cart.total.toFixed(2)}`);
    
    // Apply discount coupon
    cart.applyCoupon('SAVE20', 20, false);
    
    console.log('\nAfter applying coupon:');
    console.log(`- Discount: $${cart.discount.toFixed(2)}`);
    console.log(`- Total: $${cart.total.toFixed(2)}`);
    
    // Create order
    const order = new Order(
        'order1',
        customer.id,
        cart,
        customer.getDefaultAddress(),
        customer.getDefaultAddress()
    );
    
    order.setPayment(customer.getDefaultPaymentMethod().type, 'paid');
    order.updateStatus('processing');
    
    customer.addOrder(order);
    
    console.log('\nOrder created:');
    console.log(`- Order Number: ${order.orderNumber}`);
    console.log(`- Status: ${order.status}`);
    console.log(`- Total: $${order.total.toFixed(2)}`);
    console.log(`- Estimated Delivery: ${order.estimatedDelivery.toLocaleDateString()}`);
    
    // Simulate order processing
    setTimeout(() => {
        order.updateStatus('shipped', 'Order shipped via UPS');
        order.setTrackingNumber('1Z999AA10123456784');
        
        console.log('\nOrder updated:');
        console.log(`- Status: ${order.status}`);
        console.log(`- Tracking Number: ${order.trackingNumber}`);
        
        // Show customer updated info
        console.log('\nCustomer summary:');
        console.log(`- Total Orders: ${customer.orders.length}`);
        console.log(`- Loyalty Points: ${customer.loyaltyPoints}`);
        console.log(`- Tier: ${customer.tier}`);
    }, 2000);
}

// Export classes for use in other modules
if (typeof module !== 'undefined') {
    module.exports = {
        Product,
        ShoppingCart,
        Order,
        Customer
    };
}