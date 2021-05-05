#include <iostream>
using namespace std;

class Low {};
class High : public Low {};

void run1() {
	High bar;
	const High* pBar = &bar;
	// ...
	High* pb = const_cast<High*>(pBar);				//valid
	//const Low* p1 = const_cast<const Low*>(pBar);	// invalid, E0718
}

void change(const int* p, int d) {
    int* pc = const_cast<int*>(p);
    *pc += d;
}

void run2() {
    int valA = 100;
    const int valB = 100;

    cout << valA << " " << valB << endl;
    change(&valA, 20);
    change(&valB, 20);
    cout << valA << " " << valB << endl;
}

int main() {
    run2();
	return 0;
}


