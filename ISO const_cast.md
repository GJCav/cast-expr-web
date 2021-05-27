# ISO const_cast

> ISO/IEC 14882 中对const_cast的描述



## 对const_cast的定义

> P131
>
> 8.2.11 Const cast

1. 表达式`cast_cast<T>(v)`的结果的类型是 `T`。如果`T`是一个对象的左值引用，结果是一个左值；如果`T`是一个对象的右值引用，结果是一个消亡值；除上述情况外，结果是一个纯右值且会对`v`执行从左值到右值、从数组到指针或从函数到指针的标准转换。仅有下述列出的转换可以通过显示调用`const_cast`完成。

2. 注：对于本小节，一个表达式可以使用`const_cast`转换为表达式本身的类型。

3. 对于两个相似(similar)的类型`T1`、`T2`，`T1`的一个纯右值可以使用`const_cast`显式地转换为`T2`。其结果指向原来的实体(entity)。例：

   ```c++
   typedef int *A[3];				// 指向int的长度为3的指针数组
   typedef const int *const CA[3];	// 指向const int的长度为3的常量指针数组
   
   CA &&r = A{};	// 正确，在限定符转换后引用绑定到CA类型的临时数组对象上
   A &&r1 = const_cast<A>(CA{});	// 错误：临时数组退化为指针
   A &&r2 = const_cast<A&&>(CA{}); // 正确
   ```

   > 译注：相似的类型，指经过限定符分解后，指针层数一致且基本类型一致的两个类型。即，两个多级指针仅仅某些层上的cv修饰符有区别。该概念定义在 标准`7.5 Qualification conversions `中。

4. 对于两个类型分别为`T1`和`T2`的对象， 如果一个指向`T1`的指针可以使用`const_cast`显式转换为指向`T2`的指针，那么也可以使用执行下述转换：

   1. 使用`const_cast<T2&>`把一个`T1`的左值转换为`T2`的左值。
   2. 使用`const_cast<T2&&>`把一个`T1`的泛左值转换为一个`T2`的消亡值。
   3. 如果`T1`是一个类类型(class type)，可以使用`const_cast<T2&&>`把`T1`的纯右值转换为`T2`的消亡值。

   上述转换结果都会引用至原对象，或者是在`v`是泛左值的情况下引用至采用临时变量具体化转换(temporary materialization conversion)后的对象。

5. 空指针将会转化为目标类型的空指针，对于成员指针(pointer to member)同理。

6. 注：依赖于具体的对象类型，对通过使用`const_cast`去除`const`限定得到的指针或引用进行写操作可能会导致未定义行为。

7. 如果`T1`和`T2`不同，`T1`的cv分解(cv-decomposition, 7.5 Qualification conversions)导出 n 使得`T2`具有形如：

   ​		$cv_0^2\ \ P_0^2\ \ cv_1^2\ \ P_1^2\ \cdots\ cv_{n-1}^2\ \ P_{n-1}^2\ \ cv_n^2\ \ U_2$

   的cv分解，且不具备能将`T1`转换为

   ​		$cv_0^2\ \ P_0^1\ \ cv_1^2\ \ P_1^1\ \cdots\ cv_{n-1}^2\ \ P_{n-1}^1\ \ cv_n^2\ \ U_1$

   的限定符转换，使用`const_cast`将`T1`转换为`T2`会去除常量性。

   > 译注：参考 [stackoverflow: How to understand...](https://stackoverflow.com/questions/53127801/how-to-understand-expr-const-cast-clause-8-in-c-standard-n3337)

8. 如果将“指向`T1`的指针”的纯右值转换为“指向T2指针”类型时会移除常量性，则使用左值引用转换将`T1`的一个左值转换为`T2`的一个右值或者使用右值转换将T1的表达式转换为T2类型的消亡值时也会移除常量性。

9. 注：一些仅仅涉及到修改cv限定符的转换也不能使用`const_cast`。例如：函数指针间的转换不能使用`const_cast`，因为这会导致未定义行为。同样的道理，成员函数指针间的转换，特别是将一个常量函数指针转换为非常量函数同样不能使用`const_cast`。



## 其余出现位置

> P40
>
> 5.11 Keywords

`const_cast`是一个关键词。



> P89
>
> 6.8 Object lifetime

在常量对象所在内存上覆盖新建对象会造成未定义行为。



> P121
>
> 8.2 Postfix expression

`const_cast`是从左到右结合的后缀表达式。



> P141
>
> 8.4 Explicit type conversion(cast notation)

表明`const_cast`功能被强制类型转换覆盖。

即：`(new-type)expr`覆盖`const_cast<new-type>(expr)`功能。



> P178
>
> 10.1.7.1 The cv-qualifiers

定义cv修饰符。



> 359
>
> 17.2 Names of template specialization

说明形如`const_cast<const_cast<T>>`最后的那个`>>`被解释为两个`>`而不是右移符号`>>`。



> P372
>
> 17.5.3 Variadic templates

出现在变参模板的一个解释例子中，和`const_cast`本身关系不大。



> P396
>
> 17.6.2.2 Type-dependent expression

`const_cast`是类型依赖表达式的条件，和`const_cast`本身关系不大。



接下来是出现一系列函数的实现使用到了`const_cast`

* P633, 23.11.2.2.1 shared_ptr constructors
* P639, 23.11.2.2.9 chared_ptr casts
* P840, 26.2.1 General container requirements, 实现常量迭代器
* P890, 26.3.9.3 forward_list iterators
* P1229, 30.8.3.3 basic_istringstream, Member functions
* P1231, 20.8.4.3 basic_ostringstream, Member functions
* P1233, 30.8.5.3 stringstream, Member functions
* P1242, 30.9.3.3 ifstream, Member functions
* P1244, 30.9.4.3 ofstream, Member functions
* P1247, 30.9.5.3 fstream, Member functions
* P1476, D.6.2.2 istrstream, Member functions



> P1427
>
> Annex A

仅仅是罗列在后缀表达式中。



> P1452
>
> C.1.9 Clause 15: special member functions

有关`volatile`对象的复制问题。



> P1523
>
> Index





