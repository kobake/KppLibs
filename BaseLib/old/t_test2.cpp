#include "common/_required.h"
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "t_SimpleList.h"

SimpleList<int> a;

void f()
{
	a.put(-1,100);
}
