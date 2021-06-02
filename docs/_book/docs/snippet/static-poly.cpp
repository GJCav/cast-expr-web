#include <iostream>
using namespace std;

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

class Alice : public Base<Alice> {
public:
    void hello(){
        cout << "This is Alice." << endl;
    }

    static void static_greeting(){
        cout << "This is static Alice." << endl;
    }
};

int main(){

    Base<Bob> *bob = new Bob;
    Base<Alice> *alice = new Alice;

    bob->greeting();
    alice->greeting();

    Base<Bob>::static_greeting();
    Base<Alice>::static_greeting();


    return 0;
}