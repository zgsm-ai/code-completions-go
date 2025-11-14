/**
 * 库存管理系统类
 * 用于管理产品、供应商、库存变动和交易
 */
class InventoryManagementSystem {
    private products: Map<string, Product>;           // 产品ID到产品信息的映射
    private suppliers: Map<string, Supplier>;         // 供应商ID到供应商信息的映射
    private categories: Map<string, Category>;        // 类别ID到类别信息的映射
    private stockHistory: Map<string, StockRecord[]>; // 产品ID到库存记录的映射
    private transactions: Map<string, Transaction>;   // 交易ID到交易信息的映射
    private purchaseOrders: Map<string, PurchaseOrder>; // 采购订单ID到采购订单的映射
    private warehouses: Map<string, Warehouse>;       // 仓库ID到仓库信息的映射
    private stockLocations: Map<string, StockLocation>; // 库位ID到库位信息的映射
    
    /**
     * 构造函数
     */
    constructor() {
        this.products = new Map();
        this.suppliers = new Map();
        this.categories = new Map();
        this.stockHistory = new Map();
        this.transactions = new Map();
        this.purchaseOrders = new Map();
        this.warehouses = new Map();
        this.stockLocations = new Map();
    }
    
    /**
     * 添加新产品
     * @param productId - 产品唯一标识符
     * @param name - 产品名称
     * @param description - 产品描述
     * @param sku - SKU（库存单位）
     * @param barcode - 条形码（可选）
     * @param unitOfMeasure - 计量单位
     * @param unitPrice - 单价
     * @param categoryId - 类别ID（可选）
     * @param supplierIds - 供应商ID列表（可选）
     * @param reorderPoint - 再订购点
     * @param maxStockLevel - 最大库存水平
     * @returns 添加成功返回true，否则返回false
     */
    addProduct(productId: string, name: string, description: string, sku: string, 
                barcode: string, unitOfMeasure: string, unitPrice: number, 
                categoryId?: string, supplierIds?: string[], reorderPoint?: number, 
                maxStockLevel?: number): boolean {
        if (this.products.has(productId)) {
            return false;
        }
        
        // 检查SKU是否已被使用
        for (const product of this.products.values()) {
            if (product.sku === sku) {
                return false;
            }
        }
        
        // 检查条形码是否已被使用
        if (barcode) {
            for (const product of this.products.values()) {
                if (product.barcode === barcode) {
                    return false;
                }
            }
        }
        
        // 检查类别是否存在
        if (categoryId && !this.categories.has(categoryId)) {
            return false;
        }
        
        // 检查供应商是否存在
        if (supplierIds) {
            for (const supplierId of supplierIds) {
                if (!this.suppliers.has(supplierId)) {
                    return false;
                }
            }
        }
        
        const product = new Product(
            productId,
            name,
            description,
            sku,
            barcode,
            unitOfMeasure,
            unitPrice,
            categoryId,
            supplierIds,
            reorderPoint,
            maxStockLevel
        );
        
        this.products.set(productId, product);
        this.stockHistory.set(productId, []);
        
        return true;
    }
    
    /**
     * 更新产品信息
     * @param productId - 产品ID
     * @param name - 新的名称（可选）
     * @param description - 新的描述（可选）
     * @param barcode - 新的条形码（可选）
     * @param unitOfMeasure - 新的计量单位（可选）
     * @param unitPrice - 新的单价（可选）
     * @param categoryId - 新的类别ID（可选）
     * @param supplierIds - 新的供应商ID列表（可选）
     * @param reorderPoint - 新的再订购点（可选）
     * @param maxStockLevel - 新的最大库存水平（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateProduct(productId: string, name?: string, description?: string, 
                   barcode?: string, unitOfMeasure?: string, unitPrice?: number, 
                   categoryId?: string, supplierIds?: string[], reorderPoint?: number, 
                   maxStockLevel?: number): boolean {
        if (!this.products.has(productId)) {
            return false;
        }
        
        const product = this.products.get(productId)!;
        
        // 检查条形码是否已被其他产品使用
        if (barcode && barcode !== product.barcode) {
            for (const [id, p] of this.products) {
                if (id !== productId && p.barcode === barcode) {
                    return false;
                }
            }
            product.barcode = barcode;
        }
        
        // 检查类别是否存在
        if (categoryId && !this.categories.has(categoryId)) {
            return false;
        }
        
        // 检查供应商是否存在
        if (supplierIds) {
            for (const supplierId of supplierIds) {
                if (!this.suppliers.has(supplierId)) {
                    return false;
                }
            }
        }
        
        if (name !== undefined) {
            product.name = name;
        }
        
        if (description !== undefined) {
            product.description = description;
        }
        
        if (unitOfMeasure !== undefined) {
            product.unitOfMeasure = unitOfMeasure;
        }
        
        if (unitPrice !== undefined) {
            product.unitPrice = unitPrice;
        }
        
        if (categoryId !== undefined) {
            product.categoryId = categoryId;
        }
        
        if (supplierIds !== undefined) {
            product.supplierIds = [...supplierIds];
        }
        
        if (reorderPoint !== undefined) {
            product.reorderPoint = reorderPoint;
        }
        
        if (maxStockLevel !== undefined) {
            product.maxStockLevel = maxStockLevel;
        }
        
        product.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取产品信息
     * @param productId - 产品ID
     * @returns 产品对象，若不存在则返回null
     */
    getProduct(productId: string): Product | null {
        if (!this.products.has(productId)) {
            return null;
        }
        
        return this.products.get(productId)!;
    }
    
