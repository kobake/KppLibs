#include <BaseLib.h>
#include "std.h"
#include "array.h"
#include <stdio.h>
#include <stdlib.h>

namespace util{

int irand()
{
	return rand();
}

int irand(int from, int to)
{
	return from+rand()%(to-from+1);
}

double drand(double dmin, double dmax)
{
	return dmin+(double)rand()/RAND_MAX*(dmax-dmin);
}

double drand()
{
	return (double)rand()/RAND_MAX;
}

} //namespace util

