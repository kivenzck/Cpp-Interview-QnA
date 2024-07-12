#include <iostream>
#include <memory>

class Node {
public:
    std::weak_ptr<Node> next;
    std::weak_ptr<Node> prev;

    Node() {
        std::cout << "Node created\n";
    }

    ~Node() {
        std::cout << "Node destroyed\n";
    }
};

int main() {
    // 创建两个节点
    std::shared_ptr<Node> node1 = std::make_shared<Node>();
    std::shared_ptr<Node> node2 = std::make_shared<Node>();
    std::cout << node1.use_count() << std::endl ;
    std::cout << node2.use_count() << std::endl ;

    // 创建循环引用
    node1->next = node2;
    node2->prev = node1;
    std::cout << node1.use_count() << std::endl ;
    std::cout << node2.use_count() << std::endl ;

    if (auto lockedChildConnection = node1->next.lock()) {  
        std::cout << "Weak connection is still available. Use count: " << lockedChildConnection.use_count() << std::endl;  
    } else {  
        std::cout << "Child connection is no longer available." << std::endl;  
    }  

    // 重置 shared_ptr，理论上应该销毁节点
    node1.reset();
    node2.reset();
    std::cout << node1.use_count() << std::endl ;
    std::cout << node2.use_count() << std::endl ;

    // 由于循环引用，节点没有被销毁
    return 0;
}