    /**
     * 删除产品
     * @param productId - 产品ID
     * @returns 删除成功返回true，否则返回false
     */
    removeProduct(productId: string): boolean {
        if (!this.products.has(productId)) {
            return false;
        }
        
        // 检查产品是否在任何仓库中有库存
        for (const location of this.stockLocations.values()) {
            if (location.productId === productId && location.quantity > 0) {
                return false;
            }
        }
        
        // 删除所有与该产品相关的库存记录
        this.stockHistory.delete(productId);
        
        // 删除所有与该产品相关的库位
        for (const [locationId, location] of this.stockLocations) {
            if (location.productId === productId) {
                this.stockLocations.delete(locationId);
            }
        }
        
        // 删除产品
        this.products.delete(productId);
        
        return true;
    }
    
    /**
     * 添加新供应商
     * @param supplierId - 供应商唯一标识符
     * @param name - 供应商名称
     * @param contactPerson - 联系人
     * @param email - 电子邮件
     * @param phone - 电话
     * @param address - 地址
     * @param website - 网站（可选）
     * @param taxId - 税号（可选）
     * @param paymentTerms - 付款条件（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addSupplier(supplierId: string, name: string, contactPerson: string, 
                 email: string, phone: string, address: string, website?: string, 
                 taxId?: string, paymentTerms?: string): boolean {
        if (this.suppliers.has(supplierId)) {
            return false;
        }
        
        // 检查电子邮件是否已被使用
        for (const supplier of this.suppliers.values()) {
            if (supplier.email === email) {
                return false;
            }
        }
        
        const supplier = new Supplier(
            supplierId,
            name,
            contactPerson,
            email,
            phone,
            address,
            website,
            taxId,
            paymentTerms
        );
        
        this.suppliers.set(supplierId, supplier);
        
        return true;
    }
    
    /**
     * 更新供应商信息
     * @param supplierId - 供应商ID
     * @param name - 新的名称（可选）
     * @param contactPerson - 新的联系人（可选）
     * @param email - 新的电子邮件（可选）
     * @param phone - 新的电话（可选）
     * @param address - 新的地址（可选）
     * @param website - 新的网站（可选）
     * @param taxId - 新的税号（可选）
     * @param paymentTerms - 新的付款条件（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateSupplier(supplierId: string, name?: string, contactPerson?: string, 
                    email?: string, phone?: string, address?: string, website?: string, 
                    taxId?: string, paymentTerms?: string): boolean {
        if (!this.suppliers.has(supplierId)) {
            return false;
        }
        
        const supplier = this.suppliers.get(supplierId)!;
        
        // 检查电子邮件是否已被其他供应商使用
        if (email && email !== supplier.email) {
            for (const [id, s] of this.suppliers) {
                if (id !== supplierId && s.email === email) {
                    return false;
                }
            }
            supplier.email = email;
        }
        
        if (name !== undefined) {
            supplier.name = name;
        }
        
        if (contactPerson !== undefined) {
            supplier.contactPerson = contactPerson;
        }
        
        if (phone !== undefined) {
            supplier.phone = phone;
        }
        
        if (address !== undefined) {
            supplier.address = address;
        }
        
        if (website !== undefined) {
            supplier.website = website;
        }
        
        if (taxId !== undefined) {
            supplier.taxId = taxId;
        }
        
        if (paymentTerms !== undefined) {
            supplier.paymentTerms = paymentTerms;
        }
        
        supplier.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取供应商信息
     * @param supplierId - 供应商ID
     * @returns 供应商对象，若不存在则返回null
     */
    getSupplier(supplierId: string): Supplier | null {
        if (!this.suppliers.has(supplierId)) {
            return null;
        }
        
        return this.suppliers.get(supplierId)!;
    }
    
    /**
     * 删除供应商
     * @param supplierId - 供应商ID
     * @returns 删除成功返回true，否则返回false
     */
    removeSupplier(supplierId: string): boolean {
        if (!this.suppliers.has(supplierId)) {
            return false;
        }
        
        // 检查是否有产品使用该供应商
        for (const product of this.products.values()) {
            if (product.supplierIds && product.supplierIds.includes(supplierId)) {
                return false;
            }
        }
        
        // 检查是否有未完成的采购订单
        for (const order of this.purchaseOrders.values()) {
            if (order.supplierId === supplierId && order.status !== PurchaseOrderStatus.Completed) {
                return false;
            }
        }
        
        // 删除供应商
        this.suppliers.delete(supplierId);
        
        return true;
    }
    
