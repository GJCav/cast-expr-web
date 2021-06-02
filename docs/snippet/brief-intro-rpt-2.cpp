#include <iostream>
using namespace std;

struct A {int a;};
struct B {int b;};
struct C : A, B {};

int main(){

    C c;

    cout << &c << " " << static_cast<B*>(&c) << " " << reinterpret_cast<B*>(&c) << endl;

    return 0;
}