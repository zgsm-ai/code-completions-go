/**
 * 库存管理系统类
 * 用于管理产品库存、供应商、采购和销售
 */
class InventoryManagementSystem {
    /**
     * 构造函数
     */
    constructor() {
        this.products = new Map();      // 产品ID到产品信息的映射
        this.suppliers = new Map();     // 供应商ID到供应商信息的映射
        this.transactions = [];         // 交易记录
        this.purchaseOrders = [];       // 采购订单
        this.salesOrders = [];          // 销售订单
        this.reports = [];              // 报告记录
        this.interestRates = {
            savings: 0.02,  // 储蓄利率
            loan: 0.05,     // 贷款利率
            creditCard: 0.18 // 信用卡利率
        };
    }
    
    /**
     * 添加新产品
     * @param {string} productId - 产品唯一标识符
     * @param {string} name - 产品名称
     * @param {string} description - 产品描述
     * @param {string} category - 产品类别
     * @param {number} price - 产品价格
     * @param {number} cost - 产品成本
     * @param {number} quantity - 初始库存数量
     * @param {number} minimumStock - 最低库存阈值
     * @param {string} supplierId - 供应商ID
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addProduct(productId, name, description, category, price, cost, quantity, minimumStock, supplierId) {
        if (this.products.has(productId)) {
            return false;
        }
        
        if (!this.suppliers.has(supplierId)) {
            return false;
        }
        
        const product = {
            productId,
            name,
            description,
            category,
            price,
            cost,
            quantity,
            minimumStock,
            supplierId,
            createdAt: new Date(),
            updatedAt: new Date()
        };
        
        this.products.set(productId, product);
        return true;
    }
    
    /**
     * 添加新供应商
     * @param {string} supplierId - 供应商唯一标识符
     * @param {string} name - 供应商名称
     * @param {string} contactPerson - 联系人
     * @param {string} email - 电子邮件
     * @param {string} phone - 电话号码
     * @param {string} address - 地址
     * @returns {boolean} 添加成功返回true，否则返回false
     */
    addSupplier(supplierId, name, contactPerson, email, phone, address) {
        if (this.suppliers.has(supplierId)) {
            return false;
        }
        
        const supplier = {
            supplierId,
            name,
            contactPerson,
            email,
            phone,
            address,
            createdAt: new Date()
        };
        
        this.suppliers.set(supplierId, supplier);
        return true;
    }
    
    /**
     * 更新库存
     * @param {string} productId - 产品ID
     * @param {number} quantityChange - 数量变化（正值为增加，负值为减少）
     * @param {string} transactionType - 交易类型（purchase, sale, adjustment, return）
     * @param {string} notes - 交易备注
     * @returns {boolean} 更新成功返回true，否则返回false
     */
    updateStock(productId, quantityChange, transactionType, notes) {
        if (!this.products.has(productId)) {
            return false;
        }
        
        const product = this.products.get(productId);
        const newQuantity = product.quantity + quantityChange;
        
        if (newQuantity < 0) {
            return false;
        }
        
        // 更新产品库存
        product.quantity = newQuantity;
        product.updatedAt = new Date();
        
        // 记录交易
        const transaction = {
            transactionId: this.generateId('transaction'),
            productId,
            quantityChange,
            transactionType,
            notes,
            timestamp: new Date()
        };
        
        this.transactions.push(transaction);
        
        // 检查是否需要补货
        if (newQuantity <= product.minimumStock && transactionType === 'sale') {
            this.generateReorderAlert(productId);
        }
        
        return true;
    }
    
    /**
     * 创建采购订单
     * @param {string} supplierId - 供应商ID
     * @param {Array} items - 采购项目列表
     * @param {Date} orderDate - 订单日期
     * @param {Date} expectedDeliveryDate - 预计交货日期
     * @returns {string|null} 创建成功返回采购订单ID，否则返回null
     */
    createPurchaseOrder(supplierId, items, orderDate, expectedDeliveryDate) {
        if (!this.suppliers.has(supplierId) || !items || items.length === 0) {
            return null;
        }
        
        // 计算总金额
        let totalAmount = 0;
        for (const item of items) {
            if (!this.products.has(item.productId)) {
                return null;
            }
            const product = this.products.get(item.productId);
            totalAmount += product.cost * item.quantity;
        }
        
        const orderId = this.generateId('purchase_order');
        const order = {
            orderId,
            supplierId,
            items: [...items],
            status: 'pending',  // pending, approved, received, cancelled
            orderDate,
            expectedDeliveryDate,
            totalAmount
        };
        
        this.purchaseOrders.push(order);
        return orderId;
    }
    
