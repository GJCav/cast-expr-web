#include <iostream>
using namespace std;

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

void run1() {
    Derived obj;
    Base* ptr = &obj;

    // C style cast
    Derived* pd = (Derived*)ptr;
    pd->fun(); // output: Derived
    DDerived* pdd = (DDerived*)ptr;
    pdd->fun(); // Unexpected behavor
    pdd->bad(); // Runtime Error
}

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


int main() {

	return 0;
}

