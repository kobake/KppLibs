#include "_required.h"
#include "BoxSize.h"
#include "CRect.h"

namespace math{

template <class T>
BoxSizeBase<T>::BoxSizeBase(const t_Rect& rc)
: w(rc.GetWidth()), h(rc.GetHeight())
{
}


//明示的なインスタンス化
template class BoxSizeBase<long>;
template class BoxSizeBase<double>;


} //namespace math
