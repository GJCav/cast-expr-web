# 尴尬的向下转换

> 约定：
>
> * 向上转换：向基类转换
> * 向下转换：向子类转换



## 引言

在一个继承体系中，最常做的一件事就是把子类的对象付给基类类型的变量，通过多态性来实现程序的功能。这一步没有什么好说的，直接赋值就完事了。但也有迫不得已的尴尬情况：必须把一个父类类型的变量转换回子类类型，这样的向下转换可能是危险的，毕竟我们不知道变量指向的对象是不是真的是目标类型。此时我们面前的有两种转换运算符可以选择：

* `static_cast`
* `dynamic_cast`



二者的使用方法可以参照简要说明、详细说明和定义部分，这里仅做如下归纳：

1. 若能不使用向下转化，就不使用向下转化。
2. 若体系中无虚函数表，只能选择`static_cast`。
3. 若继承体系不是非常庞大，转换次数较少，首选`dynamic_cast`。
4. 若继承体系庞大，转换频繁，考虑使用`static_cast`以提高程序性能。

但其实一般来说都不需要考虑`dynamic_cast`的效率问题。



上述归纳的原因在于：

1. `static_cast`不会进行类型检查，所以容易出错。
2. `dynamic_cast`依托虚函数表工作，虽然运行时检查提高安全性，但一层层查看虚函数表会降低程序性能。



## `dyanmic_cast`性能问题

现在我们来实际实验一下`dynamic_cast`与`static_cast`的性能对比。

该小结对应文件夹路径为`./asset/dyn-eff`，并作为下文路径的根目录。



### 对比思路

构造一个具有100层继承关系的复杂继承体系，使每个类都有自己的虚方法来让虚函数表膨胀，然后把用根父类指针指向一个叶子子类的对象，然后重复不断地转化回某个合法的子类对象(这一步目的是让`static_cast`也能合理地转化指针，简化代码)，记录程序使用时间。



### 实际操作

由于代码又臭又长，下文都不展示代码。

先使用`python`生成继承体系，对应`code-gen`文件夹。

`stt-cast.cpp`用于测试`static_cast`，`dyn-cast.cpp`用于测试`dynamic_cast.cpp`。被测量部分代码都被重复十万次，保证效果明显。且借助`assert`防止编译器优化代码。在MSVC Debug，关闭所有优化选项下编译运行。



### 测试结果 & 结论

结果大致如下：

* `static_cast`：0.004s
* `dynamic_cast`：0.3s

确实`dynamic_cast`效率远低于`static_cast`。

但说实话，如果程序需要进行这么多次向下转换，我觉得是不是得考虑重构代码了。所以一般来说我们不需要考虑`dynamic_cast`的效率问题。



