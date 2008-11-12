#include <iostream>


class B;

class A : public B{
};

class B{
};

int main(){
	A a;
	B b;
	return 0;
}
