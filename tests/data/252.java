import java.util.*;
import java.time.*;

/**
 * 库存管理系统类
 * 用于管理产品库存、供应商、采购和销售
 */
public class InventoryManagementSystem {
    private Map<String, Product> products;
    private Map<String, Supplier> suppliers;
    private List<Transaction> transactions;
    private List<PurchaseOrder> purchaseOrders;
    private List<SalesOrder> salesOrders;
    private List<Report> reports;
    
    /**
     * 库存管理系统构造函数
     */
    public InventoryManagementSystem() {
        this.products = new HashMap<>();
        this.suppliers = new HashMap<>();
        this.transactions = new ArrayList<>();
        this.purchaseOrders = new ArrayList<>();
        this.salesOrders = new ArrayList<>();
        this.reports = new ArrayList<>();
    }
    
    /**
     * 添加新产品
     * @param productId 产品唯一标识符
     * @param name 产品名称
     * @param description 产品描述
     * @param category 产品类别
     * @param price 产品价格
     * @param cost 产品成本
     * @param quantity 初始库存数量
     * @param minimumStock 最低库存阈值
     * @param supplierId 供应商ID
     * @return 添加成功返回true，否则返回false
     */
    public boolean addProduct(String productId, String name, String description, String category,
                              double price, double cost, int quantity, int minimumStock, String supplierId) {
        if (products.containsKey(productId)) {
            return false;
        }
        
        if (!suppliers.containsKey(supplierId)) {
            return false;
        }
        
        Product product = new Product(productId, name, description, category, price, cost, 
                                     quantity, minimumStock, supplierId);
        products.put(productId, product);
        return true;
    }
    
    /**
     * 添加新供应商
     * @param supplierId 供应商唯一标识符
     * @param name 供应商名称
     * @param contactPerson 联系人
     * @param email 电子邮件
     * @param phone 电话号码
     * @param address 地址
     * @return 添加成功返回true，否则返回false
     */
    public boolean addSupplier(String supplierId, String name, String contactPerson, 
                               String email, String phone, String address) {
        if (suppliers.containsKey(supplierId)) {
            return false;
        }
        
        Supplier supplier = new Supplier(supplierId, name, contactPerson, email, phone, address);
        suppliers.put(supplierId, supplier);
        return true;
    }
    
    /**
     * 更新库存
     * @param productId 产品ID
     * @param quantityChange 数量变化（正值为增加，负值为减少）
     * @param transactionType 交易类型（purchase, sale, adjustment, return）
     * @param notes 交易备注
     * @return 更新成功返回true，否则返回false
     */
    public boolean updateStock(String productId, int quantityChange, String transactionType, String notes) {
        if (!products.containsKey(productId)) {
            return false;
        }
        
        Product product = products.get(productId);
        int newQuantity = product.getQuantity() + quantityChange;
        
        if (newQuantity < 0) {
            return false;
        }
        
        // 更新产品库存
        product.setQuantity(newQuantity);
        product.setLastUpdated(LocalDateTime.now());
        
        // 记录交易
        Transaction transaction = new Transaction(
            generateId("transaction"), 
            productId, 
            quantityChange, 
            transactionType, 
            notes, 
            LocalDateTime.now()
        );
        transactions.add(transaction);
        
        // 检查是否需要补货
        if (newQuantity <= product.getMinimumStock() && transactionType.equals("sale")) {
            generateReorderAlert(productId);
        }
        
        return true;
    }
    
    /**
     * 创建采购订单
     * @param supplierId 供应商ID
     * @param items 采购项目列表
     * @param orderDate 订单日期
     * @param expectedDeliveryDate 预计交货日期
     * @return 创建成功返回采购订单ID，否则返回null
     */
    public String createPurchaseOrder(String supplierId, List<OrderItem> items, 
                                     LocalDate orderDate, LocalDate expectedDeliveryDate) {
        if (!suppliers.containsKey(supplierId) || items.isEmpty()) {
            return null;
        }
        
        // 计算总金额
        double totalAmount = 0;
        for (OrderItem item : items) {
            if (!products.containsKey(item.getProductId())) {
                return null;
            }
            Product product = products.get(item.getProductId());
            totalAmount += product.getCost() * item.getQuantity();
        }
        
        String orderId = generateId("purchase_order");
        PurchaseOrder order = new PurchaseOrder(
            orderId, 
            supplierId, 
            items, 
            "pending",  // pending, approved, received, cancelled
            orderDate, 
            expectedDeliveryDate, 
            totalAmount
        );
        purchaseOrders.add(order);
        
        return orderId;
    }
    
