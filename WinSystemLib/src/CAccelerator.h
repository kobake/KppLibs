#pragma once

class CAccelerator{
private:
	CAccelerator(const CAccelerator&); //����֎~
public:
	CAccelerator(int nId);
	~CAccelerator();
	HACCEL GetHACCEL() const{ return m_hAccel; }
private:
	HACCEL m_hAccel;
};
