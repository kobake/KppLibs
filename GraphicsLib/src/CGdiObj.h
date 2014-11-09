#pragma once

class CGdiObj{
public:
	virtual ~CGdiObj(){}
	virtual HGDIOBJ GetHGDIOBJ() const = 0;
};

class CBrush : public CGdiObj{
public:
	CBrush()
	{
		m_hbr = (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	CBrush(COLORREF color)
	{
		m_hbr = ::CreateSolidBrush(color);
	}
	~CBrush()
	{
		::DeleteObject(m_hbr);
	}
	HGDIOBJ GetHGDIOBJ() const{ return m_hbr; }
	HBRUSH GetHBRUSH() const{ return m_hbr; }
private:
	HBRUSH	m_hbr;
};

class CPen : public CGdiObj{
public:
	CPen()
	{
		m_hpen = (HPEN)::GetStockObject(NULL_PEN);
	}
	CPen(COLORREF color)
	{
		m_hpen = ::CreatePen(PS_SOLID, 1, color);
	}
	~CPen()
	{
		::DeleteObject(m_hpen);
	}
	HGDIOBJ GetHGDIOBJ() const{ return m_hpen; }
	HPEN GetHPEN() const{ return m_hpen; }
private:
	HPEN	m_hpen;
};

class CDcSelector{
public:
	CDcSelector(HDC hdc, const CGdiObj& obj)
	: m_hdc(hdc)
	{
		m_old = ::SelectObject(hdc, obj.GetHGDIOBJ());
	}
	~CDcSelector()
	{
		::SelectObject(m_hdc, m_old);
	}
private:
	HDC		m_hdc;
	HGDIOBJ	m_old;
};
