#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <time.h>

// =============================================================================
// DATABASE SCHEMA DEFINITIONS
// =============================================================================

// User table structure
typedef struct {
    int id;
    char username[50];
    char email[100];
    char password_hash[64];
    int created_at;
    int is_active;
} User;

// Product table structure
typedef struct {
    int id;
    char name[100];
    char description[500];
    double price;
    int stock_quantity;
    int category_id;
    int created_at;
} Product;

// Order table structure
typedef struct {
    int id;
    int user_id;
    int product_id;
    int quantity;
    double unit_price;
    double total_price;
    char status[20];
    int created_at;
} Order;

// =============================================================================
// DATABASE CONNECTION MANAGEMENT
// =============================================================================

typedef struct {
    sqlite3* db;
    char* filename;
    int is_connected;
} DatabaseConnection;

DatabaseConnection db_conn;

// Initialize database connection
int initDatabase(const char* filename) {
    int result = sqlite3_open(filename, &db_conn.db);
    
    if (result != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    db_conn.filename = strdup(filename);
    db_conn.is_connected = 1;
    
    printf("Database connected successfully: %s\n", filename);
    return 1;
}

// Close database connection
void closeDatabase() {
    if (db_conn.is_connected && db_conn.db) {
        sqlite3_close(db_conn.db);
        db_conn.is_connected = 0;
        free(db_conn.filename);
        printf("Database connection closed\n");
    }
}

// Execute SQL statement
int executeSQL(const char* sql) {
    char* err_msg = 0;
    int result = sqlite3_exec(db_conn.db, sql, 0, 0, &err_msg);
    
    if (result != SQLITE_OK) {
        printf("SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    
    return 1;
}

// =============================================================================
// TABLE CREATION
// =============================================================================

int createTables() {
    const char* create_users_table = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT NOT NULL UNIQUE,"
        "email TEXT NOT NULL UNIQUE,"
        "password_hash TEXT NOT NULL,"
        "created_at INTEGER NOT NULL,"
        "is_active INTEGER DEFAULT 1"
        ");";
    
    const char* create_products_table = 
        "CREATE TABLE IF NOT EXISTS products ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "description TEXT,"
        "price REAL NOT NULL,"
        "stock_quantity INTEGER DEFAULT 0,"
        "category_id INTEGER,"
        "created_at INTEGER NOT NULL,"
        "FOREIGN KEY (category_id) REFERENCES categories(id)"
        ");";
    
    const char* create_categories_table = 
        "CREATE TABLE IF NOT EXISTS categories ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL UNIQUE,"
        "description TEXT"
        ");";
    
    const char* create_orders_table = 
        "CREATE TABLE IF NOT EXISTS orders ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "user_id INTEGER NOT NULL,"
        "product_id INTEGER NOT NULL,"
        "quantity INTEGER NOT NULL,"
        "unit_price REAL NOT NULL,"
        "total_price REAL NOT NULL,"
        "status TEXT DEFAULT 'pending',"
        "created_at INTEGER NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(id),"
        "FOREIGN KEY (product_id) REFERENCES products(id)"
        ");";
    
    // Execute table creation statements
    if (!executeSQL(create_users_table)) return 0;
    if (!executeSQL(create_categories_table)) return 0;
    if (!executeSQL(create_products_table)) return 0;
    if (!executeSQL(create_orders_table)) return 0;
    
    printf("All tables created successfully\n");
    return 1;
}

// =============================================================================
// USER MANAGEMENT
// =============================================================================

// Insert new user
int insertUser(const char* username, const char* email, const char* password_hash) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO users (username, email, password_hash, created_at) VALUES (?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    time_t now = time(NULL);
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, password_hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, (int)now);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to insert user: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("User '%s' inserted successfully\n", username);
    return sqlite3_last_insert_rowid(db_conn.db);
}

// Get user by ID
User getUserById(int user_id) {
    User user = {0};
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, username, email, password_hash, created_at, is_active FROM users WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return user;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        user.id = sqlite3_column_int(stmt, 0);
        strcpy(user.username, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(user.email, (const char*)sqlite3_column_text(stmt, 2));
        strcpy(user.password_hash, (const char*)sqlite3_column_text(stmt, 3));
        user.created_at = sqlite3_column_int(stmt, 4);
        user.is_active = sqlite3_column_int(stmt, 5);
    }
    
    sqlite3_finalize(stmt);
    return user;
}

// Update user information
int updateUser(int user_id, const char* username, const char* email) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE users SET username = ?, email = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to update user: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("User %d updated successfully\n", user_id);
    return 1;
}

// Delete user
int deleteUser(int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM users WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to delete user: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("User %d deleted successfully\n", user_id);
    return 1;
}

// =============================================================================
// PRODUCT MANAGEMENT
// =============================================================================

// Insert new product
int insertProduct(const char* name, const char* description, double price, int stock_quantity, int category_id) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO products (name, description, price, stock_quantity, category_id, created_at) VALUES (?, ?, ?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    time_t now = time(NULL);
    
    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, description, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);
    sqlite3_bind_int(stmt, 4, stock_quantity);
    sqlite3_bind_int(stmt, 5, category_id);
    sqlite3_bind_int(stmt, 6, (int)now);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to insert product: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("Product '%s' inserted successfully\n", name);
    return sqlite3_last_insert_rowid(db_conn.db);
}