    /**
     * 创建销售订单
     * @param {string} customerId - 客户ID
     * @param {Array} items - 销售项目列表
     * @param {Date} orderDate - 订单日期
     * @param {string} shippingAddress - 配送地址
     * @returns {string|null} 创建成功返回销售订单ID，否则返回null
     */
    createSalesOrder(customerId, items, orderDate, shippingAddress) {
        if (!items || items.length === 0) {
            return null;
        }
        
        // 检查库存是否足够
        for (const item of items) {
            if (!this.products.has(item.productId)) {
                return null;
            }
            const product = this.products.get(item.productId);
            if (product.quantity < item.quantity) {
                return null;  // 库存不足
            }
        }
        
        // 计算总金额
        let totalAmount = 0;
        for (const item of items) {
            const product = this.products.get(item.productId);
            totalAmount += product.price * item.quantity;
        }
        
        const orderId = this.generateId('sales_order');
        const order = {
            orderId,
            customerId,
            items: [...items],
            status: 'pending',  // pending, processing, shipped, delivered, cancelled
            orderDate,
            shippingAddress,
            totalAmount
        };
        
        this.salesOrders.push(order);
        return orderId;
    }
    
    /**
     * 处理采购订单收货
     * @param {string} orderId - 采购订单ID
     * @param {Date} receivedDate - 收货日期
     * @param {string} notes - 收货备注
     * @returns {boolean} 处理成功返回true，否则返回false
     */
    receivePurchaseOrder(orderId, receivedDate, notes) {
        const order = this.findPurchaseOrder(orderId);
        if (!order || order.status !== 'approved') {
            return false;
        }
        
        // 更新库存
        for (const item of order.items) {
            this.updateStock(item.productId, item.quantity, 'purchase', 
                           `Received from PO: ${orderId}`);
        }
        
        // 更新订单状态
        order.status = 'received';
        order.receivedDate = receivedDate;
        order.notes = notes;
        
        return true;
    }
    
    /**
     * 处理销售订单发货
     * @param {string} orderId - 销售订单ID
     * @param {Date} shippedDate - 发货日期
     * @param {string} trackingNumber - 追踪号码
     * @param {string} notes - 发货备注
     * @returns {boolean} 处理成功返回true，否则返回false
     */
    shipSalesOrder(orderId, shippedDate, trackingNumber, notes) {
        const order = this.findSalesOrder(orderId);
        if (!order || order.status !== 'processing') {
            return false;
        }
        
        // 更新库存
        for (const item of order.items) {
            this.updateStock(item.productId, -item.quantity, 'sale', 
                           `Sold from SO: ${orderId}`);
        }
        
        // 更新订单状态
        order.status = 'shipped';
        order.shippedDate = shippedDate;
        order.trackingNumber = trackingNumber;
        order.notes = notes;
        
        return true;
    }
    
    /**
     * 获取产品信息
     * @param {string} productId - 产品ID
     * @returns {Object|null} 产品信息，不存在则返回null
     */
    getProduct(productId) {
        return this.products.get(productId) || null;
    }
    
    /**
     * 获取供应商信息
     * @param {string} supplierId - 供应商ID
     * @returns {Object|null} 供应商信息，不存在则返回null
     */
    getSupplier(supplierId) {
        return this.suppliers.get(supplierId) || null;
    }
    
