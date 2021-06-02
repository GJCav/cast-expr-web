#include <iostream>
using namespace std;

class Base {
public:
	virtual void fun() {}
};
class Sub : public Base {};
//class BadGuy {};

class BadGuy {virtual void fun(){}};

int main(){
    BadGuy* pB = new BadGuy;
    Base* pD = dynamic_cast<Base*>(pB);
    cout << pD << endl;
    return 0;
}