#include <BaseLib.h>
#include "CNamable.h"
#include "../../ThreadLib/src/CCriticalSection.h"

static CCriticalSection s;

CNameHolder::CNameHolder()
{
	CRITICAL_ENTER(s);
	static int n = 0;
	wchar_t buf[64];
	swprintf(buf, L"name%04d", n);
	n++;
	m_strName = buf;
}

CNameHolder::CNameHolder(const wchar_t* szName)
{
	m_strName = szName;
}
