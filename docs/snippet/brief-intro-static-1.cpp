#include <iostream>
using namespace std;

struct High {
    int a = 10;
};
struct Low : High {
    int b = 20;
};
struct Pond {};

int main(){

    High bar;
    Low blow;

    High * pb = static_cast<High *>(&blow);		// 正确：向上转型
    Low * p1 = static_cast<Low *>(&bar);		// 正确：向下转型
    //Pond * pmer = static_cast<Pond *>(&blow);	// 编译错误

    cout << pb->a << endl;
    cout << p1->b << endl; // 错误

    return 0;
}