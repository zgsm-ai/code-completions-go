package main

import (
	"fmt"
	"math/rand"
	"time"
)

// Inventory Management System
// This Go program implements an inventory management system with products, categories, and stock tracking

// Product represents an item in inventory
type Product struct {
	ID          string    `json:"id"`
	Name        string    `json:"name"`
	Description string    `json:"description"`
	Category    string    `json:"category"`
	Price       float64   `json:"price"`
	Quantity    int       `json:"quantity"`
	MinStock    int       `json:"min_stock"`
	Supplier    string    `json:"supplier"`
	SKU         string    `json:"sku"`
	CreatedAt   time.Time `json:"created_at"`
	UpdatedAt   time.Time `json:"updated_at"`
}

// Category represents a product category
type Category struct {
	ID          string   `json:"id"`
	Name        string   `json:"name"`
	Description string   `json:"description"`
	Products    []string `json:"products"` // Product IDs
}

// Supplier represents a product supplier
type Supplier struct {
	ID      string `json:"id"`
	Name    string `json:"name"`
	Contact string `json:"contact"`
	Email   string `json:"email"`
	Phone   string `json:"phone"`
	Address string `json:"address"`
}

// StockMovement represents a stock movement
type StockMovement struct {
	ID          string    `json:"id"`
	ProductID   string    `json:"product_id"`
	MovementType string    `json:"movement_type"` // "in" for receiving stock, "out" for selling stock
	Quantity    int       `json:"quantity"`
	Reason      string    `json:"reason"`
	Timestamp   time.Time `json:"timestamp"`
	Reference   string    `json:"reference"` // Purchase order number, etc.
}

// Inventory represents the inventory system
type Inventory struct {
	Products       map[string]*Product      `json:"products"`
	Categories     map[string]*Category     `json:"categories"`
	Suppliers      map[string]*Supplier     `json:"suppliers"`
	StockMovements map[string]*StockMovement `json:"stock_movements"`
}

// NewInventory creates a new inventory instance
func NewInventory() *Inventory {
	return &Inventory{
		Products:       make(map[string]*Product),
		Categories:     make(map[string]*Category),
		Suppliers:      make(map[string]*Supplier),
		StockMovements: make(map[string]*StockMovement),
	}
}

// generateID generates a random ID
func generateID() string {
	rand.Seed(time.Now().UnixNano())
	return fmt.Sprintf("ID%d", rand.Intn(10000))
}

// AddProduct adds a new product to inventory
func (inv *Inventory) AddProduct(name, description, category, supplier, sku string, price float64, quantity, minStock int) *Product {
	id := generateID()
	product := &Product{
		ID:          id,
		Name:        name,
		Description: description,
		Category:    category,
		Price:       price,
		Quantity:    quantity,
		MinStock:    minStock,
		Supplier:    supplier,
		SKU:         sku,
		CreatedAt:   time.Now(),
		UpdatedAt:   time.Now(),
	}
	
	inv.Products[id] = product
	
	// Add to category if exists
	if cat, ok := inv.Categories[category]; ok {
		cat.Products = append(cat.Products, id)
	}
	
	// Create stock movement for initial stock
	inv.CreateStockMovement(id, "in", quantity, "Initial stock", "INIT")
	
	return product
}

// GetProduct retrieves a product by ID
func (inv *Inventory) GetProduct(id string) *Product {
	return inv.Products[id]
}

// UpdateProduct updates a product
func (inv *Inventory) UpdateProduct(id, name, description, category, supplier, sku string, price float64, quantity, minStock int) bool {
	if product, ok := inv.Products[id]; ok {
		product.Name = name
		product.Description = description
		product.Category = category
		product.Supplier = supplier
		product.SKU = sku
		product.Price = price
		product.Quantity = quantity
		product.MinStock = minStock
		product.UpdatedAt = time.Now()
		return true
	}
	return false
}

// DeleteProduct removes a product from inventory
func (inv *Inventory) DeleteProduct(id string) bool {
	if _, ok := inv.Products[id]; ok {
		delete(inv.Products, id)
		
		// Remove from category
		for _, cat := range inv.Categories {
			for i, productID := range cat.Products {
				if productID == id {
					cat.Products = append(cat.Products[:i], cat.Products[i+1:]...)
					break
				}
			}
		}
		
		return true
	}
	return false
}

// AddCategory adds a new category
func (inv *Inventory) AddCategory(name, description string) *Category {
	id := generateID()
	category := &Category{
		ID:          id,
		Name:        name,
		Description: description,
		Products:    make([]string, 0),
	}
	
	inv.Categories[id] = category
	return category
}

