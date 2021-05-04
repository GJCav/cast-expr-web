#include <iostream>
using namespace std;

class Base {
    public:
        virtual void fun() {cout << "Base" << endl;}
};

class Derived : public Base {
    public:
        void fun() override {cout << "Derived" << endl;}
};

class DDerived : public Derived {
    public:
        void fun() override {cout << "DeDerived" << endl;}
};

void run1(){
    
    Derived obj;
    Base *ptr = &obj;

    // C style cast
    Derived *pd = (Derived*)ptr;
    pd->fun(); // output: Derived
    DDerived *pdd = (DDerived*)ptr;
    pdd->fun(); // Runtime Error

    // dynamic_cast

}

int main(){
    run1();
    return 0;
}
