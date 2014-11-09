#pragma once

#include "Vec3.h"
#include "Plane3.h"
#include "Line3.h"

// -- ŒvZ -- //
namespace math{

//–Ê‚Æ’¼ü‚ÌŒğ“_
bool calc_intersection(Vec3Base<double>* out,const Plane3& plane,const Line3& line);

} //namespace math
