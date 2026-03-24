# SQLite Database Operations

This file contains comprehensive database programming examples in C using SQLite, including table creation, CRUD operations, transactions, and reporting functions for a complete e-commerce system.

## 📚 Database Programming Overview

### 🗄️ Database Concepts
- **Tables**: Structured data storage
- **Rows/Records**: Individual data entries
- **Columns/Fields**: Data attributes
- **Primary Keys**: Unique identifiers
- **Foreign Keys**: Relationships between tables

### 🔧 SQLite Features
- **Serverless**: No separate database server
- **File-based**: Database stored in single file
- **Transaction Support**: ACID compliance
- **SQL Standard**: Full SQL implementation

## 🔗 Database Connection Management

### Connection Structure
```c
typedef struct {
    sqlite3* db;
    char* filename;
    int is_connected;
} DatabaseConnection;
```

### Database Operations
```c
// Initialize database connection
int initDatabase(const char* filename) {
    int result = sqlite3_open(filename, &db_conn.db);
    
    if (result != SQLITE_OK) {
        printf("Cannot open database: %s\n", sqlite3_errmsg(db_conn.db));
        return 0;
    }
    
    db_conn.filename = strdup(filename);
    db_conn.is_connected = 1;
    
    return 1;
}

// Close database connection
void closeDatabase() {
    if (db_conn.is_connected && db_conn.db) {
        sqlite3_close(db_conn.db);
        db_conn.is_connected = 0;
        free(db_conn.filename);
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
```

## 🏗️ Table Creation

### User Table Schema
```c
const char* create_users_table = 
    "CREATE TABLE IF NOT EXISTS users ("
    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
    "username TEXT NOT NULL UNIQUE,"
    "email TEXT NOT NULL UNIQUE,"
    "password_hash TEXT NOT NULL,"
    "created_at INTEGER NOT NULL,"
    "is_active INTEGER DEFAULT 1"
    ");";
```

### Product Table Schema
```c
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
```

### Order Table Schema
```c
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
```

### Data Types in SQLite
- **INTEGER**: Signed integers
- **TEXT**: Text strings
- **REAL**: Floating point numbers
- **BLOB**: Binary data
- **NULL**: Null values

## 👤 User Management

### User Structure
```c
typedef struct {
    int id;
    char username[50];
    char email[100];
    char password_hash[64];
    int created_at;
    int is_active;
} User;
```

### CRUD Operations

#### Insert User
```c
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
    
    return sqlite3_last_insert_rowid(db_conn.db);
}
```

#### Get User by ID
```c
User getUserById(int user_id) {
    User user = {0};
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, username, email, password_hash, created_at, is_active FROM users WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
```

#### Update User
```c
int updateUser(int user_id, const char* username, const char* email) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE users SET username = ?, email = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, email, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, user_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}
```

#### Delete User
```c
int deleteUser(int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "DELETE FROM users WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, user_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}
```

## 📦 Product Management

### Product Structure
```c
typedef struct {
    int id;
    char name[100];
    char description[500];
    double price;
    int stock_quantity;
    int category_id;
    int created_at;
} Product;
```

### Product Operations

#### Insert Product
```c
int insertProduct(const char* name, const char* description, double price, int stock_quantity, int category_id) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO products (name, description, price, stock_quantity, category_id, created_at) VALUES (?, ?, ?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
    
    return result == SQLITE_DONE ? sqlite3_last_insert_rowid(db_conn.db) : 0;
}
```

#### Get Products by Category
```c
void getProductsByCategory(int category_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT id, name, description, price, stock_quantity FROM products WHERE category_id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
```

#### Update Product Stock
```c
int updateProductStock(int product_id, int new_quantity) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE products SET stock_quantity = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_int(stmt, 1, new_quantity);
    sqlite3_bind_int(stmt, 2, product_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}
```

## 📋 Order Management

### Order Structure
```c
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
```

### Order Operations