    /**
     * 获取指定供应商的所有产品
     * @param {string} supplierId - 供应商ID
     * @returns {Array} 产品列表
     */
    getProductsBySupplier(supplierId) {
        const result = [];
        
        if (!this.suppliers.has(supplierId)) {
            return result;
        }
        
        for (const product of this.products.values()) {
            if (product.supplierId === supplierId) {
                result.push(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取指定类别的所有产品
     * @param {string} category - 产品类别
     * @returns {Array} 产品列表
     */
    getProductsByCategory(category) {
        const result = [];
        
        for (const product of this.products.values()) {
            if (product.category === category) {
                result.push(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取低库存产品
     * @param {number} threshold - 库存阈值，如果为null则使用产品最低库存阈值
     * @returns {Array} 低库存产品列表
     */
    getLowStockProducts(threshold) {
        const result = [];
        
        for (const product of this.products.values()) {
            const minThreshold = threshold !== null ? threshold : product.minimumStock;
            if (product.quantity <= minThreshold) {
                result.push(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取产品交易历史
     * @param {string} productId - 产品ID
     * @param {Date} startDate - 开始日期（可选）
     * @param {Date} endDate - 结束日期（可选）
     * @param {string} transactionType - 交易类型（可选）
     * @returns {Array} 交易记录列表
     */
    getProductTransactions(productId, startDate, endDate, transactionType) {
        const result = [];
        
        if (!this.products.has(productId)) {
            return result;
        }
        
        for (const transaction of this.transactions) {
            if (transaction.productId !== productId) {
                continue;
            }
            
            if (startDate && transaction.timestamp < startDate) {
                continue;
            }
            
            if (endDate && transaction.timestamp > endDate) {
                continue;
            }
            
            if (transactionType && transaction.transactionType !== transactionType) {
                continue;
            }
            
            result.push(transaction);
        }
        
        // 按时间戳降序排序
        result.sort((a, b) => b.timestamp - a.timestamp);
        
        return result;
    }
    
    /**
     * 生成库存报告
     * @returns {string} 报告字符串
     */
    generateInventoryReport() {
        let report = 'Inventory Report\n';
        report += '================\n';
        report += `Generated on: ${new Date()}\n\n`;
        
        // 总体统计
        const totalProducts = this.products.size;
        const totalSuppliers = this.suppliers.size;
        const totalTransactions = this.transactions.length;
        
        let totalInventoryValue = 0;
        let totalInventoryCost = 0;
        let totalStockQuantity = 0;
        
        // 按类别统计
        const categoryProductCount = {};
        const categoryValue = {};
        
        for (const product of this.products.values()) {
            const productValue = product.price * product.quantity;
            const productCost = product.cost * product.quantity;
            
            totalInventoryValue += productValue;
            totalInventoryCost += productCost;
            totalStockQuantity += product.quantity;
            
            // 更新类别统计
            const category = product.category;
            if (!categoryProductCount[category]) {
                categoryProductCount[category] = 0;
                categoryValue[category] = 0;
            }
            
            categoryProductCount[category]++;
            categoryValue[category] += productValue;
        }
        
        report += 'Summary\n';
        report += '-------\n';
        report += `Total Products: ${totalProducts}\n`;
        report += `Total Suppliers: ${totalSuppliers}\n`;
        report += `Total Transactions: ${totalTransactions}\n`;
        report += `Total Stock Quantity: ${totalStockQuantity}\n`;
        report += `Total Inventory Value: $${totalInventoryValue.toFixed(2)}\n`;
        report += `Total Inventory Cost: $${totalInventoryCost.toFixed(2)}\n`;
        report += `Potential Profit: $${(totalInventoryValue - totalInventoryCost).toFixed(2)}\n\n`;
        
        // 按类别统计
        report += 'Category Statistics\n';
        report += '-------------------\n';
        
        for (const [category, count] of Object.entries(categoryProductCount)) {
            const value = categoryValue[category];
            
            report += `Category: ${category}\n`;
            report += `  Product Count: ${count}\n`;
            report += `  Total Value: $${value.toFixed(2)}\n`;
            report += `  Percentage of Total: ${((value / totalInventoryValue) * 100).toFixed(1)}%\n\n`;
        }
        
        // 低库存产品
        const lowStockProducts = this.getLowStockProducts(null);
        if (lowStockProducts.length > 0) {
            report += 'Low Stock Products\n';
            report += '-------------------\n';
            
            for (const product of lowStockProducts) {
                report += `ID: ${product.productId}, `;
                report += `Name: ${product.name}, `;
                report += `Current Stock: ${product.quantity}, `;
                report += `Minimum Stock: ${product.minimumStock}\n`;
            }
        }
        
        // 保存报告
        const reportId = this.generateId('report');
        const inventoryReport = {
            reportId,
            reportType: 'inventory',
            content: report,
            generatedAt: new Date()
        };
        
        this.reports.push(inventoryReport);
        
        return report;
    }
    
    /**
     * 搜索产品
     * @param {string} keyword - 搜索关键词
     * @returns {Array} 匹配的产品列表
     */
    searchProducts(keyword) {
        const result = [];
        keyword = keyword.toLowerCase();
        
        for (const product of this.products.values()) {
            if (product.name.toLowerCase().includes(keyword) || 
                product.description.toLowerCase().includes(keyword) ||
                product.category.toLowerCase().includes(keyword) ||
                product.productId.toLowerCase().includes(keyword)) {
                result.push(product);
            }
        }
        
        return result;
    }
    
    /**
     * 生成补货提醒
     * @param {string} productId - 产品ID
     * @returns {string} 提醒ID
     */
    generateReorderAlert(productId) {
        const product = this.products.get(productId);
        if (!product) {
            return null;
        }
        
        const alertId = this.generateId('alert');
        // 在实际应用中，这里可以向管理员发送通知或创建提醒记录
        
        return alertId;
    }
    
    /**
     * 查找采购订单
     * @param {string} orderId - 采购订单ID
     * @returns {Object|null} 采购订单对象，不存在则返回null
     */
    findPurchaseOrder(orderId) {
        for (const order of this.purchaseOrders) {
            if (order.orderId === orderId) {
                return order;
            }
        }
        return null;
    }
    
    /**
     * 查找销售订单
     * @param {string} orderId - 销售订单ID
     * @returns {Object|null} 销售订单对象，不存在则返回null
     */
    findSalesOrder(orderId) {
        for (const order of this.salesOrders) {
            if (order.orderId === orderId) {
                return order;
            }
        }
        return null;
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