// GetCategory retrieves a category by ID
func (inv *Inventory) GetCategory(id string) *Category {
	return inv.Categories[id]
}

// AddSupplier adds a new supplier
func (inv *Inventory) AddSupplier(name, contact, email, phone, address string) *Supplier {
	id := generateID()
	supplier := &Supplier{
		ID:      id,
		Name:    name,
		Contact: contact,
		Email:   email,
		Phone:   phone,
		Address: address,
	}
	
	inv.Suppliers[id] = supplier
	return supplier
}

// GetSupplier retrieves a supplier by ID
func (inv *Inventory) GetSupplier(id string) *Supplier {
	return inv.Suppliers[id]
}

// CreateStockMovement creates a stock movement
func (inv *Inventory) CreateStockMovement(productID, movementType string, quantity int, reason, reference string) *StockMovement {
	id := generateID()
	movement := &StockMovement{
		ID:          id,
		ProductID:   productID,
		MovementType: movementType,
		Quantity:    quantity,
		Reason:      reason,
		Timestamp:   time.Now(),
		Reference:   reference,
	}
	
	inv.StockMovements[id] = movement
	
	// Update product quantity
	if product, ok := inv.Products[productID]; ok {
		if movementType == "in" {
			product.Quantity += quantity
		} else if movementType == "out" {
			product.Quantity -= quantity
		}
		product.UpdatedAt = time.Now()
	}
	
	return movement
}

// GetStockMovementsByProduct retrieves all stock movements for a product
func (inv *Inventory) GetStockMovementsByProduct(productID string) []*StockMovement {
	var movements []*StockMovement
	for _, movement := range inv.StockMovements {
		if movement.ProductID == productID {
			movements = append(movements, movement)
		}
	}
	return movements
}

// GetLowStockProducts retrieves products with low stock
func (inv *Inventory) GetLowStockProducts() []*Product {
	var products []*Product
	for _, product := range inv.Products {
		if product.Quantity <= product.MinStock {
			products = append(products, product)
		}
	}
	return products
}

// GetProductsByCategory retrieves products in a specific category
func (inv *Inventory) GetProductsByCategory(categoryID string) []*Product {
	var products []*Product
	
	category, ok := inv.Categories[categoryID]
	if !ok {
		return products
	}
	
	for _, productID := range category.Products {
		if product, ok := inv.Products[productID]; ok {
			products = append(products, product)
		}
	}
	
	return products
}

// SearchProducts searches products by name or description
func (inv *Inventory) SearchProducts(query string) []*Product {
	var products []*Product
	queryLower := toLower(query)
	
	for _, product := range inv.Products {
		nameLower := toLower(product.Name)
		descLower := toLower(product.Description)
		
		if contains(nameLower, queryLower) || contains(descLower, queryLower) {
			products = append(products, product)
		}
	}
	
	return products
}

// toLower converts a string to lowercase
func toLower(s string) string {
	result := make([]byte, len(s))
	for i := 0; i < len(s); i++ {
		if s[i] >= 'A' && s[i] <= 'Z' {
			result[i] = s[i] + 32
		} else {
			result[i] = s[i]
		}
	}
	return string(result)
}

// contains checks if a string contains a substring
func contains(s, substr string) bool {
	return len(s) >= len(substr) && (s == substr || 
		(len(s) > len(substr) && (s[:len(substr)] == substr || 
		s[len(s)-len(substr):] == substr || 
		findSubstring(s, substr))))
}

// findSubstring checks if substr is in s
func findSubstring(s, substr string) bool {
	for i := 0; i <= len(s)-len(substr); i++ {
		match := true
		for j := 0; j < len(substr); j++ {
			if s[i+j] != substr[j] {
				match = false
				break
			}
		}
		if match {
			return true
		}
	}
	return false
}

// ReceiveStock adds stock to a product
func (inv *Inventory) ReceiveStock(productID string, quantity int, reason, reference string) bool {
	if _, ok := inv.Products[productID]; ok {
		inv.CreateStockMovement(productID, "in", quantity, reason, reference)
		return true
	}
	return false
}

// SellStock removes stock from a product
func (inv *Inventory) SellStock(productID string, quantity int, reason, reference string) bool {
	if product, ok := inv.Products[productID]; ok && product.Quantity >= quantity {
		inv.CreateStockMovement(productID, "out", quantity, reason, reference)
		return true
	}
	return false
}

