# 静态多态性

> Ref：[Curiously recurring template pattern](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern#Static_polymorphism)



借助模板和`static_cast`，我么可以实现有别于虚函数表的另一种多态：`静态多态`。

见如下代码：

```c++
//snippet: static-poly.cpp

template<class T>
class Base {
public:
    void greeting() {
        static_cast<T*>(this)->hello();
    }

    static void static_greeting(){
        T::static_greeting();
    }
};

class Bob : public Base<Bob> {
public:
    void hello(){
        cout << "This is Bob." << endl;
    }

    static void static_greeting(){
        cout << "This is static Bob." << endl;
    }
};
...
Base<Bob> *bob = new Bob;
Base<Alice> *alice = new Alice;

bob->greeting();
alice->greeting();

Base<Bob>::static_greeting();
Base<Alice>::static_greeting();
```

输出：

```
This is Bob.
This is Alice.
This is static Bob.
This is static Alice.
```



这就完成了静态多态的一种实现方法。这种静态多态的效率比通过虚函数表实现的动态多态要高一些，但是会失去一些灵活性。

Windows的ATL库和WTL库广泛采用了这种技术。

