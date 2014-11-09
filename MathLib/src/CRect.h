#pragma once


#include "CVec2.h"
#include "BoxSize.h"
#include "Align.h"
#include "util/std.h"
#include "range.h"
#include <DebugLib_Assert.h>

namespace math{

template <class T>
class RectBase{
private:
	typedef Vec2Base<T> t_Point;
	typedef BoxSizeBase<T> t_BoxSize;
public:
	T m_left;
	T m_top;
	T m_right;
	T m_bottom;
public:
	//コンストラクタ・デストラクタ
	explicit RectBase(const t_BoxSize& box)			{ Assign(box);         }
	RectBase()										{ Assign(0,0,0,0);     }
	RectBase(T _x,T _y,T _w,T _h)					{ Assign(_x,_y,_w,_h); }
	RectBase(T _x,T _y,T _w,T _h,const Align& _align)		{ Assign(_x,_y,_w,_h,_align); }
	RectBase(const t_Point& pt1,const t_Point& pt2)		{ Assign(pt1,pt2);     }
	RectBase(const t_Point& pt,const t_BoxSize& size)	{ Assign(pt,size);     }
	template <class T2>
	RectBase(const RectBase<T2>& rhs){ Assign(rhs); }
	//設定・取得
	T Left()		const{ return m_left; }
	T Top()			const{ return m_top; }
	T Right()		const{ return m_right; }
	T Bottom()		const{ return m_bottom; }
	T GetLeft()		const{ return m_left; }
	T GetTop()		const{ return m_top; }
	T GetRight()	const{ return m_right; }
	T GetBottom()	const{ return m_bottom; }
	void SetLeft(T n)	{ m_left=n; }
	void SetTop(T n)	{ m_top=n; }
	void SetRight(T n)	{ m_right=n; }
	void SetBottom(T n)	{ m_bottom=n; }
	//状態
	void makeBad();
	bool good() const;
	//代入
	template <class T2>
	void Assign(const RectBase<T2>& rc)						{ m_left=(T)rc.m_left;			m_top=(T)rc.m_top;				m_right=(T)rc.m_right;	m_bottom=(T)rc.m_bottom;	}
	void Assign(T _x,T _y,T _w,T _h)						{ m_left=_x;					m_top=_y;						m_right=_x+_w;			m_bottom=_y+_h;				}
	void Assign(T _x,T _y,T _w,T _h,const Align& _align)	{ m_left=_align.adjustX(_x,_w);	m_top=_align.adjustY(_y,_h);	m_right=m_left+_w;		m_bottom=m_top+_h;			}
	void Assign(const t_BoxSize& box)						{ m_left=0;						m_top=0;						m_right=box.w;			m_bottom=box.h;				}
	void Assign(const t_Point& pt,const t_BoxSize& size)	{ Assign(pt.x,pt.y,size.w,size.h); }
	void Assign(const t_Point& pt1,const t_Point& pt2); //pt1,pt2を含む最小の矩形
	void SetUpperleft(const t_Point& pt);
	void SetLowerright(const t_Point& pt);
	void SetXywh(T _x,T _y,T _w,T _h);
	void SetLtrb(T _left,T _top,T _right,T _bottom);
	//代入ラッパー
	RectBase& operator = (const RectBase& rc) { Assign(rc); return *this; }
	//サイズ
	T Width()				const { return m_right-m_left; }
	T Height()				const { return m_bottom-m_top; }
	T GetWidth()			const { return m_right-m_left; }
	T GetHeight()			const { return m_bottom-m_top; }
	void SetWidth(T _w)	{ m_right=m_left+_w; }
	void SetHeight(T _h)	{ m_bottom=m_top+_h; }
	void SetSize(const t_BoxSize& size){ SetWidth(size.w); SetHeight(size.h); }
	//特定位置
	t_Point GetUpperLeft()	const { return t_Point(m_left, m_top); } //!<左上座標を取得
	t_Point GetUpperRight()	const { return t_Point(m_right,m_top); } //!<右上座標を取得
	t_Point GetLowerLeft()	const { return t_Point(m_left, m_bottom); } //!<左下座標を取得
	t_Point GetLowerRight()	const { return t_Point(m_right,m_bottom); } //!<右下座標を取得
	t_Point GetCenter()		const { return t_Point((m_left+m_right)/2,(m_top+m_bottom)/2); } //!<中心座標を取得
	//サイズラッパー
	void SetSize(T w,T h){ return SetSize(t_BoxSize(w,h)); }
	//位置
	void MoveTo(T x,T y){ m_right+=x-m_left; m_bottom+=y-m_top; m_left=x; m_top=y; }
	void MoveOff(T offx,T offy){ m_left+=offx; m_right+=offx; m_top+=offy; m_bottom+=offy; }
	//位置ラッパー
	void MoveTo(const t_Point& pt){ MoveTo(pt.x,pt.y); }
	void MoveOff(const t_Point& pt){ MoveOff(pt.x,pt.y); }
	//サイズ調整
	void OffsetWidth(int nOffset, Align alignBase = X_LEFT)
	{
		if(alignBase==X_LEFT){
			m_right += nOffset;
		}
		else if(alignBase==X_CENTER){
			m_left -= nOffset/2;
			m_right += (nOffset-nOffset/2);
		}
		else if(alignBase==X_RIGHT){
			m_left -= nOffset;
		}
		else{
			assert(0);
		}
	}
	void OffsetTop(int nOffset){ m_top += nOffset; }
	void OffsetBottom(int nOffset){ m_bottom += nOffset; }
	//判定
	bool IsEmpty() const{ return GetWidth()<=0 || GetHeight()<=0; }
	bool IsContains(const t_Point& pt) const;
	bool IsIntersects(const RectBase& rc) const;
	bool IsContains(const RectBase& rc) const;
	//比較
	bool IsEquals(const RectBase& rc) const;
	//比較演算子
	bool operator == (const RectBase& r) const { return IsEquals(r); }
	bool operator != (const RectBase& r) const { return !IsEquals(r); }
	//RectBase - t_Point 合成演算子
	RectBase& operator += (const t_Point& pt);
	RectBase& operator -= (const t_Point& pt);
	RectBase& operator *= (double n);
	RectBase& operator /= (double n);
	RectBase& operator *= (int n);
	RectBase& operator /= (int n);
	//演算子ラッパー
	const RectBase operator + (const t_Point& pt) const{ RectBase ret=*this; return ret+=pt; }
	const RectBase operator - (const t_Point& pt) const{ RectBase ret=*this; return ret-=pt; }
	const RectBase operator * (double n) const{ RectBase ret=*this; return ret*=n; }
	const RectBase operator / (double n) const{ RectBase ret=*this; return ret/=n; }
	const RectBase operator * (int n) const{ RectBase ret=*this; return ret*=n; }
	const RectBase operator / (int n) const{ RectBase ret=*this; return ret/=n; }
	//調整
	void adjust(const RectBase& rc); //rcに収まるように縮める
	//範囲
	range<T> rangeLR() const{ return range<T>(m_left,m_right); }
	range<T> rangeTB() const{ return range<T>(m_top,m_bottom); }
	//RectBase - RectBase 合成演算子
	RectBase& operator *= (const RectBase& rc); //積集合
	RectBase& operator += (const RectBase& rc); //和集合
	//演算子ラッパー
	const RectBase operator + (const RectBase& rc) const{ RectBase ret=*this; return ret+=rc; }
	const RectBase operator * (const RectBase& rc) const{ RectBase ret=*this; return ret*=rc; }
	//RectBase - t_BoxSize サイズ加減演算子
	RectBase& operator += (const t_BoxSize& size);
	RectBase& operator -= (const t_BoxSize& size);
	//演算子ラッパー
	const RectBase operator + (const t_BoxSize& size) const{ RectBase ret=*this; return ret+=size; }
	const RectBase operator - (const t_BoxSize& size) const{ RectBase ret=*this; return ret-=size; }
};


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                        外部演算子                           //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

template <class T>
inline RectBase<T> operator + (const Vec2Base<T>& pt,const RectBase<T>& rc)
{
	return rc+pt;
}


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           使用                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

typedef RectBase<long> CRect;
typedef RectBase<double> CRectF;


// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //
//                           実装                              //
// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- //

// -- 状態 -- //
template <class T>
void RectBase<T>::makeBad()
{
	m_left=m_top=-1;
	m_right=m_bottom=-2;
}

template <class T>
bool RectBase<T>::good() const
{
	return m_left<=m_right && m_top<=m_bottom;
}

// -- 代入 -- //
template <class T>
void RectBase<T>::Assign(const t_Point& pt1,const t_Point& pt2) //pt1,pt2を含む最小の矩形
{
	m_left	=tmin(pt1.x,  pt2.x  );
	m_top	=tmin(pt1.y,  pt2.y  );
	m_right	=tmax(pt1.x+1,pt2.x+1);
	m_bottom=tmax(pt1.y+1,pt2.y+1);
}

template <class T>
void RectBase<T>::SetUpperleft(const t_Point& pt)
{
	m_left=pt.x;
	m_top=pt.y;
}

template <class T>
void RectBase<T>::SetLowerright(const t_Point& pt)
{
	m_right=pt.x;
	m_bottom=pt.y;
}

template <class T>
void RectBase<T>::SetXywh(T _x,T _y,T _w,T _h)
{
	m_left  =_x;
	m_top   =_y;
	m_right =_x+_w;
	m_bottom=_y+_h;
}

template <class T>
void RectBase<T>::SetLtrb(T _left,T _top,T _right,T _bottom)
{
	m_left  =_left;
	m_top   =_top;
	m_right =_right;
	m_bottom=_bottom;
}

// -- 判定 -- //

template <class T>
bool RectBase<T>::IsContains(const t_Point& pt) const
{
	return pt.x>=m_left && pt.x<m_right && pt.y>=m_top && pt.y<m_bottom;
}

template <class T>
bool RectBase<T>::IsIntersects(const RectBase& rc) const
{
	return
		this->m_left   < rc.m_right  &&
		this->m_top    < rc.m_bottom &&
		this->m_right  > rc.m_left   && 
		this->m_bottom > rc.m_top;
}

template <class T>
bool RectBase<T>::IsContains(const RectBase& rc) const
{
	return
		this->m_left   <= rc.m_left &&
		this->m_top    <= rc.m_top  &&
		this->m_right  >= rc.m_right &&
		this->m_bottom >= rc.m_bottom;
}

// -- 比較 -- //
template <class T>
bool RectBase<T>::IsEquals(const RectBase& rc) const
{
	return
		m_left  ==rc.m_left   &&
		m_top   ==rc.m_top    &&
		m_right ==rc.m_right  &&
		m_bottom==rc.m_bottom;
}

// -- RectBase - t_Point 合成演算子 -- //
template <class T>
RectBase<T>& RectBase<T>::operator += (const t_Point& pt)
{
	m_left+=pt.x;
	m_right+=pt.x;
	m_top+=pt.y;
	m_bottom+=pt.y;
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator -= (const t_Point& pt)
{
	m_left-=pt.x;
	m_right-=pt.x;
	m_top-=pt.y;
	m_bottom-=pt.y;
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator *= (double n)
{
	m_left   = (T)(m_left * n);
	m_right  = (T)(m_right * n);
	m_top    = (T)(m_top * n);
	m_bottom = (T)(m_bottom * n);
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator /= (double n)
{
	m_left   = (T)(m_left / n);
	m_right  = (T)(m_right / n);
	m_top    = (T)(m_top / n);
	m_bottom = (T)(m_bottom / n);
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator *= (int n)
{
	m_left   = (T)(m_left * n);
	m_right  = (T)(m_right * n);
	m_top    = (T)(m_top * n);
	m_bottom = (T)(m_bottom * n);
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator /= (int n)
{
	m_left   = (T)(m_left / n);
	m_right  = (T)(m_right / n);
	m_top    = (T)(m_top / n);
	m_bottom = (T)(m_bottom / n);
	return *this;
}


// -- 調整 -- //
template <class T>
void RectBase<T>::adjust(const RectBase& rc) //rcに収まるように縮める
{
	if(m_left  <rc.m_left  )m_left  =rc.m_left;
	if(m_top   <rc.m_top   )m_top   =rc.m_top;
	if(m_right >rc.m_right )m_right =rc.m_right;
	if(m_bottom>rc.m_bottom)m_bottom=rc.m_bottom;
}


// -- RectBase - RectBase 合成演算子 -- //
template <class T>
RectBase<T>& RectBase<T>::operator *= (const RectBase& rc) //積集合
{
	if(IsIntersects(rc)){
		m_left  =tmax(m_left,  rc.m_left);
		m_top   =tmax(m_top,   rc.m_top);
		m_right =tmin(m_right, rc.m_right);
		m_bottom=tmin(m_bottom,rc.m_bottom);
	}else{
		m_left=m_top=m_right=m_bottom=-1;
	}
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator += (const RectBase& rc) //和集合
{
	m_left  =tmin(m_left,  rc.m_left);
	m_top   =tmin(m_top,   rc.m_top);
	m_right =tmax(m_right, rc.m_right);
	m_bottom=tmax(m_bottom,rc.m_bottom);
	return *this;
}

// -- RectBase - t_BoxSize サイズ加減演算子 -- //
template <class T>
RectBase<T>& RectBase<T>::operator += (const t_BoxSize& size)
{
	m_right+=size.w;
	m_bottom+=size.h;
	return *this;
}

template <class T>
RectBase<T>& RectBase<T>::operator -= (const t_BoxSize& size)
{
	m_right-=size.w;
	m_bottom-=size.h;
	return *this;
}


} //namespace math
