#include "myvector.h"
#include "mystring.h"

class Hoge{
public:
	Hoge(const wchar_t* a[]);

};
void test_myvector()
{
	const wchar_t* a[] = {
		L"ab",
		L"cd",
	};
	Hoge hoge(a);

	myvector<int> aa;
	aa.push_back(0);

	myvector<mystring> b;
	b.push_back(L"");
}
