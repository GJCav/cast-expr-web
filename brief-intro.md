# 类型转换运算符

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

一下给出四种转换的**简明**说明，详细说明见如下链接：

1. [dynamic_cast](./dynamic_cast.md) <---- 这个地方需要大修
2. 



## `dynamic_cast`

`dyanmic_cast`运算符主要用于**向下转化**，也即使从基类类型到子类类型的转化。假设有如下继承结构：

```c++
// snippet: brief-intro-dyn-1.cpp
class Base {
public:
    void 
}
```



