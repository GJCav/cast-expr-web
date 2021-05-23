#include <iostream>
using namespace std;


class Base {};
class Sub : public Base {};

int main(){
    //main
    Base* pSub = new Sub;
    Sub* pCast = dynamic_cast<Sub*>(pSub); // E0698

    return 0;
}