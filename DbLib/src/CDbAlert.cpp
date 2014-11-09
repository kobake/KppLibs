#include "_required.h"
#include "CDbAlert.h"
#include "util/mystring.h"

void CDbAlert::PrintProviderError(_ConnectionPtr pConnection) const
{
	printf("%hs\n", GetProviderError(pConnection).c_str());
}

void CDbAlert::PrintComError(const _com_error& e) const
{
	printf("%hs\n", GetComError(e).c_str());
}

std::string CDbAlert::GetProviderError(_ConnectionPtr pConnection) const
{
	std::string ret;
	char buf[256];

	ErrorPtr  pErr = NULL;

	if( (pConnection->Errors->Count) > 0)
	{
		ret += "ProviderError\n";
		long nCount = pConnection->Errors->Count;
		for(long i = 0; i < nCount; i++)
		{
			pErr = pConnection->Errors->GetItem(i);
			ret += buf_sprintf(buf, "\tError number: %x\n", pErr->Number );
			ret += buf_sprintf(buf, "\tDescription: %hs\n", (LPSTR)pErr->Description);
		}
	}

	return ret;
}

std::string CDbAlert::GetComError(const _com_error& e) const
{
	std::string ret;
	char buf[256];

	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());

	// Print Com errors.
	ret += "ComError\n";
	ret += buf_sprintf(buf, "\tCode = %08lx\n", e.Error());
	ret += buf_sprintf(buf, "\tCode meaning = %ls\n", e.ErrorMessage());
	ret += buf_sprintf(buf, "\tSource = %hs\n", (LPCSTR) bstrSource);
	ret += buf_sprintf(buf, "\tDescription = %hs\n", (LPCSTR) bstrDescription);

	return ret;
}
