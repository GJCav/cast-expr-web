# ISO static_cast

> Ref: ISO/IEC 14882



## 定义

> P127
>
> 8.2.9 Static cast



1. `static_cast<T>(v)`的结果就是把v转换为T的结果。

   如果T是左值引用类型或者引用至函数的右值引用类型，转换结果是左值。

   如果T是对对象的右值引用，结果是消亡值。其他情况下，结果是纯右值。`static_cast`不能去除常量性。

2. 假设B是一个类类型(class type)、D是B的一个子类，在`cv2`等于`cv1`或限制比`cv1`更强时，一个`cv1 B`类型的左值可以转换为`cv2 D`的引用类型。

   如果B是D的虚基类、D的虚基类的基类，或者如果不存在从“指向D的指正”向“指向B的指针”的标准转换，那么程序是非规范的。

   一个`cv1 B`类型的消亡值可以被转换为与类型`cv1 B`拥有相同限制的`cv2 D`的右值引用类型。

   如果`cv1 B`的一个对象确实是类型D的一个对象的基类子对象，转化结果将指向那个封闭(enclosing)类型D的对象。

   其他情况下，转化行为是未定义的。例如：

   ```c++
   struct B{};
   struct D : public B{};
   D d;
   B &br = d;
   static_cast<D&>(br); // 结果是指向最初d对象的左值引用。
   ```

3. 如果`cv2 T2`与`cv1 T1`是引用兼容的(11.6.3)，一个`cv1 T1`类型的左值可以被转换为`cv2 T2`的右值引用.

   如要转换的值不是位域(bit-field)，则转换结果就指向那个对象或者是那个对象的基类子对象；否则，将对位域进行左值到右值的转换(7.1)，并把转换得到的纯右值作为`static_cast`的表达式部分。

   如果T2是T1的不可访问或有歧义基类，则包含这样的代码的程序是非规范的。

4. 对于`static_cast<T>(e)`：

   如果存在一条从`e`到`T`的隐式转换链，或者至少重载了一个可用的从e的对象或引用直接初始化(direct-initialization)T的函数，那么e可以被转换为T类型。

   如果T是一个引用类型，效果等价为先执行`T t(e);`再把 t 作为转换的结果。否则，结果对象从e直接初始化而来。

   注：当尝试把一个表达式转化为它的不可访问或者有歧义基类类型时，这个转换时非规范的。

5. 除上述情况，`static_cast`将执行下述转换，且没有其他转换可以使用`static_cast`。

6. 所有表达式都可以被转化为`cv void`类型，这种情况下表达式会变成值被抛弃的表达式(术语 8)。注：但如果这个值存在于临时对象(15.2)中，因为这个对象需要在之后调用析构函数，这个对象会被保留。

7. 如果一个标准转换链(术语 7)并不包含左值到右值(7.1)、数组到指针(7.2)、函数到指针(7.3)、空指针(7.11)、空成员指针(7.12)、布尔转换(7.14)、函数指针转换(7.13)，那么可以通过`static_cast`执行这个转换链的逆向转换。如果一个标准转换链是非规范的，则它的逆向转换也是非规范的。例：

   ```c++
   struct B {};
   struct D {} : private B{};
   void f(){
       static_cast<D*>((B*)0);				// 错误：B 是 D 的私有基类。
       static_cast<int B::*>((int D::*)0);	// 同上。
   }
   ```

8. `static_cast`也可以执行左值到右值(7.1)、数组到指针(7.2)、函数到指针(7.3)的转换，但存在限制：不能去除常量性(8.2.11)，且对于下述特殊情况有额外限制：

9. 一个限域枚举可以被显式转换为一个整形类型。当目标类型是`cv bool`时，如果原来的值为0，转换结果为是`false`；其他情况下，结果未`true`。对于其他整形类型，如果目标类型可以表示原有值，那么原有值不会被改变。除此之外，结果的值是不确定的。

   一个限域枚举的值也可以被显式转化为浮点类型，转化结果就是把原有值转换为浮点类型。

10. 一个整形类型或枚举类型的值可以被显式转化为类型完整的枚举类型。如果原有值在枚举值域内，值不会被改变。其他情况下，转换行为是未定义的。

    一个浮点类型的值也可以被显式转化为枚举类型。转化结果等价为先把原有值转化为枚举的底层类型，然后再转化为枚举类型。

11. 设B为类类型，一个`cv1 B`类型的纯右值指针可以被转化为`cv2 D`类型的纯右值指正，这里D是B的子类且cv2与cv1相同或更强。

    如果B是D的虚基类或D的虚基类的基类，或不存在从D类型指针到B类型指针的标准转换，那么程序是非规范的。

    空指针类型可以被转换为目标类型的空指针。

    如果一个`cv1 B`类型的指针指向一个B对象，且这个对象是一个类型D对象的子对象，转换结果为指向那个封闭类型D对象的指针。

    其他情况下，行为是未定义的。

12. 一个”指向D的cv1 T类型成员的指针“可以被转换为”指向cv2 B类型成员的指针“，这里B是D的基类且cv2不比cv1弱。

    如果不存在从“指向B的T类型成员指针”到“指向D类型的T类型成员指针”的标准转换，程序是非规范的。

    空成员指针可以转化为目标类型的成员指正。

    如果类B包含原有成员，或是包含了原有成员的基类或子类，则转化结果指向原有成员。除此之外，行为是未定义的。注：虽然类B不需要包含原有成员，这个被执行转化的指针的间接对象的动态类型必须包含原有成员。

