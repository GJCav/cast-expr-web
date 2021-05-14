# `static_cast` 详细说明

> Ref: https://en.cppreference.com/w/cpp/language/static_cast

## 功能

通过一系列隐式转换或者用户自定义转换执行类型转换。



**Syntax**

```c++
static_cast<new_type>(expression)
```

返回`new_type`的值。



## 功能细节

只有下述转换在不涉及cv修饰符更改时可以使用`static_cast`完成：

1. 如果存在从`expression`到`new_type`的一些列隐式转换，或者`new_type`至少重载一个函数能直接从`expression`的值或引用初始化这个`new_type`类型，那么`static_cast<new_type>`返回一个假想的如同使用`new_type Temp(expression);`构造的`Temp`对象。这可能会涉及到隐式类型转换，`new_type`的构造函数或是用户自定义类型转换运算符。对于非引用类型的`new_type`，对纯右值`expression`类型的转化对象即是直接初始化的对象。

2. 如果`new_type`是某个D类型的引用或者指针，且`expression`是它的非虚基类B或是指向它的纯右值指针，`static_cast`会执行一个向下转换（当B有歧义的、不可访问、D的虚基类、D的虚基类的基类时，这个向下转化是非规范的）。这种向下转换不会进行运行时检查来确保这个对象的类型确实是D，所以此时的向下转换只能安全地用于上述条件能被其他方法保证时，例如静态多态性。安全的向下转换可以使用`dynamic_cast`。

   ```c++
   struct B{};
   struct D : B {};
   D d;
   B& br = d;
   static_cast<D&>(br); // 表示原来的d对象的左值。
   ```

3. 如果`new_type`是右值引用类型，`static_cast`会把泛左值、类类型纯右值、数组类型纯右值、任何左值类型的`expression`转换为代表原对象的消亡值，或原对象的基类子对象。如果目标类型是`expression`的不可访问或有歧义基类，该程序是非规范的。如果`expression`是一个位域(bit field)类型左值，会先转化为底层类型的纯右值。`std::move`移动语义就是用`static_cast`实现。

4. 如果`new_type`是`void`类型，`static_cast`会在对`expression`求值后直接抛弃这个值。

5. 如果存在一个从`new_type`到`expression`的标准转换([standard conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion))链，且这个转换中不包含左值到右值、数组到指针、函数到指针、空指针、空成员指针、函数指针和布尔转换，则`static_cast`也能进行逆向的隐式转换。

6. 如果一个从`expression`到`new_type`涉及到左值到右值、数组到指针、函数到指针的转换，这个转换可以显式地通过`static_cast`完成。

7. 限域枚举类型可以被转换为整型或浮点型。

   > 到C++ 20为止：
   >
   > ​	当目标类型为`bool`，`expression`值为0时转化结果为`false`，为其他值时结果为`true`。对于其他的整型类型，如果它能被目标类型表示，则转换结果是枚举的值，否则是未指明的。
   >
   > 从C++ 20开始：
   >
   > ​	转化结果同从枚举的底层类型到目标类型的隐式转换结果相同。

8. 一个指针或者枚举的值可以被转换为任何完整的枚举类型。

   * 如果底层类型未指定，如果`expression`的值超出了目标类型可以储存的范围，转化结果是未定义的。
   * 如果底层类型确定，转化过程可以视为先把`expression`枚举的底层类型，然后再转换为枚举类型。

   一个浮点类型的值也可以被转换为任意完成的枚举类型。

   * 转化过程可以视为先把`expression`枚举的底层类型，然后再转换为枚举类型。

9. 一个类D的成员指针可以被向上转换为他的无歧义、可访问基类B的成员指针。此时`static_cast`不会进行运行时检查来保证在运行时指向的成员确实存在。

10. 一个`void*`的纯右值指针可以被转换为任意类型指针。如果原指针的值代表的内存地址不满足目标类型的内存对齐条件，那么转化结果是不确定的。否则，如果原指针指向对象a，且存在一个和a是指针可互换的(pointer-interconvertible)目标类型的对象b，转化结果是指向b的指正。否则，指针的值不会被改变。所有将一个指针转化为`void*`类型再转化为原类型的转换会保留指针原有的值。

对于所有的上述转换，转换结果满足：

* 如果`new_type`是代表函数的左值引用或右值引用类型，则结果是左值。
* 如果`new_type`是对象的右值引用，则结果是消亡值。
* 其他情况下，结果是纯右值。

如果满足下述条件，则两个对象a、b是指针可互换的：

* 他们是同一个对象；或者
* 一个是联合体类型（union）且另一个是那个对象的非静态数据成员；或
* 一个是标准结构类型(standard-layout class)对象，另一个是那个对象的第一个非静态数据成员，或如果该对象不具有非静态数据成员、任何基类类型的子对象；或
* 存在一个对象c是的a和c是指针可互换的且c和b是指针可互换的

例如：

```c++
union U {int a; double b;} u;
void* x = &u;						 // x 是 “指向u的指针”
double* y = static_cast<double*>(x); // y 是 “指向u.b的指针”
char* z = static_cast<char*>(x);	 // z 是 “指向u”的指针
```



注：

`static_cast`也可以通过执行一个函数到指针的转换来消除函数重载的模糊性，例如：

```c++
std::for_each(files.begin(), files.end(),
             static_cast<std::ostream&(*)(std::ostream&)>(std::flush));
```



##  例

```c++
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
```

Output：

```
n = 3
v.size() = 10
Hello world, this is B!
Hello world, this is D!
after move, v.size() = 0
*ni = 3
1
0
```