#### Create Order
```c
int createOrder(int user_id, int product_id, int quantity, double unit_price) {
    sqlite3_stmt* stmt;
    const char* sql = "INSERT INTO orders (user_id, product_id, quantity, unit_price, total_price, created_at) VALUES (?, ?, ?, ?, ?, ?)";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
    
    return result == SQLITE_DONE ? sqlite3_last_insert_rowid(db_conn.db) : 0;
}
```

#### Get User Orders
```c
void getUserOrders(int user_id) {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT o.id, p.name, o.quantity, o.unit_price, o.total_price, o.status, o.created_at "
                      "FROM orders o JOIN products p ON o.product_id = p.id WHERE o.user_id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
```

#### Update Order Status
```c
int updateOrderStatus(int order_id, const char* new_status) {
    sqlite3_stmt* stmt;
    const char* sql = "UPDATE orders SET status = ? WHERE id = ?";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_bind_text(stmt, 1, new_status, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, order_id);
    
    int result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    return result == SQLITE_DONE;
}
```

## 🔄 Transactions

### Transaction Management
```c
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
```

### Transaction Commands
```c
// Begin transaction
executeSQL("BEGIN TRANSACTION");

// Commit transaction
executeSQL("COMMIT");

// Rollback transaction
executeSQL("ROLLBACK");
```

### ACID Properties
- **Atomicity**: All operations succeed or none
- **Consistency**: Database remains valid
- **Isolation**: Concurrent transactions don't interfere
- **Durability**: Changes persist after commit

## 📊 Reporting Functions

### Sales Report
```c
void generateSalesReport() {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT COUNT(*) as total_orders, "
                      "SUM(total_price) as total_revenue, "
                      "COUNT(DISTINCT user_id) as unique_customers "
                      "FROM orders WHERE status = 'completed'";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
```

### Inventory Report
```c
void generateInventoryReport() {
    sqlite3_stmt* stmt;
    const char* sql = "SELECT p.name, p.stock_quantity, p.price, "
                      "(p.stock_quantity * p.price) as total_value "
                      "FROM products p ORDER BY total_value DESC";
    
    if (sqlite3_prepare_v2(db_conn.db, sql, -1, &stmt, NULL) != SQLITE_OK) {
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
```

## 💡 Advanced SQL Operations

### JOIN Operations
```c
// Inner Join
const char* inner_join = "SELECT u.username, o.total_price "
                        "FROM users u JOIN orders o ON u.id = o.user_id";

// Left Join
const char* left_join = "SELECT u.username, COUNT(o.id) as order_count "
                       "FROM users u LEFT JOIN orders o ON u.id = o.user_id "
                       "GROUP BY u.id";

// Multiple Joins
const char* multi_join = "SELECT u.username, p.name, o.quantity "
                         "FROM users u "
                         "JOIN orders o ON u.id = o.user_id "
                         "JOIN products p ON o.product_id = p.id";
```

### Aggregation Functions
```c
// COUNT
const char* count_query = "SELECT COUNT(*) FROM users";

// SUM
const char* sum_query = "SELECT SUM(total_price) FROM orders";

// AVG
const char* avg_query = "SELECT AVG(price) FROM products";

// MAX/MIN
const char* max_query = "SELECT MAX(price) FROM products";
const char* min_query = "SELECT MIN(price) FROM products";
```

### Subqueries
```c
// Subquery in WHERE clause
const char* subquery_where = "SELECT * FROM products WHERE id IN "
                           "(SELECT product_id FROM orders WHERE user_id = ?)";

// Subquery in SELECT clause
const char* subquery_select = "SELECT name, (SELECT COUNT(*) FROM orders WHERE product_id = products.id) as order_count "
                             "FROM products";
```

### Window Functions (SQLite 3.25+)
```c
// ROW_NUMBER
const char* row_number = "SELECT name, price, ROW_NUMBER() OVER (ORDER BY price DESC) as rank "
                       "FROM products";

// RANK
const char* rank_query = "SELECT name, price, RANK() OVER (ORDER BY price DESC) as price_rank "
                       "FROM products";

// LAG/LEAD
const char* lag_query = "SELECT name, price, LAG(price, 1) OVER (ORDER BY id) as prev_price "
                       "FROM products";
```