// Get products by category
void getProductsByCategory(int category_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, description, price, stock_quantity FROM products WHERE category_id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, category_id);
    
    printf("Products in category %d:\n", category_id);
    printf("%-5s %-20s %-30s %-10s %-10s\n", "ID", "Name", "Description", "Price", "Stock");
    printf("------------------------------------------------------------\n");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* name = (const char*)sqlite3_column_text(stmt, 1);
        const char* description = (const char*)sqlite3_column_text(stmt, 2);
        double price = sqlite3_column_double(stmt, 3);
        int stock = sqlite3_column_int(stmt, 4);
        
        printf("%-5d %-20s %-30s $%-9.2f %-10d\n", id, name, description, price, stock);
    }
    
    sqlite3_finalize(stmt);
}

// Update product stock
int updateProductStock(int product_id, int new_quantity) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE products SET stock_quantity = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, new_quantity);
    sqlite3_bind_int(stmt, 2, product_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to update product stock: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("Product %d stock updated to %d\n", product_id, new_quantity);
    return 1;
}

// =============================================================================
// ORDER MANAGEMENT
// =============================================================================

// Create new order
int createOrder(int user_id, int product_id, int quantity, double unit_price) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO orders (user_id, product_id, quantity, unit_price, total_price, created_at) VALUES (?, ?, ?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    time_t now = time(NULL);
    double total_price = quantity * unit_price;
    
    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, product_id);
    sqlite3_bind_int(stmt, 3, quantity);
    sqlite3_bind_double(stmt, 4, unit_price);
    sqlite3_bind_double(stmt, 5, total_price);
    sqlite3_bind_int(stmt, 6, (int)now);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to create order: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("Order created successfully\n");
    return sqlite3_last_insert_rowid(db_conn.db);
}

// Get user orders
void getUserOrders(int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT o.id, p.name, o.quantity, o.unit_price, o.total_price, o.status, o.created_at "
                      "FROM orders o JOIN products p ON o.product_id = p.id WHERE o.user_id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    printf("Orders for user %d:\n", user_id);
    printf("%-5s %-20s %-8s %-10s %-10s %-10s %-20s\n", "ID", "Product", "Quantity", "Unit Price", "Total", "Status", "Date");
    printf("--------------------------------------------------------------------------------\n");
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* product_name = (const char*)sqlite3_column_text(stmt, 1);
        int quantity = sqlite3_column_int(stmt, 2);
        double unit_price = sqlite3_column_double(stmt, 3);
        double total_price = sqlite3_column_double(stmt, 4);
        const char* status = (const char*)sqlite3_column_text(stmt, 5);
        int created_at = sqlite3_column_int(stmt, 6);
        
        char date_str[20];
        strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime((time_t*)&created_at));
        
        printf("%-5d %-20s %-8d $%-9.2f $%-9.2f %-10s %-20s\n", 
               id, product_name, quantity, unit_price, total_price, status, date_str);
    }
    
    sqlite3_finalize(stmt);
}

// Update order status
int updateOrderStatus(int order_id, const char* new_status) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE orders SET status = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, new_status, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, order_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    if (result != SQLITE_DONE) {
        printf("Failed to update order status: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    printf("Order %d status updated to '%s'\n", order_id, new_status);
    return 1;
}

// =============================================================================
// ADVANCED DATABASE OPERATIONS
// =============================================================================

// Transaction example
int processOrderWithTransaction(int user_id, int product_id, int quantity) {
    // Begin transaction
    if (!executeSQL("BEGIN TRANSACTION")) {
        return 0;
    }
    
    // Check product availability
    sqlite3_stmt* stmt;
    const char* sql = "SELECT stock_quantity, price FROM products WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        executeSQL("ROLLBACK");
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, product_id);
    
    int stock_quantity = 0;
    double price = 0.0;
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        stock_quantity = sqlite3_column_int(stmt, 0);
        price = sqlite3_column_double(stmt, 1);
    }
    
    sqlite3_finalize(stmt);
    
    // Check if enough stock
    if (stock_quantity < quantity) {
        printf("Insufficient stock\n");
        executeSQL("ROLLBACK");
        return 0;
    }
    
    // Update stock
    if (!updateProductStock(product_id, stock_quantity - quantity)) {
        executeSQL("ROLLBACK");
        return 0;
    }
    
    // Create order
    if (!createOrder(user_id, product_id, quantity, price)) {
        executeSQL("ROLLBACK");
        return 0;
    }
    
    // Commit transaction
    if (!executeSQL("COMMIT")) {
        return 0;
    }
    
    printf("Order processed successfully\n");
    return 1;
}

