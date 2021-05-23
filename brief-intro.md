# 类型转换运算符 简明介绍

> 主要参考资料：
>
> 1. C++ Primer Plus 6th Edition
> 2. [C语言中文网](http://c.biancheng.net/cpp/biancheng/view/3297.html)



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

> 向上转化时不常用`dynamic_cast`，因为大多数向上转化都是隐式进行的，完全没有必要使用任何类型转换运算符。
>
> 向下转化时也可以使用`static_cast`，但`static_cast`不会进行运行时检查，所以有一定风险。

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
}catch(bad_cast& e){
    // handle failed cast
}
```

使用上与指针形式基本没有区别，出了因为不存在空的引用，所以当`dynamic_cast`转换失败时将抛出`bad_cast`异常。`bad_cast`继承自`exception`且在`typeinfo`头文件中定义(有趣的是我们很容易间接引入`bad_cast`，因为`iostream`头文件间接包含了`bad_cast`)。



### 使用限制

`dynamic_cast`有如下使用限制：

1. 必须在存在虚函数表的继承体系中使用。
   首先如果没有继承体系那么完全不会出现向上、向下类型转换的需求。其次，因为`dynamic_cast`依托虚函数表进行工作，即使要求是少要有虚函数。若转化类型缺失虚函数表，一般会编译失败，但也存在编译通过发生运行时错误的情况。
2. 不能混用指针形式语法和引用形式语法。
3. 转换目标必须是引用或者指针。



### 参考

1. [详细说明](dynamic_cast.md)
2. [标准定义](ISO dynamic_cast.md)
3. [摘要页](README.md)



## `const_cast`

### 目的

该类型转换运算符的目的单纯是为了改变指针或引用的`const`性和`volatile`性。

`const_cast`表达式的目的是偶尔我们会有修改一个大部分时间都是`const`变量的需求，在这种情况下，我们可以把这个变量声明为`const`的然后在需要修改的时候使用`const_cast`。当然我们可以使用C语言风格的类型转换，但使用`const_cast`总能在我们头脑发热时为我们检查一下，给出一些错误信息或者警告。

### 语法

`const_cast`的语法如下：

```c++
const_cast<type-name>(expression)
```

其中`type-name`和`expression`除了`cv-qualifier`之外必须是同一个指针或引用类型，如果条件不满足，一般来说编译器会报错。

例如：

```c++
// snippet: brief-intro-con-1.cpp, line 4-13.
// Low 是 High 的基类
High bar;
const High *pBar = &bar;
// ...
High *pb = const_cast<High*>(pbar);				// valid
const Low * pl = const_cast<const Low*> (pBar);	// invalid
```

第一个转换得到一个可以用来修改`bar`对象的指针`pb`，`const`的限定被移除掉了。第二是转换是非法的，因为它尝试从`const High*`转化到`const Low*`。

### 使用限制

但是`const_cast`对`const`性的移除也不是非常强大，在使用时甚至会有一些未定义行为。`const_cast`允许我们修改一个指针对一个量的可访问性，但修改一个别声明为`const`的量的值是一个未定义行为，例如下述代码。

```c++
// snippet: brief-intro-con-1.cpp
void change(const int* p, int d) {
    int* pc = const_cast<int*>(p);
    *pc += d;
}

void run2() {
    int valA = 100;
    const int valB = 100;
    int *pB = const_cast<int *>(&valB);

    cout << valA << " " << valB << endl;
    change(&valA, 20);
    change(&valB, 20);
    cout << valA << " " << valB << endl;
    cout << "*pB = " << *pB << endl;
}
```

在`change`中，因为`const_cast`移除了`pt`的`const`性，所以编译器允许`*pc += n;`语句的存在。我们预期的输出大多是这样的：

```
100 100
120 120
*pB = 120
```

但在MSVC、g++、clang编译运行结果：

```
100 100
120 100
*pB = 120
```

这下代码就迷惑起来了，`valB`荣获矛盾文学奖，它变了，但又没变。这样的输出是因为编译器会吧`const`的在编译器就翻译为了它的值，也就是说`valB`在编译时就变成了`100`，所以运行时修改了他的值输出也不会体现出来。但以上解释都是瞎说，在ISO C++定义中明确表示这样的代码导致未定义行为，虽然主流编译器下通过指针访问都得到了我们期待的值，但最好不要出现这样的代码。

> 注：这个例子来自C++ Primer Plus，但书中的解释有些小问题，编译器并没有保护const int的值不被改变，通过指针访问可以说明这一问题。



### 参考

1. [详细说明](const_cast.md)
2. [标准定义](ISO const_cast.md)
3. [摘要页](README.md)



## `static_cast`

### 目的

用来进行编译器可以进行的隐式转换，但不能修改变量的cv修饰符。



> 注：C语言中文网中说`static_cast`是在编译期进行的转换，这句话多半有误，因为`static_cast`可能会调用构造函数等函数，这些函数都是在运行时执行的，所以`static_cast`并不意味着在编译时进行转换。



### 语法

```c++
static_cast<type-name>(expression);
```

上述转换只有在从`expression`到`type-name`存在一系列的隐式转换、或存在反方向的一系列隐式转换、或有用户自定义转换存在时，这个语句才是合法的。假设`High`是`Low`的基类，`Pond`是与两者无关的类，那么从`High`到`Low`或反方向转换时可行的，但不允许从`Low`到`Pond`的转换。

注：上述转换不太准确，准确的定义见[ISO static_cast](./ISO static_cast.md)

```c++
//snippet: brief-intro-static-1.cpp