    /**
     * 添加新类别
     * @param categoryId - 类别唯一标识符
     * @param name - 类别名称
     * @param description - 类别描述
     * @param parentId - 父类别ID（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addCategory(categoryId: string, name: string, description: string, parentId?: string): boolean {
        if (this.categories.has(categoryId)) {
            return false;
        }
        
        // 检查父类别是否存在
        if (parentId && !this.categories.has(parentId)) {
            return false;
        }
        
        const category = new Category(categoryId, name, description, parentId);
        
        this.categories.set(categoryId, category);
        
        return true;
    }
    
    /**
     * 更新类别信息
     * @param categoryId - 类别ID
     * @param name - 新的名称（可选）
     * @param description - 新的描述（可选）
     * @param parentId - 新的父类别ID（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateCategory(categoryId: string, name?: string, description?: string, parentId?: string): boolean {
        if (!this.categories.has(categoryId)) {
            return false;
        }
        
        const category = this.categories.get(categoryId)!;
        
        // 检查父类别是否存在
        if (parentId !== undefined && !this.categories.has(parentId)) {
            return false;
        }
        
        // 检查是否会创建循环引用
        if (parentId !== undefined && parentId !== category.parentId) {
            let currentId: string | null = parentId;
            while (currentId) {
                if (currentId === categoryId) {
                    return false;  // 循环引用
                }
                
                const currentCategory = this.categories.get(currentId);
                if (!currentCategory) {
                    break;
                }
                
                currentId = currentCategory.parentId || null;
            }
        }
        
        if (name !== undefined) {
            category.name = name;
        }
        
        if (description !== undefined) {
            category.description = description;
        }
        
        if (parentId !== undefined) {
            category.parentId = parentId;
        }
        
        category.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取类别信息
     * @param categoryId - 类别ID
     * @returns 类别对象，若不存在则返回null
     */
    getCategory(categoryId: string): Category | null {
        if (!this.categories.has(categoryId)) {
            return null;
        }
        
        return this.categories.get(categoryId)!;
    }
    
    /**
     * 删除类别
     * @param categoryId - 类别ID
     * @returns 删除成功返回true，否则返回false
     */
    removeCategory(categoryId: string): boolean {
        if (!this.categories.has(categoryId)) {
            return false;
        }
        
        // 检查是否有产品使用该类别
        for (const product of this.products.values()) {
            if (product.categoryId === categoryId) {
                return false;
            }
        }
        
        // 检查是否有子类别
        for (const category of this.categories.values()) {
            if (category.parentId === categoryId) {
                return false;
            }
        }
        
        // 删除类别
        this.categories.delete(categoryId);
        
        return true;
    }
    
    /**
     * 添加新仓库
     * @param warehouseId - 仓库唯一标识符
     * @param name - 仓库名称
     * @param location - 仓库位置
     * @param manager - 仓库经理
     * @param phone - 电话
     * @param email - 电子邮件
     * @param capacity - 最大容量
     * @returns 添加成功返回true，否则返回false
     */
    addWarehouse(warehouseId: string, name: string, location: string, manager: string, 
                 phone: string, email: string, capacity: number): boolean {
        if (this.warehouses.has(warehouseId)) {
            return false;
        }
        
        const warehouse = new Warehouse(
            warehouseId,
            name,
            location,
            manager,
            phone,
            email,
            capacity
        );
        
        this.warehouses.set(warehouseId, warehouse);
        
        return true;
    }
    
    /**
     * 更新仓库信息
     * @param warehouseId - 仓库ID
     * @param name - 新的名称（可选）
     * @param location - 新的位置（可选）
     * @param manager - 新的经理（可选）
     * @param phone - 新的电话（可选）
     * @param email - 新的电子邮件（可选）
     * @param capacity - 新的最大容量（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateWarehouse(warehouseId: string, name?: string, location?: string, 
                    manager?: string, phone?: string, email?: string, capacity?: number): boolean {
        if (!this.warehouses.has(warehouseId)) {
            return false;
        }
        
        const warehouse = this.warehouses.get(warehouseId)!;
        
        if (name !== undefined) {
            warehouse.name = name;
        }
        
        if (location !== undefined) {
            warehouse.location = location;
        }
        
        if (manager !== undefined) {
            warehouse.manager = manager;
        }
        
        if (phone !== undefined) {
            warehouse.phone = phone;
        }
        
        if (email !== undefined) {
            warehouse.email = email;
        }
        
        if (capacity !== undefined) {
            warehouse.capacity = capacity;
        }
        
        warehouse.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取仓库信息
     * @param warehouseId - 仓库ID
     * @returns 仓库对象，若不存在则返回null
     */
    getWarehouse(warehouseId: string): Warehouse | null {
        if (!this.warehouses.has(warehouseId)) {
            return null;
        }
        
        return this.warehouses.get(warehouseId)!;
    }
    
