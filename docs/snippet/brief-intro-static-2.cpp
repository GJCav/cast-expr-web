#include <iostream>
using namespace std;

struct vector {int a, b;};

class Complex{
public:
    int real = 0, imag = 0;

    Complex(){}
    Complex(vector &v) {
        real = v.a; 
        imag = v.b; 
        cout << "Constructor" << endl;
    }

    operator double(){
        cout << "Type conversion operator." << endl;
        return real * real + imag * imag;
    }
};

int main(){

    vector v = {1, 2};
    Complex c = static_cast<Complex>(v); // 调用了Complex的构造函数
    cout << "real: " << c.real << " imag: " << c.imag << endl; 
    double lenSqr = static_cast<double>(c); // 调用了Complex的转换运算符
    cout << "len ^ 2 = " << lenSqr << endl;

    return 0;
}