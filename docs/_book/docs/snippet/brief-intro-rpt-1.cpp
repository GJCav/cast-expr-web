#include <iostream>
using namespace std;

struct dat {
    short a;
    short b;
};

int main(){

    long value = 0xA224B118;
    dat *pd = reinterpret_cast<dat*>(&value);
    cout << hex << pd->a; // 输出 value 的前两个字节的数据
    return 0;
}