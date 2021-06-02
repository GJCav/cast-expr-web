#include <iostream>
using namespace std;

struct type{
    int i;
    type(): i(3){}
    void f(int v) const {
        // this->i = v;		// 编译错误，this是指向常量的指针
        const_cast<type*>(this)->i = v; // 只要这个方法的对象不是常量的，程序正确；否则为未定义行为
    }
};
int main(){
    int i = 3;
    const int& rci = i;
    const_cast<int&>(rci) = 4; // 正确：i = 4
    cout << "i=" << i << endl;
    
    type t;
    t.f(4);
    cout << "type::i=" << t.i <<endl;
    
    const int j = 3; // j被定义为常量
    int* pj = const_cast<int*>(&j);
    *pj = 4;      // 未定义行为，主流编译器下j的值不会有任何改变
    cout << "j="<<j << endl;

    void (type::* pmf)(int) const = &type::f; // 指向成员函数的指针
    // const_cast<void(type::*)(int)>(pmf);   // 编译错误：const_cast不适用于函数指针
    return 0;
}
