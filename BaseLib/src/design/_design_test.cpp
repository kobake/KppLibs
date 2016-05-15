#include "common/_required.h"
#include "TInstanceHolder.h"

class Abc : public TInstanceHolder<Abc>{
public:
	Abc(){ }
};

static void f()
{
	Abc a;
}
