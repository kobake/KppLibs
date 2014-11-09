#pragma once

#include "math/Vec3.h"

namespace opengl{

//キャスト
GLdouble* gl_double(math::Vec3Base<double>& v);
const GLdouble* gl_double(const math::Vec3Base<double>& v);

//コマンド
void gl_vertex(const math::Vec3Base<double>& v);

} //namespace opengl
