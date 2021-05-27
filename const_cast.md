# const_cast 详细说明

> Ref: [cpp reference, const_cast](https://en.cppreference.com/w/cpp/language/const_cast)



## 语法

```c++
const_cast<new_type>(expression)
```

返回类型为new_type



## 解释

### 背景术语

**cv-qualifer**：译为 `cv 修饰符`，指的是声明变量时在类型前或变量名前的`const`或`volatile`修饰符。例如：

```c++
const int a = 10;
const int* const c;
volatile int b;
```

**多级指针**：在本文中为指向指针的指针或者更深的指针结构，例如`int *****b;`是一个5级指针。

**左值、右值、纯右值、泛左值、消亡值**：参见[微软文档的解释](https://docs.microsoft.com/en-us/cpp/cpp/lvalues-and-rvalues-visual-cpp?view=msvc-160)



### 说明

`const_cast`被用于移除`cv-qualifier`。

只有下述转换可以使用`const_cast`完成：

1. 对于两个指向同一类型的多级指针，尽管每一级的`cv-qualifier`可能有所不同，但二者可以相互转换。
2. 类型`T`的左值可以被转换为`T`类型的具有不同`cv-qualifier`的左值引用或右值引用。类似的，同一个类型的纯右值以及任意消亡值可以被转化为具有不同 cv 修饰符的右值引用，如果`expression`是一个泛左值，则`const_cast`结果指向原来的对象；其他情况下`const_cast`结果指向被实例化的临时区（materialized temporary）。
3. 上述规则同样适用于指向成员变量或者数组的多级指针。
4. 空指针可以被转换成`new_type`类型的空指针。



转换结果满足：

* 如果`new_type`是对一个对象的右值引用，则结果为消亡值。
* 其他情况下，结果为纯右值。



注：

1. `const_cast`不适用于指向函数的指针和指向成员函数的指针
2. `const_cast`可能会把一个指向常量值的常量指针转化为普通指针，但转化后的指针指向的还是那个常量值，此时通过转换后的指针修改常量的值是一个未定义行为。该情况也适用于引用形式和`volatile`属性。



## 示例

### 例1 基本使用

```c++
// snippet: const_cast-ex1.cpp
#include <iostream>
using namespace std;

struct type{
    int i;
    type(): i(3){}
    void f(int v) const {
        // this->i = v;		// 编译错误，this是指向常量的指针
        const_cast<type*>(this)->i = v; // 可能出现未定义行为
    }
};
int main(){
    int i = 3;
    const int& rci = i;
    const_cast<int&> rci = 4; // 正确：i = 4
    cout << "i=" << i << endl;
    
    type t;
    t.f(4);
    cout << "type::i=" << t.i <<endl;
    
    const int j = 3; // j被定义为常量
    int* pj = const_cast<int*>(&j);
    *pj = 4;      // 未定义行为，主流编译器下j的值不会有任何改变
    cout << "j="<<j<<endl;
    
    void (type::* pmf)(int) const = &type::f; // 指向成员函数的指针
    // const_cast<void(type::*)(int)>(pmf);   // 编译错误：const_cast不适用于函数指针
    return 0;
}
```

输出：

```
i=4
type::i=4
j=3
```