    /**
     * 创建销售订单
     * @param customerId 客户ID
     * @param items 销售项目列表
     * @param orderDate 订单日期
     * @param shippingAddress 配送地址
     * @return 创建成功返回销售订单ID，否则返回null
     */
    public String createSalesOrder(String customerId, List<OrderItem> items, 
                                  LocalDate orderDate, String shippingAddress) {
        if (items.isEmpty()) {
            return null;
        }
        
        // 检查库存是否足够
        for (OrderItem item : items) {
            if (!products.containsKey(item.getProductId())) {
                return null;
            }
            Product product = products.get(item.getProductId());
            if (product.getQuantity() < item.getQuantity()) {
                return null;  // 库存不足
            }
        }
        
        // 计算总金额
        double totalAmount = 0;
        for (OrderItem item : items) {
            Product product = products.get(item.getProductId());
            totalAmount += product.getPrice() * item.getQuantity();
        }
        
        String orderId = generateId("sales_order");
        SalesOrder order = new SalesOrder(
            orderId, 
            customerId, 
            items, 
            "pending",  // pending, processing, shipped, delivered, cancelled
            orderDate, 
            shippingAddress, 
            totalAmount
        );
        salesOrders.add(order);
        
        return orderId;
    }
    
    /**
     * 处理采购订单收货
     * @param orderId 采购订单ID
     * @param receivedDate 收货日期
     * @param notes 收货备注
     * @return 处理成功返回true，否则返回false
     */
    public boolean receivePurchaseOrder(String orderId, LocalDate receivedDate, String notes) {
        PurchaseOrder order = findPurchaseOrder(orderId);
        if (order == null || !order.getStatus().equals("approved")) {
            return false;
        }
        
        // 更新库存
        for (OrderItem item : order.getItems()) {
            updateStock(item.getProductId(), item.getQuantity(), "purchase", 
                       "Received from PO: " + orderId);
        }
        
        // 更新订单状态
        order.setStatus("received");
        order.setReceivedDate(receivedDate);
        order.setNotes(notes);
        
        return true;
    }
    
    /**
     * 处理销售订单发货
     * @param orderId 销售订单ID
     * @param shippedDate 发货日期
     * @param trackingNumber 追踪号码
     * @param notes 发货备注
     * @return 处理成功返回true，否则返回false
     */
    public boolean shipSalesOrder(String orderId, LocalDate shippedDate, 
                                 String trackingNumber, String notes) {
        SalesOrder order = findSalesOrder(orderId);
        if (order == null || !order.getStatus().equals("processing")) {
            return false;
        }
        
        // 更新库存
        for (OrderItem item : order.getItems()) {
            updateStock(item.getProductId(), -item.getQuantity(), "sale", 
                       "Sold from SO: " + orderId);
        }
        
        // 更新订单状态
        order.setStatus("shipped");
        order.setShippedDate(shippedDate);
        order.setTrackingNumber(trackingNumber);
        order.setNotes(notes);
        
        return true;
    }
    
    /**
     * 获取产品信息
     * @param productId 产品ID
     * @return 产品信息，不存在则返回null
     */
    public Product getProduct(String productId) {
        return products.get(productId);
    }
    
    /**
     * 获取供应商信息
     * @param supplierId 供应商ID
     * @return 供应商信息，不存在则返回null
     */
    public Supplier getSupplier(String supplierId) {
        return suppliers.get(supplierId);
    }
    