    /**
     * 删除仓库
     * @param warehouseId - 仓库ID
     * @returns 删除成功返回true，否则返回false
     */
    removeWarehouse(warehouseId: string): boolean {
        if (!this.warehouses.has(warehouseId)) {
            return false;
        }
        
        // 检查仓库中是否有库存
        for (const location of this.stockLocations.values()) {
            if (location.warehouseId === warehouseId && location.quantity > 0) {
                return false;
            }
        }
        
        // 删除所有与该仓库相关的库位
        for (const [locationId, location] of this.stockLocations) {
            if (location.warehouseId === warehouseId) {
                this.stockLocations.delete(locationId);
            }
        }
        
        // 删除仓库
        this.warehouses.delete(warehouseId);
        
        return true;
    }
    
    /**
     * 添加库位
     * @param locationId - 库位唯一标识符
     * @param warehouseId - 仓库ID
     * @param productId - 产品ID
     * @param zone - 区域
     * @param aisle - 通道
     * @param shelf - 货架
     * @param bin - 箱位
     * @param quantity - 数量
     * @param minQuantity - 最小数量（可选）
     * @param maxQuantity - 最大数量（可选）
     * @returns 添加成功返回true，否则返回false
     */
    addStockLocation(locationId: string, warehouseId: string, productId: string, 
                      zone: string, aisle: string, shelf: string, bin: string, 
                      quantity: number, minQuantity?: number, maxQuantity?: number): boolean {
        if (this.stockLocations.has(locationId)) {
            return false;
        }
        
        if (!this.warehouses.has(warehouseId) || !this.products.has(productId)) {
            return false;
        }
        
        if (quantity < 0) {
            return false;
        }
        
        const location = new StockLocation(
            locationId,
            warehouseId,
            productId,
            zone,
            aisle,
            shelf,
            bin,
            quantity,
            minQuantity,
            maxQuantity
        );
        
        this.stockLocations.set(locationId, location);
        
        // 记录库存变动
        this.recordStockMovement(productId, quantity, StockMovementType.Initial, warehouseId, locationId);
        
        return true;
    }
    
    /**
     * 更新库位信息
     * @param locationId - 库位ID
     * @param quantity - 新的数量（可选）
     * @param minQuantity - 新的最小数量（可选）
     * @param maxQuantity - 新的最大数量（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updateStockLocation(locationId: string, quantity?: number, 
                          minQuantity?: number, maxQuantity?: number): boolean {
        if (!this.stockLocations.has(locationId)) {
            return false;
        }
        
        const location = this.stockLocations.get(locationId)!;
        const productId = location.productId;
        const warehouseId = location.warehouseId;
        const previousQuantity = location.quantity;
        
        if (quantity !== undefined) {
            if (quantity < 0) {
                return false;
            }
            
            const difference = quantity - previousQuantity;
            location.quantity = quantity;
            
            // 记录库存变动
            if (difference > 0) {
                this.recordStockMovement(productId, difference, StockMovementType.StockIn, warehouseId, locationId);
            } else if (difference < 0) {
                this.recordStockMovement(productId, -difference, StockMovementType.StockOut, warehouseId, locationId);
            }
        }
        
        if (minQuantity !== undefined) {
            location.minQuantity = minQuantity;
        }
        
        if (maxQuantity !== undefined) {
            location.maxQuantity = maxQuantity;
        }
        
        location.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 获取库位信息
     * @param locationId - 库位ID
     * @returns 库位对象，若不存在则返回null
     */
    getStockLocation(locationId: string): StockLocation | null {
        if (!this.stockLocations.has(locationId)) {
            return null;
        }
        
        return this.stockLocations.get(locationId)!;
    }
    
    /**
     * 删除库位
     * @param locationId - 库位ID
     * @returns 删除成功返回true，否则返回false
     */
    removeStockLocation(locationId: string): boolean {
        if (!this.stockLocations.has(locationId)) {
            return false;
        }
        
        const location = this.stockLocations.get(locationId)!;
        
        // 检查库位中是否有库存
        if (location.quantity > 0) {
            return false;
        }
        
        // 删除库位
        this.stockLocations.delete(locationId);
        
        return true;
    }
    
    /**
     * 记录库存变动
     * @param productId - 产品ID
     * @param quantity - 变动数量
     * @param movementType - 变动类型
     * @param warehouseId - 仓库ID
     * @param locationId - 库位ID（可选）
     * @param referenceId - 参考ID（如交易ID、采购订单ID等，可选）
     * @param notes - 备注（可选）
     * @returns 记录成功返回true，否则返回false
     */
    recordStockMovement(productId: string, quantity: number, movementType: StockMovementType, 
                         warehouseId: string, locationId?: string, referenceId?: string, 
                         notes?: string): boolean {
        if (!this.products.has(productId)) {
            return false;
        }
        
        if (quantity <= 0) {
            return false;
        }
        
        const stockRecord = new StockRecord(
            quantity,
            movementType,
            warehouseId,
            locationId,
            referenceId,
            notes
        );
        
        if (!this.stockHistory.has(productId)) {
            this.stockHistory.set(productId, []);
        }
        
        this.stockHistory.get(productId)!.push(stockRecord);
        
        return true;
    }
    
