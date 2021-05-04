# 类型转换运算符

> 主要参考资料：C++ Primer Plus 6th Edition



## 导入

C++兼容C大部分语法，这意味着C++中也能使用C语言风格的类型转换。即`(type-name)var-name`形式的强制类型转换。但这个转换在C++之父本贾尼·斯特劳斯特卢普看来是过于草率的。考虑如下代码片段：

```c++
struct Data{
    double data[200];
};

struct Junk{
    int junk[100];
};
Data d = {2.5e33, 3.5e-19, 20.2e32};
char *pch =  (char*)(&d);	// 情况1
char ch = (char)&d;			// 情况2
Junk *pj = (Junk*)(&d);		// 情况3
```

上述代码每一个是有明确意义的，但上述转换对C语言都是符合、可以编译的。为了消除这种草率的类型转换，C++引入了四种更加规范的类型转换方式：

* dynamic_cast
* const_cast
* static_cast
* reinterpret_cast

C++更加推荐根据实际情况使用以上四种类型转换而不是使用通用类型转换，因为这能表现出类型转换的目的并让编译器有机会帮我们确认代码的行为和我们的预期相符。



## `dynamic_cast`

### 目的

`dynamic_cast`运算符主要用于**向下转化**，也即使从基类类型到子类类型的转化。

> 向上转化时不常用`dynamic_cast`，因为大多数向上转化都是隐式进行的，

假设有如下继承结构：

```c++
// snippet: brief-intro-dyn-1.cpp, line 4-19.
class Base {
public:
    virtual void fun() { cout << "Base" << endl; }
};

class Derived : public Base {
public:
    void fun() override { cout << "Derived" << endl; }
};

class DDerived : public Derived {
public:
    void fun() override { cout << "DeDerived" << endl; }
    void bad() { cout << "dangerous: " << a << endl; a = 10; }
    int a = 0;
};
```

考虑如下代码：

```c++
// snippet: brief-intro-dyn-1.cpp, function run1.
void run1() {
    Derived obj;
    Base* ptr = &obj;
    
    // after some complicated process

    // C style cast
    Derived* pd = (Derived*)ptr;
    pd->fun(); // output: Derived
    
    DDerived* pdd = (DDerived*)ptr;
    pdd->fun(); // Unexpected behavor
    pdd->bad(); // Runtime Error
}
```

我们先创建一个`Derived`类型对象`obj`并用基类指针`ptr`指向它，经过一系列复杂的过程，甚至在该函数之外，我们想要把`Base*`类型指针向下转化为`Derived*`类型。这是如果我们头脑清醒，或者使用其他一些方法（比如添加一个获取类型描述字符串的方法）能够确定`ptr`指向的是一个`Derived`类型对象，那么我们当然可以直接使用C风格的类型转换，不会出现任何问题。上述代码第10行能正常执行。但如果程序非常复杂，我们不小心把`ptr`指向对象类型当成了`DDerived`，然后又进行C风格类型转换，编译器不会给出任何错误信息，甚至第13行代码都能够正确输出，直到14行，尝试修改一个`a`的值时才发生`Runtime Error`。这样的转换风险是极高的，说不定什么地方一个疏忽，就造成了一个隐蔽的bug。而此处正是`dynamic_cast`发挥作用的时候。



### 语法

`dynamic_cast`语法非常简单，**指针形式**语法如下：

```c++
// pt is a pointer pointing to an object
Type *pm = dynamic_cast<Type*>(pt);
```

如果`*pt`实例是`Type`类型或者是其直接或间接公共继承的子类，上述语句将`pt`转化为一个`Type*`类型的指针，否则，返回`nullptr`。

现在使用如下转换代码：

```c++
// snippet: brief-intro-dyn-1.cpp, function run2.
void run2() {
    Derived obj;
    Base* ptr = &obj;

    // dynamic_cast
    Derived* pd = dynamic_cast<Derived*>(ptr);
    if (pd == nullptr) { cout << "cast to Derived failed" << endl; } 
    else pd->fun();

    DDerived* pdd = dynamic_cast<DDerived*>(ptr);
    if (pdd == nullptr) { cout << "cast to DDerived failed" << endl; } 
    else pdd->bad();
}
```

上述代码完美符合预期，即使我们写程序时忘记`ptr`指向对象的类型，我们也可以大胆使用`dynamic_cast`进行类型转化，转化成功皆大欢喜，转化失败也由明确的`nullptr`指明，不会向代码中埋下难以预测的Bug，`dynamic_cast`的最大用处便在此处。



除了指针形式，`dynamic_cast`还有**引用形式**，语法如下：

```c++
#include <typeinfo>
...
try{
    // suppose rg is of type: TypeB&
    TypeA& ra = dynamic_cast<TypeA&>(rg);
}catch(bad_cast& ){
    // handle failed cast
}
```

使用上与指针形式基本没有区别，出了因为不存在空的引用，所以当`dynamic_cast`转换失败时将抛出`bad_cast`异常。`bad_cast`继承自`exception`且在`typeinfo`头文件中定义(有趣的是我们很容易间接引入`bad_cast`，因为`iostream`头文件间接包含了`bad_cast`)。



### 使用限制

`dynamic_cast`非常强大，但也有如下使用限制：

1. 必须在存在虚函数表的继承体系中使用。
   首先如果没有继承体系那么完全不会出现向上、向下类型转换的需求。其次，因为`dynamic_cast`依托虚函数表进行工作，即使要求是少要有虚函数。若转化类型缺失虚函数表，一般会编译失败，但也存在编译通过发生运行时错误的情况。
2. 不能混用指针形式语法和引用形式语法。
3. 转换目标必须是引用或者指针。