// Reporting functions
void generateSalesReport() {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(*) as total_orders, "
                      "SUM(total_price) as total_revenue, "
                      "COUNT(DISTINCT user_id) as unique_customers "
                      "FROM orders WHERE status = 'completed'";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return;
    }
    
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int total_orders = sqlite3_column_int(stmt, 0);
        double total_revenue = sqlite3_column_double(stmt, 1);
        int unique_customers = sqlite3_column_int(stmt, 2);
        
        printf("=== SALES REPORT ===\n");
        printf("Total Orders: %d\n", total_orders);
        printf("Total Revenue: $%.2f\n", total_revenue);
        printf("Unique Customers: %d\n", unique_customers);
    }
    
    sqlite3_finalize(stmt);
}

void generateInventoryReport() {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT p.name, p.stock_quantity, p.price, "
                      "(p.stock_quantity * p.price) as total_value "
                      "FROM products p ORDER BY total_value DESC";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement: %s\n", sqlite3_errmsg(db_conn.db));
        return;
    }
    
    printf("=== INVENTORY REPORT ===\n");
    printf("%-20s %-10s %-10s %-12s\n", "Product", "Stock", "Price", "Total Value");
    printf("------------------------------------------------\n");
    
    double total_inventory_value = 0;
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name = (const char*)sqlite3_column_text(stmt, 0);
        int stock = sqlite3_column_int(stmt, 1);
        double price = sqlite3_column_double(stmt, 2);
        double total_value = sqlite3_column_double(stmt, 3);
        
        printf("%-20s %-10d $%-9.2f $%-11.2f\n", name, stock, price, total_value);
        total_inventory_value += total_value;
    }
    
    printf("------------------------------------------------\n");
    printf("Total Inventory Value: $%.2f\n", total_inventory_value);
    
    sqlite3_finalize(stmt);
}

// =============================================================================
// DEMONSTRATION FUNCTIONS
// =============================================================================

void demonstrateUserManagement() {
    printf("=== USER MANAGEMENT DEMO ===\n");
    
    // Insert users
    int user1_id = insertUser("john_doe", "john@example.com", "hashed_password_1");
    int user2_id = insertUser("jane_smith", "jane@example.com", "hashed_password_2");
    
    // Get user by ID
    User user = getUserById(user1_id);
    printf("Retrieved user: %s, %s\n", user.username, user.email);
    
    // Update user
    updateUser(user1_id, "john_doe_updated", "john.updated@example.com");
    
    // Delete user
    deleteUser(user2_id);
    
    printf("\n");
}

void demonstrateProductManagement() {
    printf("=== PRODUCT MANAGEMENT DEMO ===\n");
    
    // Insert categories first
    executeSQL("INSERT INTO categories (name, description) VALUES ('Electronics', 'Electronic devices')");
    executeSQL("INSERT INTO categories (name, description) VALUES ('Books', 'Books and magazines')");
    
    // Insert products
    int product1_id = insertProduct("Laptop", "High-performance laptop", 999.99, 50, 1);
    int product2_id = insertProduct("Programming Book", "C programming guide", 29.99, 100, 2);
    
    // Get products by category
    getProductsByCategory(1);
    
    // Update stock
    updateProductStock(product1_id, 45);
    
    printf("\n");
}

void demonstrateOrderManagement() {
    printf("=== ORDER MANAGEMENT DEMO ===\n");
    
    // Create orders
    int order1_id = createOrder(1, 1, 2, 999.99);
    int order2_id = createOrder(1, 2, 3, 29.99);
    
    // Get user orders
    getUserOrders(1);
    
    // Update order status
    updateOrderStatus(order1_id, "shipped");
    updateOrderStatus(order2_id, "delivered");
    
    printf("\n");
}

void demonstrateTransactions() {
    printf("=== TRANSACTION DEMO ===\n");
    
    // Process order with transaction
    processOrderWithTransaction(1, 1, 1);
    
    // Try to process order with insufficient stock
    processOrderWithTransaction(1, 1, 100);
    
    printf("\n");
}

void demonstrateReporting() {
    printf("=== REPORTING DEMO ===\n");
    
    generateSalesReport();
    printf("\n");
    generateInventoryReport();
    
    printf("\n");
}

// =============================================================================
// MAIN FUNCTION
// =============================================================================

int main() {
    printf("SQLite Database Operations\n");
    printf("===========================\n\n");
    
    // Initialize database
    if (!initDatabase("ecommerce.db")) {
        return 1;
    }
    
    // Create tables
    if (!createTables()) {
        closeDatabase();
        return 1;
    }
    
    // Run demonstrations
    demonstrateUserManagement();
    demonstrateProductManagement();
    demonstrateOrderManagement();
    demonstrateTransactions();
    demonstrateReporting();
    
    // Close database
    closeDatabase();
    
    printf("Database operations demonstrated successfully!\n");
    return 0;
}