13. 当T是一个对象类型且cv2不比cv1弱时，一个“cv1 void类型指针”可以被转换为“cv2 T类型指针”。

    如果原指针的值代表内存中一个字节的地址A，A不满足T的内存对齐要求，那么结果指针的值是不确定的。

    除此之外，如果原指针指向一个对象a且存在一个T类型对象b，在忽略cv限定符的情况下与a是指针兼容的(6.9.2)，则结果是指向b的对象。例如：

    ```c++
    T* p1 = new T;
    const T* p2 = static_cast<const T*>(static_cast<void*>(p1));
    bool b = p1 == p2;	// b = true。
    ```

    

    



## 其他出现`static_cast`位置

> P40
>
> 5.11 Keywords

`static_cast`是c++预留关键字。



> P53
>
> 6.3 Declarations and definitions
>
> 条目 3

出现在对C++自动生成的默认移动构造函数、移动赋值函数中，利用的是`static_cast<T&&>(expr)`强制转化为右值语义。



> P55
>
> 6.2 One-definition rule
>
> 条目 5.6

使用`static_cast`进行转换的某些情况下需要要求类型是完整的。



> P87
>
> 6.8 Object lifetime
>
> 条目 6.4

在对象内存已经分配但对象生命周期还未开始、对象生命周期已经结束但内存还未回首时的`static_cast`的行为。



> P106
>
> 8 Expressions
>
> 条目 7.4

说明了导致消亡值的一些情况，`stati_cast`的一种使用方法是其中之一。



> P121
>
> 8.2 Postfix expressions
>
> 条目 1、条目2

`static_cast`是从左至右结合的后缀表达式。

`static_cast<type-id>`中`type-id`后可能会出现符号`>>`会被理解为两个`>`而不是右移符号`>>`。



> P130
>
> 8.2.10 Reinterpret cast
>
> 条目6

`reinterpret_cast`的一种转换情况等价于使用两次连续的`static_cast`



> P141
>
> 8.4 Explicit type conversion (cast notation)
>
> 条目2、条目4.2、条目4.8、条目5

条目2：显示类型转换可以用`static_cast`。

条目4.2：说明`static_cast`也可以使用C风格的强制类型转换完成，除了某些特别情况，编译器采取相同的语义限制。

条目4.8：如果紧接着`static_cast`再进行`const_cast`且这个转换有不止一种转换路径，这种转换是非规范的。

例如:

```c++
struct A {};
struct I1 : A {};
struct I2 : A {};
struct D : I1, I2 {};
A *foo(D* p){
    return (A*)(p); 	// 非规范的`static_cast`转换。
}
```

条目5：当强制转换涉及到指向不完整类型的纯右值指针时，强制类型转换的特殊行为。



> P235
>
> 11.6 Initializers
>
> 条目16

通过`static_cast`等等方式（详细见标注文档）进行的初始化称之为直接初始化（direct-initialization）。



> P242
>
> 11.6.3 References
>
> 条目5.2.1.2

有关引用的初始化。



> P297
>
> 15.3.1 Conversion by constructor
>
> 条目2

有`explicit`修饰的构造函数可以被`static_cast`显示调用。



> P315
>
> 15.8.1 Copy/move constructors
>
> 条目14.2

`static_cast`参与到默认生成的复制构造函数、移动构造函数的行为中。



> P359
>
> 17.2 Names of template specializations
>
> 脚注136）

基本同P121的说明，同样是把的`>>`理解为两个`>`。



> P396
>
> 17.6.2.2 Type-dependent expressions
>
> 条目3.8

`static_cast`将导致类型依赖表达式（type-dependent expressions）



> P397
>
> 17.6.2.3 Value-dependent expressions
>
> 条目3

`static_cast<type-id>(expression)`是值依赖(value-dependent)的条件。



> P464
>
> 20.4.2.1.4 Bitmask types
>
> 条目2



> P476、478
>
> 20.5.3.5 Allocator requirements
>
> 表31 Allocator requirements、条目6

表31：对内存分配器的要求。

条目6：`static_cast`参与相等关系的判定。



以下条目都是使用`static_cast`来描述一些函数的效果：

* P491，21.2.5 byte type operations。
* P535，22.5.3.5 Class error_code non-member functions。
* P537，22.5.4.5 Class error_condition non-member functions。
* P545，23.2.5 Forward/move helpers，转发函数和移动函数。
* P575，23.6.3.5 Observers，条目19。
* P597，23.8.4 Non-member functions，条目6。
* P615，23.10.8.2 Allocator traits static member functions，条目5。
* P616-618，23.10.10 Specialized algorithms。
* P639，23.11.2.2.9 shared_ptr casts。
* P649，23.12.3.2 polymorphic_allocator member functions，条目1
* P660，23.13.4 Scoped allocator adaptor members，条目3、条目4
* P666，23.14.2 Function object, requirements，条目1.7
* P721，23.17.5.1 duration constructors，条目3
* P724，23.17.5.7 duration_cast，条目2
* P752，24.3.2.2 basic_string constructors and assignment operators，条目16
* P757，24.3.2.6.2，条目25
* P795，25.3.1.1.2 Class locale::facet，条目3
* P830，25.4.6.3 Class template moneypunct，条目1
* P987，27.5.3.3.4 move_iterator::operator*
* P1070，29.5.10 Suffixes for complex number literals，条目2、条目3、条目4
* P1187，30.5.6.5 Error reporting，条目1、条目2
* P1202，30.7.4.2.2 Arithmetic extractors， 条目2、条目3
* P1214，30.7.5.2.2 Arithmetic inserters，条目1
* P1290，30.10.15.7 Create directory，条目1
* P1293，30.10.15.14 File Size，条目1
* P1293，30.10.15.15 Hard link count
* P1297，30.10.15.31 Remove all
* P1298，30.10.15.34 Space
* P1405，33.6.3 Error handling



> P1427
>
> Annex A
>
> 列表 postfix-expression



> Index
>
> P1528、P1545

