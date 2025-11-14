/**
 * 电子商务平台类
 * 用于管理产品、订单、支付和用户服务
 */
class ECommercePlatform {
    /**
     * 构造函数
     */
    constructor() {
        this.users = new Map();         // 用户ID到用户信息的映射
        this.products = new Map();      // 产品ID到产品信息的映射
        this.categories = new Map();    // 类别ID到类别信息的映射
        this.orders = new Map();        // 订单ID到订单信息的映射
        this.payments = new Map();      // 支付ID到支付信息的映射
        this.shipments = new Map();     // 运输ID到运输信息的映射
        this.reviews = new Map();       // 评论ID到评论信息的映射
        this.shoppingCarts = new Map(); // 购物车ID到购物车信息的映射
        this.wishlists = new Map();    // 愿望清单ID到愿望清单信息的映射
        this.coupons = new Map();      // 优惠券ID到优惠券信息的映射
    }
    
    /**
     * 创建新用户
     * @param {string} userId - 用户唯一标识符
     * @param {string} username - 用户名
     * @param {string} email - 电子邮件
     * @param {string} password - 密码
     * @param {Object} shippingAddress - 配送地址（可选）
     * @param {Object} billingAddress - 账单地址（可选）
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createUser(userId, username, email, password, shippingAddress, billingAddress) {
        if (this.users.has(userId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const user of this.users.values()) {
            if (user.email === email) {
                return false;
            }
        }
        
        // 检查用户名是否已被使用
        for (const user of this.users.values()) {
            if (user.username === username) {
                return false;
            }
        }
        
        const user = {
            userId,
            username,
            email,
            password,
            shippingAddress: shippingAddress ? {...shippingAddress} : null,
            billingAddress: billingAddress ? {...billingAddress} : null,
            orderIds: [],
            shoppingCartId: null,
            wishlistId: null,
            createdAt: new Date()
        };
        
        this.users.set(userId, user);
        
        // 为用户创建购物车和愿望清单
        const shoppingCartId = this.generateId('cart');
        this.shoppingCarts.set(shoppingCartId, {
            shoppingCartId,
            userId,
            items: [],
            createdAt: new Date(),
            updatedAt: new Date()
        });
        user.shoppingCartId = shoppingCartId;
        
        const wishlistId = this.generateId('wishlist');
        this.wishlists.set(wishlistId, {
            wishlistId,
            userId,
            items: [],
            createdAt: new Date(),
            updatedAt: new Date()
        });
        user.wishlistId = wishlistId;
        
        return true;
    }
    
    /**
     * 创建产品类别
     * @param {string} categoryId - 类别唯一标识符
     * @param {string} name - 类别名称
     * @param {string} description - 类别描述（可选）
     * @param {string} parentId - 父类别ID（可选）
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createCategory(categoryId, name, description, parentId) {
        if (this.categories.has(categoryId)) {
            return false;
        }
        
        if (parentId && !this.categories.has(parentId)) {
            return false;
        }
        
        this.categories.set(categoryId, {
            categoryId,
            name,
            description,
            parentId,
            createdAt: new Date()
        });
        
        return true;
    }
    
    /**
     * 创建新产品
     * @param {string} productId - 产品唯一标识符
     * @param {string} name - 产品名称
     * @param {string} description - 产品描述
     * @param {number} price - 产品价格
     * @param {string} categoryId - 产品类别ID
     * @param {number} stockQuantity - 库存数量
     * @param {string} sku - SKU（可选）
     * @param {number} weight - 产品重量（可选）
     * @param {Object} dimensions - 产品尺寸（可选）
     * @param {Array} images - 图片URL列表（可选）
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createProduct(productId, name, description, price, categoryId, stockQuantity, 
                sku, weight, dimensions, images) {
        if (this.products.has(productId)) {
            return false;
        }
        
        if (!this.categories.has(categoryId)) {
            return false;
        }
        
        this.products.set(productId, {
            productId,
            name,
            description,
            price,
            categoryId,
            stockQuantity,
            sku: sku || `SKU-${productId}`,
            weight,
            dimensions: dimensions ? {...dimensions} : null,
            images: images ? [...images] : [],
            createdAt: new Date(),
            updatedAt: new Date()
        });
        
        return true;
    }
    
    /**
     * 添加产品到购物车
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @param {number} quantity - 数量
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addToCart(userId, productId, quantity) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return false;
        }
        
        if (quantity <= 0) {
            return false;
        }
        
        const product = this.products.get(productId);
        if (product.stockQuantity < quantity) {
            return false;  // 库存不足
        }
        
        const shoppingCartId = this.users.get(userId).shoppingCartId;
        if (!this.shoppingCarts.has(shoppingCartId)) {
            return false;
        }
        
        const shoppingCart = this.shoppingCarts.get(shoppingCartId);
        
        // 检查产品是否已在购物车中
        for (const item of shoppingCart.items) {
            if (item.productId === productId) {
                item.quantity += quantity;
                shoppingCart.updatedAt = new Date();
                return true;
            }
        }
        
        // 添加新产品到购物车
        shoppingCart.items.push({
            productId,
            quantity,
            addedAt: new Date()
        });
        shoppingCart.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 从购物车中移除产品
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @returns {boolean} 移除成功返回true，否则返回false
     */
    removeFromCart(userId, productId) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return false;
        }
        
        const shoppingCartId = this.users.get(userId).shoppingCartId;
        if (!this.shoppingCarts.has(shoppingCartId)) {
            return false;
        }
        
        const shoppingCart = this.shoppingCarts.get(shoppingCartId);
        
        // 查找并移除产品
        for (let i = 0; i < shoppingCart.items.length; i++) {
            if (shoppingCart.items[i].productId === productId) {
                shoppingCart.items.splice(i, 1);
                shoppingCart.updatedAt = new Date();
                return true;
            }
        }
        
        return false;  // 产品不在购物车中
    }
    
    /**
     * 更新购物车中产品的数量
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @param {number} newQuantity - 新数量
     * @returns {boolean} 更新成功返回true，否则返回false
     */
    updateCartQuantity(userId, productId, newQuantity) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return false;
        }
        
        if (newQuantity <= 0) {
            return false;
        }
        
        const product = this.products.get(productId);
        if (product.stockQuantity < newQuantity) {
            return false;  // 库存不足
        }
        
        const shoppingCartId = this.users.get(userId).shoppingCartId;
        if (!this.shoppingCarts.has(shoppingCartId)) {
            return false;
        }
        
        const shoppingCart = this.shoppingCarts.get(shoppingCartId);
        
        // 查找并更新产品数量
        for (const item of shoppingCart.items) {
            if (item.productId === productId) {
                item.quantity = newQuantity;
                shoppingCart.updatedAt = new Date();
                return true;
            }
        }
        
        return false;  // 产品不在购物车中
    }
    
    /**
     * 获取用户购物车
     * @param {string} userId - 用户ID
     * @returns {Object|null} 购物车信息，用户不存在返回null
     */
    getShoppingCart(userId) {
        if (!this.users.has(userId)) {
            return null;
        }
        
        const shoppingCartId = this.users.get(userId).shoppingCartId;
        if (!this.shoppingCarts.has(shoppingCartId)) {
            return null;
        }
        
        const shoppingCart = {...this.shoppingCarts.get(shoppingCartId)};
        
        // 添加产品详细信息
        const itemsWithDetails = [];
        for (const item of shoppingCart.items) {
            const productId = item.productId;
            if (this.products.has(productId)) {
                const productInfo = {...this.products.get(productId)};
                productInfo.quantity = item.quantity;
                productInfo.subtotal = productInfo.price * item.quantity;
                itemsWithDetails.push(productInfo);
            }
        }
        
        shoppingCart.items = itemsWithDetails;
        
        // 计算总价
        shoppingCart.totalPrice = itemsWithDetails.reduce((sum, item) => sum + item.subtotal, 0);
        
        return shoppingCart;
    }
    
    /**
     * 添加产品到愿望清单
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addToWishlist(userId, productId) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return false;
        }
        
        const wishlistId = this.users.get(userId).wishlistId;
        if (!this.wishlists.has(wishlistId)) {
            return false;
        }
        
        const wishlist = this.wishlists.get(wishlistId);
        
        // 检查产品是否已在愿望清单中
        for (const item of wishlist.items) {
            if (item.productId === productId) {
                return false;  // 产品已在愿望清单中
            }
        }
        
        // 添加新产品到愿望清单
        wishlist.items.push({
            productId,
            addedAt: new Date()
        });
        wishlist.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 从愿望清单中移除产品
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @returns {boolean} 移除成功返回true，否则返回false
     */
    removeFromWishlist(userId, productId) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return false;
        }
        
        const wishlistId = this.users.get(userId).wishlistId;
        if (!this.wishlists.has(wishlistId)) {
            return false;
        }
        
        const wishlist = this.wishlists.get(wishlistId);
        
        // 查找并移除产品
        for (let i = 0; i < wishlist.items.length; i++) {
            if (wishlist.items[i].productId === productId) {
                wishlist.items.splice(i, 1);
                wishlist.updatedAt = new Date();
                return true;
            }
        }
        
        return false;  // 产品不在愿望清单中
    }
    
    /**
     * 获取用户愿望清单
     * @param {string} userId - 用户ID
     * @returns {Object|null} 愿望清单信息，用户不存在返回null
     */
    getWishlist(userId) {
        if (!this.users.has(userId)) {
            return null;
        }
        
        const wishlistId = this.users.get(userId).wishlistId;
        if (!this.wishlists.has(wishlistId)) {
            return null;
        }
        
        const wishlist = {...this.wishlists.get(wishlistId)};
        
        // 添加产品详细信息
        const itemsWithDetails = [];
        for (const item of wishlist.items) {
            const productId = item.productId;
            if (this.products.has(productId)) {
                const productInfo = {...this.products.get(productId)};
                productInfo.addedAt = item.addedAt;
                itemsWithDetails.push(productInfo);
            }
        }
        
        wishlist.items = itemsWithDetails;
        
        return wishlist;
    }
    
    /**
     * 创建订单
     * @param {string} userId - 用户ID
     * @param {Object} shippingAddress - 配送地址（可选）
     * @param {Object} billingAddress - 账单地址（可选）
     * @param {string} couponId - 优惠券ID（可选）
     * @returns {string|null} 创建成功返回订单ID，否则返回null
     */
    createOrder(userId, shippingAddress, billingAddress, couponId) {
        if (!this.users.has(userId)) {
            return null;
        }
        
        const shoppingCart = this.getShoppingCart(userId);
        if (!shoppingCart || shoppingCart.items.length === 0) {
            return null;  // 购物车为空
        }
        
        // 使用提供的地址或用户默认地址
        const user = this.users.get(userId);
        const finalShippingAddress = shippingAddress ? 
            {...shippingAddress} : (user.shippingAddress ? 
            {...user.shippingAddress} : null);
        
        const finalBillingAddress = billingAddress ? 
            {...billingAddress} : (user.billingAddress ? 
            {...user.billingAddress} : finalShippingAddress);
        
        if (!finalShippingAddress) {
            return null;  // 缺少配送地址
        }
        
        // 检查优惠券
        let discount = 0;
        if (couponId && this.coupons.has(couponId)) {
            const coupon = this.coupons.get(couponId);
            if (this.isCouponValid(coupon)) {
                discount = this.calculateCouponDiscount(coupon, shoppingCart.totalPrice);
            }
        }
        
        // 创建订单
        const orderId = this.generateId('order');
        const order = {
            orderId,
            userId,
            items: shoppingCart.items.map(item => ({
                productId: item.productId,
                quantity: item.quantity,
                price: item.price,
                subtotal: item.subtotal
            })),
            subtotal: shoppingCart.totalPrice,
            discount,
            finalAmount: shoppingCart.totalPrice - discount,
            status: 'pending',  // pending, paid, shipped, delivered, cancelled
            shippingAddress: finalShippingAddress,
            billingAddress: finalBillingAddress,
            couponId,
            createdAt: new Date(),
            updatedAt: new Date()
        };
        
        this.orders.set(orderId, order);
        
        // 更新用户的订单列表
        user.orderIds.push(orderId);
        
        // 更新库存
        for (const item of order.items) {
            const product = this.products.get(item.productId);
            if (product) {
                product.stockQuantity -= item.quantity;
                product.updatedAt = new Date();
            }
        }
        
        // 清空购物车
        this.shoppingCarts.get(user.shoppingCartId).items = [];
        this.shoppingCarts.get(user.shoppingCartId).updatedAt = new Date();
        
        return orderId;
    }
    
    /**
     * 处理订单支付
     * @param {string} orderId - 订单ID
     * @param {string} paymentMethod - 支付方式
     * @param {Object} paymentDetails - 支付详情
     * @returns {string|null} 支付成功返回支付ID，否则返回null
     */
    processPayment(orderId, paymentMethod, paymentDetails) {
        if (!this.orders.has(orderId)) {
            return null;
        }
        
        const order = this.orders.get(orderId);
        if (order.status !== 'pending') {
            return null;  // 订单状态不正确
        }
        
        // 创建支付记录
        const paymentId = this.generateId('payment');
        const payment = {
            paymentId,
            orderId,
            amount: order.finalAmount,
            paymentMethod,
            paymentDetails: {...paymentDetails},
            status: 'success',  // success, failed, pending
            createdAt: new Date()
        };
        
        this.payments.set(paymentId, payment);
        
        // 更新订单状态
        order.status = 'paid';
        order.paymentId = paymentId;
        order.updatedAt = new Date();
        
        return paymentId;
    }
    
    /**
     * 配送订单
     * @param {string} orderId - 订单ID
     * @param {string} shippingMethod - 配送方式
     * @param {string} trackingNumber - 追踪号码（可选）
     * @returns {string|null} 配送成功返回运输ID，否则返回null
     */
    shipOrder(orderId, shippingMethod, trackingNumber) {
        if (!this.orders.has(orderId)) {
            return null;
        }
        
        const order = this.orders.get(orderId);
        if (order.status !== 'paid') {
            return null;  // 订单状态不正确
        }
        
        // 创建运输记录
        const shipmentId = this.generateId('shipment');
        const shipment = {
            shipmentId,
            orderId,
            shippingMethod,
            trackingNumber,
            status: 'shipped',  // shipped, in_transit, delivered, returned
            shippedAt: new Date(),
            estimatedDelivery: this.calculateEstimatedDelivery(shippingMethod)
        };
        
        this.shipments.set(shipmentId, shipment);
        
        // 更新订单状态
        order.status = 'shipped';
        order.shipmentId = shipmentId;
        order.updatedAt = new Date();
        
        return shipmentId;
    }
    
    /**
     * 标记订单为已送达
     * @param {string} orderId - 订单ID
     * @returns {boolean} 更新成功返回true，否则返回false
     */
    deliverOrder(orderId) {
        if (!this.orders.has(orderId)) {
            return false;
        }
        
        const order = this.orders.get(orderId);
        if (order.status !== 'shipped') {
            return false;  // 订单状态不正确
        }
        
        // 更新订单状态
        order.status = 'delivered';
        order.deliveredAt = new Date();
        order.updatedAt = new Date();
        
        // 更新运输记录状态
        const shipmentId = order.shipmentId;
        if (shipmentId && this.shipments.has(shipmentId)) {
            const shipment = this.shipments.get(shipmentId);
            shipment.status = 'delivered';
            shipment.deliveredAt = new Date();
        }
        
        return true;
    }
    
    /**
     * 取消订单
     * @param {string} orderId - 订单ID
     * @param {string} reason - 取消原因
     * @returns {boolean} 取消成功返回true，否则返回false
     */
    cancelOrder(orderId, reason) {
        if (!this.orders.has(orderId)) {
            return false;
        }
        
        const order = this.orders.get(orderId);
        if (order.status !== 'pending') {
            return false;  // 订单状态不正确
        }
        
        // 更新订单状态
        order.status = 'cancelled';
        order.cancellationReason = reason;
        order.cancelledAt = new Date();
        order.updatedAt = new Date();
        
        // 恢复库存
        for (const item of order.items) {
            const product = this.products.get(item.productId);
            if (product) {
                product.stockQuantity += item.quantity;
                product.updatedAt = new Date();
            }
        }
        
        return true;
    }
    
    /**
     * 添加产品评论
     * @param {string} userId - 用户ID
     * @param {string} productId - 产品ID
     * @param {number} rating - 评分（1-5）
     * @param {string} comment - 评论内容（可选）
     * @returns {string|null} 评论成功返回评论ID，否则返回null
     */
    addProductReview(userId, productId, rating, comment) {
        if (!this.users.has(userId) || !this.products.has(productId)) {
            return null;
        }
        
        if (rating < 1 || rating > 5) {
            return null;
        }
        
        // 检查用户是否已购买该产品
        let hasPurchased = false;
        const user = this.users.get(userId);
        
        for (const orderId of user.orderIds) {
            if (this.orders.has(orderId)) {
                const order = this.orders.get(orderId);
                if (order.status === 'delivered') {
                    for (const item of order.items) {
                        if (item.productId === productId) {
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
        for (const [reviewId, review] of this.reviews) {
            if (review.userId === userId && review.productId === productId) {
                return null;  // 用户已评论过该产品
            }
        }
        
        // 创建评论
        const reviewId = this.generateId('review');
        this.reviews.set(reviewId, {
            reviewId,
            userId,
            productId,
            rating,
            comment,
            createdAt: new Date()
        });
        
        return reviewId;
    }
    
    /**
     * 获取产品评论
     * @param {string} productId - 产品ID
     * @param {number} limit - 返回评论数量限制
     * @returns {Array} 评论列表
     */
    getProductReviews(productId, limit) {
        if (!this.products.has(productId)) {
            return [];
        }
        
        const result = [];
        for (const [reviewId, review] of this.reviews) {
            if (review.productId === productId) {
                const reviewCopy = {...review};
                reviewCopy.reviewId = reviewId;
                reviewCopy.username = this.users.get(review.userId).username;
                result.push(reviewCopy);
            }
        }
        
        // 按创建时间降序排序
        result.sort((a, b) => b.createdAt - a.createdAt);
        
        if (result.length > limit) {
            return result.slice(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取产品详细信息
     * @param {string} productId - 产品ID
     * @returns {Object|null} 产品详细信息，产品不存在返回null
     */
    getProductDetails(productId) {
        if (!this.products.has(productId)) {
            return null;
        }
        
        const product = {...this.products.get(productId)};
        
        // 添加类别信息
        const categoryId = product.categoryId;
        if (this.categories.has(categoryId)) {
            product.category = this.categories.get(categoryId).name;
        }
        
        // 计算平均评分
        const reviews = this.getProductReviews(productId, 1000);
        if (reviews.length > 0) {
            const averageRating = reviews.reduce((sum, review) => sum + review.rating, 0) / reviews.length;
            product.averageRating = Math.round(averageRating * 10) / 10;
            product.reviewCount = reviews.length;
        } else {
            product.averageRating = 0;
            product.reviewCount = 0;
        }
        
        return product;
    }
    
    /**
     * 搜索产品
     * @param {string} keyword - 搜索关键词
     * @param {string} categoryId - 类别筛选（可选）
     * @param {number} minPrice - 最低价格（可选）
     * @param {number} maxPrice - 最高价格（可选）
     * @param {string} sortBy - 排序方式（price_asc, price_desc, rating, newest）
     * @param {number} limit - 返回产品数量限制
     * @returns {Array} 匹配的产品列表
     */
    searchProducts(keyword, categoryId, minPrice, maxPrice, sortBy, limit) {
        keyword = keyword.toLowerCase();
        let result = [];
        
        for (const [productId, product] of this.products) {
            // 检查关键词匹配
            if (!product.name.toLowerCase().includes(keyword) && 
                !product.description.toLowerCase().includes(keyword) &&
                !product.sku.toLowerCase().includes(keyword)) {
                continue;
            }
            
            // 检查类别匹配
            if (categoryId && product.categoryId !== categoryId) {
                continue;
            }
            
            // 检查价格范围
            const price = product.price;
            if (minPrice !== undefined && price < minPrice) {
                continue;
            }
            
            if (maxPrice !== undefined && price > maxPrice) {
                continue;
            }
            
            // 添加到结果列表
            const productDetails = this.getProductDetails(productId);
            if (productDetails) {
                result.push(productDetails);
            }
        }
        
        // 排序
        switch (sortBy) {
            case 'price_asc':
                result.sort((a, b) => a.price - b.price);
                break;
            case 'price_desc':
                result.sort((a, b) => b.price - a.price);
                break;
            case 'rating':
                result.sort((a, b) => b.averageRating - a.averageRating);
                break;
            case 'newest':
                result.sort((a, b) => b.createdAt - a.createdAt);
                break;
            default:
                // 默认按相关性排序（这里简化为按名称匹配度）
                result.sort((a, b) => !a.name.toLowerCase().startsWith(keyword));
        }
        
        // 限制返回数量
        if (result.length > limit) {
            result = result.slice(0, limit);
        }
        
        return result;
    }
    
    /**
     * 获取用户订单
     * @param {string} userId - 用户ID
     * @param {string} status - 订单状态筛选（可选）
     * @param {number} limit - 返回订单数量限制
     * @returns {Array} 订单列表
     */
    getUserOrders(userId, status, limit) {
        if (!this.users.has(userId)) {
            return [];
        }
        
        const result = [];
        const user = this.users.get(userId);
        
        for (const orderId of user.orderIds) {
            if (this.orders.has(orderId)) {
                const order = {...this.orders.get(orderId)};
                order.orderId = orderId;
                
                if (status && order.status !== status) {
                    continue;
                }
                
                result.push(order);
            }
        }
        
        // 按创建时间降序排序
        result.sort((a, b) => b.createdAt - a.createdAt);
        
        if (result.length > limit) {
            return result.slice(0, limit);
        }
        
        return result;
    }
    
    /**
     * 创建优惠券
     * @param {string} couponId - 优惠券唯一标识符
     * @param {string} code - 优惠券代码
     * @param {string} discountType - 折扣类型（percentage, fixed）
     * @param {number} discountValue - 折扣值
     * @param {number} minOrderAmount - 最低订单金额（可选）
     * @param {Date} expiryDate - 过期日期
     * @param {number} usageLimit - 使用限制（可选）
     * @returns {boolean} 创建成功返回true，否则返回false
     */
    createCoupon(couponId, code, discountType, discountValue, minOrderAmount, expiryDate, usageLimit) {
        if (this.coupons.has(couponId)) {
            return false;
        }
        
        if (discountType !== 'percentage' && discountType !== 'fixed') {
            return false;
        }
        
        if (discountValue <= 0) {
            return false;
        }
        
        if (discountType === 'percentage' && discountValue > 100) {
            return false;
        }
        
        this.coupons.set(couponId, {
            couponId,
            code,
            discountType,
            discountValue,
            minOrderAmount,
            expiryDate,
            usageLimit,
            usageCount: 0,
            createdAt: new Date()
        });
        
        return true;
    }
    
    /**
     * 应用优惠券
     * @param {string} couponId - 优惠券ID
     * @param {number} orderAmount - 订单金额
     * @returns {number} 应用成功返回折扣金额，否则返回0
     */
    applyCoupon(couponId, orderAmount) {
        if (!this.coupons.has(couponId)) {
            return 0;
        }
        
        const coupon = this.coupons.get(couponId);
        if (!this.isCouponValid(coupon)) {
            return 0;
        }
        
        if (coupon.minOrderAmount !== undefined && orderAmount < coupon.minOrderAmount) {
            return 0;
        }
        
        const discount = this.calculateCouponDiscount(coupon, orderAmount);
        if (discount <= 0) {
            return 0;
        }
        
        // 更新优惠券使用次数
        coupon.usageCount++;
        
        return discount;
    }
    
    /**
     * 检查优惠券是否有效
     * @param {Object} coupon - 优惠券对象
     * @returns {boolean} 有效返回true，否则返回false
     */
    isCouponValid(coupon) {
        // 检查是否已过期
        if (coupon.expiryDate && new Date() > coupon.expiryDate) {
            return false;
        }
        
        // 检查是否超过使用限制
        if (coupon.usageLimit !== undefined && coupon.usageCount >= coupon.usageLimit) {
            return false;
        }
        
        return true;
    }
    
    /**
     * 计算优惠券折扣金额
     * @param {Object} coupon - 优惠券对象
     * @param {number} orderAmount - 订单金额
     * @returns {number} 折扣金额
     */
    calculateCouponDiscount(coupon, orderAmount) {
        if (!this.isCouponValid(coupon)) {
            return 0;
        }
        
        if (coupon.minOrderAmount !== undefined && orderAmount < coupon.minOrderAmount) {
            return 0;
        }
        
        if (coupon.discountType === 'percentage') {
            return orderAmount * (coupon.discountValue / 100);
        } else {  // fixed
            return Math.min(coupon.discountValue, orderAmount);
        }
    }
    
    /**
     * 计算预计送达时间
     * @param {string} shippingMethod - 配送方式
     * @returns {string} 预计送达时间
     */
    calculateEstimatedDelivery(shippingMethod) {
        const now = new Date();
        let estimatedDelivery;
        
        switch (shippingMethod) {
            case 'standard':
                estimatedDelivery = new Date(now.getTime() + (5 * 24 * 60 * 60 * 1000));
                break;
            case 'express':
                estimatedDelivery = new Date(now.getTime() + (2 * 24 * 60 * 60 * 1000));
                break;
            case 'overnight':
                estimatedDelivery = new Date(now.getTime() + (1 * 24 * 60 * 60 * 1000));
                break;
            default:
                estimatedDelivery = new Date(now.getTime() + (7 * 24 * 60 * 60 * 1000));
        }
        
        return estimatedDelivery.toISOString();
    }
    
    /**
     * 生成唯一ID
     * @param {string} prefix - ID前缀
     * @returns {string} 唯一ID字符串
     */
    generateId(prefix) {
        return `${prefix}_${Math.random().toString(36).substring(2, 10)}`;
    }
}