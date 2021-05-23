# C++ 类型转换运算符 研究报告

> Author: 顾家铭
>
> Id: 2020013059
>
> E-mails: gjm20@mails.tsinghua.edu.cn
>
> Phone: 18810623126



## 摘要

在C语言中的类型转换非常简单，只需要形如`(new-type)expression`的代码即可完成各种类型的转换，这样简洁的语法给予程序员便捷的同时也引入了风险：有些转换是非常怪异但是有用的，有些却会造成致命的Bug。C++通过对各种类型转换进行归类，引入了四个新的类型转换运算符来减少类型转换中可能出现的错误。

* dynamic_cast：借助RTTI，进行安全地向下转型。
* const_cast：用于修改变量的cv修饰符。
* static_cast：进行隐式转换或自定义转换。
* reinterpret_cast：风险极高的对二进制位的重新解释。



## 本报告的组织方式

### 目录结构

目录结构及其说明如下：

cast-expr/

- archive/：参考文献保存，包括网页快照、书籍等资料
- asset/：报告中使用的图片等资源
- snippet/：示例代码
- [brief-intro.md](./brief-intro.md)：对四个转换的简要说明
- [dynamic_cast.md](./dynamic_cast.md)：对运算符的详细说明，下同
- [const_cast.md](./const_cast.md)
- [static_cast.md](./static_cast.md)
- [reinterpret_cast.md](reinterpret_cast.md)
- [ISO dynamic_cast.md](ISO dynamic_cast.md)：C++标准中的定义，下同
- [ISO const_cast.md](./ISO const_cast.md)
- [ISO static_cast.md](ISO static_cast.md)
- [ISO reinterpret_cast.md](ISO reinterpret_cast.md)



### 简要-详细-定义 设计

对四个运算符的说明都分为三个部分：简要说明、详细说明、定义。其中：

`brief-intro.md`中包含了所有的简要说明。简要说明的目的是让没有了解过这四个运算符的人看完之后可以**不考虑细节**地直接使用这三个运算符，简要说明涵盖了运算符最常见的使用方式和最常见的误用方式，足以覆盖大多数情况。简要说明中基本不会涉及到“左值”、“右值”等的细节，避免加大理解难度。

详细说明部分除了`dynamic_cast`外，基本翻译自`cppreference`网站。这一部分的目的是明确各个表达式的具体行为细节但又不至于直接援引标准文档，且`cppreference`中给出了比标准文档更加人性化的解释。这里的”具体细节“包括返回的值到底是左值、右值，执行过程是否会创建临时变量，多级指针等内容。至于`dynamic_cast`的详细说明摘自`C++ primer plus`，说明了RTTI的简单使用。

定义部分。这部分翻译自C++标准文档，记录了对应运算符在标准的中的所有出现位置。



### 推荐阅读顺序

1. [简要说明](./brief-intro.md)
2. 运用实例，由简要说明部分跳转，或通过如下链接：
   1. [向上、向下转换](updown-cast.md)
   2. 静态多态性
   3. 大小端检测
3. 详细说明或标准定义，由简要说明部分跳转。





## 一些约定

在`定义`部分，所有页码无额外说明时指的是PDF页码，从封面开始编号，方便跳转。