## 🔧 Database Optimization

### Index Creation
```c
// Create index on frequently queried columns
executeSQL("CREATE INDEX idx_users_username ON users(username)");
executeSQL("CREATE INDEX idx_orders_user_id ON orders(user_id)");
executeSQL("CREATE INDEX idx_orders_status ON orders(status)");

// Composite index
executeSQL("CREATE INDEX idx_orders_user_status ON orders(user_id, status)");
```

### Query Optimization
```c
// Use EXPLAIN QUERY PLAN
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, "EXPLAIN QUERY PLAN SELECT * FROM users WHERE username = ?", -1, &stmt, NULL);
sqlite3_bind_text(stmt, 1, "john_doe", -1, SQLITE_STATIC);

while (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("%s\n", sqlite3_column_text(stmt, 0));
}
```

### Performance Tips
```c
// Use LIMIT for large result sets
const char* limited_query = "SELECT * FROM orders LIMIT 100";

// Use specific columns instead of *
const char* specific_columns = "SELECT id, name, price FROM products";

// Use WHERE clause to filter early
const char* filtered_query = "SELECT * FROM orders WHERE status = 'completed'";
```

## 🛡️ Security Considerations

### SQL Injection Prevention
```c
// Wrong - vulnerable to SQL injection
char query[256];
sprintf(query, "SELECT * FROM users WHERE username = '%s'", username);

// Right - use parameterized queries
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE username = ?", -1, &stmt, NULL);
sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
```

### Input Validation
```c
int validateUsername(const char* username) {
    // Check length
    if (strlen(username) < 3 || strlen(username) > 50) {
        return 0;
    }
    
    // Check for valid characters
    for (int i = 0; username[i]; i++) {
        if (!isalnum(username[i]) && username[i] != '_' && username[i] != '-') {
            return 0;
        }
    }
    
    return 1;
}
```

### Password Hashing
```c
// Simple hash function (use proper cryptographic hashing in production)
void hashPassword(const char* password, char* hash) {
    // In production, use bcrypt, scrypt, or Argon2
    sprintf(hash, "hashed_%s", password);
}
```

## 🔄 Backup and Recovery

### Database Backup
```c
int backupDatabase(const char* backup_filename) {
    sqlite3* backup_db;
    
    if (sqlite3_open(backup_filename, &backup_db) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_backup* backup = sqlite3_backup_init(backup_db, "main", db_conn.db, "main");
    
    int result = sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    sqlite3_close(backup_db);
    
    return result == SQLITE_DONE;
}
```

### Database Restore
```c
int restoreDatabase(const char* backup_filename) {
    // Close current database
    closeDatabase();
    
    // Copy backup file to main database file
    // Implementation depends on operating system
    
    // Reopen database
    return initDatabase("ecommerce.db");
}
```

## 📊 Data Analysis

### Statistical Queries
```c
void calculateStatistics() {
    sqlite3_stmt* stmt;
    
    // Product price statistics
    const char* price_stats = "SELECT "
                           "MIN(price) as min_price, "
                           "MAX(price) as max_price, "
                           "AVG(price) as avg_price, "
                           "COUNT(*) as product_count "
                           "FROM products";
    
    if (sqlite3_prepare_v2(db_conn.db, price_stats, -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            double min_price = sqlite3_column_double(stmt, 0);
            double max_price = sqlite3_column_double(stmt, 1);
            double avg_price = sqlite3_column_double(stmt, 2);
            int product_count = sqlite3_column_int(stmt, 3);
            
            printf("Price Statistics:\n");
            printf("Min: $%.2f\n", min_price);
            printf("Max: $%.2f\n", max_price);
            printf("Avg: $%.2f\n", avg_price);
            printf("Count: %d\n", product_count);
        }
        sqlite3_finalize(stmt);
    }
}
```

