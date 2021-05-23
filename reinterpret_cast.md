# reinterpret_cast 详细说明

`reinterpret_cast`仅仅是对二进制位的重新解释，不会对数据进行调整，风险较高。

> Ref: [cpp reference reinterpret_cast](https://en.cppreference.com/w/cpp/language/reinterpret_cast)

## 语法

`reinterpret_cast<new_type>(expression)`

返回`new_type`类型的一个值。



## 解释

### 规则列表

与`static_cast`不同，`reinterpret_cast`除了在进行整型和指针或者在指针表示依赖于类型的糟糕的系统架构上，不会被编译为任何CPU指令。这仅仅是一个编译期指令，让编译器把`expression`的类型当成`new_type`处理。

仅有下述转换可以通过`reinterpret_cast`实现，且`reinterpret_cast`不能去除任何常量性(constness)和易变性(volatility)。

1. 一个整型、枚举型、指针、成员指针类型的表达式可以被转换为自己的类型。转换结果的值就是表达式的值。

2. 指针可以被转换为任意足够大能容纳所有指针值的整型类型。

3. 任何整型或者枚举类型可以被转换为指针类型。可以保证一个指针转换成足够大整型后再转换回原类型指针的值就是原有的值，否则转换结果指针不能被安全地解引用（该保证不包括另一个方向的回环转换；相同的指针也许会有不同的整型表示）。空指针常量`NULL`或0值整型不保证一定能转换成目标类型的空指针，此时应该使用`static_cast`或者隐式转换。

4. `std::nullptr_t`类型的任何值，以及值`nullptr`，可以被转换成任意整型类型，就像是转换的是没有值的`(void)*0`一样，甚至`nullptr`也不能被转换为`std::nullptr_t`，这样的转换应该使用`static_cast`实现。

5. 任意`T1*`类型指针可以转换为`cv T2*`类型指针。这完全等价为：

   ```c++
   static_cast<cv T2*>(static_cast<cv void*>(expression))
   ```

   此时T2的内存对齐要求不比T1严格，指针的值保持不变，且将转换结果再转换成原类型其值也保持原指针的值。

   在任何情况下，只有在满足类型别名规则(type aliasing rules)时，结果指针才能被安全地解引用。

6. 任何T1类型的左值引用可以转换为T2类型的右值引用。转换结果是指向原对象的一个不同类型的左值或消亡值。这个过程不会创建临时变量、不会进行拷贝、不会调用构造函数或转换函数。只有在满足类型别名规则时，对结果引用的访问才是安全的。

7. 任何指向函数的指针可以被转换成指向另一个函数类型的指针。通过另一个函数类型的指针调用该函数的行为是未定义的，但将转换结果指针再转换回原函数指针类型将得到指向原来函数的指针。

8. 在一些操作系统实现下（特别是兼容[dlsym](http://pubs.opengroup.org/onlinepubs/9699919799/functions/dlsym.html)的POSIX系统中），一个函数指针可以被转换为`void*`类型或者任何其他对象类型指针，反方向亦然。如果该操作系统支持双向转换，把结果转换成原类型将得到最初的值，除此之外结果指针不能被安全地调用或解引用。

9. 任指针类型的空指针可以被转换问任意其他指针类型，结果就是目标类型的空指针。注意空指针常量`nullptr`或`std::nullptr_t`类型的任意值不能使用`reinterpret_cast`转化为其他指针，这种转换应该使用隐式转换或者`static_cast`。

10. 一个指向成员函数的指针可以转换为指向另一个不同类型的成员函数的指针。再把它转换回原类型将得到最初的值，否则结果指针不能被安全地使用。

11. 一个指向某个类T的成员对象的指针可以转换为指向另一个类T2的成员对象的指针。如果T2对齐要求不比T1严格，则再将指针转化回原T1类型将得到最初的值，否则转换结果对象无法被安全地使用。



对于所有的`reinterpret_cast`，结果满足：

* 如果`new_type`是左值引用类型或对函数类型的右值引用，结果为左值。
* 如果`new_type`是对对象的右值引用类型，结果为消亡值。
* 其他情况下结果为纯右值。



### 类型别名

尝试通过类型`AliasedType`的泛左值读取或修改`DynamicType`类型对象的值将产生未定义行为，除非下述规则之一满足：

* `AlisedType`和`DynamicType`是相似的。
* `AlisedType`是`DynamicType`的有符号或无符号变体。
* `AlisedType`是`std::byte`、`char`或`unsigned char`。该规则运行把任何对象当成字节数组来检查该对象的表示([object representation](https://en.cppreference.com/w/cpp/language/object#Object_representation_and_value_representation))。



### 类型相似

不太严谨的说，忽略最高层级的cv限定符，两个类型是相似的，如果：

* 他们是同一个类型；或

* 他们都是指针，且指向的类型相同；或

* 都是指向同一个类的成员指针，且指向的成员是相似的；或

* <div style="height: auto; width: 100%; border: lightgrey solid 0.5px">
  	<div style="display: flex; align-items: center">
          <span style="flex-grow: 1;">都是相同大小的数组或都是未知边界的数组，且数组元素类型相似；或</span>
          <span style="display:block; text-align: center;flex-shrink: 1; color: green;font-size: 0.8em;min-width: 7em">(until C++20)</span>
      </div>
  </div>

* 都是相同大小的数组或二者中至少有一个是未知边界的数组，且元素类型相似。

例如：

* `const int * volatile * `和`int * * const`是相似的
* `const int (* volatile S::* const)[20]` 和`int (* const S::* volatile)[20]` 是相似的
* `int (* const *)(int *)` 和`int (* volatile *)(int *)`是相似的
* `int (S::*)() const` 和`int (S::*)()` 不相似。
* `int (*)(int *)` 和`int (*)(const int *)` 不相似;
* `const int (*)(int *)` 和`int (*)(int *)` 不相似;
* `int (*)(int * const)` 和`int (*)(int *)`  相似;
* `std::pair<int, int>` 和`std::pair<const int, int>` 不相似；

这个规则支持了类型别名分析，这里编译器假设通过一个类型的泛左值读取数据不会因通过另一种类型的泛左值写入数据遭到修改。

注1：许多C++编译器会引入非标准语言拓展来放松上述规则，使通过共同体的不活动成员访问错误的类型。

注2：标准中上述规则还包括从C继承而来的两点额外规则，但下述情况可能在C++中出现，所以被忽略掉了。

1. `AliasedType`是一个聚合体类型([aggregate type](https://en.cppreference.com/w/cpp/language/aggregate_initialization))或是共同体类型，且该类型包含上述提到类型之一作为一个元素或非静态成员（递归地包括子聚合体和包含共同体的非静态数据成员）
2. `AliasedType`是`DynamicType`的基类。



## 代码示例

### 基本使用

```c++
//snippet: rpt-cast-ex1.cpp

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
```

一种可能的输出：

```c++
The value of &i is 0x7fff352c3580
42
This system is little-endian
42
```





### 指针可交换

假设满足内存对齐要求，`reinterpret_cast`不会改变指针的值，除了一些指针可互换的情况：

```c++
struct S1 { int a; } s1;
struct S2 { int a; private: int b; } s2; // S2 不是标准布局类型
union U { int a; double b; } u = {0};
int arr[2];
 
int* p1 = reinterpret_cast<int*>(&s1); // p1 的值是 "指向s1.a的指针的值"因为 s1.a 和 s1 是指针可互换的
 
int* p2 = reinterpret_cast<int*>(&s2); // p2 的值不会改变，就是“指向s2的指针的值”
 
int* p3 = reinterpret_cast<int*>(&u);  // p3 的值是 “指向u.a的指针的值”。u.a 和 u 是指针可互换的。
 
double* p4 = reinterpret_cast<double*>(p3); // p4 的值是 “指向u.b” 的值。u.a 和 u.b 也是指针可互换的，
											//因为他们与 u 是指针可互换的。
 
int* p5 = reinterpret_cast<int*>(&arr); // p5 值保持不变，就是“指向arr的指针”
```



### 未定义行为情况

对一个泛左值访问一个类型的非静态成员数据或非静态成员函数，但这个泛左值恰好不是这个类型时，将导致未定义行为：

```c++
struct S { int x; };
struct T { int x; int f(); };
struct S1 : S {}; 		// 标准布局
struct ST : S, T {}; 	// 非标准布局
 
S s = {};
auto p = reinterpret_cast<T*>(&s); // p 是 "指向s的指针"
auto i = p->x; 	// 未定义行为：s 不是一个 T 类型对象
p->x = 1; 		// 未定义行为
p->f();   		// 同上
 
S1 s1 = {};
auto p1 = reinterpret_cast<S*>(&s1); // p1 是 "指向 s1 的 S类型子对象的指针"
auto i = p1->x; // 程序正确
p1->x = 1; // 正确
 
ST st = {};
auto p2 = reinterpret_cast<S*>(&st); // p2 是 "指向 st 的指针"
auto i = p2->x; // 未定义行为
p2->x = 1; // 未定义行为
```

即使理论上说这些代码违背了前文提到的严格别名规则，许多编译器只会对这些情况报出“strict aliasing”的警告；有些编译器甚至不会有任何警告，例如：clang编译器。

严格别名规则以及相关规则的目的是启用基于类型的别名分析(type-based alias analysis)，但当一个程序使两个毫无关联的类型的指针(如`float*`和`double*`)同时用于访问同一块内存时该机制极易被破坏。所以，任何有可能造成上述情况的技术比然造成未定义行为。



### 与`std::memcpy`、<span style="border: lightgrey solid 1px">`std::bit_cast`<span style="color: green; font-size: 0.5em">(since C++ 20)</span></span>结合

但需要把一个对象的字节解释为另一个类型时，可以考虑使用`std::memcpy`、<span style="border: lightgrey solid 1px">`std::bit_cast`<span style="color: green; font-size: 0.8em">(since C++ 20)</span></span>。

```c++
double d = 0.1;
std::int64_t n;
static_assert(sizeof n == sizeof d);
// n = *reinterpret_cast<std::int64_t*>(&d); // 未定义行为
std::memcpy(&n, &d, sizeof d); // OK
n = std::bit_cast<std::int64_t>(d); // also OK
```







