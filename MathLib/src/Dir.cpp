#include "_required.h"
#include "Dir.h"
#include "Angle.h"
#include "CVec2.h"

Dir::Dir(const math::CVec2& v)
{
	if(v.x<0)d=DIR_L.value();
	else if(v.x>0)d=DIR_R.value();
	else if(v.y<0)d=DIR_U.value();
	else if(v.y>0)d=DIR_D.value();
	else d=DIR_NONE.value();
}

math::CVec2 Dir::toVec() const
{
	if(*this==DIR_L){ return math::CVec2(-1,0); }
	else if(*this==DIR_U){ return math::CVec2(0,-1); }
	else if(*this==DIR_R){ return math::CVec2(1,0); }
	else if(*this==DIR_D){ return math::CVec2(0,1); }
	else{
		return math::CVec2(0,0);
	}
}



/*
const Dir DIR_NONE(-1);
const Dir DIR_L(0);
const Dir DIR_U(1);
const Dir DIR_R(2);
const Dir DIR_D(3);
*/
