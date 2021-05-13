# `static_cast` 详细说明

> Ref: https://en.cppreference.com/w/cpp/language/static_cast

## 功能

通过一系列隐式转换或者用户自定义转换执行类型转换。



**Syntax**

```c++
static_cast<new_type>(expression)
```

返回`new_type`的值。



## 功能细节

只有下述转换在不涉及cv修饰符更改时可以使用`static_cast`完成：

1. 如果存在从`expression`到`new_type`的一些列隐式转换，或者`new_type`至少重载一个函数能直接从`expression`的值或引用初始化这个`new_type`类型，那么`static_cast<new_type>`返回一个假想的如同使用`new_type Temp(expression);`构造的`Temp`对象。这可能会涉及到隐式类型转换，`new_type`的构造函数或是用户自定义类型转换运算符。对于非引用类型的`new_type`，对纯右值`expression`类型的转化对象即是直接初始化的对象。

2. 如果`new_type`是某个D类型的引用或者指针，且`expression`是它的非虚基类B或是指向它的纯右值指针，`static_cast`会执行一个向下转换（当B有歧义的、不可访问、D的虚基类、D的虚基类的基类时，这个向下转化是非规范的）。这种向下转换不会进行运行时检查来确保这个对象的类型确实是D，所以此时的向下转换只能安全地用于上述条件能被其他方法保证时，例如静态多态性。安全的向下转换可以使用`dynamic_cast`。

   ```c++
   struct B{};
   struct D : B {};
   D d;
   B& br = d;
   static_cast<D&>(br); // 表示原来的d对象的左值。
   ```

3. 如果`new_type`是右值引用类型，`static_cast`会把泛左值、类类型纯右值、数组类型纯右值、任何左值类型的`expression`转换为代表原对象的消亡值，或原对象的基类子对象。如果目标类型是`expression`的不可访问或有歧义基类，该程序是非规范的。如果`expression`是一个位域(bit field)类型左值，会先转化为底层类型的纯右值。`std::move`移动语义就是用`static_cast`实现。

4. 如果`new_type`是`void`类型，`static_cast`会在对`expression`求值后直接抛弃这个值。

5. 如果存在一个从`new_type`到`expression`的标准转换([standard conversion](https://en.cppreference.com/w/cpp/language/implicit_conversion))链，且这个转换中不包含左值到右值、数组到指针、函数到指针、空指针、空成员指针、函数指针和布尔转换，则`static_cast`也能进行逆向的隐式转换。

6. 如果一个从`expression`到`new_type`涉及到左值到右值、数组到指针、函数到指针的转换，这个转换可以显式地通过`static_cast`完成。

7. 限域枚举类型可以被转换为整型或浮点型。

   > 到C++ 20为止：
   >
   > ​	当目标类型为`bool`，`expression`值为0时转化结果为`false`，为其他值时结果为`true`。对于其他的整型类型，如果它能被目标类型表示，则转换结果是枚举的值，否则是未指明的。
   >
   > 从C++ 20开始：
   >
   > ​	转化结果同从枚举的底层类型到目标类型的隐式转换结果相同。

8. 

