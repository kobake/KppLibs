#include <stdio.h>

//�O��
class B{
public:
	void Print();
};

class A{
public:
	A()
	{
		b.Print();
	}
private:
	B b;
};

void f()
{
	A a;

}