### Time Series Analysis
```c
void analyzeSalesTrends() {
    sqlite3_stmt* stmt;
    const char* trend_query = "SELECT DATE(created_at, 'unixepoch') as date, "
                            "COUNT(*) as orders, "
                            "SUM(total_price) as revenue "
                            "FROM orders "
                            "WHERE status = 'completed' "
                            "GROUP BY DATE(created_at) "
                            "ORDER BY date";
    
    if (sqlite3_prepare_v2(db_conn.db, trend_query, -1, &stmt, NULL) == SQLITE_OK) {
        printf("Daily Sales Trends:\n");
        printf("Date        Orders    Revenue\n");
        printf("----------------------------\n");
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int date = sqlite3_column_int(stmt, 0);
            int orders = sqlite3_column_int(stmt, 1);
            double revenue = sqlite3_column_double(stmt, 2);
            
            char date_str[20];
            strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime((time_t*)&date));
            
            printf("%-12s %-8d $%.2f\n", date_str, orders, revenue);
        }
        sqlite3_finalize(stmt);
    }
}
```

## ⚠️ Common Pitfalls

### 1. Memory Leaks
```c
// Wrong - Forgetting to finalize statements
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_step(stmt);
// Forgot: sqlite3_finalize(stmt);

// Right - Always finalize statements
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_step(stmt);
sqlite3_finalize(stmt);
```

### 2. Not Checking Return Values
```c
// Wrong - Not checking return values
sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
sqlite3_step(stmt);

// Right - Check all return values
if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
    printf("Error: %s\n", sqlite3_errmsg(db));
    return;
}

if (sqlite3_step(stmt) != SQLITE_DONE) {
    printf("Error: %s\n", sqlite3_errmsg(db));
    return;
}
```

### 3. SQL Injection
```c
// Wrong - Vulnerable to injection
char query[256];
sprintf(query, "SELECT * FROM users WHERE username = '%s'", input);
sqlite3_exec(db, query, 0, 0, 0);

// Right - Use parameterized queries
sqlite3_stmt* stmt;
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE username = ?", -1, &stmt, NULL);
sqlite3_bind_text(stmt, 1, input, -1, SQLITE_STATIC);
```

### 4. Not Handling Transactions
```c
// Wrong - No transaction handling
updateStock(product_id, new_quantity);
createOrder(user_id, product_id, quantity);

// Right - Use transactions
if (!executeSQL("BEGIN TRANSACTION")) return;
if (!updateStock(product_id, new_quantity)) {
    executeSQL("ROLLBACK");
    return;
}
if (!createOrder(user_id, product_id, quantity)) {
    executeSQL("ROLLBACK");
    return;
}
executeSQL("COMMIT");
```

## 🔧 Real-World Applications

### 1. E-commerce System
```c
// Complete order processing
int processEcommerceOrder(int user_id, int product_id, int quantity) {
    // Validate user
    if (!isUserActive(user_id)) return 0;
    
    // Check product availability
    if (!isProductAvailable(product_id, quantity)) return 0;
    
    // Process payment
    if (!processPayment(user_id, quantity * getProductPrice(product_id))) return 0;
    
    // Create order with transaction
    return processOrderWithTransaction(user_id, product_id, quantity);
}
```

### 2. Inventory Management
```c
// Automatic stock management
void updateInventoryLevels() {
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT id, stock_quantity FROM products WHERE stock_quantity < 10", 
                   -1, &stmt, NULL);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int product_id = sqlite3_column_int(stmt, 0);
        int stock = sqlite3_column_int(stmt, 1);
        
        if (stock < 5) {
            // Send low stock alert
            sendLowStockAlert(product_id);
        }
        
        if (stock == 0) {
            // Remove from catalog
            removeProductFromCatalog(product_id);
        }
    }
    
    sqlite3_finalize(stmt);
}
```