// TransferStock moves stock between products
func (inv *Inventory) TransferStock(fromProductID, toProductID string, quantity int, reason string) bool {
	fromProduct, fromOk := inv.Products[fromProductID]
	toProduct, toOk := inv.Products[toProductID]
	
	if !fromOk || !toOk || fromProduct.Quantity < quantity {
		return false
	}
	
	// Remove stock from source
	inv.CreateStockMovement(fromProductID, "out", quantity, reason, "TRANSFER-"+toProductID)
	
	// Add stock to destination
	inv.CreateStockMovement(toProductID, "in", quantity, reason, "TRANSFER-"+fromProductID)
	
	return true
}

// CalculateInventoryValue calculates total value of inventory
func (inv *Inventory) CalculateInventoryValue() float64 {
	var totalValue float64
	for _, product := range inv.Products {
		totalValue += product.Price * float64(product.Quantity)
	}
	return totalValue
}

// GenerateInventoryReport generates a comprehensive inventory report
func (inv *Inventory) GenerateInventoryReport() {
	fmt.Printf("=== Inventory Report ===\n")
	fmt.Printf("Generated at: %s\n\n", time.Now().Format("2006-01-02 15:04:05"))
	
	// Summary statistics
	totalProducts := len(inv.Products)
	totalCategories := len(inv.Categories)
	totalSuppliers := len(inv.Suppliers)
	totalStockValue := inv.CalculateInventoryValue()
	
	fmt.Printf("Summary:\n")
	fmt.Printf("Total Products: %d\n", totalProducts)
	fmt.Printf("Total Categories: %d\n", totalCategories)
	fmt.Printf("Total Suppliers: %d\n", totalSuppliers)
	fmt.Printf("Total Stock Value: $%.2f\n\n", totalStockValue)
	
	// Low stock products
	lowStockProducts := inv.GetLowStockProducts()
	if len(lowStockProducts) > 0 {
		fmt.Printf("Low Stock Products:\n")
		for _, product := range lowStockProducts {
			fmt.Printf("- %s: %d (min: %d)\n", product.Name, product.Quantity, product.MinStock)
		}
		fmt.Printf("\n")
	}
	
	// Category breakdown
	fmt.Printf("Category Breakdown:\n")
	for _, category := range inv.Categories {
		productCount := len(category.Products)
		var categoryValue float64
		
		for _, productID := range category.Products {
			if product, ok := inv.Products[productID]; ok {
				categoryValue += product.Price * float64(product.Quantity)
			}
		}
		
		fmt.Printf("- %s: %d products, $%.2f value\n", category.Name, productCount, categoryValue)
	}
}

// DisplayProduct displays detailed product information
func DisplayProduct(product *Product, showHistory bool, inv *Inventory) {
	fmt.Printf("Product Details:\n")
	fmt.Printf("ID: %s\n", product.ID)
	fmt.Printf("Name: %s\n", product.Name)
	fmt.Printf("Description: %s\n", product.Description)
	fmt.Printf("Category: %s\n", product.Category)
	fmt.Printf("Price: $%.2f\n", product.Price)
	fmt.Printf("Quantity: %d\n", product.Quantity)
	fmt.Printf("Min Stock: %d\n", product.MinStock)
	fmt.Printf("Supplier: %s\n", product.Supplier)
	fmt.Printf("SKU: %s\n", product.SKU)
	fmt.Printf("Created: %s\n", product.CreatedAt.Format("2006-01-02 15:04:05"))
	fmt.Printf("Updated: %s\n", product.UpdatedAt.Format("2006-01-02 15:04:05"))
	
	if showHistory {
		movements := inv.GetStockMovementsByProduct(product.ID)
		if len(movements) > 0 {
			fmt.Printf("\nStock Movement History:\n")
			for _, movement := range movements {
				fmt.Printf("- %s %d on %s (%s) - %s\n", 
					movement.MovementType, movement.Quantity,
					movement.Timestamp.Format("2006-01-02"), movement.Reason, movement.Reference)
			}
		}
	}
	
	if product.Quantity <= product.MinStock {
		fmt.Printf("\nWARNING: Low stock level!\n")
	}
}

