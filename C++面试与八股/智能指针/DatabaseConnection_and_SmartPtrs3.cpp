#include <iostream>
#include <memory>

class DatabaseConnection_and_SmartPtrs3
{
private:
    /* data */
public:
    DatabaseConnection_and_SmartPtrs3(/* args */){
        std::cout << "Database connection established (this = " << this << ")" << std::endl;
    };
    ~DatabaseConnection_and_SmartPtrs3(){
        std::cout << "closed this = " << this << std::endl;
    };
    void fun(){
        std::cout << "1 this = " << this << std::endl;
    }
};

int main(){
    //unique
    std::unique_ptr<DatabaseConnection_and_SmartPtrs3> uni = std::make_unique<DatabaseConnection_and_SmartPtrs3>();
    uni -> fun();
    std::unique_ptr<DatabaseConnection_and_SmartPtrs3> uni2 = std::move(uni);
    uni -> fun();
    uni2 -> fun();

    //shared
    std::shared_ptr<DatabaseConnection_and_SmartPtrs3> sh = std::make_shared<DatabaseConnection_and_SmartPtrs3>();
    {
        std::shared_ptr<DatabaseConnection_and_SmartPtrs3> sh2 = sh;
        std::cout << sh.use_count() << std::endl; 
        sh2 -> fun();
    }
    std::cout << sh.use_count() << std::endl; 

    //weak

    return 0;
}
