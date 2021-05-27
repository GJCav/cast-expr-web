# `dynamic_cast`详细描述

> Ref: C++ Primer Plus
>
> Chapter 15. Friends, Exceptions, and More
>
> * Runtime Type identification



## 引入目的：RTTI

RTTI: Runtime type identification，运行时类型检测

目的：The intent of RTTI is to **provide a standard way** for a program to determine the type of object during runtime.



RTTI如何工作：

由三个部分支撑：

* `dynamic_cast`运算符：如果可能`dynamic_cast`将一个指向基类类型的指针转化为指向子类类型的指针。如果这种装换无法完成，返回`nullptr`。
* `typeid`运算符：返回描述一个实例的类型的值
* `type_info`结构体：描述某个类型的信息

注：只能将RTTI用于拥有虚函数的继承体系。理由：只有在拥有虚函数的继承体系下，将子类实例的地址赋给基类指针才是有意义的。且RTTI依托虚函数表工作。



## `dynamic_cast`运算符

虽然该`dynamic_cast`不会告诉我们一个指针指向的实例的类型，但它会回答是否能安全地将一个实例的地址赋给某个类型的指针的问题。值得注意的是，“判断一个类型转换是否安全”比“精确获取实例类型”更为广泛和有用。因为我们希望获取实例类型的原因在于我们想知道调用一个函数是不是可行的。



### 指针形式

语法：

```c++
// pt is a pointer pointing to an object
Type *pm = dynamic_cast<Type*>(pt);
```

如果`*pt`实例是`Type`类型或者是其直接或间接公共继承的子类，上述语句将`pt`转化为一个`Type*`类型的指针，否则，返回`nullptr`。

`dynamic_cast`主要用于向下转型，即：从基类到子类的类型转换。



**注：**只有在拥有**虚函数**继承体系中才能使用`dynamic_cast`，因为`dynamic_cast`依赖**虚函数表**工作。

例如下述代码会报错(msvc编译)：`E0698：运行时 dynamic_cast 的操作数必须包含多态类型`

```c++
//snippet: dyn-cast-ex1.cpp
class Base {};
class Sub : public Base {};

//main
Base* pSub = new Sub;
Sub* pCast = dynamic_cast<Sub*>(pSub); // E0698
```

*在拥有虚函数的继承体系*，更进一步，指的是`dynamic_cast`的目标、源都需要具有**虚函数表**，否侧`dynamic_cast`不会正常工作。例如：

```c++
//snippet: dyn-cast-ex2.cpp

class Base {
public:
	virtual void fun() {}
};
class Sub : public Base {};
class BadGuy {};

//main
BadGuy* pB = new BadGuy;
Base* pD = dynamic_cast<Base*>(pB); // E0698
```

但上述代码第6行若改为

```c++
//snippet: dyn-cast-ex3.cpp

class BadGuy {
public:
    virtual void bad(){}
};
```

编译时不会提示任何错误，运行时`dynamic_cast<Base*>(pB);`的值为`nullptr`，符合我们的预期。



虽然有这些比较麻烦的设置，但情况还不是非常严峻，编译器能够给我们非常确切的提示。

但如果将`reinterpret_cast`也牵扯进来，事情就复杂起来了，见如下代码：

```c++
//snippet: dyn-cast-ex4.cpp
class Base {
public:
	virtual void fun() {}
};

class Sub : public Base {};

class NotThatBad {
public:
	virtual void fun() {}
};
class BadGuy {};

//main
// case 1
NotThatBad* pNTB = new NotThatBad;
Base* pBase1 = reinterpret_cast<Base*>(pNTB);
Sub* pSub1 = dynamic_cast<Sub*>(pBase1); //pSub1 = nullptr

// case 2
BadGuy* pBad = new BadGuy;
Base* pBase2 = reinterpret_cast<Base*>(pBad);
Sub* pSub2 = dynamic_cast<Sub*>(pBase2); // Runtime Error
```

`case 1`虽然进行了危险`reinterpret_cast`跨继承进行转换，但是`dynamic_cast`仍然正常执行了，毕竟`NotThatBad`类型实例具有虚函数表，能支持`dynamic_cast`的工作。

但`case 2`在运行时出现内存错误，这是因为`BadGuy`类型实例不具备虚函数表，当`dynamic_cast`尝试通过虚函数表获取实例的类型时便触发内存错误。



### 引用形式

`dynamic_cast`也可以作用于引用，与指针形式稍有区别，因为不存在空的引用，所以当`dynamic_cast`转换失败时将抛出`bad_cast`异常。

`bad_cast`继承自`exception`且在`typeinfo`头文件中定义(有趣的是我们很容易间接引入`bad_cast`，因为`iostream`头文件间接包含了`bad_cast`)。

参考代码如下：

```c++
#include <typeinfo>
...
try{
    // suppose rg is of type: TypeB&
    TypeA& ra = dynamic_cast<TypeA&>(rg);
}catch(bad_cast& ){
    
}
```



