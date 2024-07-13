#include <memory>  
#include <iostream>  

class Department;

class Manager {  
public:  
    std::shared_ptr<Department> belongsTo; // 员工所属的部门  
    ~Manager() { std::cout << "Manager destroyed\n"; }  
};  
  
class Department {  
public:  
    std::weak_ptr<Manager> manager; // 部门的经理  
    ~Department() { std::cout << "Department destroyed\n"; }  
};  
  
int main() {  
    std::shared_ptr<Manager> managerPtr = std::make_shared<Manager>();  
    std::shared_ptr<Department> deptPtr = std::make_shared<Department>();  
    managerPtr->belongsTo = deptPtr; // 经理属于这个部门  
    deptPtr->manager = std::weak_ptr<Manager>(managerPtr);; // 这个部门的经理是这位经理  
  
    // 这里形成了循环引用  
    // 当main函数结束时，managerPtr和deptPtr的引用计数都不会降到0  
    // 导致内存泄漏  
}