High bar;
Low blow;
High * pb = static_cast<High *>(&blow);		// 正确：向上转型
Low * p1 = static_cast<Low *>(&bar);		// 正确：向下转型
Pond * pmer = static_cast<Pond *>(&blow);	// 编译错误
```

第一个转换是向上转型，可以隐式进行，所以是合法的。第二个转换是向下转换，必须显示转换，但因为其逆向的转换是合法的，所以也能使用`static_cast`。

相似的，如下转换也能进行：

1. 因为一个枚举类型的值可以被隐式地转换为整形，所以一个整形可以使用`static_cast`转换未枚举类型。

2. 进行一些列数值类型的转换，例如：`double`到`int`、`float`到`long`等。

3. `void`类型指针与具体指针类型之间的转换，例如：`void*`转`int*`，`char*`转`void*`等。但`static_cast`不能直接在两个具体指针类型之间转换，例如从`int*`到`char*`。也不能直接把一个整型转换为指针。

4. 如果存在自定义的转换运算符或转换构造函数，也能使用`static_cast`进行转换，此时会直接调用这些转换\构造函数，例如：

   ```c++
   //snippet: brief-intro-static-2.cpp
   
   struct vector {int a, b;};
   
   class Complex{
   public:
       int real = 0, imag = 0;
   
       Complex(){}
       Complex(vector &v) {
           real = v.a; 
           imag = v.b; 
           cout << "Constructor" << endl;
       }
   
       operator double(){
           cout << "Type conversion operator." << endl;
           return real * real + imag * imag;
       }
   };
   
   ...
   vector v = {1, 2};
   Complex c = static_cast<Complex>(v);	// 调用了Complex的构造函数
   cout << "real: " << c.real << " imag: " << c.imag << endl; 
   double lenSqr = static_cast<double>(c);	// 调用了Complex的转换运算符
   cout << "len ^ 2 = " << lenSqr << endl;
   ```

   输出为：

   ```
   Constructor
   real: 1 imag: 2
   Type conversion operator.
   len ^ 2 = 5
   ```

   



### 使用限制

1. 不能修改cv修饰符

2. 向下转换时不会进行类型检查，可能造成运行时错误。例如：

   ```c++
   //snippet: brief-intro-static-1.cpp
   struct High {
       int a = 10;
   };
   struct Low : High {
       int b = 20;
   };
   int main(){
       High bar;
       Low * p1 = static_cast<Low *>(&bar);
       cout << p1->b << endl; // 未定义行为。
       return 0;
   
   ```

3. ... 见[ISO static_cast](ISO static_cast.md)



### 参考

1. [详细说明](static_cast.md)
2. [标准定义](ISO static_cast.md)
3. [摘要页](README.md)



## `reinterpret_cast`

### 目的

`reinterpret_cast`将会进行一些具有极大风险性的类型转换。虽然这个运算符不能去除常量性，但它允许一些极其怪异的转换。有时一个程序需要做一些依赖于实现的事情，此时使用`reinterpret_cast`可以简化这种操作。



### 语法

```c++
reinterpret_cast<type-name>(expression);
```

`reinterpret_cast`进行转换时仅仅是**对数据二进制位的重新解释**，一般不会借助转换规则来调整对象。

使用示例：

```c++
//snippet: brief-intro-rpt-1.cpp

struct dat {short a; short b;};

long value = 0xA224B118;
dat *pd = reinterpret_cast<dat*>(&value);
cout << hex << pd->a; // 输出 value 的前两个字节的数据
```

一般来说，`reinterpret_cast`会用于底层的、实现依赖的以及不可迁移的程序。就连上述的例子在小端机和大端机上的行为都不同。例如在作者的操作系统下，输出为`b118`说明我的机器为小端机，但在其他的电脑上有可能输出为`A224`。





### 使用限制

1. 把一个指针转换为一个整型时，这个整型需要足够大，保证能储存指针的值。

2. 不能把指针转换为浮点类型。

3. 不能把函数指针转换为普通的指向数据类型的指针。

4. 不要尝试在继承体系中使用`reinterpret_cast`，例如：

   ```c++
   //snippet: brief-intro-rpt-2.cpp
   
   struct A {int a;};
   struct B {int b;};
   struct C : A, B {};
   
   ...
   cout << &c << " " << static_cast<B*>(&c) << " " << reinterpret_cast<B*>(&c) << endl;
   ```

   某次运行结果：

   ```
   0097FC20 0097FC24 0097FC20
   ```

   向上转换时`static_cast`正确地偏移了指正，但是`reinterpret_cast`没有。注：这里也不能使用`dynamic_cast`因为没有虚函数表。



### 参考

1. [详细说明](reinterpret_cast.md)
2. [标准定义](ISO reinterpret_cast.md)
3. [摘要页](README.md)







