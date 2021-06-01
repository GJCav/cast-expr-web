#include <vector>
#include <iostream>
 
struct B {
    int m = 0;
    void hello() const {
        std::cout << "Hello world, this is B!\n";
    }
};
struct D : B {
    void hello() const {
        std::cout << "Hello world, this is D!\n";
    }
};
 
enum class E { ONE = 1, TWO, THREE };
enum EU { ONE = 1, TWO, THREE };
 
int main()
{
    // 1: 通过初始化进行转换
    int n = static_cast<int>(3.14); 
    std::cout << "n = " << n << '\n';
    std::vector<int> v = static_cast<std::vector<int>>(10);
    std::cout << "v.size() = " << v.size() << '\n';
 
    // 2: 静态向下转换
    D d;
    B& br = d; // 隐式向上转换
    br.hello();
    D& another_d = static_cast<D&>(br); // 向下转换
    another_d.hello();
 
    // 3: 左值转化为右值
    std::vector<int> v2 = static_cast<std::vector<int>&&>(v);
    std::cout << "after move, v.size() = " << v.size() << '\n';
 
    // 4: 抛弃表达式的值
    static_cast<void>(v2.size());
 
    // 5. 隐式转换逆转换
    void* nv = &n;
    int* ni = static_cast<int*>(nv);
    std::cout << "*ni = " << *ni << '\n';
 
    // 6. 先执行数组到指针，紧跟着向上转换
    D a[10];
    B* dp = static_cast<B*>(a);
 
    // 7. 限域枚举转换为int或float
    E e = E::ONE;
    int one = static_cast<int>(e);
    std::cout << one << '\n';
 
    // 8. int到枚举，枚举到枚举
    E e2 = static_cast<E>(one);
    EU eu = static_cast<EU>(e2);
 
    // 9. 对成员指针进行向上转换
    int D::*pm = &D::m;
    std::cout << br.*static_cast<int B::*>(pm) << '\n';
 
    // 10. void* 指针到任意类型指针转换
    void* voidp = &e;
    std::vector<int>* p = static_cast<std::vector<int>*>(voidp);
}