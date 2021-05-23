#include <cstdint>
#include <cassert>
#include <iostream>
int f() { return 42; }
int main()
{
    int i = 7;
 
    // 指针与整型间相互转换
    std::uintptr_t v1 = reinterpret_cast<std::uintptr_t>(&i); // 使用 static_cast 将导致编译错误
    std::cout << "The value of &i is 0x" << std::hex << v1 << '\n';
    int* p1 = reinterpret_cast<int*>(v1);
    assert(p1 == &i);
 
    // 函数指针间的转换
    void(*fp1)() = reinterpret_cast<void(*)()>(f);
    // fp1(); undefined behavior
    int(*fp2)() = reinterpret_cast<int(*)()>(fp1);
    std::cout << std::dec << fp2() << '\n'; // 这个调用是安全的
 
    // 通过指针的类型别名
    char* p2 = reinterpret_cast<char*>(&i);
    if(p2[0] == '\x7')
        std::cout << "This system is little-endian\n";
    else
        std::cout << "This system is big-endian\n";
 
    // 通过引用的类型别名
    reinterpret_cast<unsigned int&>(i) = 42;
    std::cout << i << '\n';
 
    const int &const_iref = i;
    //int &iref = reinterpret_cast<int&>(const_iref); //编译错误 - 无法去除const
    //此时必须使用const_cast:  int &iref = const_cast<int&>(const_iref);
}