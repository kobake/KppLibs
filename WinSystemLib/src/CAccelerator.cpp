#include "common/_required.h"
#include "CAccelerator.h"

CAccelerator::CAccelerator(int nId)
{
	m_hAccel = ::LoadAccelerators(c2lib::GetInstance(), MAKEINTRESOURCE(nId));
}

CAccelerator::~CAccelerator()
{
	::DeleteObject(m_hAccel);
}