    /**
     * 获取指定供应商的所有产品
     * @param supplierId 供应商ID
     * @return 产品列表
     */
    public List<Product> getProductsBySupplier(String supplierId) {
        List<Product> result = new ArrayList<>();
        
        if (!suppliers.containsKey(supplierId)) {
            return result;
        }
        
        for (Product product : products.values()) {
            if (product.getSupplierId().equals(supplierId)) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取指定类别的所有产品
     * @param category 产品类别
     * @return 产品列表
     */
    public List<Product> getProductsByCategory(String category) {
        List<Product> result = new ArrayList<>();
        
        for (Product product : products.values()) {
            if (product.getCategory().equals(category)) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取低库存产品
     * @param threshold 库存阈值，如果为null则使用产品最低库存阈值
     * @return 低库存产品列表
     */
    public List<Product> getLowStockProducts(Integer threshold) {
        List<Product> result = new ArrayList<>();
        
        for (Product product : products.values()) {
            int minThreshold = threshold != null ? threshold : product.getMinimumStock();
            if (product.getQuantity() <= minThreshold) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * 获取产品交易历史
     * @param productId 产品ID
     * @param startDate 开始日期（可选）
     * @param endDate 结束日期（可选）
     * @param transactionType 交易类型（可选）
     * @return 交易记录列表
     */
    public List<Transaction> getProductTransactions(String productId, LocalDate startDate, 
                                                   LocalDate endDate, String transactionType) {
        List<Transaction> result = new ArrayList<>();
        
        if (!products.containsKey(productId)) {
            return result;
        }
        
        for (Transaction transaction : transactions) {
            if (!transaction.getProductId().equals(productId)) {
                continue;
            }
            
            if (startDate != null && transaction.getTimestamp().toLocalDate().isBefore(startDate)) {
                continue;
            }
            
            if (endDate != null && transaction.getTimestamp().toLocalDate().isAfter(endDate)) {
                continue;
            }
            
            if (transactionType != null && !transaction.getTransactionType().equals(transactionType)) {
                continue;
            }
            
            result.add(transaction);
        }
        
        // 按时间戳降序排序
        result.sort((t1, t2) -> t2.getTimestamp().compareTo(t1.getTimestamp()));
        
        return result;
    }
    
    /**
     * 生成库存报告
     * @return 报告字符串
     */
    public String generateInventoryReport() {
        StringBuilder report = new StringBuilder();
        report.append("Inventory Report\n");
        report.append("================\n");
        report.append("Generated on: ").append(LocalDateTime.now()).append("\n\n");
        
        // 总体统计
        int totalProducts = products.size();
        int totalSuppliers = suppliers.size();
        int totalTransactions = transactions.size();
        
        double totalInventoryValue = 0;
        double totalInventoryCost = 0;
        int totalStockQuantity = 0;
        
        // 按类别统计
        Map<String, Integer> categoryProductCount = new HashMap<>();
        Map<String, Double> categoryValue = new HashMap<>();
        
        for (Product product : products.values()) {
            double productValue = product.getPrice() * product.getQuantity();
            double productCost = product.getCost() * product.getQuantity();
            
            totalInventoryValue += productValue;
            totalInventoryCost += productCost;
            totalStockQuantity += product.getQuantity();
            
            // 更新类别统计
            String category = product.getCategory();
            categoryProductCount.put(category, categoryProductCount.getOrDefault(category, 0) + 1);
            categoryValue.put(category, categoryValue.getOrDefault(category, 0.0) + productValue);
        }
        
        report.append("Summary\n");
        report.append("-------\n");
        report.append("Total Products: ").append(totalProducts).append("\n");
        report.append("Total Suppliers: ").append(totalSuppliers).append("\n");
        report.append("Total Transactions: ").append(totalTransactions).append("\n");
        report.append("Total Stock Quantity: ").append(totalStockQuantity).append("\n");
        report.append("Total Inventory Value: $").append(String.format("%.2f", totalInventoryValue)).append("\n");
        report.append("Total Inventory Cost: $").append(String.format("%.2f", totalInventoryCost)).append("\n");
        report.append("Potential Profit: $").append(String.format("%.2f", totalInventoryValue - totalInventoryCost)).append("\n\n");
        
        // 按类别统计
        report.append("Category Statistics\n");
        report.append("-------------------\n");
        for (Map.Entry<String, Integer> entry : categoryProductCount.entrySet()) {
            String category = entry.getKey();
            int count = entry.getValue();
            double value = categoryValue.get(category);
            
            report.append("Category: ").append(category).append("\n");
            report.append("  Product Count: ").append(count).append("\n");
            report.append("  Total Value: $").append(String.format("%.2f", value)).append("\n");
            report.append("  Percentage of Total: ").append(String.format("%.1f", (value / totalInventoryValue) * 100)).append("%\n\n");
        }
        
        // 低库存产品
        List<Product> lowStockProducts = getLowStockProducts(null);
        if (!lowStockProducts.isEmpty()) {
            report.append("Low Stock Products\n");
            report.append("-------------------\n");
            for (Product product : lowStockProducts) {
                report.append("ID: ").append(product.getProductId()).append(", ");
                report.append("Name: ").append(product.getName()).append(", ");
                report.append("Current Stock: ").append(product.getQuantity()).append(", ");
                report.append("Minimum Stock: ").append(product.getMinimumStock()).append("\n");
            }
        }
        
        // 保存报告
        String reportId = generateId("report");
        Report inventoryReport = new Report(reportId, "inventory", report.toString(), LocalDateTime.now());
        reports.add(inventoryReport);
        
        return report.toString();
    }
    
    /**
     * 搜索产品
     * @param keyword 搜索关键词
     * @return 匹配的产品列表
     */
    public List<Product> searchProducts(String keyword) {
        List<Product> result = new ArrayList<>();
        keyword = keyword.toLowerCase();
        
        for (Product product : products.values()) {
            if (product.getName().toLowerCase().contains(keyword) || 
                product.getDescription().toLowerCase().contains(keyword) ||
                product.getCategory().toLowerCase().contains(keyword) ||
                product.getProductId().toLowerCase().contains(keyword)) {
                result.add(product);
            }
        }
        
        return result;
    }
    
    /**
     * 生成补货提醒
     * @param productId 产品ID
     * @return 提醒ID
     */
    private String generateReorderAlert(String productId) {
        Product product = products.get(productId);
        if (product == null) {
            return null;
        }
        
        String alertId = generateId("alert");
        // 在实际应用中，这里可以向管理员发送通知或创建提醒记录
        
        return alertId;
    }
    
    /**
     * 查找采购订单
     * @param orderId 采购订单ID
     * @return 采购订单对象，不存在则返回null
     */
    private PurchaseOrder findPurchaseOrder(String orderId) {
        for (PurchaseOrder order : purchaseOrders) {
            if (order.getOrderId().equals(orderId)) {
                return order;
            }
        }
        return null;
    }
    
    /**
     * 查找销售订单
     * @param orderId 销售订单ID
     * @return 销售订单对象，不存在则返回null
     */
    private SalesOrder findSalesOrder(String orderId) {
        for (SalesOrder order : salesOrders) {
            if (order.getOrderId().equals(orderId)) {
                return order;
            }
        }
        return null;
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
 * 产品类
 */
class Product {
    private String productId;
    private String name;
    private String description;
    private String category;
    private double price;
    private double cost;
    private int quantity;
    private int minimumStock;
    private String supplierId;
    private LocalDateTime lastUpdated;
    
    public Product(String productId, String name, String description, String category, 
                   double price, double cost, int quantity, int minimumStock, String supplierId) {
        this.productId = productId;
        this.name = name;
        this.description = description;
        this.category = category;
        this.price = price;
        this.cost = cost;
        this.quantity = quantity;
        this.minimumStock = minimumStock;
        this.supplierId = supplierId;
        this.lastUpdated = LocalDateTime.now();
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
    
    public String getCategory() {
        return category;
    }
    
    public double getPrice() {
        return price;
    }
    
    public double getCost() {
        return cost;
    }
    
    public int getQuantity() {
        return quantity;
    }
    
    public int getMinimumStock() {
        return minimumStock;
    }
    
    public String getSupplierId() {
        return supplierId;
    }
    
    public LocalDateTime getLastUpdated() {
        return lastUpdated;
    }
    
    public void setQuantity(int quantity) {
        this.quantity = quantity;
    }
    
    public void setLastUpdated(LocalDateTime lastUpdated) {
        this.lastUpdated = lastUpdated;
    }
}

/**
 * 供应商类
 */
class Supplier {
    private String supplierId;
    private String name;
    private String contactPerson;
    private String email;
    private String phone;
    private String address;
    
    public Supplier(String supplierId, String name, String contactPerson, 
                   String email, String phone, String address) {
        this.supplierId = supplierId;
        this.name = name;
        this.contactPerson = contactPerson;
        this.email = email;
        this.phone = phone;
        this.address = address;
    }
    
    // Getters and setters
    public String getSupplierId() {
        return supplierId;
    }
    
    public String getName() {
        return name;
    }
    
    public String getContactPerson() {
        return contactPerson;
    }
    
    public String getEmail() {
        return email;
    }
    
    public String getPhone() {
        return phone;
    }
    
    public String getAddress() {
        return address;
    }
}

/**
 * 交易记录类
 */
class Transaction {
    private String transactionId;
    private String productId;
    private int quantityChange;
    private String transactionType;
    private String notes;
    private LocalDateTime timestamp;
    
    public Transaction(String transactionId, String productId, int quantityChange, 
                      String transactionType, String notes, LocalDateTime timestamp) {
        this.transactionId = transactionId;
        this.productId = productId;
        this.quantityChange = quantityChange;
        this.transactionType = transactionType;
        this.notes = notes;
        this.timestamp = timestamp;
    }
    
    // Getters and setters
    public String getTransactionId() {
        return transactionId;
    }
    
    public String getProductId() {
        return productId;
    }
    
    public int getQuantityChange() {
        return quantityChange;
    }
    
    public String getTransactionType() {
        return transactionType;
    }
    
    public String getNotes() {
        return notes;
    }
    
    public LocalDateTime getTimestamp() {
        return timestamp;
    }
}

/**
 * 订单项目类
 */
class OrderItem {
    private String productId;
    private int quantity;
    
    public OrderItem(String productId, int quantity) {
        this.productId = productId;
        this.quantity = quantity;
    }
    
    // Getters and setters
    public String getProductId() {
        return productId;
    }
    
    public int getQuantity() {
        return quantity;
    }
}

/**
 * 采购订单类
 */
class PurchaseOrder {
    private String orderId;
    private String supplierId;
    private List<OrderItem> items;
    private String status;
    private LocalDate orderDate;
    private LocalDate expectedDeliveryDate;
    private LocalDate receivedDate;
    private String notes;
    private double totalAmount;
    
    public PurchaseOrder(String orderId, String supplierId, List<OrderItem> items, 
                         String status, LocalDate orderDate, LocalDate expectedDeliveryDate, 
                         double totalAmount) {
        this.orderId = orderId;
        this.supplierId = supplierId;
        this.items = new ArrayList<>(items);
        this.status = status;
        this.orderDate = orderDate;
        this.expectedDeliveryDate = expectedDeliveryDate;
        this.totalAmount = totalAmount;
    }
    
    // Getters and setters
    public String getOrderId() {
        return orderId;
    }
    
    public String getSupplierId() {
        return supplierId;
    }
    
    public List<OrderItem> getItems() {
        return new ArrayList<>(items);
    }
    
    public String getStatus() {
        return status;
    }
    
    public LocalDate getOrderDate() {
        return orderDate;
    }
    
    public LocalDate getExpectedDeliveryDate() {
        return expectedDeliveryDate;
    }
    
    public LocalDate getReceivedDate() {
        return receivedDate;
    }
    
    public String getNotes() {
        return notes;
    }
    
    public double getTotalAmount() {
        return totalAmount;
    }
    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public void setReceivedDate(LocalDate receivedDate) {
        this.receivedDate = receivedDate;
    }
    
    public void setNotes(String notes) {
        this.notes = notes;
    }
}

/**
 * 销售订单类
 */
class SalesOrder {
    private String orderId;
    private String customerId;
    private List<OrderItem> items;
    private String status;
    private LocalDate orderDate;
    private LocalDate shippedDate;
    private String shippingAddress;
    private String trackingNumber;
    private String notes;
    private double totalAmount;
    
    public SalesOrder(String orderId, String customerId, List<OrderItem> items, 
                      String status, LocalDate orderDate, String shippingAddress, 
                      double totalAmount) {
        this.orderId = orderId;
        this.customerId = customerId;
        this.items = new ArrayList<>(items);
        this.status = status;
        this.orderDate = orderDate;
        this.shippingAddress = shippingAddress;
        this.totalAmount = totalAmount;
    }
    
    // Getters and setters
    public String getOrderId() {
        return orderId;
    }
    
    public String getCustomerId() {
        return customerId;
    }
    
    public List<OrderItem> getItems() {
        return new ArrayList<>(items);
    }
    
    public String getStatus() {
        return status;
    }
    
    public LocalDate getOrderDate() {
        return orderDate;
    }
    
    public LocalDate getShippedDate() {
        return shippedDate;
    }
    
    public String getShippingAddress() {
        return shippingAddress;
    }
    
    public String getTrackingNumber() {
        return trackingNumber;
    }
    
    public String getNotes() {
        return notes;
    }
    
    public double getTotalAmount() {
        return totalAmount;
    }
    
    public void setStatus(String status) {
        this.status = status;
    }
    
    public void setShippedDate(LocalDate shippedDate) {
        this.shippedDate = shippedDate;
    }
    
    public void setTrackingNumber(String trackingNumber) {
        this.trackingNumber = trackingNumber;
    }
    
    public void setNotes(String notes) {
        this.notes = notes;
    }
}

/**
 * 报告类
 */
class Report {
    private String reportId;
    private String reportType;
    private String content;
    private LocalDateTime generatedAt;
    
    public Report(String reportId, String reportType, String content, LocalDateTime generatedAt) {
        this.reportId = reportId;
        this.reportType = reportType;
        this.content = content;
        this.generatedAt = generatedAt;
    }
    
    // Getters and setters
    public String getReportId() {
        return reportId;
    }
    
    public String getReportType() {
        return reportType;
    }
    
    public String getContent() {
        return content;
    }
    
    public LocalDateTime getGeneratedAt() {
        return generatedAt;
    }
}