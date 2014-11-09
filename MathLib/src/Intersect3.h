#pragma once

#include "_fwd.h"

namespace math{

class Intersect3{
public:
	//Obb“¯m
	static bool check_intersect(const Obb3& obb0,const Obb3& obb1);
	//Aabb‚Æü•ª
	static bool check_intersect(const Aabb3& aabb,const Segment3& segline);
};

} //namespace math