    /**
     * 获取产品库存记录
     * @param productId - 产品ID
     * @param movementType - 变动类型筛选（可选）
     * @param warehouseId - 仓库ID筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @returns 库存记录列表
     */
    getStockHistory(productId: string, movementType?: StockMovementType, 
                    warehouseId?: string, startDate?: Date, endDate?: Date): StockRecord[] {
        if (!this.products.has(productId) || !this.stockHistory.has(productId)) {
            return [];
        }
        
        let records = [...this.stockHistory.get(productId)!];
        
        if (movementType) {
            records = records.filter(record => record.movementType === movementType);
        }
        
        if (warehouseId) {
            records = records.filter(record => record.warehouseId === warehouseId);
        }
        
        if (startDate) {
            records = records.filter(record => record.timestamp >= startDate);
        }
        
        if (endDate) {
            records = records.filter(record => record.timestamp <= endDate);
        }
        
        return records;
    }
    
    /**
     * 获取产品当前库存
     * @param productId - 产品ID
     * @param warehouseId - 仓库ID筛选（可选）
     * @returns 库存数量
     */
    getCurrentStock(productId: string, warehouseId?: string): number {
        if (!this.products.has(productId)) {
            return 0;
        }
        
        let totalQuantity = 0;
        
        for (const location of this.stockLocations.values()) {
            if (location.productId === productId) {
                if (!warehouseId || location.warehouseId === warehouseId) {
                    totalQuantity += location.quantity;
                }
            }
        }
        
        return totalQuantity;
    }
    
    /**
     * 创建采购订单
     * @param purchaseOrderId - 采购订单唯一标识符
     * @param supplierId - 供应商ID
     * @param items - 采购项目列表
     * @param expectedDeliveryDate - 预计交货日期
     * @param notes - 备注（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createPurchaseOrder(purchaseOrderId: string, supplierId: string, items: PurchaseOrderItem[], 
                         expectedDeliveryDate: Date, notes?: string): boolean {
        if (this.purchaseOrders.has(purchaseOrderId)) {
            return false;
        }
        
        if (!this.suppliers.has(supplierId)) {
            return false;
        }
        
        // 验证所有采购项目
        for (const item of items) {
            if (!this.products.has(item.productId)) {
                return false;
            }
            
            if (item.quantity <= 0 || item.unitPrice <= 0) {
                return false;
            }
        }
        
        // 计算总金额
        let totalAmount = 0;
        for (const item of items) {
            totalAmount += item.quantity * item.unitPrice;
        }
        
        const purchaseOrder = new PurchaseOrder(
            purchaseOrderId,
            supplierId,
            items,
            expectedDeliveryDate,
            totalAmount,
            notes
        );
        
        this.purchaseOrders.set(purchaseOrderId, purchaseOrder);
        
        return true;
    }
    
    /**
     * 更新采购订单状态
     * @param purchaseOrderId - 采购订单ID
     * @param status - 新的状态
     * @param receivedItems - 已收货项目列表（可选）
     * @param notes - 备注（可选）
     * @returns 更新成功返回true，否则返回false
     */
    updatePurchaseOrderStatus(purchaseOrderId: string, status: PurchaseOrderStatus, 
                               receivedItems?: ReceivedItem[], notes?: string): boolean {
        if (!this.purchaseOrders.has(purchaseOrderId)) {
            return false;
        }
        
        const purchaseOrder = this.purchaseOrders.get(purchaseOrderId)!;
        
        // 验证状态转换是否有效
        if (!this.isValidStatusTransition(purchaseOrder.status, status)) {
            return false;
        }
        
        purchaseOrder.status = status;
        
        if (notes !== undefined) {
            purchaseOrder.notes = notes;
        }
        
        // 如果状态为已收货，记录库存变动
        if (status === PurchaseOrderStatus.Received && receivedItems) {
            for (const item of receivedItems) {
                const productId = item.productId;
                const quantity = item.quantity;
                
                // 找到合适的库位并更新库存
                let locationUpdated = false;
                for (const location of this.stockLocations.values()) {
                    if (location.productId === productId) {
                        const newQuantity = location.quantity + quantity;
                        this.updateStockLocation(location.locationId, newQuantity);
                        locationUpdated = true;
                        break;
                    }
                }
                
                // 如果没有找到现有库位，创建一个新库位
                if (!locationUpdated) {
                    // 需要确定仓库ID，这里简化为使用第一个仓库
                    let warehouseId = '';
                    for (const [id, warehouse] of this.warehouses) {
                        warehouseId = id;
                        break;
                    }
                    
                    if (warehouseId) {
                        const locationId = `loc_${productId}_${Date.now()}`;
                        this.addStockLocation(locationId, warehouseId, productId, 'A', '01', '01', '01', quantity);
                        this.recordStockMovement(productId, quantity, StockMovementType.Purchase, warehouseId, undefined, purchaseOrderId);
                    }
                }
            }
        }
        
        purchaseOrder.updatedAt = new Date();
        
        return true;
    }
    
