import java.util.*;
import java.time.*;

/**
 * 电子商务平台类
 * 用于管理产品、订单、支付和用户服务
 */
public class ECommercePlatform {
    private Map<String, User> users;
    private Map<String, Product> products;
    private Map<String, Category> categories;
    private Map<String, Order> orders;
    private Map<String, Payment> payments;
    private Map<String, Shipment> shipments;
    private Map<String, Review> reviews;
    private Map<String, ShoppingCart> shoppingCarts;
    private Map<String, Wishlist> wishlists;
    private Map<String, Coupon> coupons;
    
    /**
     * 电子商务平台构造函数
     */
    public ECommercePlatform() {
        this.users = new HashMap<>();
        this.products = new HashMap<>();
        this.categories = new HashMap<>();
        this.orders = new HashMap<>();
        this.payments = new HashMap<>();
        this.shipments = new HashMap<>();
        this.reviews = new HashMap<>();
        this.shoppingCarts = new HashMap<>();
        this.wishlists = new HashMap<>();
        this.coupons = new HashMap<>();
    }
    
    /**
     * 创建新用户
     * @param userId 用户唯一标识符
     * @param username 用户名
     * @param email 电子邮件
     * @param password 密码
     * @param shippingAddress 配送地址（可选）
     * @param billingAddress 账单地址（可选）
     * @return 创建成功返回true，否则返回false
     */
    public boolean createUser(String userId, String username, String email, String password,
                               Map<String, String> shippingAddress, Map<String, String> billingAddress) {
        if (users.containsKey(userId)) {
            return false;
        }
        
        if (users.values().stream().anyMatch(u -> u.getEmail().equals(email))) {
            return false;
        }
        
        if (users.values().stream().anyMatch(u -> u.getUsername().equals(username))) {
            return false;
        }
        
        User user = new User(userId, username, email, password, shippingAddress, billingAddress);
        users.put(userId, user);
        
        // 为用户创建购物车和愿望清单
        String shoppingCartId = generateId("cart");
        ShoppingCart shoppingCart = new ShoppingCart(shoppingCartId, userId);
        shoppingCarts.put(shoppingCartId, shoppingCart);
        user.setShoppingCartId(shoppingCartId);
        
        String wishlistId = generateId("wishlist");
        Wishlist wishlist = new Wishlist(wishlistId, userId);
        wishlists.put(wishlistId, wishlist);
        user.setWishlistId(wishlistId);
        
        return true;
    }
    
    /**
     * 创建产品类别
     * @param categoryId 类别唯一标识符
     * @param name 类别名称
     * @param description 类别描述（可选）
     * @param parentId 父类别ID（可选）
     * @return 创建成功返回true，否则返回false
     */
    public boolean createCategory(String categoryId, String name, String description, String parentId) {
        if (categories.containsKey(categoryId)) {
            return false;
        }
        
        if (parentId != null && !categories.containsKey(parentId)) {
            return false;
        }
        
        Category category = new Category(categoryId, name, description, parentId);
        categories.put(categoryId, category);
        return true;
    }
    
    /**
     * 创建新产品
     * @param productId 产品唯一标识符
     * @param name 产品名称
     * @param description 产品描述
     * @param price 产品价格
     * @param categoryId 产品类别ID
     * @param stockQuantity 库存数量
     * @param sku SKU（可选）
     * @param weight 产品重量（可选）
     * @param dimensions 产品尺寸（可选）
     * @param images 图片URL列表（可选）
     * @return 创建成功返回true，否则返回false
     */
    public boolean createProduct(String productId, String name, String description, double price,
                                 String categoryId, int stockQuantity, String sku, Double weight,
                                 Map<String, Double> dimensions, List<String> images) {
        if (products.containsKey(productId)) {
            return false;
        }
        
        if (!categories.containsKey(categoryId)) {
            return false;
        }
        
        Product product = new Product(productId, name, description, price, categoryId, 
                                      stockQuantity, sku, weight, dimensions, images);
        products.put(productId, product);
        return true;
    }
    
