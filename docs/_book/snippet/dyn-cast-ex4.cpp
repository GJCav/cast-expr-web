#include <iostream>
using namespace std;

class Base {
public:
	virtual void fun() {}
};

class Sub : public Base {};

class NotThatBad {
public:
	virtual void fun() {}
};
class BadGuy {};

//main
int main(){

    // case 1
    NotThatBad* pNTB = new NotThatBad;
    Base* pBase1 = reinterpret_cast<Base*>(pNTB);
    Sub* pSub1 = dynamic_cast<Sub*>(pBase1); //pSub1 = nullptr

    // case 2
    BadGuy* pBad = new BadGuy;
    Base* pBase2 = reinterpret_cast<Base*>(pBad);
    Sub* pSub2 = dynamic_cast<Sub*>(pBase2); // Runtime Error

    return 0;
}