// GenerateSampleInventory creates a sample inventory with products, categories, and suppliers
func GenerateSampleInventory() *Inventory {
	inv := NewInventory()
	
	// Create categories
	electronics := inv.AddCategory("Electronics", "Electronic devices and accessories")
	clothing := inv.AddCategory("Clothing", "Apparel and clothing items")
	office := inv.AddCategory("Office", "Office supplies and equipment")
	
	// Create suppliers
	techCorp := inv.AddSupplier("Tech Corp", "John Smith", "john@techcorp.com", "555-0101", "123 Tech Street, Tech City")
	fashionInc := inv.AddSupplier("Fashion Inc", "Jane Doe", "jane@fashion.com", "555-0102", "456 Fashion Ave, Style City")
	officeSupplies := inv.AddSupplier("Office Supplies", "Bob Johnson", "bob@office.com", "555-0103", "789 Office Blvd, Work City")
	
	// Add products
	laptop := inv.AddProduct("Laptop Pro", "High-performance laptop with 16GB RAM and 512GB SSD", 
		electronics.ID, techCorp.ID, "LP-12345", 1299.99, 10, 5)
	
	mouse := inv.AddProduct("Wireless Mouse", "Ergonomic wireless mouse with precision tracking", 
		electronics.ID, techCorp.ID, "WM-67890", 29.99, 25, 10)
	
	keyboard := inv.AddProduct("Mechanical Keyboard", "RGB backlit mechanical keyboard", 
		electronics.ID, techCorp.ID, "MK-11223", 79.99, 15, 5)
	
	tshirt := inv.AddProduct("Cotton T-Shirt", "100% cotton t-shirt with company logo", 
		clothing.ID, fashionInc.ID, "TS-44556", 19.99, 50, 20)
	
	jeans := inv.AddProduct("Denim Jeans", "Classic fit denim jeans", 
		clothing.ID, fashionInc.ID, "DJ-77889", 49.99, 30, 15)
		
	paper := inv.AddProduct("A4 Paper", "500 sheets of A4 office paper", 
		office.ID, officeSupplies.ID, "AP-99001", 12.99, 100, 25)
		
	pen := inv.AddProduct("Ballpoint Pen", "Pack of 12 blue ballpoint pens", 
		office.ID, officeSupplies.ID, "BP-22334", 5.99, 200, 50)
	
	// Simulate some stock movements
	inv.ReceiveStock(laptop.ID, 5, "Additional stock", "PO-1001")
	inv.SellStock(laptop.ID, 2, "Customer order", "SO-2001")
	inv.SellStock(mouse.ID, 10, "Customer order", "SO-2002")
	inv.ReceiveStock(tshirt.ID, 20, "Restock", "PO-1002")
	inv.SellStock(paper.ID, 50, "Customer order", "SO-2003")
	inv.TransferStock(mouse.ID, keyboard.ID, 5, "Inventory adjustment")
	
	// Create some low stock situations
	inv.SellStock(keyboard.ID, 12, "Customer order", "SO-2004")
	inv.SellStock(jeans.ID, 20, "Customer order", "SO-2005")
	
	return inv
}

func main() {
	// Create a sample inventory
	inv := GenerateSampleInventory()
	
	// Generate inventory report
	inv.GenerateInventoryReport()
	
	// Display detailed information for a specific product
	fmt.Printf("\n=== Detailed Product Information ===\n")
	laptop := inv.GetProduct("ID") // This will likely be nil, so let's get the first product
	if len(inv.Products) > 0 {
		var firstProductID string
		for id := range inv.Products {
			firstProductID = id
			break
		}
		
		product := inv.GetProduct(firstProductID)
		<｜fim▁hole｜>DisplayProduct(product, true, inv)
	}
	
	// Search for products
	fmt.Printf("\n=== Search Results ===\n")
	searchResults := inv.SearchProducts("keyboard")
	fmt.Printf("Products matching 'keyboard':\n")
	for _, product := range searchResults {
		fmt.Printf("- %s (%.2f)\n", product.Name, product.Price)
	}
	
	// Get products by category
	fmt.Printf("\n=== Products by Category ===\n")
	if len(inv.Categories) > 0 {
		var firstCategoryID string
		for id := range inv.Categories {
			firstCategoryID = id
			break
		}
		
		category := inv.GetCategory(firstCategoryID)
		categoryProducts := inv.GetProductsByCategory(firstCategoryID)
		fmt.Printf("%s category (%d products):\n", category.Name, len(categoryProducts))
		for _, product := range categoryProducts {
			fmt.Printf("- %s: %d in stock\n", product.Name, product.Quantity)
		}
	}
	
	// Display low stock products
	fmt.Printf("\n=== Low Stock Alert ===\n")
	lowStockProducts := inv.GetLowStockProducts()
	if len(lowStockProducts) > 0 {
		fmt.Printf("Products that need restocking:\n")
		for _, product := range lowStockProducts {
			fmt.Printf("- %s: %d in stock (min: %d)\n", product.Name, product.Quantity, product.MinStock)
		}
	} else {
		fmt.Printf("All products have adequate stock levels.\n")
	}
	
	// Calculate total inventory value
	totalValue := inv.CalculateInventoryValue()
	fmt.Printf("\n=== Total Inventory Value ===\n")
	fmt.Printf("Total value of all inventory: $%.2f\n", totalValue)
}