### 3. User Analytics
```c
void generateUserAnalytics() {
    sqlite3_stmt* stmt;
    const char* analytics_query = "SELECT "
                               "u.id, u.username, "
                               "COUNT(o.id) as total_orders, "
                               "SUM(o.total_price) as total_spent, "
                               "MAX(o.created_at) as last_order "
                               "FROM users u "
                               "LEFT JOIN orders o ON u.id = o.user_id "
                               "GROUP BY u.id";
    
    if (sqlite3_prepare_v2(db_conn.db, analytics_query, -1, &stmt, NULL) == SQLITE_OK) {
        printf("User Analytics:\n");
        printf("ID    Username    Orders    Spent      Last Order\n");
        printf("------------------------------------------------\n");
        
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const char* username = (const char*)sqlite3_column_text(stmt, 1);
            int orders = sqlite3_column_int(stmt, 2);
            double spent = sqlite3_column_double(stmt, 3);
            int last_order = sqlite3_column_int(stmt, 4);
            
            char date_str[20];
            strftime(date_str, sizeof(date_str), "%Y-%m-%d", localtime((time_t*)&last_order));
            
            printf("%-6d %-12s %-8d $%-9.2f %s\n", id, username, orders, spent, date_str);
        }
        sqlite3_finalize(stmt);
    }
}
```

### 4. Data Migration
```c
void migrateDatabase() {
    // Create new table structure
    executeSQL("CREATE TABLE users_new (id INTEGER PRIMARY KEY, username TEXT UNIQUE, email TEXT UNIQUE)");
    
    // Migrate data
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, "SELECT id, username, email FROM users", -1, &stmt, NULL);
    
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const char* username = (const char*)sqlite3_column_text(stmt, 1);
        const char* email = (const char*)sqlite3_column_text(stmt, 2);
        
        // Insert into new table
        sqlite3_stmt* insert_stmt;
        sqlite3_prepare_v2(db, "INSERT INTO users_new (id, username, email) VALUES (?, ?, ?)", 
                       -1, &insert_stmt, NULL);
        sqlite3_bind_int(insert_stmt, 1, id);
        sqlite3_bind_text(insert_stmt, 2, username, -1, SQLITE_STATIC);
        sqlite3_bind_text(insert_stmt, 3, email, -1, SQLITE_STATIC);
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    sqlite3_finalize(stmt);
    
    // Replace old table
    executeSQL("DROP TABLE users");
    executeSQL("ALTER TABLE users_new RENAME TO users");
}
```

## 🎓 Best Practices

### 1. Connection Management
```c
// Always close connections
void cleanupDatabase() {
    if (db_conn.is_connected) {
        closeDatabase();
    }
}

// Use connection pooling for web applications
DatabaseConnection* getConnectionFromPool() {
    // Implementation depends on application architecture
}
```

### 2. Error Handling
```c
// Comprehensive error handling
int safeExecuteSQL(const char* sql) {
    char* err_msg = 0;
    int result = sqlite3_exec(db_conn.db, sql, 0, 0, &err_msg);
    
    if (result != SQLITE_OK) {
        fprintf(stderr, "SQL Error in '%s': %s\n", sql, err_msg);
        sqlite3_free(err_msg);
        return 0;
    }
    
    return 1;
}
```

### 3. Performance Optimization
```c
// Use prepared statements for repeated queries
sqlite3_stmt* prepared_stmt;
sqlite3_prepare_v2(db, "SELECT * FROM users WHERE id = ?", -1, &prepared_stmt, NULL);

// Bind parameters and reuse
sqlite3_bind_int(prepared_stmt, 1, user_id);
sqlite3_step(prepared_stmt);
sqlite3_reset(prepared_stmt);
```

### 4. Security
```c
// Always validate input
int safeInsertUser(const char* username, const char* email, const char* password) {
    if (!validateUsername(username)) return 0;
    if (!validateEmail(email)) return 0;
    if (!validatePassword(password)) return 0;
    
    char password_hash[64];
    hashPassword(password, password_hash);
    
    return insertUser(username, email, password_hash);
}
```

### 5. Documentation
```c
/**
 * @brief Insert a new user into the database
 * @param username User's username (3-50 characters, alphanumeric only)
 * @param email User's email address
 * @param password_hash Hashed password
 * @return User ID on success, 0 on failure
 * @note This function validates input before insertion
 */
int insertUser(const char* username, const char* email, const char* password_hash);
```

Database programming in C with SQLite provides powerful data persistence capabilities for applications. Master these concepts to build robust, scalable data-driven applications!
