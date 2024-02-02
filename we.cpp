#include <iostream>
#include <memory>
#include <vector>


class A{
    public:
        A(int a):a(a){}
        int a;
};

int main(){

    std::vector<std::shared_ptr<A>> vec;
    vec.push_back(std::make_shared<A>(1));
    vec.push_back(std::make_shared<A>(2));
    vec.push_back(std::make_shared<A>(3));
    vec.push_back(std::make_shared<A>(4));
    vec.push_back(std::make_shared<A>(5));

    for(auto &v: vec){
        std::cout << v->a << std::endl;
    }

    A a(10);
    std::shared_ptr<A> ptr = std::make_shared<A>(a);
    std::cout << ptr->a << std::endl;

    return 0;
}