#include <iostream>  
#include <memory> // 智能指针  
  
class DatabaseConnection {  
public:  
    DatabaseConnection() {  
        std::cout << "Database connection established (this = " << this << ")" << std::endl;  
    }  
  
    ~DatabaseConnection() {  
        std::cout << "Database connection closed (this = " << this << ")" << std::endl;  
    }  
  
    void query(const std::string& queryStr) {  
        std::cout << "Executing query: " << queryStr << " (this = " << this << ")" << std::endl; 
        // 假设这里有一些实际的数据库操作  
    }  
};
int main() {  
    // 使用 unique_ptr 管理一个独占的数据库连接  
    std::unique_ptr<DatabaseConnection> uniqueConnection = std::make_unique<DatabaseConnection>();  
    uniqueConnection->query("SELECT * FROM users;");  
    std::unique_ptr<DatabaseConnection> uniqueConnection2 = std::move(uniqueConnection);
    uniqueConnection->query("SELECT * FROM users;");
    uniqueConnection2->query("SELECT * FROM users;");  
    uniqueConnection->query("123;");

    // 当 uniqueConnection 超出作用域时，数据库连接自动关闭  
  
    // 使用 shared_ptr 管理一个共享的数据库连接  
    std::shared_ptr<DatabaseConnection> sharedConnection = std::make_shared<DatabaseConnection>();  
    {  
        std::shared_ptr<DatabaseConnection> sharedConnectionCopy = sharedConnection;  
        std::cout << "Shared connection copy created. Use count: " << sharedConnection.use_count() << std::endl;  
        sharedConnectionCopy->query("INSERT INTO users (name) VALUES ('Alice');");  
  
        // 当这个作用域结束时，sharedConnectionCopy 被销毁，  
        // 但数据库连接仍然保持打开状态，因为 sharedConnection 仍然持有连接的引用。  
    }  
    std::cout << "Exiting inner scope. Use count: " << sharedConnection.use_count() << std::endl;  
  
    // 当 sharedConnection 超出作用域时，数据库连接自动关闭  
  
    // 使用 weak_ptr 解决潜在的循环引用问题  
    std::shared_ptr<DatabaseConnection> parentConnection = std::make_shared<DatabaseConnection>();  
    std::shared_ptr<DatabaseConnection> childConnection = parentConnection; // 假设这是通过某种方式获得的“子”连接  
    std::cout << "Exiting inner scope. Use count: " << parentConnection.use_count() << std::endl;  
    // 假设我们想要让 parentConnection 引用 childConnection，但不想造成循环引用  
    // 我们可以使用 weak_ptr 来引用 childConnection  
    std::weak_ptr<DatabaseConnection> weakChildConnection = childConnection;  
    // 当需要访问 childConnection 时，我们可以将 weak_ptr 提升为 shared_ptr  
    if (auto lockedChildConnection = weakChildConnection.lock()) {  
        std::cout << "Weak connection is still available. Use count: " << lockedChildConnection.use_count() << std::endl;  
        lockedChildConnection->query("UPDATE users SET name = 'Bob' WHERE name = 'Alice';");  
    } else {  
        std::cout << "Child connection is no longer available." << std::endl;  
    }  
  
    // 当 parentConnection 和 childConnection 超出作用域时，它们的数据库连接自动关闭  
    // weakChildConnection 不拥有连接，因此不会影响连接的关闭  

    return 0;  
}