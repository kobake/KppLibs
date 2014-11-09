#pragma once

#include "math/Vec3.h"

namespace opengl{

//�L���X�g
GLdouble* gl_double(math::Vec3Base<double>& v);
const GLdouble* gl_double(const math::Vec3Base<double>& v);

//�R�}���h
void gl_vertex(const math::Vec3Base<double>& v);

} //namespace opengl