    /**
     * 验证状态转换是否有效
     * @param currentStatus - 当前状态
     * @param newStatus - 新状态
     * @returns 转换有效返回true，否则返回false
     */
    private isValidStatusTransition(currentStatus: PurchaseOrderStatus, newStatus: PurchaseOrderStatus): boolean {
        switch (currentStatus) {
            case PurchaseOrderStatus.Draft:
                return newStatus === PurchaseOrderStatus.Approved || 
                       newStatus === PurchaseOrderStatus.Cancelled;
            case PurchaseOrderStatus.Approved:
                return newStatus === PurchaseOrderStatus.Ordered || 
                       newStatus === PurchaseOrderStatus.Cancelled;
            case PurchaseOrderStatus.Ordered:
                return newStatus === PurchaseOrderStatus.Shipped || 
                       newStatus === PurchaseOrderStatus.Cancelled;
            case PurchaseOrderStatus.Shipped:
                return newStatus === PurchaseOrderStatus.Received;
            case PurchaseOrderStatus.Received:
                return newStatus === PurchaseOrderStatus.Completed;
            case PurchaseOrderStatus.Completed:
                return false;  // 已完成的订单不能更改状态
            case PurchaseOrderStatus.Cancelled:
                return false;  // 已取消的订单不能更改状态
            default:
                return false;
        }
    }
    
    /**
     * 获取采购订单信息
     * @param purchaseOrderId - 采购订单ID
     * @returns 采购订单对象，若不存在则返回null
     */
    getPurchaseOrder(purchaseOrderId: string): PurchaseOrder | null {
        if (!this.purchaseOrders.has(purchaseOrderId)) {
            return null;
        }
        
        return this.purchaseOrders.get(purchaseOrderId)!;
    }
    
    /**
     * 获取供应商的采购订单
     * @param supplierId - 供应商ID
     * @param status - 状态筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @returns 采购订单列表
     */
    getPurchaseOrdersBySupplier(supplierId: string, status?: PurchaseOrderStatus, 
                                  startDate?: Date, endDate?: Date): PurchaseOrder[] {
        if (!this.suppliers.has(supplierId)) {
            return [];
        }
        
        let orders: PurchaseOrder[] = [];
        
        for (const order of this.purchaseOrders.values()) {
            if (order.supplierId === supplierId) {
                if (status && order.status !== status) {
                    continue;
                }
                
                if (startDate && order.createdAt < startDate) {
                    continue;
                }
                
                if (endDate && order.createdAt > endDate) {
                    continue;
                }
                
                orders.push(order);
            }
        }
        
        // 按创建时间降序排序
        orders.sort((a, b) => b.createdAt.getTime() - a.createdAt.getTime());
        
        return orders;
    }
    
    /**
     * 创建交易
     * @param transactionId - 交易唯一标识符
     * @param type - 交易类型
     * @param items - 交易项目列表
     * @param customerId - 客户ID（可选）
     * @param notes - 备注（可选）
     * @returns 创建成功返回true，否则返回false
     */
    createTransaction(transactionId: string, type: TransactionType, items: TransactionItem[], 
                       customerId?: string, notes?: string): boolean {
        if (this.transactions.has(transactionId)) {
            return false;
        }
        
        // 验证所有交易项目
        for (const item of items) {
            if (!this.products.has(item.productId)) {
                return false;
            }
            
            if (item.quantity <= 0) {
                return false;
            }
            
            // 检查库存是否足够
            if (type === TransactionType.Sale) {
                const currentStock = this.getCurrentStock(item.productId);
                if (currentStock < item.quantity) {
                    return false;
                }
            }
        }
        
        // 计算总金额
        let totalAmount = 0;
        for (const item of items) {
            const product = this.products.get(item.productId)!;
            totalAmount += item.quantity * product.unitPrice;
        }
        
        const transaction = new Transaction(
            transactionId,
            type,
            items,
            totalAmount,
            customerId,
            notes
        );
        
        this.transactions.set(transactionId, transaction);
        
        // 更新库存
        for (const item of items) {
            const productId = item.productId;
            const quantity = item.quantity;
            
            if (type === TransactionType.Sale) {
                // 找到合适的库位并减少库存
                for (const location of this.stockLocations.values()) {
                    if (location.productId === productId && location.quantity > 0) {
                        const deduct = Math.min(quantity, location.quantity);
                        const newQuantity = location.quantity - deduct;
                        this.updateStockLocation(location.locationId, newQuantity);
                        
                        // 记录库存变动
                        this.recordStockMovement(productId, deduct, StockMovementType.Sale, location.warehouseId, location.locationId, transactionId);
                        
                        // 更新剩余需要扣除的数量
                        item.quantity -= deduct;
                        
                        if (item.quantity <= 0) {
                            break;
                        }
                    }
                }
            } else if (type === TransactionType.Return) {
                // 找到合适的库位并增加库存
                let locationUpdated = false;
                for (const location of this.stockLocations.values()) {
                    if (location.productId === productId) {
                        const newQuantity = location.quantity + quantity;
                        this.updateStockLocation(location.locationId, newQuantity);
                        locationUpdated = true;
                        break;
                    }
                }
                
                // 如果没有找到现有库位，创建一个新库位
                if (!locationUpdated) {
                    // 需要确定仓库ID，这里简化为使用第一个仓库
                    let warehouseId = '';
                    for (const [id, warehouse] of this.warehouses) {
                        warehouseId = id;
                        break;
                    }
                    
                    if (warehouseId) {
                        const locationId = `loc_${productId}_${Date.now()}`;
                        this.addStockLocation(locationId, warehouseId, productId, 'A', '01', '01', '01', quantity);
                        this.recordStockMovement(productId, quantity, StockMovementType.Return, warehouseId, undefined, transactionId);
                    }
                }
            }
        }
        
        return true;
    }
    
