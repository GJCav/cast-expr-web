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



