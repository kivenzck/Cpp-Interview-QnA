# 目录

[toc]



# 自媒体

1. B站：视频

2. GitHub：代码+md



# 一、语言基础

## 1、三种智能指针

### 1.1基本知识

智能指针**主要作用**：自动管理动态分配的内存，确保在不再需要时能够正确地释放内存资源，从而避免内存泄漏。智能指针通过在对象的生命周期结束时自动调用析构函数来实现这一点。

- `std::unique_ptr` 适用于**独占**资源管理，确保资源在不再需要时被自动释放。
- `std::shared_ptr` 适用于多个指针**共享**资源所有权的场景，确保资源在最后一个指针被销毁时被释放。
- `std::weak_ptr` 适用于解决 `std::shared_ptr` 可能导致的**循环引用**问题，它不拥有资源，但可以观察资源是否还存在。

通过合理使用这些智能指针，可以有效地管理资源的生命周期，避免内存泄漏和资源竞争等问题。



### 1.2问问题

a.独占怎么体现？

1. 1.**单一所有权**：`std::unique_ptr` 确保在任何时刻，只有一个 `std::unique_ptr` 实例拥有对所管理对象的所有权。这意味着，当一个 `std::unique_ptr` 实例被销毁或被重新赋值给另一个对象时，它所管理的对象也会被自动销毁。

2. 2.**不允许复制**：`std::unique_ptr` 不支持复制操作。尝试复制一个 `std::unique_ptr` 实例会导致编译错误。这确保了所有权的唯一性，防止了多个 `std::unique_ptr` 实例同时拥有同一个对象的所有权。

3. 3.**支持移动语义**：`std::unique_ptr` 支持移动操作，允许将所有权从一个 `std::unique_ptr` 实例转移到另一个。当所有权被移动后，原来的 `std::unique_ptr` 实例将不再拥有对象的所有权，而新的 `std::unique_ptr` 实例将拥有。

b,共享怎么体现？

use_count()

c.循环引用 

d.Weak connection is still available. Use count: 3



### 1.3实操

全部重新写一遍



## 2、理解shared_ptr导致的循环引用和如何用weak_ptr解决循环引用

### 2.1基本知识

**理解shared_ptr导致的循环引用**

在C++中，`std::shared_ptr` 是一种智能指针，它使用引用计数的方式来自动管理内存，即当最后一个拥有指向某对象的 `shared_ptr` 被销毁时，指向的对象也会被自动删除。然而，当两个或多个 `shared_ptr` 实例相互持有对方的引用时，就会形成一个循环引用，导致它们指向的对象即使不再被程序中的其他部分使用，也无法被自动删除，因为每个 `shared_ptr` 都认为至少还有一个 `shared_ptr` 在指向它所管理的对象，从而保持对方（实际上是彼此）的引用计数非零。

**使用weak_ptr解决循环引用**

为了解决这个问题，C++ 引入了 `std::weak_ptr`。`weak_ptr` 是一种不拥有其所指向对象的智能指针，它不会增加对象的引用计数。它主要用于解决 `shared_ptr` 之间的循环引用问题。当一个 `shared_ptr` 需要指向另一个 `shared_ptr` 所管理的对象，但又不想增加该对象的引用计数时，可以使用 `weak_ptr`。

### 2.2实例

### 例子：员工与部门的关系

假设在一个公司中，每个员工(`Employee`)都属于一个部门(`Department`)，并且每个部门都有一个经理(`Manager`)，这个经理本身也是员工。如果我们简单地使用`shared_ptr`来表示这些关系，就可能会遇到循环引用的问题。

#### 原始设计（存在循环引用）

```cpp
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
    std::shared_ptr<Manager> manager; // 部门的经理  
    ~Department() { std::cout << "Department destroyed\n"; }  
};  
  
int main() {  
    std::shared_ptr<Manager> managerPtr = std::make_shared<Manager>();  
    std::shared_ptr<Department> deptPtr = std::make_shared<Department>();  
    managerPtr->belongsTo = deptPtr; // 经理属于这个部门  
    deptPtr->manager = managerPtr; // 这个部门的经理是这位经理  
  
    // 这里形成了循环引用  
    // 当main函数结束时，managerPtr和deptPtr的引用计数都不会降到0  
    // 导致内存泄漏  
}
```

#### 改进设计（使用weak_ptr解决循环引用）

为了解决这个问题，我们可以让`Department`类中的`manager`成员使用`weak_ptr`，因为部门不“拥有”其经理（经理是由其他部分或机制创建的，并且可能属于多个角色），而只是引用它。

```cpp
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
    deptPtr->manager = managerPtr; // 这个部门的经理是这位经理  
  
    // 这里形成了循环引用  
    // 当main函数结束时，managerPtr和deptPtr的引用计数都不会降到0  
    // 导致内存泄漏  
}
```

注意：上面的代码在`deptPtr->manager = managerPtr;`这一行实际上是有误的，因为`manager`是`weak_ptr`类型，所以我们应该这样赋值：

```cpp
deptPtr->manager = managerPtr; // 这是错误的，因为类型不匹配  
deptPtr->manager = std::weak_ptr<Manager>(managerPtr); // 这是正确的
```

但更常见的做法是，在创建部门时直接通过`managerPtr`来初始化`weak_ptr`：

```cpp
cpp复制代码

deptPtr->manager = std::weak_ptr<Manager>(managerPtr);
```

或者，如果`Department`的构造函数接受一个`std::shared_ptr<Manager>`作为参数，并在内部转换为`std::weak_ptr<Manager>`，那么可以这样做：

```cpp
class Department {  
public:  
    std::weak_ptr<Manager> manager;  
  
    Department(std::shared_ptr<Manager> m) : manager(m) {} // 在构造函数中初始化weak_ptr  
    ~Department() { std::cout << "Department destroyed" << std::endl;  }
```

占有（独占） 拥有（共享） 引用（观察）