    /**
     * 获取交易信息
     * @param transactionId - 交易ID
     * @returns 交易对象，若不存在则返回null
     */
    getTransaction(transactionId: string): Transaction | null {
        if (!this.transactions.has(transactionId)) {
            return null;
        }
        
        return this.transactions.get(transactionId)!;
    }
    
    /**
     * 获取交易历史
     * @param type - 交易类型筛选（可选）
     * @param productId - 产品ID筛选（可选）
     * @param customerId - 客户ID筛选（可选）
     * @param startDate - 开始日期筛选（可选）
     * @param endDate - 结束日期筛选（可选）
     * @returns 交易列表
     */
    getTransactions(type?: TransactionType, productId?: string, 
                    customerId?: string, startDate?: Date, endDate?: Date): Transaction[] {
        let transactions: Transaction[] = [];
        
        for (const transaction of this.transactions.values()) {
            if (type && transaction.type !== type) {
                continue;
            }
            
            if (productId) {
                let hasProduct = false;
                for (const item of transaction.items) {
                    if (item.productId === productId) {
                        hasProduct = true;
                        break;
                    }
                }
                if (!hasProduct) {
                    continue;
                }
            }
            
            if (customerId && transaction.customerId !== customerId) {
                continue;
            }
            
            if (startDate && transaction.timestamp < startDate) {
                continue;
            }
            
            if (endDate && transaction.timestamp > endDate) {
                continue;
            }
            
            transactions.push(transaction);
        }
        
        // 按时间降序排序
        transactions.sort((a, b) => b.timestamp.getTime() - a.timestamp.getTime());
        
        return transactions;
    }
    
    /**
     * 获取需要补货的产品列表
     * @returns 需要补货的产品列表
     */
    getRestockProducts(): Product[] {
        const restockProducts: Product[] = [];
        
        for (const product of this.products.values()) {
            const currentStock = this.getCurrentStock(product.productId);
            
            if (product.reorderPoint !== undefined && currentStock <= product.reorderPoint) {
                restockProducts.push(product);
            }
        }
        
        return restockProducts;
    }
    
    /**
     * 获取库存过剩的产品列表
     * @returns 库存过剩的产品列表
     */
    getOverstockProducts(): Product[] {
        const overstockProducts: Product[] = [];
        
        for (const product of this.products.values()) {
            const currentStock = this.getCurrentStock(product.productId);
            
            if (product.maxStockLevel !== undefined && currentStock > product.maxStockLevel) {
                overstockProducts.push(product);
            }
        }
        
        return overstockProducts;
    }
}

/**
 * 产品类
 */