    /**
     * 添加产品到购物车
     * @param userId 用户ID
     * @param productId 产品ID
     * @param quantity 数量
     * @return 添加成功返回true，否则返回false
     */
    public boolean addToCart(String userId, String productId, int quantity) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return false;
        }
        
        if (quantity <= 0) {
            return false;
        }
        
        Product product = products.get(productId);
        if (product.getStockQuantity() < quantity) {
            return false;  // 库存不足
        }
        
        String shoppingCartId = users.get(userId).getShoppingCartId();
        if (!shoppingCarts.containsKey(shoppingCartId)) {
            return false;
        }
        
        ShoppingCart shoppingCart = shoppingCarts.get(shoppingCartId);
        return shoppingCart.addItem(productId, quantity);
    }
    
    /**
     * 从购物车中移除产品
     * @param userId 用户ID
     * @param productId 产品ID
     * @return 移除成功返回true，否则返回false
     */
    public boolean removeFromCart(String userId, String productId) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return false;
        }
        
        String shoppingCartId = users.get(userId).getShoppingCartId();
        if (!shoppingCarts.containsKey(shoppingCartId)) {
            return false;
        }
        
        ShoppingCart shoppingCart = shoppingCarts.get(shoppingCartId);
        return shoppingCart.removeItem(productId);
    }
    
    /**
     * 更新购物车中产品的数量
     * @param userId 用户ID
     * @param productId 产品ID
     * @param newQuantity 新数量
     * @return 更新成功返回true，否则返回false
     */
    public boolean updateCartQuantity(String userId, String productId, int newQuantity) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return false;
        }
        
        if (newQuantity <= 0) {
            return false;
        }
        
        Product product = products.get(productId);
        if (product.getStockQuantity() < newQuantity) {
            return false;  // 库存不足
        }
        
        String shoppingCartId = users.get(userId).getShoppingCartId();
        if (!shoppingCarts.containsKey(shoppingCartId)) {
            return false;
        }
        
        ShoppingCart shoppingCart = shoppingCarts.get(shoppingCartId);
        return shoppingCart.updateItemQuantity(productId, newQuantity);
    }
    
    /**
     * 获取用户购物车
     * @param userId 用户ID
     * @return 购物车信息，用户不存在返回null
     */
    public ShoppingCart getShoppingCart(String userId) {
        if (!users.containsKey(userId)) {
            return null;
        }
        
        String shoppingCartId = users.get(userId).getShoppingCartId();
        if (!shoppingCarts.containsKey(shoppingCartId)) {
            return null;
        }
        
        return shoppingCarts.get(shoppingCartId);
    }
    
    /**
     * 添加产品到愿望清单
     * @param userId 用户ID
     * @param productId 产品ID
     * @return 添加成功返回true，否则返回false
     */
    public boolean addToWishlist(String userId, String productId) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return false;
        }
        
        String wishlistId = users.get(userId).getWishlistId();
        if (!wishlists.containsKey(wishlistId)) {
            return false;
        }
        
        Wishlist wishlist = wishlists.get(wishlistId);
        return wishlist.addItem(productId);
    }
    
    /**
     * 从愿望清单中移除产品
     * @param userId 用户ID
     * @param productId 产品ID
     * @return 移除成功返回true，否则返回false
     */
    public boolean removeFromWishlist(String userId, String productId) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return false;
        }
        
        String wishlistId = users.get(userId).getWishlistId();
        if (!wishlists.containsKey(wishlistId)) {
            return false;
        }
        
        Wishlist wishlist = wishlists.get(wishlistId);
        return wishlist.removeItem(productId);
    }
    
    /**
     * 获取用户愿望清单
     * @param userId 用户ID
     * @return 愿望清单信息，用户不存在返回null
     */
    public Wishlist getWishlist(String userId) {
        if (!users.containsKey(userId)) {
            return null;
        }
        
        String wishlistId = users.get(userId).getWishlistId();
        if (!wishlists.containsKey(wishlistId)) {
            return null;
        }
        
        return wishlists.get(wishlistId);
    }
    
    /**
     * 创建订单
     * @param userId 用户ID
     * @param shippingAddress 配送地址（可选）
     * @param billingAddress 账单地址（可选）
     * @param couponId 优惠券ID（可选）
     * @return 创建成功返回订单ID，否则返回null
     */
    public String createOrder(String userId, Map<String, String> shippingAddress, 
                              Map<String, String> billingAddress, String couponId) {
        if (!users.containsKey(userId)) {
            return null;
        }
        
        ShoppingCart shoppingCart = getShoppingCart(userId);
        if (shoppingCart == null || shoppingCart.getItems().isEmpty()) {
            return null;  // 购物车为空
        }
        
        // 使用提供的地址或用户默认地址
        User user = users.get(userId);
        Map<String, String> finalShippingAddress = shippingAddress != null ? 
            new HashMap<>(shippingAddress) : (user.getShippingAddress() != null ? 
            new HashMap<>(user.getShippingAddress()) : null);
        
        Map<String, String> finalBillingAddress = billingAddress != null ? 
            new HashMap<>(billingAddress) : (user.getBillingAddress() != null ? 
            new HashMap<>(user.getBillingAddress()) : finalShippingAddress);
        
        if (finalShippingAddress == null) {
            return null;  // 缺少配送地址
        }
        
        // 检查优惠券
        double discount = 0;
        if (couponId != null && coupons.containsKey(couponId)) {
            Coupon coupon = coupons.get(couponId);
            if (coupon.isValid()) {
                discount = coupon.getDiscountAmount(shoppingCart.getTotalPrice());
            }
        }
        
        // 创建订单
        String orderId = generateId("order");
        Order order = new Order(orderId, userId, new ArrayList<>(shoppingCart.getItems()), 
                               shoppingCart.getTotalPrice(), discount, finalShippingAddress, 
                               finalBillingAddress, couponId);
        orders.put(orderId, order);
        
        // 更新用户的订单列表
        user.addOrder(orderId);
        
        // 更新库存
        for (ShoppingCartItem item : shoppingCart.getItems()) {
            Product product = products.get(item.getProductId());
            product.setStockQuantity(product.getStockQuantity() - item.getQuantity());
        }
        
        // 清空购物车
        shoppingCart.clearItems();
        
        return orderId;
    }
    
    /**
     * 处理订单支付
     * @param orderId 订单ID
     * @param paymentMethod 支付方式
     * @param paymentDetails 支付详情
     * @return 支付成功返回支付ID，否则返回null
     */
    public String processPayment(String orderId, String paymentMethod, Map<String, String> paymentDetails) {
        if (!orders.containsKey(orderId)) {
            return null;
        }
        
        Order order = orders.get(orderId);
        if (!order.getStatus().equals("pending")) {
            return null;  // 订单状态不正确
        }
        
        // 创建支付记录
        String paymentId = generateId("payment");
        Payment payment = new Payment(paymentId, orderId, order.getFinalAmount(), 
                                    paymentMethod, paymentDetails);
        payments.put(paymentId, payment);
        
        // 更新订单状态
        order.setStatus("paid");
        order.setPaymentId(paymentId);
        
        return paymentId;
    }
    
    /**
     * 配送订单
     * @param orderId 订单ID
     * @param shippingMethod 配送方式
     * @param trackingNumber 追踪号码（可选）
     * @return 配送成功返回运输ID，否则返回null
     */
    public String shipOrder(String orderId, String shippingMethod, String trackingNumber) {
        if (!orders.containsKey(orderId)) {
            return null;
        }
        
        Order order = orders.get(orderId);
        if (!order.getStatus().equals("paid")) {
            return null;  // 订单状态不正确
        }
        
        // 创建运输记录
        String shipmentId = generateId("shipment");
        Shipment shipment = new Shipment(shipmentId, orderId, shippingMethod, trackingNumber);
        shipments.put(shipmentId, shipment);
        
        // 更新订单状态
        order.setStatus("shipped");
        order.setShipmentId(shipmentId);
        
        return shipmentId;
    }
    
    /**
     * 标记订单为已送达
     * @param orderId 订单ID
     * @return 更新成功返回true，否则返回false
     */
    public boolean deliverOrder(String orderId) {
        if (!orders.containsKey(orderId)) {
            return false;
        }
        
        Order order = orders.get(orderId);
        if (!order.getStatus().equals("shipped")) {
            return false;  // 订单状态不正确
        }
        
        // 更新订单状态
        order.setStatus("delivered");
        order.setDeliveredDate(LocalDateTime.now());
        
        // 更新运输记录状态
        String shipmentId = order.getShipmentId();
        if (shipmentId != null && shipments.containsKey(shipmentId)) {
            shipments.get(shipmentId).setStatus("delivered");
            shipments.get(shipmentId).setDeliveredDate(LocalDateTime.now());
        }
        
        return true;
    }
    
    /**
     * 取消订单
     * @param orderId 订单ID
     * @param reason 取消原因
     * @return 取消成功返回true，否则返回false
     */
    public boolean cancelOrder(String orderId, String reason) {
        if (!orders.containsKey(orderId)) {
            return false;
        }
        
        Order order = orders.get(orderId);
        if (!order.getStatus().equals("pending")) {
            return false;  // 订单状态不正确
        }
        
        // 更新订单状态
        order.setStatus("cancelled");
        order.setCancellationReason(reason);
        order.setCancelledDate(LocalDateTime.now());
        
        // 恢复库存
        for (OrderItem item : order.getItems()) {
            Product product = products.get(item.getProductId());
            if (product != null) {
                product.setStockQuantity(product.getStockQuantity() + item.getQuantity());
            }
        }
        
        return true;
    }
    
    /**
     * 添加产品评论
     * @param userId 用户ID
     * @param productId 产品ID
     * @param rating 评分（1-5）
     * @param comment 评论内容（可选）
     * @return 评论成功返回评论ID，否则返回null
     */
    public String addProductReview(String userId, String productId, int rating, String comment) {
        if (!users.containsKey(userId) || !products.containsKey(productId)) {
            return null;
        }
        
        if (rating < 1 || rating > 5) {
            return null;
        }
        
        // 检查用户是否已购买该产品
        boolean hasPurchased = false;
        for (String orderId : users.get(userId).getOrderIds()) {
            if (orders.containsKey(orderId)) {
                Order order = orders.get(orderId);
                if (order.getStatus().equals("delivered")) {
                    for (OrderItem item : order.getItems()) {
                        if (item.getProductId().equals(productId)) {
                            hasPurchased = true;
                            break;
                        }
                    }
                }
            }
            if (hasPurchased) {
                break;
            }
        }
        
        if (!hasPurchased) {
            return null;  // 用户未购买该产品，不能评论
        }
        
        // 检查用户是否已评论过该产品
        for (Review review : reviews.values()) {
            if (review.getUserId().equals(userId) && review.getProductId().equals(productId)) {
                return null;  // 用户已评论过该产品
            }
        }
        
        // 创建评论
        String reviewId = generateId("review");
        Review review = new Review(reviewId, userId, productId, rating, comment);
        reviews.put(reviewId, review);
        
        return reviewId;
    }
    
    /**
     * 获取产品评论
     * @param productId 产品ID
     * @param limit 返回评论数量限制
     * @return 评论列表
     */
    public List<Review> getProductReviews(String productId, int limit) {
        if (!products.containsKey(productId)) {
            return new ArrayList<>();
        }
        
        List<Review> result = new ArrayList<>();
        for (Review review : reviews.values()) {
            if (review.getProductId().equals(productId)) {
                result.add(review);
            }
        }
        
        // 按创建时间降序排序
        result.sort((r1, r2) -> r2.getCreatedAt().compareTo(r1.getCreatedAt()));
        
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取产品详细信息
     * @param productId 产品ID
     * @return 产品详细信息，产品不存在返回null
     */
    public ProductDetails getProductDetails(String productId) {
        if (!products.containsKey(productId)) {
            return null;
        }
        
        Product product = products.get(productId);
        ProductDetails details = new ProductDetails(product);
        
        // 添加类别信息
        String categoryId = product.getCategoryId();
        if (categories.containsKey(categoryId)) {
            details.setCategoryName(categories.get(categoryId).getName());
        }
        
        // 计算平均评分
        List<Review> productReviews = getProductReviews(productId, 1000);
        if (!productReviews.isEmpty()) {
            double averageRating = productReviews.stream()
                .mapToInt(Review::getRating)
                .average()
                .orElse(0);
            details.setAverageRating(Math.round(averageRating * 10.0) / 10.0);
            details.setReviewCount(productReviews.size());
        }
        
        return details;
    }
    
    /**
     * 搜索产品
     * @param keyword 搜索关键词
     * @param categoryId 类别筛选（可选）
     * @param minPrice 最低价格（可选）
     * @param maxPrice 最高价格（可选）
     * @param sortBy 排序方式（price_asc, price_desc, rating, newest）
     * @param limit 返回产品数量限制
     * @return 匹配的产品列表
     */
    public List<ProductDetails> searchProducts(String keyword, String categoryId, 
                                               Double minPrice, Double maxPrice, 
                                               String sortBy, int limit) {
        keyword = keyword.toLowerCase();
        List<ProductDetails> result = new ArrayList<>();
        
        for (Product product : products.values()) {
            // 检查关键词匹配
            if (!product.getName().toLowerCase().contains(keyword) && 
                !product.getDescription().toLowerCase().contains(keyword) &&
                !product.getSku().toLowerCase().contains(keyword)) {
                continue;
            }
            
            // 检查类别匹配
            if (categoryId != null && !product.getCategoryId().equals(categoryId)) {
                continue;
            }
            
            // 检查价格范围
            double price = product.getPrice();
            if (minPrice != null && price < minPrice) {
                continue;
            }
            
            if (maxPrice != null && price > maxPrice) {
                continue;
            }
            
            // 添加到结果列表
            ProductDetails details = getProductDetails(product.getProductId());
            if (details != null) {
                result.add(details);
            }
        }
        
        // 排序
        switch (sortBy) {
            case "price_asc":
                result.sort(Comparator.comparing(ProductDetails::getPrice));
                break;
            case "price_desc":
                result.sort((p1, p2) -> Double.compare(p2.getPrice(), p1.getPrice()));
                break;
            case "rating":
                result.sort((p1, p2) -> Double.compare(p2.getAverageRating(), p1.getAverageRating()));
                break;
            case "newest":
                result.sort((p1, p2) -> p2.getCreatedAt().compareTo(p1.getCreatedAt()));
                break;
            default:
                // 默认按相关性排序（这里简化为按名称匹配度）
                result.sort(Comparator.comparing(p -> !p.getName().toLowerCase().startsWith(keyword)));
        }
        
        // 限制返回数量
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取用户订单
     * @param userId 用户ID
     * @param status 订单状态筛选（可选）
     * @param limit 返回订单数量限制
     * @return 订单列表
     */
    public List<Order> getUserOrders(String userId, String status, int limit) {
        if (!users.containsKey(userId)) {
            return new ArrayList<>();
        }
        
        List<Order> result = new ArrayList<>();
        for (String orderId : users.get(userId).getOrderIds()) {
            if (orders.containsKey(orderId)) {
                Order order = orders.get(orderId);
                
                if (status != null && !order.getStatus().equals(status)) {
                    continue;
                }
                
                result.add(order);
            }
        }
        
        // 按创建时间降序排序
        result.sort((o1, o2) -> o2.getCreatedAt().compareTo(o1.getCreatedAt()));
        
        if (result.size() > limit) {
            result = result.subList(0, limit);
        }
        
        return result;
    }
    
    /**
     * 创建优惠券
     * @param couponId 优惠券唯一标识符
     * @param code 优惠券代码
     * @param discountType 折扣类型（percentage, fixed）
     * @param discountValue 折扣值
     * @param minOrderAmount 最低订单金额（可选）
     * @param expiryDate 过期日期
     * @param usageLimit 使用限制（可选）
     * @return 创建成功返回true，否则返回false
     */
    public boolean createCoupon(String couponId, String code, String discountType, 
                               double discountValue, Double minOrderAmount, 
                               LocalDateTime expiryDate, Integer usageLimit) {
        if (coupons.containsKey(couponId)) {
            return false;
        }
        
        if (!"percentage".equals(discountType) && !"fixed".equals(discountType)) {
            return false;
        }
        
        if (discountValue <= 0) {
            return false;
        }
        
        if ("percentage".equals(discountType) && discountValue > 100) {
            return false;
        }
        
        Coupon coupon = new Coupon(couponId, code, discountType, discountValue, 
                                   minOrderAmount, expiryDate, usageLimit);
        coupons.put(couponId, coupon);
        
        return true;
    }
    
    /**
     * 应用优惠券
     * @param couponId 优惠券ID
     * @param orderAmount 订单金额
     * @return 应用成功返回折扣金额，否则返回0
     */
    public double applyCoupon(String couponId, double orderAmount) {
        if (!coupons.containsKey(couponId)) {
            return 0;
        }
        
        Coupon coupon = coupons.get(couponId);
        if (!coupon.isValid()) {
            return 0;
        }
        
        if (coupon.getMinOrderAmount() != null && orderAmount < coupon.getMinOrderAmount()) {
            return 0;
        }
        
        double discount = coupon.getDiscountAmount(orderAmount);
        if (discount <= 0) {
            return 0;
        }
        
        // 更新优惠券使用次数
        coupon.incrementUsageCount();
        
        return discount;
    }
    
    /**
     * 生成唯一ID
     * @param prefix ID前缀
     * @return 唯一ID字符串
     */
    private String generateId(String prefix) {
        return prefix + "_" + UUID.randomUUID().toString().substring(0, 8);
    }
}

/**
 * 用户类
 */
class User {
    private String userId;
    private String username;
    private String email;
    private String password;
    private Map<String, String> shippingAddress;
    private Map<String, String> billingAddress;
    private List<String> orderIds;
    private String shoppingCartId;
    private String wishlistId;
    private LocalDateTime createdAt;
    
    public User(String userId, String username, String email, String password,
                Map<String, String> shippingAddress, Map<String, String> billingAddress) {
        this.userId = userId;
        this.username = username;
        this.email = email;
        this.password = password;
        this.shippingAddress = shippingAddress != null ? new HashMap<>(shippingAddress) : null;
        this.billingAddress = billingAddress != null ? new HashMap<>(billingAddress) : null;
        this.orderIds = new ArrayList<>();
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getUserId() {
        return userId;
    }
    
    public String getUsername() {
        return username;
    }
    
    public String getEmail() {
        return email;
    }
    
    public Map<String, String> getShippingAddress() {
        return shippingAddress != null ? new HashMap<>(shippingAddress) : null;
    }
    
    public Map<String, String> getBillingAddress() {
        return billingAddress != null ? new HashMap<>(billingAddress) : null;
    }
    
    public List<String> getOrderIds() {
        return new ArrayList<>(orderIds);
    }
    
    public String getShoppingCartId() {
        return shoppingCartId;
    }
    
    public String getWishlistId() {
        return wishlistId;
    }
    
    public void setShoppingCartId(String shoppingCartId) {
        this.shoppingCartId = shoppingCartId;
    }
    
    public void setWishlistId(String wishlistId) {
        this.wishlistId = wishlistId;
    }
    
    public void addOrder(String orderId) {
        orderIds.add(orderId);
    }
}

/**
 * 产品类
 */
class Product {
    private String productId;
    private String name;
    private String description;
    private double price;
    private String categoryId;
    private int stockQuantity;
    private String sku;
    private Double weight;
    private Map<String, Double> dimensions;
    private List<String> images;
    private LocalDateTime createdAt;
    private LocalDateTime updatedAt;
    
    public Product(String productId, String name, String description, double price,
                    String categoryId, int stockQuantity, String sku, Double weight,
                    Map<String, Double> dimensions, List<String> images) {
        this.productId = productId;
        this.name = name;
        this.description = description;
        this.price = price;
        this.categoryId = categoryId;
        this.stockQuantity = stockQuantity;
        this.sku = sku != null ? sku : "SKU-" + productId;
        this.weight = weight;
        this.dimensions = dimensions != null ? new HashMap<>(dimensions) : null;
        this.images = images != null ? new ArrayList<>(images) : new ArrayList<>();
        this.createdAt = LocalDateTime.now();
        this.updatedAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getProductId() {
        return productId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getDescription() {
        return description;
    }
    
    public double getPrice() {
        return price;
    }
    
    public String getCategoryId() {
        return categoryId;
    }
    
    public int getStockQuantity() {
        return stockQuantity;
    }
    
    public String getSku() {
        return sku;
    }
    
    public Double getWeight() {
        return weight;
    }
    
    public Map<String, Double> getDimensions() {
        return dimensions != null ? new HashMap<>(dimensions) : null;
    }
    
    public List<String> getImages() {
        return new ArrayList<>(images);
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public void setStockQuantity(int stockQuantity) {
        this.stockQuantity = stockQuantity;
        this.updatedAt = LocalDateTime.now();
    }
}

/**
 * 产品详情类
 */
class ProductDetails {
    private String productId;
    private String name;
    private String description;
    private double price;
    private String categoryId;
    private String categoryName;
    private int stockQuantity;
    private String sku;
    private Double weight;
    private Map<String, Double> dimensions;
    private List<String> images;
    private double averageRating;
    private int reviewCount;
    private LocalDateTime createdAt;
    
    public ProductDetails(Product product) {
        this.productId = product.getProductId();
        this.name = product.getName();
        this.description = product.getDescription();
        this.price = product.getPrice();
        this.categoryId = product.getCategoryId();
        this.stockQuantity = product.getStockQuantity();
        this.sku = product.getSku();
        this.weight = product.getWeight();
        this.dimensions = product.getDimensions();
        this.images = product.getImages();
        this.averageRating = 0;
        this.reviewCount = 0;
        this.createdAt = product.getCreatedAt();
    }
    
    // Getters and setters
    public String getProductId() {
        return productId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getDescription() {
        return description;
    }
    
    public double getPrice() {
        return price;
    }
    
    public String getCategoryId() {
        return categoryId;
    }
    
    public String getCategoryName() {
        return categoryName;
    }
    
    public void setCategoryName(String categoryName) {
        this.categoryName = categoryName;
    }
    
    public int getStockQuantity() {
        return stockQuantity;
    }
    
    public String getSku() {
        return sku;
    }
    
    public Double getWeight() {
        return weight;
    }
    
    public Map<String, Double> getDimensions() {
        return dimensions;
    }
    
    public List<String> getImages() {
        return images;
    }
    
    public double getAverageRating() {
        return averageRating;
    }
    
    public void setAverageRating(double averageRating) {
        this.averageRating = averageRating;
    }
    
    public int getReviewCount() {
        return reviewCount;
    }
    
    public void setReviewCount(int reviewCount) {
        this.reviewCount = reviewCount;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 类别类
 */
class Category {
    private String categoryId;
    private String name;
    private String description;
    private String parentId;
    private LocalDateTime createdAt;
    
    public Category(String categoryId, String name, String description, String parentId) {
        this.categoryId = categoryId;
        this.name = name;
        this.description = description;
        this.parentId = parentId;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getCategoryId() {
        return categoryId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getDescription() {
        return description;
    }
    
    public String getParentId() {
        return parentId;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 订单类
 */
class Order {
    private String orderId;
    private String userId;
    private List<OrderItem> items;
    private double subtotal;
    private double discount;
    private double finalAmount;
    private String status;  // pending, paid, shipped, delivered, cancelled
    private Map<String, String> shippingAddress;
    private Map<String, String> billingAddress;
    private String paymentId;
    private String shipmentId;
    private String couponId;
    private String cancellationReason;
    private LocalDateTime cancelledDate;
    private LocalDateTime deliveredDate;
    private LocalDateTime createdAt;
    
    public Order(String orderId, String userId, List<OrderItem> items, double subtotal,
                double discount, Map<String, String> shippingAddress, 
                Map<String, String> billingAddress, String couponId) {
        this.orderId = orderId;
        this.userId = userId;
        this.items = new ArrayList<>(items);
        this.subtotal = subtotal;
        this.discount = discount;
        this.finalAmount = subtotal - discount;
        this.status = "pending";
        this.shippingAddress = new HashMap<>(shippingAddress);
        this.billingAddress = new HashMap<>(billingAddress);
        this.couponId = couponId;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getOrderId() {
        return orderId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public List<OrderItem> getItems() {
        return new ArrayList<>(items);
    }
    
    public double getSubtotal() {
        return subtotal;
    }
    
    public double getDiscount() {
        return discount;
    }
    
    public double getFinalAmount() {
        return finalAmount;
    }
    
    public String getStatus() {
        return status;
    }
    
    public Map<String, String> getShippingAddress() {
        return new HashMap<>(shippingAddress);
    }
    
    public Map<String, String> getBillingAddress() {
        return new HashMap<>(billingAddress);
    }
    
    public String getPaymentId() {
        return paymentId;
    }
    
    public String getShipmentId() {
        return shipmentId;
    }
    
    public String getCouponId() {
        return couponId;
    }
    
    public String getCancellationReason() {
        return cancellationReason;
    }
    
    public LocalDateTime getCancelledDate() {
        return cancelledDate;
    }
    
    public LocalDateTime getDeliveredDate() {
        return deliveredDate;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public void setPaymentId(String paymentId) {
        this.paymentId = paymentId;
    }
    
    public void setShipmentId(String shipmentId) {
        this.shipmentId = shipmentId;
    }
    
    public void setCancellationReason(String cancellationReason) {
        this.cancellationReason = cancellationReason;
    }
    
    public void setCancelledDate(LocalDateTime cancelledDate) {
        this.cancelledDate = cancelledDate;
    }
    
    public void setDeliveredDate(LocalDateTime deliveredDate) {
        this.deliveredDate = deliveredDate;
    }
}

/**
 * 订单项类
 */
class OrderItem {
    private String productId;
    private int quantity;
    private double price;
    private double subtotal;
    
    public OrderItem(String productId, int quantity, double price) {
        this.productId = productId;
        this.quantity = quantity;
        this.price = price;
        this.subtotal = quantity * price;
    }
    
    // Getters and setters
    public String getProductId() {
        return productId;
    }
    
    public int getQuantity() {
        return quantity;
    }
    
    public double getPrice() {
        return price;
    }
    
    public double getSubtotal() {
        return subtotal;
    }
}

/**
 * 支付类
 */
class Payment {
    private String paymentId;
    private String orderId;
    private double amount;
    private String paymentMethod;
    private Map<String, String> paymentDetails;
    private String status;  // success, failed, pending
    private LocalDateTime createdAt;
    
    public Payment(String paymentId, String orderId, double amount, String paymentMethod,
                   Map<String, String> paymentDetails) {
        this.paymentId = paymentId;
        this.orderId = orderId;
        this.amount = amount;
        this.paymentMethod = paymentMethod;
        this.paymentDetails = new HashMap<>(paymentDetails);
        this.status = "success";
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getPaymentId() {
        return paymentId;
    }
    
    public String getOrderId() {
        return orderId;
    }
    
    public double getAmount() {
        return amount;
    }
    
    public String getPaymentMethod() {
        return paymentMethod;
    }
    
    public Map<String, String> getPaymentDetails() {
        return new HashMap<>(paymentDetails);
    }
    
    public String getStatus() {
        return status;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 运输类
 */
class Shipment {
    private String shipmentId;
    private String orderId;
    private String shippingMethod;
    private String trackingNumber;
    private String status;  // shipped, in_transit, delivered, returned
    private LocalDateTime shippedAt;
    private LocalDateTime deliveredAt;
    private LocalDateTime estimatedDelivery;
    
    public Shipment(String shipmentId, String orderId, String shippingMethod, String trackingNumber) {
        this.shipmentId = shipmentId;
        this.orderId = orderId;
        this.shippingMethod = shippingMethod;
        this.trackingNumber = trackingNumber;
        this.status = "shipped";
        this.shippedAt = LocalDateTime.now();
        
        // 计算预计送达时间
        switch (shippingMethod) {
            case "standard":
                this.estimatedDelivery = LocalDateTime.now().plusDays(5);
                break;
            case "express":
                this.estimatedDelivery = LocalDateTime.now().plusDays(2);
                break;
            case "overnight":
                this.estimatedDelivery = LocalDateTime.now().plusDays(1);
                break;
            default:
                this.estimatedDelivery = LocalDateTime.now().plusDays(7);
        }
    }
    
    // Getters and setters
    public String getShipmentId() {
        return shipmentId;
    }
    
    public String getOrderId() {
        return orderId;
    }
    
    public String getShippingMethod() {
        return shippingMethod;
    }
    
    public String getTrackingNumber() {
        return trackingNumber;
    }
    
    public String getStatus() {
        return status;
    }
    
    public LocalDateTime getShippedAt() {
        return shippedAt;
    }
    
    public LocalDateTime getDeliveredAt() {
        return deliveredAt;
    }
    
    public LocalDateTime getEstimatedDelivery() {
        return estimatedDelivery;
    }
    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public void setDeliveredDate(LocalDateTime deliveredAt) {
        this.deliveredAt = deliveredAt;
    }
}

/**
 * 评论类
 */
class Review {
    private String reviewId;
    private String userId;
    private String productId;
    private int rating;
    private String comment;
    private LocalDateTime createdAt;
    
    public Review(String reviewId, String userId, String productId, int rating, String comment) {
        this.reviewId = reviewId;
        this.userId = userId;
        this.productId = productId;
        this.rating = rating;
        this.comment = comment;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters and setters
    public String getReviewId() {
        return reviewId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public String getProductId() {
        return productId;
    }
    
    public int getRating() {
        return rating;
    }
    
    public String getComment() {
        return comment;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
}

/**
 * 购物车类
 */
class ShoppingCart {
    private String shoppingCartId;
    private String userId;
    private List<ShoppingCartItem> items;
    private LocalDateTime updatedAt;
    
    public ShoppingCart(String shoppingCartId, String userId) {
        this.shoppingCartId = shoppingCartId;
        this.userId = userId;
        this.items = new ArrayList<>();
        this.updatedAt = LocalDateTime.now();
    }
    
    // Getters
    public String getShoppingCartId() {
        return shoppingCartId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public List<ShoppingCartItem> getItems() {
        return new ArrayList<>(items);
    }
    
    public double getTotalPrice() {
        return items.stream().mapToDouble(ShoppingCartItem::getSubtotal).sum();
    }
    
    public LocalDateTime getUpdatedAt() {
        return updatedAt;
    }
    
    /**
     * 添加商品到购物车
     * @param productId 产品ID
     * @param quantity 数量
     * @return 添加成功返回true，否则返回false
     */
    public boolean addItem(String productId, int quantity) {
        // 检查产品是否已在购物车中
        for (ShoppingCartItem item : items) {
            if (item.getProductId().equals(productId)) {
                item.setQuantity(item.getQuantity() + quantity);
                this.updatedAt = LocalDateTime.now();
                return true;
            }
        }
        
        // 添加新产品到购物车
        items.add(new ShoppingCartItem(productId, quantity));
        this.updatedAt = LocalDateTime.now();
        return true;
    }
    
    /**
     * 从购物车中移除产品
     * @param productId 产品ID
     * @return 移除成功返回true，否则返回false
     */
    public boolean removeItem(String productId) {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getProductId().equals(productId)) {
                items.remove(i);
                this.updatedAt = LocalDateTime.now();
                return true;
            }
        }
        return false;
    }
    
    /**
     * 更新购物车中产品的数量
     * @param productId 产品ID
     * @param newQuantity 新数量
     * @return 更新成功返回true，否则返回false
     */
    public boolean updateItemQuantity(String productId, int newQuantity) {
        for (ShoppingCartItem item : items) {
            if (item.getProductId().equals(productId)) {
                item.setQuantity(newQuantity);
                this.updatedAt = LocalDateTime.now();
                return true;
            }
        }
        return false;
    }
    
    /**
     * 清空购物车
     */
    public void clearItems() {
        items.clear();
        this.updatedAt = LocalDateTime.now();
    }
}

/**
 * 购物车项类
 */
class ShoppingCartItem {
    private String productId;
    private int quantity;
    private double price;
    private double subtotal;
    
    public ShoppingCartItem(String productId, int quantity) {
        this.productId = productId;
        this.quantity = quantity;
        // 在实际应用中，这里应该从产品数据库获取价格
        this.price = 0; // 占位符
        this.subtotal = quantity * price;
    }
    
    // Getters and setters
    public String getProductId() {
        return productId;
    }
    
    public int getQuantity() {
        return quantity;
    }
    
    public void setQuantity(int quantity) {
        this.quantity = quantity;
        this.subtotal = quantity * price;
    }
    
    public double getPrice() {
        return price;
    }
    
    public double getSubtotal() {
        return subtotal;
    }
}

/**
 * 愿望清单类
 */
class Wishlist {
    private String wishlistId;
    private String userId;
    private List<WishlistItem> items;
    private LocalDateTime updatedAt;
    
    public Wishlist(String wishlistId, String userId) {
        this.wishlistId = wishlistId;
        this.userId = userId;
        this.items = new ArrayList<>();
        this.updatedAt = LocalDateTime.now();
    }
    
    // Getters
    public String getWishlistId() {
        return wishlistId;
    }
    
    public String getUserId() {
        return userId;
    }
    
    public List<WishlistItem> getItems() {
        return new ArrayList<>(items);
    }
    
    public LocalDateTime getUpdatedAt() {
        return updatedAt;
    }
    
    /**
     * 添加产品到愿望清单
     * @param productId 产品ID
     * @return 添加成功返回true，否则返回false
     */
    public boolean addItem(String productId) {
        // 检查产品是否已在愿望清单中
        for (WishlistItem item : items) {
            if (item.getProductId().equals(productId)) {
                return false;
            }
        }
        
        // 添加新产品到愿望清单
        items.add(new WishlistItem(productId));
        this.updatedAt = LocalDateTime.now();
        return true;
    }
    
    /**
     * 从愿望清单中移除产品
     * @param productId 产品ID
     * @return 移除成功返回true，否则返回false
     */
    public boolean removeItem(String productId) {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).getProductId().equals(productId)) {
                items.remove(i);
                this.updatedAt = LocalDateTime.now();
                return true;
            }
        }
        return false;
    }
}

/**
 * 愿望清单项类
 */
class WishlistItem {
    private String productId;
    private LocalDateTime addedAt;
    
    public WishlistItem(String productId) {
        this.productId = productId;
        this.addedAt = LocalDateTime.now();
    }
    
    // Getters
    public String getProductId() {
        return productId;
    }
    
    public LocalDateTime getAddedAt() {
        return addedAt;
    }
}

/**
 * 优惠券类
 */
class Coupon {
    private String couponId;
    private String code;
    private String discountType;  // percentage, fixed
    private double discountValue;
    private Double minOrderAmount;
    private LocalDateTime expiryDate;
    private Integer usageLimit;
    private int usageCount;
    private LocalDateTime createdAt;
    
    public Coupon(String couponId, String code, String discountType, double discountValue,
                 Double minOrderAmount, LocalDateTime expiryDate, Integer usageLimit) {
        this.couponId = couponId;
        this.code = code;
        this.discountType = discountType;
        this.discountValue = discountValue;
        this.minOrderAmount = minOrderAmount;
        this.expiryDate = expiryDate;
        this.usageLimit = usageLimit;
        this.usageCount = 0;
        this.createdAt = LocalDateTime.now();
    }
    
    // Getters
    public String getCouponId() {
        return couponId;
    }
    
    public String getCode() {
        return code;
    }
    
    public String getDiscountType() {
        return discountType;
    }
    
    public double getDiscountValue() {
        return discountValue;
    }
    
    public Double getMinOrderAmount() {
        return minOrderAmount;
    }
    
    public LocalDateTime getExpiryDate() {
        return expiryDate;
    }
    
    public Integer getUsageLimit() {
        return usageLimit;
    }
    
    public int getUsageCount() {
        return usageCount;
    }
    
    public LocalDateTime getCreatedAt() {
        return createdAt;
    }
    
    /**
     * 检查优惠券是否有效
     * @return 有效返回true，否则返回false
     */
    public boolean isValid() {
        // 检查是否已过期
        if (expiryDate != null && LocalDateTime.now().isAfter(expiryDate)) {
            return false;
        }
        
        // 检查是否超过使用限制
        if (usageLimit != null && usageCount >= usageLimit) {
            return false;
        }
        
        return true;
    }
    
    /**
     * 计算折扣金额
     * @param orderAmount 订单金额
     * @return 折扣金额
     */
    public double getDiscountAmount(double orderAmount) {
        if (!isValid()) {
            return 0;
        }
        
        if (minOrderAmount != null && orderAmount < minOrderAmount) {
            return 0;
        }
        
        if ("percentage".equals(discountType)) {
            return orderAmount * (discountValue / 100);
        } else {  // fixed
            return Math.min(discountValue, orderAmount);
        }
    }
    
    /**
     * 增加使用次数
     */
    public void incrementUsageCount() {
        usageCount++;
    }
}