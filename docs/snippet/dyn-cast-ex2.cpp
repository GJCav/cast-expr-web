#include <iostream>
using namespace std;

class Base {
public:
	virtual void fun() {}
};
class Sub : public Base {};
class BadGuy {};

int main(){
    BadGuy* pB = new BadGuy;
    Base* pD = dynamic_cast<Base*>(pB); // E0698
    return 0;
}