class Product {
    productId: string;
    name: string;
    description: string;
    sku: string;
    barcode?: string;
    unitOfMeasure: string;
    unitPrice: number;
    categoryId?: string;
    supplierIds?: string[];
    reorderPoint?: number;
    maxStockLevel?: number;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(productId: string, name: string, description: string, sku: string, 
                barcode: string, unitOfMeasure: string, unitPrice: number, 
                categoryId?: string, supplierIds?: string[], reorderPoint?: number, 
                maxStockLevel?: number) {
        this.productId = productId;
        this.name = name;
        this.description = description;
        this.sku = sku;
        this.barcode = barcode;
        this.unitOfMeasure = unitOfMeasure;
        this.unitPrice = unitPrice;
        this.categoryId = categoryId;
        this.supplierIds = supplierIds ? [...supplierIds] : undefined;
        this.reorderPoint = reorderPoint;
        this.maxStockLevel = maxStockLevel;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 供应商类
 */
class Supplier {
    supplierId: string;
    name: string;
    contactPerson: string;
    email: string;
    phone: string;
    address: string;
    website?: string;
    taxId?: string;
    paymentTerms?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(supplierId: string, name: string, contactPerson: string, 
                email: string, phone: string, address: string, website?: string, 
                taxId?: string, paymentTerms?: string) {
        this.supplierId = supplierId;
        this.name = name;
        this.contactPerson = contactPerson;
        this.email = email;
        this.phone = phone;
        this.address = address;
        this.website = website;
        this.taxId = taxId;
        this.paymentTerms = paymentTerms;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 类别类
 */
class Category {
    categoryId: string;
    name: string;
    description: string;
    parentId?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(categoryId: string, name: string, description: string, parentId?: string) {
        this.categoryId = categoryId;
        this.name = name;
        this.description = description;
        this.parentId = parentId;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 仓库类
 */
class Warehouse {
    warehouseId: string;
    name: string;
    location: string;
    manager: string;
    phone: string;
    email: string;
    capacity: number;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(warehouseId: string, name: string, location: string, manager: string, 
                phone: string, email: string, capacity: number) {
        this.warehouseId = warehouseId;
        this.name = name;
        this.location = location;
        this.manager = manager;
        this.phone = phone;
        this.email = email;
        this.capacity = capacity;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 库位类
 */
class StockLocation {
    locationId: string;
    warehouseId: string;
    productId: string;
    zone: string;
    aisle: string;
    shelf: string;
    bin: string;
    quantity: number;
    minQuantity?: number;
    maxQuantity?: number;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(locationId: string, warehouseId: string, productId: string, 
                zone: string, aisle: string, shelf: string, bin: string, 
                quantity: number, minQuantity?: number, maxQuantity?: number) {
        this.locationId = locationId;
        this.warehouseId = warehouseId;
        this.productId = productId;
        this.zone = zone;
        this.aisle = aisle;
        this.shelf = shelf;
        this.bin = bin;
        this.quantity = quantity;
        this.minQuantity = minQuantity;
        this.maxQuantity = maxQuantity;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 库存记录类
 */
class StockRecord {
    quantity: number;
    movementType: StockMovementType;
    warehouseId: string;
    locationId?: string;
    referenceId?: string;
    notes?: string;
    timestamp: Date;
    
    constructor(quantity: number, movementType: StockMovementType, warehouseId: string, 
                locationId?: string, referenceId?: string, notes?: string) {
        this.quantity = quantity;
        this.movementType = movementType;
        this.warehouseId = warehouseId;
        this.locationId = locationId;
        this.referenceId = referenceId;
        this.notes = notes;
        this.timestamp = new Date();
    }
}

/**
 * 库存变动类型枚举
 */
enum StockMovementType {
    Initial = "initial",
    Purchase = "purchase",
    Sale = "sale",
    Return = "return",
    StockIn = "stock_in",
    StockOut = "stock_out",
    Adjustment = "adjustment",
    Transfer = "transfer"
}

/**
 * 采购订单类
 */
class PurchaseOrder {
    purchaseOrderId: string;
    supplierId: string;
    items: PurchaseOrderItem[];
    expectedDeliveryDate: Date;
    status: PurchaseOrderStatus;
    totalAmount: number;
    notes?: string;
    createdAt: Date;
    updatedAt: Date;
    
    constructor(purchaseOrderId: string, supplierId: string, items: PurchaseOrderItem[], 
                expectedDeliveryDate: Date, totalAmount: number, notes?: string) {
        this.purchaseOrderId = purchaseOrderId;
        this.supplierId = supplierId;
        this.items = items;
        this.expectedDeliveryDate = expectedDeliveryDate;
        this.status = PurchaseOrderStatus.Draft;
        this.totalAmount = totalAmount;
        this.notes = notes;
        this.createdAt = new Date();
        this.updatedAt = new Date();
    }
}

/**
 * 采购订单项目接口
 */
interface PurchaseOrderItem {
    productId: string;
    quantity: number;
    unitPrice: number;
}

/**
 * 已收货项目接口
 */
interface ReceivedItem {
    productId: string;
    quantity: number;
}

/**
 * 采购订单状态枚举
 */
enum PurchaseOrderStatus {
    Draft = "draft",
    Approved = "approved",
    Ordered = "ordered",
    Shipped = "shipped",
    Received = "received",
    Completed = "completed",
    Cancelled = "cancelled"
}

/**
 * 交易类
 */
class Transaction {
    transactionId: string;
    type: TransactionType;
    items: TransactionItem[];
    totalAmount: number;
    customerId?: string;
    notes?: string;
    timestamp: Date;
    
    constructor(transactionId: string, type: TransactionType, items: TransactionItem[], 
                totalAmount: number, customerId?: string, notes?: string) {
        this.transactionId = transactionId;
        this.type = type;
        this.items = items;
        this.totalAmount = totalAmount;
        this.customerId = customerId;
        this.notes = notes;
        this.timestamp = new Date();
    }
}

/**
 * 交易项目接口
 */
interface TransactionItem {
    productId: string;
    quantity: number;
}

/**
 * 交易类型枚举
 */
enum TransactionType {
    Sale = "sale",
    Return = "return",
    Adjustment = "adjustment"
}