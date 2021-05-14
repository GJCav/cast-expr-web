# ISO static_cast

> Ref: ISO/IEC 14882



## 定义

> P127
>
> 8.2.9 Static cast

1. `static_cast<T>(v)`的结果就是把v转换为T的结果。如果T是左值引用类型或者引用至函数的右值引用类型，转换结果是左值。如果T是对对象的右值引用，结果是消亡值。其他情况下，结果是纯右值。`static_cast`不能去除常量性。

2. 假设B是一个类类型(class type)、D是B的一个子类，在`cv2`等于`cv1`或限制比`cv1`更强时，一个`cv1 B`类型的左值可以转换为`cv2 D`的引用类型。如果B是D的虚基类、D的虚基类的基类，或者如果不存在从“指向D的指正”向“指向B的指正”的标准转换，那么程序是非规范的。一个`cv1 B`类型的消亡值可以被转换为与类型`cv1 B`拥有相同限制的`cv2 D`的右值引用类型。如果`cv1 B`的一个对象确实是类型D的一个对象的基类子对象，转化结果将指向那个封闭(enclosing)类型D的对象。其他情况下，转化行为是未定义的。例如：

   ```c++
   struct B{};
   struct D : public B{};
   D d;
   B &br = d;
   static_cast<D&>(br); // 结果是指向最初d对象的左值引用。
   ```

3. 





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

